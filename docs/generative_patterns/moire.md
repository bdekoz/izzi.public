# Family: moire

Header: `src/izzi-svg-moire.h` (1698 lines).

## API surface

- `svg::moire::point`, `path`, `api_version = "izzi-moire-1"`,
  `geometry_provenance`.
- Enums: `render_profile`, `field_kind`, `degeneration`, `blend_mode`,
  `diagnostic_severity`; `diagnostic`.
- Specs: `family_spec`, `layer_spec`, `resource_budget`.

## Config

Field families + layers with blend modes and resource budgets.

## Field vocabulary

All kinds share `origin`, `count`, `samples_per_path`, `registration`, and
the wave controls `wave_amplitude`, `wave_frequency`, and `phase_step`.
Scene-level profile, degeneration, seed, blend, and budget controls apply to
all kinds.

| `field_kind` | Kind-specific parameter surface | Construction and tuning helpers |
| --- | --- | --- |
| `linear` | `spacing`, `angle`, `phase`, `extent`; optional wave controls | Direct `family_spec`; `make_m1_linear`; M3 hybrid and line-degeneration helpers |
| `concentric` | `spacing`, `radius`, `aspect_ratio`, `aspect_step`, `angle`, `phase` | Direct `family_spec`; `make_m1_concentric`; M3 hybrid and line-degeneration helpers |
| `radial` | `radius`, `extent`, `angular_span`, `angle`, `phase`, `spacing` | Direct `family_spec`; `make_m1_radial`; M3 hybrid and line-degeneration helpers |
| `dot_grid` | `cell_size`, `dot_radius`, `angle`, `phase` | `make_m2_dot_grid`; M3 hybrid and motif-grid degeneration helpers |
| `negative_positive` | `cell_size`, `dot_radius`, `inverted`, `parity`, `grid_skew`, `angle`, `phase`; jitter uses the scene seed | `make_m2_negative_positive`; M3 hybrid and motif-grid degeneration helpers |
| `square_grid` | `cell_size`, `grid_skew`, `angle`, `phase` | `make_m2_square_grid`; M3 hybrid and motif-grid degeneration helpers |
| `slanted_line_grid` | `spacing`, `angle`, `phase`, `extent`, `grid_skew`; optional wave controls | `make_m2_slanted_line_grid`; M3 hybrid and line-degeneration helpers |
| `rotated_line_grid` | `spacing`, `angle`, `phase`, `extent`, `rotation_step`; optional wave controls | `make_m2_rotated_line_grid`; M3 line-degeneration helper; no M3 hybrid beat estimate |
| `variable_grid` | `spacing`, `angle`, `phase`, `extent`, `variable_density`; optional wave controls | `make_m2_variable_grid`; M3 line-degeneration helper; no M3 hybrid beat estimate |

Helper layers are additive and leave the ordinary specification defaults
intact:

- M1 supplies `make_m1_linear`, `make_m1_concentric`, `make_m1_radial`, and
  the mode-aware `make_m1_degeneration_amount`.
- M2 supplies one `m2_*_tuning` struct and `make_m2_*` helper for each of
  its six grid kinds.
- M3 supplies `make_m3_hybrid` for pairs with an estimable beat and
  `make_m3_degeneration_amount` overloads for one or two kinds. Motif-grid
  amounts cover dot, negative-positive, and square grids; other kinds use
  line bounds.

## Tests and examples

- `tests/moire.cc`.
- Package-consumer canonical test (`tests/package-consumer/canonical.cc`)
  exercises the family through the canonical include.
- Example: `examples/moire-plates.cc`.

## Usage

- Moire proofs are part of the visual experiments method
  (`docs/visual_workflow/visual_experiments_method.md`).

## M1 tuning

M1 is an additive, opt-in tuning layer. The ordinary `family_spec` and
`scene_spec` defaults are unchanged, so existing output is unaffected unless
a caller selects an M1 helper or assigns a nonzero `aspect_step`.

- `make_m1_linear(m1_linear_tuning)` favors fine spacing, near-vertical
  orientation, restrained waves, and small phase progression between lines.
- `make_m1_concentric(m1_concentric_tuning)` favors dense ellipses and
  applies `aspect_step` per ring to move gradually between compact and open
  forms.
- `make_m1_radial(m1_radial_tuning)` favors dense rays and a slightly open
  angular aperture rather than duplicating the first ray at a full turn.
- `make_m1_degeneration_amount(mode, m1_degeneration_tuning)` selects
  explicit amounts; all displacement and glitch variation remains
  seed-driven.

### Preset defaults

| Family | Density and spacing | Geometry | Wave defaults |
| --- | --- | --- | --- |
| Linear | 172 paths, 160 samples, spacing 10.8 | angle `pi / 2`, extent 2800 | amplitude 2.6, frequency 1.4, phase step 0.008 |
| Concentric | 132 paths, 320 samples, spacing 8.6 | radius 12, aspect 0.68, aspect step 0.0012 | amplitude 1.8, frequency 2, phase step 0.006 |
| Radial | 216 paths, 128 samples, spacing 10.4 | radius 18, extent 1680, span `1.9 * pi` | amplitude 2.2, frequency 2.4, phase step 0.004 |

Degeneration amount defaults are 0 for `repeat`, 0.16 for `interfere`, 0.38
for `destabilize`, and 0.62 for `glitch`.

### Recommended exploration ranges

| Family | Recommended range |
| --- | --- |
| Linear | spacing 8-16; paired angle delta 0.015-0.10 radians; wave amplitude 0-8; frequency 0.75-4 |
| Concentric | spacing 6-12; aspect 0.55-1.25; aspect step -0.002-0.002; wave amplitude 0-6 |
| Radial | span `1.5 * pi`-`2 * pi`; radius 4-36; wave amplitude 0-6; frequency 0.75-4 |
| Degeneration | interfere 0.08-0.24; destabilize 0.25-0.48; glitch 0.48-0.72 |

The fine spacing and low-amplitude waves preserve a legible grid before
interference is introduced. Small angle, registration, spacing, and aspect
relationships create beats through mathematical superposition rather than
copied contours. All preset variation is deterministic by seed. The geometry
provenance remains `independently-authored-mathematical-field`, and rendered
metadata continues to report `archive-geometry-used=false`.

## M2 extension

M2 adds six opt-in `field_kind` values. Existing family defaults and the M1
helpers remain unchanged; an M2 kind is selected only through direct
configuration or its corresponding `make_m2_*` helper.

### Kinds and defaults

| Kind | Meaning | M2 helper defaults |
| --- | --- | --- |
| `dot_grid` | Closed dot motifs on a centered, rotatable cell lattice | 960 cells, 12 samples, cell size 28, dot radius 2.4 |
| `negative_positive` | Seed-jittered compact and expanded motifs selected by inversion and parity | 760 cells, 8 samples, cell size 32, dot radius 3.2, parity 2, skew 0.12 |
| `square_grid` | Closed square motifs on a rotatable or skewed cell lattice | 720 cells, cell size 30, skew 0 |
| `slanted_line_grid` | Parallel lines whose common slant is derived from `grid_skew` | 156 lines, 128 samples, spacing 12, skew 0.08 |
| `rotated_line_grid` | Lines with a per-line angular increment around the family center | 156 lines, 128 samples, spacing 12, rotation step 0.0007 |
| `variable_grid` | Parallel lines with spacing scaled by normalized distance from the center | 160 lines, 160 samples, spacing 11.5, density 0.45 |

The trailing `family_spec` defaults are dot radius 3, cell size 24,
`inverted = false`, parity 2, and zero grid skew, rotation step, and variable
density. These values do not participate in the original or M1 field kinds.

### Recommended ranges

| Kind | Recommended range |
| --- | --- |
| `dot_grid` | cell size 18-48; dot radius 1 to less than half the cell size; angle delta 0-0.08 radians |
| `negative_positive` | cell size 22-52; parity 1-8; skew -0.35-0.35; seed-jitter character is deterministic |
| `square_grid` | cell size 20-48; skew -0.25-0.25; paired angle delta 0.01-0.08 radians |
| `slanted_line_grid` | spacing 8-18; skew -0.25-0.25; wave amplitude 0-5 |
| `rotated_line_grid` | spacing 8-18; rotation step -0.002-0.002 radians per line |
| `variable_grid` | spacing 8-18; variable density -0.45-4; wave amplitude 0-5 |

Dot and square lattices expose cell geometry directly, while polarity uses
parity and complementary motif scale to create inversion structure. Slanted,
rotated, and variable grids separate a common shear, a per-line angle change,
and a center-to-edge density gradient. All variation remains seed-driven.
Cell-based kinds use cell size for beat estimates. Rotated and variable grids
report no beat period because they have no single global orientation or
spacing. Geometry provenance remains
`independently-authored-mathematical-field`, and rendered metadata continues
to report `archive-geometry-used=false`.

## M3 hybrid

M3 composes exactly two caller-provided `family_spec` values with
`make_m3_hybrid(first, second, m3_hybrid_tuning)`. The returned `scene_spec`
owns one canvas, background, profile, degeneration mode, seed, and resource
budget shared by both layers.

`m3_hybrid_tuning` applies registration, angle, and spacing changes
additively. Cell-based families update both `cell_size` and `spacing`,
keeping their rendered lattice and degeneration scale synchronized.

### Defaults

| Surface | Default |
| --- | --- |
| Shared scene | 1080 by 1920, background `#02030A`, experimental profile, repeat mode, amount 0, seed 0 |
| Registration | first `(-6, -2)`, second `(6, 2)` |
| Angle deltas | first -0.014, second 0.019 radians |
| Spacing deltas | first -0.12, second 0.16 canvas units |
| Layers | opacities 0.48 and 0.44; stroke widths 0.72 and 0.68; screen blend |
| Degeneration | line amounts 0.14, 0.32, 0.56; motif-grid amounts 0.06, 0.14, 0.24 |

The helper validates family parameters before and after tuning, scene and
layer identifiers, colors, dimensions, profile, mode, blend, opacity, stroke,
and budget limits. It rejects adjusted pairs whose representative spacing and
angle produce no finite positive beat estimate. Rotated-line and
variable-density fields therefore require a different kind before they can
participate in this two-family helper.

### Recommended ranges

| Parameter | Recommended range |
| --- | --- |
| Registration offset | -24 to 24 canvas units per axis |
| Angle delta | -0.12 to 0.12 radians; keep the adjusted pair distinct |
| Spacing delta | within 10 percent of each input spacing and always positive after adjustment |
| Opacity | 0.25-0.65 per layer |
| Stroke width | 0.4-1.4 canvas units |
| Blend | `screen` for luminous fields, `multiply` for light backgrounds, or `source_over` for neutral composition |
| Degeneration | line caps 0.30, 0.50, 0.72; motif-grid caps 0.14, 0.26, 0.38 for interfere, destabilize, and glitch |

`make_m3_degeneration_amount(kind, mode, tuning)` chooses a line or
motif-grid preset and clamps custom values to the corresponding mode cap. The
two-kind overload returns the more conservative amount, so mixed fields stay
within both families' bounds. Existing degeneration defaults are unchanged.

The asymmetric registration, angle, and spacing changes prevent exact
coincidence while keeping the component fields close enough for a legible
mathematical beat. Opacity and a shared blend mode expose that beat without
adding non-geometric content. All variation remains seed-driven. Geometry
provenance remains `independently-authored-mathematical-field`, and rendered
metadata continues to report `archive-geometry-used=false`.

## Assessment

Status: `ASSESSED-M4-VOCABULARY-AND-PROFILE-COVERAGE`.
Open items: none.
