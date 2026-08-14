#!/usr/bin/env bash

set -euo pipefail

script_dir=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)
repo_root=$(cd -- "${script_dir}/../.." && pwd)
plan=${1:-"${repo_root}/outputs/ad-hoc/synthetic/synthetic-form-check-02.plan.json"}
output=${2:-"${repo_root}/outputs/ad-hoc/synthetic/synthetic-form-check-02.mkv"}
assets=${3:-"${repo_root}/outputs/ad-hoc/synthetic/synthetic-form-check-02.assets"}
filmstrip=${4:-"${repo_root}/outputs/ad-hoc/synthetic/synthetic-form-check-02-episode-filmstrip.png"}

for tool in fc-match ffmpeg ffprobe jq magick mkvmerge montage sha256sum; do
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
  .schema == "izzi.synthetic-vertical-form-check/v2"
  and .form_status == "USER-DIRECTED-CANDIDATE"
  and .visual_acceptance_status == "PENDING"
  and .output_path == "outputs/ad-hoc/synthetic/synthetic-form-check-02.mkv"
  and .filmstrip_path == "outputs/ad-hoc/synthetic/synthetic-form-check-02-episode-filmstrip.png"
  and .video.width == 1080
  and .video.height == 1920
  and .video.frame_rate == 30
  and .video.pixel_format == "yuv420p"
  and .video.audio == "none"
  and .title_cards.duration_seconds == 3
  and .title_cards.visible_text == "episode number only"
  and .title_cards.typeface == "Atkinson Hyperlegible Mono"
  and .title_cards.weight == 800
  and .title_cards.style == "house-style"
  and .title_cards.specification_version == "2.3"
  and .title_cards.specification_commit
      == "ce1a08e5180d6572343c53918a30ff5c706d29e6"
  and .title_cards.font_binary_delivery == "PROHIBITED"
  and .composition.layout == "three-horizontal-bands"
  and .composition.band_height == 640
  and .composition.primary_band == "middle"
  and .composition.unused_outer_band_policy == "black"
  and .unchanged_content_episode_numbers == [2, 7, 10]
  and (.episodes | length) == 10
  and ([.episodes[].episode_number] == [range(1; 11)])
  and ([.episodes[].title_card_seconds] | all(. == 3))
  and ([.episodes[].content_duration_seconds]
       | all((type == "number") and (floor == .) and . >= 60 and . <= 180))
  and ([.episodes[] | select(.mode == "reuse-test-01-content")
       | .episode_number] == [2, 7, 10])
  and ([.episodes[] | select(.mode == "three-band-dual-landscape")]
       | length) == 7
  and ([.episodes[] | select(.mode == "three-band-dual-landscape")]
       | all(
           .primary.band == "middle"
           and (.secondary.band == "top" or .secondary.band == "bottom")
           and .blank_band != .secondary.band
           and (.blank_band == "top" or .blank_band == "bottom")
           and .primary.path != .secondary.path
           and (.primary.path
                | startswith("resources/asama-loops/") and endswith(".mkv"))
           and (.secondary.path
                | startswith("resources/asama-loops/") and endswith(".mkv"))))
  and .summary.episode_count == 10
  and .summary.unchanged_content_episode_count == 3
  and .summary.dual_landscape_episode_count == 7
  and .summary.title_seconds == 30
  and .summary.total_seconds
      == ([.episodes[] | .title_card_seconds + .content_duration_seconds] | add)
  and .summary.total_frames == (.summary.total_seconds * .video.frame_rate)
  and .authority.provider_media_used == false
  and .authority.provider_interaction_authorized == false
' "${plan}" >/dev/null

while IFS=$'\t' read -r relative_path expected_hash; do
  artifact_path="${repo_root}/${relative_path}"
  if [[ ! -f "${artifact_path}" ]]; then
    printf 'artifact not found: %s\n' "${relative_path}" >&2
    exit 1
  fi
  observed_hash=$(sha256sum "${artifact_path}")
  observed_hash=${observed_hash%% *}
  if [[ "${observed_hash}" != "${expected_hash}" ]]; then
    printf 'artifact hash mismatch: %s\n' "${relative_path}" >&2
    exit 1
  fi
done < <(
  jq -r '
    ([
       {path: .base.plan_path, sha256: .base.plan_sha256},
       {path: .base.output_path, sha256: .base.output_sha256},
       {path: .base.filmstrip_path, sha256: .base.filmstrip_sha256}
     ]
     + [.episodes[]
        | if .mode == "reuse-test-01-content" then
            {path: .content_source_path, sha256: .content_source_sha256}
          else
            {path: .primary.path, sha256: .primary.sha256},
            {path: .secondary.path, sha256: .secondary.sha256}
          end])
    | unique_by(.path)[]
    | [.path, .sha256]
    | @tsv
  ' "${plan}"
)

while IFS=$'\t' read -r relative_path role episode_number; do
  dimensions=$(ffprobe \
    -v error \
    -select_streams v:0 \
    -show_entries stream=width,height \
    -of csv=p=0:s=x \
    "${repo_root}/${relative_path}")
  IFS=x read -r source_width source_height <<< "${dimensions}"
  if ((source_width <= source_height)); then
    printf 'episode %s %s source is not landscape: %s (%sx%s)\n' \
      "${episode_number}" "${role}" "${relative_path}" \
      "${source_width}" "${source_height}" >&2
    exit 1
  fi
done < <(
  jq -r '.episodes[]
         | select(.mode == "three-band-dual-landscape")
         | (.episode_number | tostring) as $episode
         | [.primary.path, "primary", $episode],
           [.secondary.path, "secondary", $episode]
         | @tsv' "${plan}"
)

font_path=$(fc-match -f '%{file}' 'Atkinson Hyperlegible Mono:style=ExtraBold')
if [[ ! -f "${font_path}" \
      || "${font_path##*/}" != "AtkinsonHyperlegibleMono-ExtraBold.otf" ]]; then
  printf 'required house-style font unavailable: Atkinson Hyperlegible Mono ExtraBold\n' >&2
  exit 1
fi

mkdir -p -- "${assets}/title-cards" "${assets}/episodes" \
  "${assets}/episode-sheets"

fps=$(jq -r '.video.frame_rate' "${plan}")
width=$(jq -r '.video.width' "${plan}")
height=$(jq -r '.video.height' "${plan}")
band_height=$(jq -r '.composition.band_height' "${plan}")
episode_count=$(jq -r '.summary.episode_count' "${plan}")
title_seconds=$(jq -r '.title_cards.duration_seconds' "${plan}")
title_frames=$((title_seconds * fps))
concat_file="${assets}/episodes.concat.txt"
: > "${concat_file}"

for episode_number in $(seq 1 "${episode_count}"); do
  episode=$(jq -c ".episodes[$((episode_number - 1))]" "${plan}")
  mode=$(jq -r '.mode' <<< "${episode}")
  duration=$(jq -r '.content_duration_seconds' <<< "${episode}")
  content_frames=$((duration * fps))
  total_frames=$((content_frames + title_frames))
  total_seconds=$((duration + title_seconds))
  title_card="${assets}/title-cards/episode-$(printf '%02d' "${episode_number}").png"
  episode_video="${assets}/episodes/episode-$(printf '%02d' "${episode_number}").mkv"
  episode_probe="${assets}/episodes/episode-$(printf '%02d' "${episode_number}").ffprobe.json"
  episode_sheet="${assets}/episode-sheets/episode-$(printf '%02d' "${episode_number}").png"

  printf '[episode %02d/%02d] mode=%s card=%ss content=%ss\n' \
    "${episode_number}" "${episode_count}" "${mode}" \
    "${title_seconds}" "${duration}"

  magick \
    -size "${width}x${height}" xc:black \
    -stroke white -strokewidth 8 -fill none \
    -draw 'rectangle 108,192 972,1728' \
    -strokewidth 2 \
    -draw 'rectangle 126,210 954,1710' \
    -draw 'line 270,350 810,350 line 270,1570 810,1570' \
    -fill white -stroke none -font "${font_path}" -pointsize 420 \
    -gravity center -annotate +0+0 "${episode_number}" \
    "${title_card}"

  if [[ "${mode}" == "reuse-test-01-content" ]]; then
    content_source=$(jq -r '.content_source_path' <<< "${episode}")
    content_first_frame=$(jq -r '.content_source_first_frame' <<< "${episode}")
    content_end_frame=$((content_first_frame + content_frames))

    ffmpeg \
      -y \
      -hide_banner \
      -loglevel error \
      -stats \
      -loop 1 \
      -framerate "${fps}" \
      -i "${title_card}" \
      -i "${repo_root}/${content_source}" \
      -filter_complex \
        "[0:v]trim=end_frame=${title_frames},setpts=N/(${fps}*TB),scale=${width}:${height},format=yuv420p[card];[1:v]trim=start_frame=${content_first_frame}:end_frame=${content_end_frame},setpts=N/(${fps}*TB),fps=${fps},format=yuv420p[content];[card][content]concat=n=2:v=1:a=0[outv]" \
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
      -metadata title="Synthetic form check 02 — episode ${episode_number}" \
      -metadata comment="Local Izzi/Asama three-band visual-form test; no provider media" \
      "${episode_video}"
  else
    primary=$(jq -r '.primary.path' <<< "${episode}")
    primary_offset=$(jq -r '.primary.source_offset_seconds' <<< "${episode}")
    secondary=$(jq -r '.secondary.path' <<< "${episode}")
    secondary_offset=$(jq -r '.secondary.source_offset_seconds' <<< "${episode}")
    secondary_band=$(jq -r '.secondary.band' <<< "${episode}")

    if [[ "${secondary_band}" == "top" ]]; then
      stack='[secondary][primary][blank]vstack=inputs=3[content]'
    else
      stack='[blank][primary][secondary]vstack=inputs=3[content]'
    fi

    ffmpeg \
      -y \
      -hide_banner \
      -loglevel error \
      -stats \
      -loop 1 \
      -framerate "${fps}" \
      -i "${title_card}" \
      -stream_loop -1 \
      -ss "${primary_offset}" \
      -i "${repo_root}/${primary}" \
      -stream_loop -1 \
      -ss "${secondary_offset}" \
      -i "${repo_root}/${secondary}" \
      -filter_complex \
        "[0:v]trim=end_frame=${title_frames},setpts=N/(${fps}*TB),scale=${width}:${height},format=yuv420p[card];[1:v]scale=${width}:${band_height}:force_original_aspect_ratio=decrease:force_divisible_by=2:flags=lanczos,pad=${width}:${band_height}:(ow-iw)/2:(oh-ih)/2:color=black,fps=${fps},trim=end_frame=${content_frames},setpts=N/(${fps}*TB),format=yuv420p[primary];[2:v]scale=${width}:${band_height}:force_original_aspect_ratio=decrease:force_divisible_by=2:flags=lanczos,pad=${width}:${band_height}:(ow-iw)/2:(oh-ih)/2:color=black,fps=${fps},trim=end_frame=${content_frames},setpts=N/(${fps}*TB),format=yuv420p[secondary];color=c=black:s=${width}x${band_height}:r=${fps},trim=end_frame=${content_frames},setpts=N/(${fps}*TB),format=yuv420p[blank];${stack};[card][content]concat=n=2:v=1:a=0[outv]" \
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
      -metadata title="Synthetic form check 02 — episode ${episode_number}" \
      -metadata comment="Local Izzi/Asama three-band visual-form test; no provider media" \
      "${episode_video}"
  fi

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
    "${episode_sheet}"

  episode_absolute=$(realpath -- "${episode_video}")
  printf "file '%s'\n" "${episode_absolute}" >> "${concat_file}"
done

encoded_output="${assets}/synthetic-form-check-02.concat.mkv"
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
  --deterministic 2026080902 \
  --regenerate-track-uids \
  --no-date \
  --title 'Synthetic form check 02' \
  -o "${output}" \
  "${encoded_output}"

montage \
  -background '#111111' \
  -fill white \
  -font Atkinson-Hyperlegible-Next-Medium \
  -pointsize 24 \
  -label '%t' \
  "${assets}"/episode-sheets/*.png \
  -tile 2x5 \
  -geometry +16+44 \
  "${filmstrip}"

ffprobe \
  -v error \
  -count_frames \
  -show_entries \
  'format=duration,size,bit_rate:stream=index,codec_type,codec_name,width,height,pix_fmt,r_frame_rate,nb_read_frames' \
  -of json \
  -o "${assets}/synthetic-form-check-02.ffprobe.json" \
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
  "${assets}/synthetic-form-check-02.ffprobe.json" >/dev/null

sha256sum \
  "${plan}" \
  "${output}" \
  "${filmstrip}" \
  "${assets}/synthetic-form-check-02.ffprobe.json"
