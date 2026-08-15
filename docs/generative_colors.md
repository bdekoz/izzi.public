# Generative color families

First-pass index for the generative color and palette surface implemented in
`izzi/src/`. This is a pointer index only; the family entries hold the
content. The section mirrors the structure of
[generative pattern families](generative_patterns.md) as a parallel tree.

## Document map

| File | Content |
| --- | --- |
| `docs/generative_colors/index.md` | Directory entry page mirroring this index |
| `docs/generative_colors/color.md` | Family entry: color (palettes, bands, RGB/HSV quantization, perceptual tint helpers) |
| `docs/generative_colors/assessment_plan.md` | Assessment plan and per-family entry template |
| `docs/generative_colors/images/` | Rendered previews from the parameter-space visual testers (gallery below; regenerated per the 2026-08-14 color proposal) |

## Family index (izzi/src)

| Family | Headers | Docs / notes |
| --- | --- | --- |
| color | `src/izzi-svg-color.h`, `src/izzi-svg-color-palette.h`, `src/izzi-svg-color-band.h` (grid renderer `display_color_qis` in `src/izzi-svg-render-basics.h`) | `docs/generative_colors/color.md`; proposal `docs/development/sessions/explore_futures.20260814.color.md` |

## Visual-tester gallery

Each example's parameter-space explorer renders a swatch grid or band sweep.
Previews are regenerated from current source during the color render step
(Plan C of the 2026-08-14 proposal) and land in
`docs/generative_colors/images/`; the promoted generation baseline is stale
and is not copied.

| Example | Content | Preview status |
| --- | --- | --- |
| `color-palette-1..5` | curated basic palettes (2–133 swatches) | LIVE — [palette-20260814-index](https://situationshipin.space/review/media/palette-20260814/) |
| `color-palette-source-*` (renamed from `color-palette-6-*`) | source palettes: izzi (154), jp (118), ciecam02 (73), ciecam16j70 (89), colorbrewer2 3/9 | LIVE (same index) |
| `color-band-*` | band sweeps (deterministic after the seeded-RNG fix) | LIVE (same index) |
| `color-rgb-hsv-2/3` | RGB↔HSV quantization grids (up to 266 swatches) | LIVE (same index) |
| `color-tint-perceptual-1/2` | perceptual tint exercises | LIVE (same index) |

Status: `SECTION-CURRENT-20260814; FAMILY-ENTRY-ASSESSED;
PORTAL-INDEX-LIVE; SHARED-VIA-PUBLIC-MIRROR`
