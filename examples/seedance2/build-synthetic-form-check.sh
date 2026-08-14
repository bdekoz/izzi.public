#!/usr/bin/env bash

set -euo pipefail

script_dir=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)
repo_root=$(cd -- "${script_dir}/../.." && pwd)
plan=${1:-"${repo_root}/outputs/ad-hoc/synthetic/synthetic-form-check-01.plan.json"}
output=${2:-"${repo_root}/outputs/ad-hoc/synthetic/synthetic-form-check-01.mkv"}
assets=${3:-"${repo_root}/outputs/ad-hoc/synthetic/synthetic-form-check-01.assets"}

for tool in ffmpeg ffprobe jq magick mkvmerge sha256sum; do
  if ! command -v "${tool}" >/dev/null 2>&1; then
    printf 'required tool not found: %s\n' "${tool}" >&2
    exit 1
  fi
done

if [[ -e "${output}" ]]; then
  printf 'refusing to overwrite existing output: %s\n' "${output}" >&2
  exit 1
fi

jq -e '
  .schema == "izzi.synthetic-vertical-form-check/v1"
  and .form_status == "USER-CONFIRMED"
  and .output_path == "outputs/ad-hoc/synthetic/synthetic-form-check-01.mkv"
  and .video.width == 1080
  and .video.height == 1920
  and .video.frame_rate == 30
  and .video.pixel_format == "yuv420p"
  and .video.audio == "none"
  and (.episodes | length) == 10
  and ([.episodes[].episode_number] == [range(1; 11)])
  and ([.episodes[].title_card_seconds] | all(. == 3))
  and ([.episodes[].content_duration_seconds]
       | all((type == "number") and (floor == .) and . >= 60 and . <= 180))
  and ([.episodes[].source]
       | all(startswith("resources/asama-loops/") and endswith(".mkv")))
  and ([.episodes[].source] | unique | length) == 10
  and .summary.episode_count == 10
  and .summary.title_seconds == 30
  and .summary.total_seconds
      == ([.episodes[] | .title_card_seconds + .content_duration_seconds] | add)
  and .summary.total_frames == (.summary.total_seconds * .video.frame_rate)
  and .authority.provider_media_used == false
  and .authority.provider_interaction_authorized == false
' "${plan}" >/dev/null

while IFS=$'\t' read -r source expected_hash; do
  source_path="${repo_root}/${source}"
  if [[ ! -f "${source_path}" ]]; then
    printf 'source not found: %s\n' "${source}" >&2
    exit 1
  fi
  observed_hash=$(sha256sum "${source_path}")
  observed_hash=${observed_hash%% *}
  if [[ "${observed_hash}" != "${expected_hash}" ]]; then
    printf 'source hash mismatch: %s\n' "${source}" >&2
    exit 1
  fi
done < <(jq -r '.episodes[] | [.source, .source_sha256] | @tsv' "${plan}")

mkdir -p -- "${assets}/title-cards" "${assets}/episodes" \
  "${assets}/episode-sheets"

fps=$(jq -r '.video.frame_rate' "${plan}")
width=$(jq -r '.video.width' "${plan}")
height=$(jq -r '.video.height' "${plan}")
episode_count=$(jq -r '.summary.episode_count' "${plan}")
concat_file="${assets}/episodes.concat.txt"
: > "${concat_file}"

for episode_number in $(seq 1 "${episode_count}"); do
  episode=$(jq -c ".episodes[$((episode_number - 1))]" "${plan}")
  duration=$(jq -r '.content_duration_seconds' <<< "${episode}")
  source=$(jq -r '.source' <<< "${episode}")
  offset=$(jq -r '.source_offset_seconds' <<< "${episode}")
  source_path="${repo_root}/${source}"
  title_card="${assets}/title-cards/episode-$(printf '%02d' "${episode_number}").png"
  episode_video="${assets}/episodes/episode-$(printf '%02d' "${episode_number}").mkv"
  episode_probe="${assets}/episodes/episode-$(printf '%02d' "${episode_number}").ffprobe.json"
  content_frames=$((duration * fps))
  title_frames=$((3 * fps))
  total_frames=$((content_frames + title_frames))
  total_seconds=$((duration + 3))

  printf '[episode %02d/%02d] card=3s content=%ss source=%s offset=%s\n' \
    "${episode_number}" "${episode_count}" "${duration}" \
    "${source##*/}" "${offset}"

  magick \
    -size "${width}x${height}" xc:black \
    -stroke white -strokewidth 8 -fill none \
    -draw 'roundrectangle 108,192 972,1728 9,9' \
    -strokewidth 2 \
    -draw 'roundrectangle 126,210 954,1710 4,4' \
    -draw 'line 270,350 810,350 line 270,1570 810,1570' \
    -fill white -stroke none -font DejaVu-Serif -pointsize 420 \
    -gravity center -annotate +0+0 "${episode_number}" \
    "${title_card}"

  ffmpeg \
    -y \
    -hide_banner \
    -loglevel error \
    -stats \
    -loop 1 \
    -framerate "${fps}" \
    -i "${title_card}" \
    -stream_loop -1 \
    -ss "${offset}" \
    -i "${source_path}" \
    -filter_complex \
      "[0:v]trim=end_frame=${title_frames},setpts=N/(${fps}*TB),scale=${width}:${height},format=yuv420p[card];[1:v]scale=${width}:${height}:force_original_aspect_ratio=decrease:force_divisible_by=2:flags=lanczos,pad=${width}:${height}:(ow-iw)/2:(oh-ih)/2:color=black,fps=${fps},trim=end_frame=${content_frames},setpts=N/(${fps}*TB),format=yuv420p[content];[card][content]concat=n=2:v=1:a=0[outv]" \
    -map '[outv]' \
    -an \
    -r "${fps}" \
    -fps_mode cfr \
    -frames:v "${total_frames}" \
    -t "${total_seconds}" \
    -c:v libx264 \
    -preset veryfast \
    -crf 23 \
    -g $((fps * 2)) \
    -keyint_min $((fps * 2)) \
    -sc_threshold 0 \
    -pix_fmt yuv420p \
    -metadata title="Synthetic form check 01 — episode ${episode_number}" \
    -metadata comment="Local Izzi/Asama visual-form test; no provider media" \
    "${episode_video}"

  ffprobe \
    -v error \
    -count_frames \
    -show_entries \
    'format=duration,size:stream=codec_type,codec_name,width,height,pix_fmt,r_frame_rate,nb_read_frames' \
    -of json \
    -o "${episode_probe}" \
    "${episode_video}"

  jq -e \
    --arg rate "${fps}/1" \
    --argjson frames "${total_frames}" \
    --argjson seconds "${total_seconds}" \
    --argjson width "${width}" \
    --argjson height "${height}" \
    '(.streams | length) == 1
     and .streams[0].codec_type == "video"
     and .streams[0].codec_name == "h264"
     and .streams[0].width == $width
     and .streams[0].height == $height
     and .streams[0].pix_fmt == "yuv420p"
     and .streams[0].r_frame_rate == $rate
     and (.streams[0].nb_read_frames | tonumber) == $frames
     and (.format.duration | tonumber) == $seconds' \
    "${episode_probe}" >/dev/null

  middle_frame=$((title_frames + content_frames / 2))
  ffmpeg \
    -y \
    -hide_banner \
    -loglevel error \
    -i "${episode_video}" \
    -vf "select='eq(n,0)+eq(n,${title_frames})+eq(n,${middle_frame})',scale=270:480:flags=lanczos,tile=3x1:padding=8:margin=8:color=black" \
    -frames:v 1 \
    "${assets}/episode-sheets/episode-$(printf '%02d' "${episode_number}").png"

  episode_absolute=$(realpath -- "${episode_video}")
  printf "file '%s'\n" "${episode_absolute}" >> "${concat_file}"
done

encoded_output="${assets}/synthetic-form-check-01.concat.mkv"
ffmpeg \
  -y \
  -hide_banner \
  -loglevel error \
  -stats \
  -f concat \
  -safe 0 \
  -i "${concat_file}" \
  -map 0:v:0 \
  -c copy \
  "${encoded_output}"

mkvmerge \
  --quiet \
  --deterministic 20260809 \
  --regenerate-track-uids \
  --no-date \
  --title 'Synthetic form check 01' \
  -o "${output}" \
  "${encoded_output}"

montage \
  -background '#111111' \
  -fill white \
  -font DejaVu-Sans \
  -pointsize 24 \
  -label '%t' \
  "${assets}"/episode-sheets/*.png \
  -tile 2x5 \
  -geometry +16+44 \
  "${assets}/contact-sheet.png"

ffprobe \
  -v error \
  -count_frames \
  -show_entries \
  'format=duration,size,bit_rate:stream=index,codec_type,codec_name,width,height,pix_fmt,r_frame_rate,nb_read_frames' \
  -of json \
  -o "${assets}/synthetic-form-check-01.ffprobe.json" \
  "${output}"

expected_seconds=$(jq -r '.summary.total_seconds' "${plan}")
expected_frames=$(jq -r '.summary.total_frames' "${plan}")
jq -e \
  --arg rate "${fps}/1" \
  --argjson frames "${expected_frames}" \
  --argjson seconds "${expected_seconds}" \
  --argjson width "${width}" \
  --argjson height "${height}" \
  '(.streams | length) == 1
   and .streams[0].codec_type == "video"
   and .streams[0].codec_name == "h264"
   and .streams[0].width == $width
   and .streams[0].height == $height
   and .streams[0].pix_fmt == "yuv420p"
   and .streams[0].r_frame_rate == $rate
   and (.streams[0].nb_read_frames | tonumber) == $frames
   and (.format.duration | tonumber) == $seconds' \
  "${assets}/synthetic-form-check-01.ffprobe.json" >/dev/null

sha256sum \
  "${plan}" \
  "${output}" \
  "${assets}/contact-sheet.png" \
  "${assets}/synthetic-form-check-01.ffprobe.json"
