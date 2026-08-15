# Family: color (palettes, bands, quantization)

Headers: `src/izzi-svg-color.h` (901 lines), `src/izzi-svg-color-palette.h`
(704 lines), `src/izzi-svg-color-band.h` (228 lines). Grid renderer:
`display_color_qis` in `src/izzi-svg-render-basics.h`.

## Terminology

- **Palette** — a finite, ordered set of colors
  (`palette_qi<N>` / `palette_qf<N>`); canonical spelling is "palette"
  (misspelled doubled-l variants are avoided).
- **Swatch grid** — the standard render of a palette: 20×80 blocks with
  rotated 7 pt RGB labels (`display_color_qis`).
- **Color band** — a spectrum region (`colorband` = starting color + sample
  count) used to seed generation of more colors in similar hues.
- **Quantized color** — `color_qi` (RGB integrals 0–255) and `color_qf`
  (HSV floats 0–1).
- **Hypervisible** — user shorthand for **Atkinson Hyperlegible**, the
  canonical default label typography (`k::hyperl_typo` / `k::default_typo`).

## API surface

### `izzi-svg-color.h` — core types and conversions

- `enum class color` — named color registry including the WCAG gray ramp
  (`wcag_lgray` / `wcag_gray` / `wcag_dgray`).
- `struct color_qi` — RGB integrals; `from_string` / `to_string`
  (`rgb(r, g, b)`), enum conversion.
- `struct color_qf` — HSV floats; `rgb_to_hsv` / `hsv_to_rgb`,
  `to_color_qi`, `to_string` (`hsv(h, s, v)`).
- `using color_qis = std::vector<color_qi>`; `combine_color_qi`,
  `color_qf_lt` (hue sort comparator).

### `izzi-svg-color-palette.h` — palette tables and accessors

- `izzi_palette` (~150 colors: grays, WCAG ramp, named hues, dubois series),
  `izzi_hue_palette` (no gray/black/white), `jp_palette` (118),
  `colorbrewer2s3s/7s/9s_palette`, `ciecam02_palette` (73),
  `ciecam16_palette` (61), `ciecam16j70_palette` (89),
  `esri_{s,m}_bathymetry_palette` (8 / 12).
- `active_spectrum(sortbyhuep)` — currently a mutable function-local static;
  planned to become an explicit `constexpr` selection by tag.
- `random_color` ×2, `next_color`, `start_at_color` — currently
  hidden-state/`std::random_device`; planned to take explicit seeds/cursors.

### `izzi-svg-color-band.h` — band generation

- `colorband` tuple plus nine `constexpr` seeds (`cband_bw`, `cband_gray`,
  `cband_brown`, `cband_r`, `cband_o`, `cband_y`, `cband_yo`, `cband_g`,
  `cband_b`, `cband_p`).
- `make_color_band_v1` (average-two interpolation; currently
  `std::random_device`-seeded — the `M0-DETERMINISM-001` source),
  `make_color_band_v2` (deterministic HSV mutation path), `make_color_band`
  forwarding function, `next_in_color_band` (stateful per-band caches).

## Config

Palette choice, band seed (`colorband`), band size, optional hue sort, and
label typography. The default label typography converges to Atkinson
Hyperlegible (`k::default_typo`, Plan F of the 2026-08-14 proposal).

## Tests and examples

17 top-level `examples/color-*.cc` files are covered by the generation
baseline (`tests/baselines/generation/current/`) with `xmllint` and
double-run determinism evidence; CTest registration, `color-check` /
`color-review` Make targets, a vocabulary gate, and a seeded-RNG
determinism fix are planned (Plan B of the 2026-08-14 proposal):

| Example | Content | Repeatability |
| --- | --- | --- |
| `color-palette-1..5` | curated basic palettes (2–133 swatches) | STABLE |
| `color-palette-6-ciecam02` | ciecam02 73-swatch grid | STABLE (duplicate output stem planned fix) |
| `color-palette-6-ciecam16j70` | ciecam16j70 89-swatch grid | STABLE (mislabeled output planned fix) |
| `color-palette-6-colorbrewer2` | ColorBrewer 2.0 3- and 9-class grids | STABLE |
| `color-palette-6-izzi-full` | izzi palette 154-swatch, unsorted + hue-sorted | STABLE |
| `color-palette-6-jp` | jp palette 118-swatch | STABLE |
| `color-band-expand-to-larger` | 100-swatch band sweeps (p/o/r) | **VARIES** (`M0-DETERMINISM-001`) |
| `color-band-next` | functional assertion | NONVISUAL (planned move to `tests/`) |
| `color-qf-tint-perceptual-1/2` | perceptual tint exercises | STABLE |
| `color-rgb-to-hsv-1` | functional assertion | NONVISUAL (planned move to `tests/`) |
| `color-rgb-to-hsv-2/3` | RGB↔HSV grids (42 / 266 swatches) | STABLE |

## Usage

- izzi house palette and WCAG gray ramp (`color::wcag_*`).
- Guilloche scene palettes (`scene_palette` in
  `docs/generative_patterns/guilloche.md`).
- ai-time-to-die overlay colors and the frame-palette OCR path
  (`docs/development/time-to-die-ai/`).
- Color remains a cross-cutting review dimension in
  `docs/visual_workflow/visual_experiments_method.md`.

## Assessment

Status: `ASSESSED-FIRST-PASS`.

Open items (2026-08-14 proposal
[`explore_futures.20260814.color.md`](../development/sessions/explore_futures.20260814.color.md)):

- Seeded-RNG determinism fix closing `M0-DETERMINISM-001`.
- Constexpr data module (`.inc`) and `constexpr` `color_qi`/tables; explicit
  spectrum/cursor/cache state.
- Naming and output-stem cleanups; duplicate/mislabeled outputs.
- CTest registration, `color-check` / `color-review`, vocabulary gate.
- Default typography convergence to Atkinson Hyperlegible; resource
  `/home/bkoz/.fonts/atkinson_hyperlegible.2026/` (32 files, all families
  and weights).
- Fresh gallery previews under `docs/generative_colors/images/` and the
  `palette-20260814-index` portal review item.

## Links

- Proposal: `docs/development/sessions/explore_futures.20260814.color.md`
- Generation baseline: `tests/baselines/generation/current/`
- House typography direction: `docs/dependencies.md`,
  `docs/visual_workflow/visual_experiments_method.md`
