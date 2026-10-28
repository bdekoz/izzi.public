# Audit Usage Blackout — plan draft v2

Status: `APPROVED 2026-09-26; RENDERED 2026-09-26`

This updates [plan draft v1](plan-draft-v1.md) for the PDFs currently in
`resources.rizal/audit-usage/`. The prior promoted render was draft 3:
`scripts/render-audit-usage-blackout-draft3.py`
and its
`manifest.json`.
That render used the first 10 reports (145 pages). The resource folder now has
20 reports (320 pages), so its output does not contain the 10 newer reports.

## Production configuration and revision trail

The original plan described fixed 3-second black spacers. Feedback and the
subsequent renders changed these to **variable-duration black interstitials**.
The available local scripts and manifests record this sequence:

| Render | Black interstitials | PDF-to-image production | Master encode |
| --- | --- | --- | --- |
| Draft 1 | 11 instances rising from 3.0 to 6.0 s | `pdftoppm` fit to 1080 px wide | H.264/AAC in MKV |
| Draft 2 (revision #49) | 11 instances rising from 2.0 to 8.0 s | Ghostscript 300 dpi PNG; retain full raster before final downscale | H.264/AAC in MKV |
| Draft 3 (revision #50) | Same 2.0 to 8.0 s sequence | Same Ghostscript 300 dpi PNG pipeline | HEVC/MP3 in MKV |

The published review catalog at
`/home/bkoz/src/situationshipin.space/data/review-items.json` corroborates
the draft-2 300 dpi raster and 2.0–8.0 second interstitial settings, and
describes draft 3 as the same composition with a revised master encode. Its
current draft-3 entry is marked `KEEP` and `PROMOTED`. These records and the
render scripts establish the implemented production baseline; they do not
contain the full original text of revision issues #49 and #50.

The high resolution PDF raster was introduced with the variable-spacer
draft 2 and preserved in draft 3. The draft-3 script invokes Ghostscript with
`-sDEVICE=png16m -r300 -dGraphicsAlphaBits=4 -dTextAlphaBits=4
-dUseCropBox`, one PNG per PDF page. Its stored first-page PNGs are
2550 × 3300 pixels; the manifest records a 1080 × 1398 page image after
Lanczos downscaling onto the 1080 × 1920 video canvas. The 300 dpi setting
applies to the PDF raster intermediates; the deliverable remains a 1080 × 1920
portrait video.

Draft 3 was the promoted baseline. The approved v2 plan changes its
interstitial endpoint to **10.0 seconds** while preserving the high resolution
PDF pipeline and other production settings:

| Element | Current setting |
| --- | --- |
| Format | Portrait 1080 × 1920, 30 fps; one channel; no title card |
| Ordering | Reports sorted by filename (chronological timestamps), every page in PDF order |
| Page hold | Seed 48; uniform draw from 0.3–1.1 seconds per page, rounded to a whole frame |
| Black interstitial | Draft-3 baseline: one before the first report and one after every report; durations rise linearly from 2.0 to 8.0 seconds. V2 target: 2.0 to 10.0 seconds across all reports |
| PDF raster | Ghostscript `png16m`, 300 dpi, crop box, 4-bit graphics/text antialiasing |
| Page fit | Keep raster size if at most 1080 px wide; otherwise downscale with Lanczos to 1080 px; center vertically on a 1080 × 1920 canvas |
| Letterbox | Extract a chromatic fill from page 1 of each PDF via `pdftocairo -svg`; use that report's color around its pages; house-blue fallback `#173A55` |
| Audio | Mono shinkansen files `20190914-shinkansen-1.wav` (22.06 s) and `20190914-shinkansen-2.wav` (21.38 s), concatenated and repeated; each 43.44 s unit fades in and out over 3 s; trim to timeline |
| Master | MKV, HEVC `libx265 -preset slow -qp 18`, `yuv420p`, MP3 `libmp3lame -b:a 384k`; script also passes `-c:s copy` but maps no subtitle stream |
| Review artifacts | 360 × 640 silent H.264 MP4 proxy and JSON manifest with inputs, timing, locks, and hashes |

The draft-3 implementation fixes `SPACER_COUNT = 11`, which fits its
10-report input. With 20 reports, it would index past the spacer list on the
eleventh report. The v2 renderer
derives the count from the discovered PDFs (`len(reports) + 1`) and checks the
approved 20-report, 320-page inventory before rendering.

## Verified resource inventory

Counts were measured with `pdfinfo` on 2026-09-26. Accent values were extracted
with the draft-3 `accent_for_report` function; they describe the current
letterbox algorithm, not a new palette choice. Files after the August 18
report are new relative to plan v1 and the completed draft-3 render.

| Report in chronological order | Pages | Letterbox accent | In draft 3? |
| --- | ---: | --- | --- |
| `audit-usage-2026-08-06-1315-PT.pdf` | 7 | `#173A55` | Yes |
| `audit-usage-2026-08-06-1538-PT.pdf` | 11 | `#9D174D` | Yes |
| `audit-usage-2026-08-07-0930-PT.pdf` | 12 | `#15578F` | Yes |
| `audit-usage-2026-08-07-1152-PT.pdf` | 16 | `#15578F` | Yes |
| `audit-usage-2026-08-07-2329-PT.pdf` | 19 | `#15578F` | Yes |
| `audit-usage-2026-08-09-0711-PT.pdf` | 14 | `#15578F` | Yes |
| `audit-usage-2026-08-10-0838-PT.pdf` | 14 | `#FACC15` | Yes |
| `audit-usage-2026-08-11-0428-PT.pdf` | 14 | `#581C87` | Yes |
| `audit-usage-2026-08-16-1830-PT.pdf` | 19 | `#134E4A` | Yes |
| `audit-usage-2026-08-18-1657-PT.pdf` | 19 | `#134E4A` | Yes |
| `audit-usage-2026-08-24-2115-PT.pdf` | 19 | `#0F172A` | No |
| `audit-usage-2026-08-28-1841-PT.pdf` | 15 | `#581C87` | No |
| `audit-usage-2026-08-31-0845-PT.pdf` | 19 | `#7C2D12` | No |
| `audit-usage-2026-09-07-0857-PT.pdf` | 17 | `#0C4A6E` | No |
| `audit-usage-2026-09-09-2030-PT.pdf` | 17 | `#1E3A8A` | No |
| `audit-usage-2026-09-12-0907-PT.pdf` | 17 | `#134E4A` | No |
| `audit-usage-2026-09-14-1657-PT.pdf` | 17 | `#831843` | No |
| `audit-usage-2026-09-17-1140-PT.pdf` | 14 | `#14532D` | No |
| `audit-usage-2026-09-22-1955-PT.pdf` | 20 | `#713F12` | No |
| `audit-usage-2026-09-26-0858-PT.pdf` | 20 | `#15578F` | No |
| **Total** | **320** | | **145 existing + 175 new pages** |

The first report remains `audit-usage-2026-08-06-1315-PT.pdf`; the last is now
`audit-usage-2026-09-26-0858-PT.pdf`. Sort by the timestamped filenames so
new arrivals stay in chronological position.

## Updated timeline for these inputs

Retain the variable-timer pattern and extend its endpoint to 10.0 seconds
across the full set:
**21 black interstitials**, indexed `i = 0…20`, each lasting
`2.0 + 8.0 × i / 20` seconds. At 30 fps these are exact 0.4-second steps:
2.0 seconds before report 1, then 2.4 through 10.0 seconds after reports
1 through 20. They total **126.0 seconds**. There is no additional fixed
3-second opener or report spacer.

Using the existing seed, hold range, report order, and frame rounding for all
320 pages produces **6,794 page frames (226.467 s)**. The full planned
timeline is **10,574 frames (352.467 s, about 5:52.5)**. This calculation
matches the completed render manifest. The first 145 hold draws remain the
same as draft 3; appended PDFs consume the next draws. Nine plays of the
43.44-second shinkansen pair cover this duration before trimming.

## Render and review procedure

1. Discover and count the PDFs before constructing events. Derive the black
   interstitial count as `len(reports) + 1`; interpolate from 2.0 to 10.0
   seconds over that count. Keep the existing seed, page-hold algorithm,
   300 dpi Ghostscript raster, fit, accent, audio, and master encode settings.
2. Write a new render and manifest under a new output directory so the
   10-report draft-3 files remain available for comparison. Record the
   20 report names, measured page counts and accents, 320 hold values,
   21 spacer values, total frames, and output hashes.
3. Check the manifest against the inventory above; inspect the first and
   last report frames, one new report with a contrasting letterbox color,
   and the 2.0-second opener and 10.0-second final interstitial. Verify
   the master stream is 1080 × 1920 at 30 fps with audio and that the
   proxy is 360 × 640. Check a stored raster is 300 dpi, with the
   expected 2550 × 3300 geometry for a US Letter page.

## Completed v2 artifacts

The approved plan was rendered with the v2 renderer on 2026-09-26:

- HEVC/MP3 MKV master
- 360 × 640 H.264 review proxy
- Render manifest

The manifest records 20 reports, 320 pages, 21 interstitials from 2.0 to
10.0 seconds, 10,574 video frames, and nine audio-unit plays. `ffprobe`
counted **10,574 frames in both master and proxy**. The video timeline is
352.467 seconds; the MKV container extends to 352.536 seconds because of MP3
packet padding. All 320 page PNGs were present at 300 dpi (2550 × 3300 for a
US Letter page), and the output SHA-256 values matched the manifest. Sampled
frames showed black at 0.5 and 342.6 seconds, page content at 2.5 and
342.4 seconds, and black at 352.0 seconds, covering both the opener and the
final 10-second interstitial.
