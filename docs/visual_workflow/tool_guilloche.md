# Visual workflow — guilloche reels and title cards

## Current production — Meta guilloche 2, Review 4

The [September 29 production record](../development/sessions/20260929.meta_guilloche_2.md)
contains the selection schedule, reference sources and human revision
history. Both complete reels use Izzi hues and expand every plate fully
offscreen. Stills, PDF and all four MKVs are technically verified. Review
of the latest artwork and full human playback/listening remains open.

| Local delivery | Plates | Picture duration / frames at 30 fps | Vertical MKV | Clockwise horizontal MKV |
| --- | ---: | --- | ---: | ---: |
| Wild guilloche Draft 3 | 89 | 14:54 / 26,820 | 227.0 MB | 212.5 MB |
| Moiré and visual illusions Draft 1 | 23 | 3:54 / 7,020 | 54.7 MB | 52.5 MB |

Sizes are decimal MB. Each reel has a four-second opening followed by
ten-second plates. The two reels retain all 64 Draft 2 plates exactly once
and add 48 selections. The review index
contains 78 stills: 48 selected forms, 16 width comparisons, six illusion
controls and eight alternatives. There are 13 additional detail views and a
44-page review-stills PDF.

The subsequent [moiré motion/glitch pass](../development/sessions/20260929.moire_glitch_motion.md)
uses the companion's vertical MKV as its moving input. It applies the seven
`glitch-motion-studies` families to all 23 plates, removes the opening title,
and delivers 3:50 vertical and clockwise horizontal versions. Its
separate output index
contains the movies and encoded comparisons.

The [current revision details](../development/sessions/20260929.meta_guilloche_2.md#review-4--latest-human-revision-requirements)
include the warm central Imam vault with an onion outline, three ledges and
concave triangular coffers; tighter saguaro arms and irregular trunk widths;
and the accepted brain-coral network with closer cropping and relief that
turns away at the perimeter. Earlier gray/semicircular Imam candidates and
partly clearing exits are superseded. The accepted wall studies remain
separate from the ceiling passes.

## Native sources and review

meta_guilloche_production.py
orchestrates the native bridge, palette conversion, motion, rasterization
and clip receipts. The [meta header](../html/izzi-svg-guilloche-meta_8h.html)
supplies production-specific botanical, reef, coffer and illusion recipes.
The [generic enrichment gallery](../generative_patterns.md#native-guilloche-patterns)
and frozen Draft 2 sources supply the other forms. Production recipes use
`izzi.meta-guilloche-recipe/1`; they are separate from generic scene JSON
preset names.

Run commands from the repository root. The renderer requires the existing
Draft 2 native assets, C++20/RapidJSON, Python Cairo/GI with librsvg, OpenCV,
NumPy, lxml and Pillow, plus FFmpeg/FFprobe. The PDF exporter uses headless
Chrome and `pdfinfo`. Font paths in the scripts refer to the local Atkinson
Hyperlegible Next installation; reference images are cached under the
review directory and identified in `references.json`.

For revisions, keep the existing `outputs/meta-guilloche-2/review.json`,
`recipes/` and each reel's `production.json`. The `prepare` stage initializes
these fixed output paths and rewrites schedules and recipes, so it belongs
to initial preparation. Rebuild the native bridge before refreshing an
existing review:

```sh
CCACHE_DISABLE=1 python3 - <<'PY'
import sys
sys.path.insert(0, 'scripts')
import meta_guilloche_production as m
m.compile_native()
PY
python3 scripts/render-meta-guilloche.py stills --workers 4
python3 scripts/review-meta-guilloche.py
python3 scripts/export-meta-guilloche-review-pdf.py
```

The review scripts regenerate selected images, comparisons, native details,
reference pages and palette checks. Rebuild the HTML index with `stills`
before adding the detail/reference and PDF links. Record artistic feedback
in the session plan and `human-review.json`; technical checks alone do not
establish artistic approval. The Chie Tanaka book citation remains recorded
as supplied by the user, without a claim that it was located or inspected.

## Clip rendering and compact delivery

After updating the selected recipes and framing, render from the existing
production manifests and assemble both orientations:

```sh
python3 scripts/render-meta-guilloche.py clips --reel both --workers 4
python3 scripts/finish-meta-guilloche.py assemble --reel both
python3 scripts/finish-meta-guilloche.py rotate --reel both
python3 scripts/finish-meta-guilloche.py verify --reel both
```

`clips` supports `--reel main` or `--reel optical` and a comma-separated
`--indices` list of zero-based manifest indices. It reuses a clip only when
its input key and recorded video hash match. `stills` always processes the
review set; the reel/index filters apply to clip selection. Existing intro
clips and audio mixes can be reused for artwork revisions that preserve the
timeline. Initial production or timeline changes also require
`render-meta-guilloche.py intro` and `finish-meta-guilloche.py audio` before
assembly.

Deliveries use H.264/libx264 CRF 24, medium preset, 3 Mbit/s maximum rate,
6 Mbit buffer, 30 fps, square pixels, yuv420p and limited-range Rec.709.
GOPs are closed, at most 60 frames, with no B frames. Vertical is
1080 × 1920; horizontal is 1920 × 1080, rotated 90° clockwise. Audio is
128 kbit/s MP3, 48 kHz stereo, copied identically between orientations.
`verify` also refreshes the delivery index, contact sheet and checksums.

## Complete-background exits and verification

Every exit combines uniform enlargement with smooth outward translation
based on measured native ink bounds, including geometry beyond the nominal
SVG page. Native clip frames 289–299 must be exact ivory. Force a keyframe
at local frame 289, also preserved when encoding the horizontal reel, so
compression does not carry residual artwork into the clear background.
The completed edit achieves this within the existing ten-second slots.

Verification checks complete video/audio decode, picture frame counts and
timing, codecs and color metadata, GOP length, dimensions, size limits,
audio stream identity and true peak below −2 dBTP, and sampled clockwise
rotation comparisons. Every plate's encoded frames 289 and 299 are checked
at full resolution in both orientations: **448 frames across 112 plates**.
Encoded backgrounds allow at most one code value of spatial range per RGB
channel and eight values of color conversion from native ivory.

| Evidence | Record |
| --- | --- |
| Complete delivery inventory and hashes | delivery.json and SHA256SUMS |
| Native/palette checks and implementation sources | validation.json and implementation.json |
| Both orientation pairs | Main verification and optical verification |
| Every clear-background ending | complete-exit-verification.json |
| Per-plate encoded identity, motion and compression review | encoded-review.json |
| Human feedback and remaining review | human-review.json |

Four focused native/schema tests passed for the delivered source. All 43
unchanged enrichment recipes matched the archived geometry at progress
0, 0.37 and 1. Review 4 preserved the first 228 encoded picture frames of
111 clips, rendered the warm Imam clip in full and replaced every ending.
The prefix audit
and production-local Review 4 helpers describe that particular archived
edit; the normal clip renderer above renders complete clips.

Historical clip, audio-mix and paint-cache receipts preserve the producer
hashes and paths from their original runs. Those paths can now hold newer
revisions. Use `delivery.json` and the reel checksum inventories for current
deliverables, `implementation.json` for current renderer sources, and the
archived files when checking earlier edits.

Eight retained dense-line midpoint comparisons remain flagged for
compression review: seven in the main reel and one in the companion.
The encoded images were inspected, with softening/grain recorded as compact
delivery tradeoffs. Human playback, listening and latest-artwork acceptance
remain separate from the technical pass.

## Earlier v4 renderer and title cards

- Shared modules: `scripts/guilloche_v4_common.py`,
  `scripts/guilloche_v4_contours.py`.
- Renderers/checks: `scripts/render-guilloche-v4.py`,
  `scripts/render-guilloche-v4-title-cards.py`,
  `scripts/check-guilloche-v4.py`, `scripts/finalize-guilloche-v4.py`.
- Here Lies Trouble title card (madredeus type, light gray, guilloche style):
  `scripts/render-hlt-title-card-madredeus.py`; sample
  `…/episode-01-local-trial/title-card-v5-madredeus.png` (SHA-256
  `6a112554…`). Font: `resources.static/typeFONTBAT/007_madredeus/madredeus.ttf`
  (non-commercial license; readme must accompany the font).
