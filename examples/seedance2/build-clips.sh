#!/usr/bin/env bash

set -euo pipefail

script_dir=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)
repo_root=$(cd -- "${script_dir}/../.." && pwd)
output_dir=${1:-"${script_dir}/generated"}
compiler=${CXX:-c++}

for tool in "${compiler}" ffmpeg ffprobe jq mkvmerge; do
  if ! command -v "${tool}" >/dev/null 2>&1; then
    printf 'required tool not found: %s\n' "${tool}" >&2
    exit 1
  fi
done

mkdir -p -- "${output_dir}"
work_dir=$(mktemp -d)
cleanup()
{
  rm -rf -- "${work_dir}"
}
trap cleanup EXIT

"${compiler}" \
  -std=c++20 \
  -O2 \
  -Wall \
  -Wextra \
  -Wpedantic \
  -Werror \
  -I"${repo_root}/src" \
  "${script_dir}/title-transition-study.cc" \
  -o "${work_dir}/title-transition-study"

for config in \
  "${script_dir}/hamonshu-10s.json" \
  "${script_dir}/roulette-10s.json"
do
  stem=$(basename -- "${config}" .json)
  frame_dir="${work_dir}/${stem}-frames"
  mkdir -p -- "${frame_dir}"

  "${work_dir}/title-transition-study" "${config}" "${frame_dir}"

  rate=$(jq -r \
    '.timeline.frame_rate | "\(.numerator)/\(.denominator)"' \
    "${config}")
  frame_count=$(jq -r '.timeline.frame_count' "${config}")
  expected_duration=$(jq -r \
    '.timeline | .frame_count * .frame_rate.denominator
                 / .frame_rate.numerator' \
    "${config}")
  width=$(jq -r '.canvas.width_px' "${config}")
  height=$(jq -r '.canvas.height_px' "${config}")
  deterministic_seed=$(jq -r '.transition.seed' "${config}")
  poster_frame=$(jq -r '.poster_frame' "${frame_dir}/manifest.json")
  printf -v poster_name 'frame-%08d.svg' "${poster_frame}"

  video="${output_dir}/${stem}.mkv"
  encoded_video="${work_dir}/${stem}-ffmpeg.mkv"
  poster="${output_dir}/${stem}-poster.png"

  ffmpeg \
    -y \
    -hide_banner \
    -loglevel warning \
    -framerate "${rate}" \
    -start_number 0 \
    -i "${frame_dir}/frame-%08d.svg" \
    -frames:v "${frame_count}" \
    -c:v libx264 \
    -preset medium \
    -crf 18 \
    -pix_fmt yuv420p \
    -metadata title="${stem}" \
    -metadata comment="Izzi Seedance 2 title-transition study" \
    "${encoded_video}"

  mkvmerge \
    --quiet \
    --deterministic "${deterministic_seed}" \
    --regenerate-track-uids \
    --no-date \
    --title "${stem}" \
    -o "${video}" \
    "${encoded_video}"

  ffmpeg \
    -y \
    -hide_banner \
    -loglevel warning \
    -i "${frame_dir}/${poster_name}" \
    -frames:v 1 \
    -update 1 \
    "${poster}"

  cp -- "${frame_dir}/manifest.json" \
    "${output_dir}/${stem}-render-manifest.json"

  ffprobe \
    -v error \
    -count_frames \
    -show_entries \
    'format=duration,size,bit_rate:stream=index,codec_type,codec_name,width,height,pix_fmt,r_frame_rate,nb_read_frames' \
    -of json \
    -o "${output_dir}/${stem}-ffprobe.json" \
    "${video}"

  jq -e \
    --arg rate "${rate}" \
    --argjson frame_count "${frame_count}" \
    --argjson expected_duration "${expected_duration}" \
    --argjson width "${width}" \
    --argjson height "${height}" \
    '(.streams | length) == 1
     and .streams[0].codec_type == "video"
     and .streams[0].codec_name == "h264"
     and .streams[0].width == $width
     and .streams[0].height == $height
     and .streams[0].pix_fmt == "yuv420p"
     and .streams[0].r_frame_rate == $rate
     and (.streams[0].nb_read_frames | tonumber) == $frame_count
     and (.format.duration | tonumber) == $expected_duration' \
    "${output_dir}/${stem}-ffprobe.json" >/dev/null

  sha256sum "${video}" "${poster}"
done
