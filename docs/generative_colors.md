# Generative color families

First-pass index for the generative color and palette surface implemented in
`izzi/src/`. This is a pointer index only; the family entries hold the
content. The section mirrors the structure of
[generative pattern families](generative_patterns.md) as a parallel tree.

## docs/generative_colors index

| File | Content |
| --- | --- |
| `index.md` | Directory entry page mirroring this index |
| `color.md` | Family entry: color (palettes, bands, RGB/HSV quantization, perceptual tint helpers) |
| `assessment_plan.md` | Assessment plan and per-family entry template |
| `images/` | Rendered previews from the parameter-space visual testers (gallery below; regenerated per the 2026-08-14 color proposal) |

## Family index (izzi/src)

| Family | Headers | Docs / notes |
| --- | --- | --- |
| color | `src/izzi-svg-color.h`, `src/izzi-svg-color-palette.h`, `src/izzi-svg-color-band.h` (grid renderer `display_color_qis` in `src/izzi-svg-render-basics.h`) | `docs/generative_colors/color.md`; proposal `docs/development/sessions/20260814.color.md` |

## Visual-tester gallery

Each palette object behind the `palette_kind` selection tag in
`src/izzi-svg-color-palette.h` has an individual review page; the band, tint,
and RGB↔HSV parameter-space exercises are carried forward from the
2026-08-14 proposal. Previews are regenerated from current source during the
color render step (Plan C of the 2026-08-14 proposal) and land in
`docs/generative_colors/images/`; the prior 2026-08-14 generation baseline is
removed from the portal.

| Example | Content | Preview status |
| --- | --- | --- |
| `palette-izzi` | full default izzi palette (234 swatches) | LIVE — [palette-20260822-index](https://situationshipin.space/review/palette-20260822-index/) |
| `palette-izzi-hue` | hue-only izzi palette, black/white/gray removed (213 swatches) | LIVE (same index) |
| `palette-jp` | traditional colors of Japan (117 swatches) | LIVE (same index) |
| `palette-colorbrewer2s3s/7s/9s` | ColorBrewer 2.0 single-hue 3/7/9-class sequential ramps | LIVE (same index) |
| `palette-ciecam02` | CIECAM02 category-constrained set (72 swatches) | LIVE (same index) |
| `palette-ciecam16` | CIECAM16 palette (60 swatches) | LIVE (same index) |
| `palette-ciecam16j70` | CIECAM16 at fixed lightness J=70 (88 swatches) | LIVE (same index) |
| `palette-esri-s-bathymetry` | ESRI shallow bathymetry ramp (7 swatches) | LIVE (same index) |
| `palette-esri-m-bathymetry` | ESRI mid bathymetry ramp (11 swatches) | LIVE (same index) |
| `color-band-expand-to-larger` | three 100-swatch band sweeps (deterministic after the seeded-RNG fix) | LIVE (same index) |
| `color-tint-perceptual-1/2` | perceptual tint exercises | LIVE (same index) |
| `color-rgb-hsv-2/3` | RGB↔HSV quantization grids (up to 266 swatches) | LIVE (same index) |

Status: `SECTION-CURRENT-20260822; PALETTE-KIND-PAGES-COMPLETE;
PORTAL-INDEX-LIVE; PRIOR-20260814-BASELINE-REMOVED;
SHARED-VIA-PUBLIC-MIRROR`
