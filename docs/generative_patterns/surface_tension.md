# Family: surface tension

Header: `src/izzi-svg-surface-tension.h`.

## API surface

- `izzi::surface_tension::point`, `segment`, `api_version =
  "izzi-surface-tension-1"`, `geometry_provenance`.
- Enums: `render_profile`, `degeneration`, `blend_mode`,
  `diagnostic_severity`; `diagnostic`.
- Specs: `source_spec`, `contour_layer_spec`, `resource_budget`,
  `scene_spec`.

## Config

Source and contour-layer specs with degeneration handling and budgets.

## Source and level vocabulary

A `scene_spec` combines source parameters, contour levels, and scene
controls. The `id` members identify output elements; the fields below are
the tuning vocabulary.

### `source_spec`

| Parameter | Purpose and default |
| --- | --- |
| `center` | Finite source center in canvas coordinates; defaults to `(0, 0)`. |
| `radius` | Positive base radius of the source Gaussian; defaults to `180`. |
| `strength` | Positive multiplier for the source contribution; defaults to `1`. |
| `aspect_ratio` | Positive ratio that scales the local y radius relative to `radius`; defaults to `1`. |
| `angle` | Finite source-axis rotation in radians; defaults to `0`. |

### `contour_layer_spec`

| Parameter | Purpose and default |
| --- | --- |
| `levels` | Nonempty finite base thresholds marched independently for the layer. |
| `level_offset` | Finite value added to every base threshold; defaults to `0`. |
| `registration` | Finite sampling translation subtracted from the canvas sample; defaults to `(0, 0)`. |
| `color` | Layer stroke color in `#RRGGBB` form; defaults to `#F7F4EB`. |
| `stroke_width` | Positive SVG stroke width; defaults to `1`. |
| `opacity` | SVG stroke opacity in the inclusive range `0`-`1`; defaults to `0.75`. |
| `blend` | Presentation blend mode: `source_over`, `multiply`, or `screen`; defaults to `source_over`. |

### Scene-level controls

| Control | Purpose and default |
| --- | --- |
| `grid_columns`, `grid_rows` | Marching-grid dimensions, each at least `2`; defaults to `120` columns and `212` rows. |
| `surface_exponent` | Positive multiplier in every source's exponential falloff; defaults to `1`. |
| `boundary_attraction` | Nonnegative edge contribution that decays with distance from the canvas boundary; defaults to `0`. |
| `mode`, `degeneration_amount` | Select `repeat`, `interfere`, `destabilize`, or `glitch` and an amount in `0`-`1`; defaults to `interfere` at `0.25`. |
| `seed` | Deterministic phase and random-stream input used by degeneration and additive helpers; defaults to `0`. |
| `budget` | `resource_budget` limits checked before or during contour generation and SVG rendering. |

The default `resource_budget` limits are:

| Limit | Default |
| --- | --- |
| `maximum_cells` | `2'000'000` |
| `maximum_segments` | `500'000` |
| `maximum_points` | `1'000'000` |
| `maximum_svg_bytes` | `64 * 1024 * 1024` |

### Additive S1 helper set

S1 is opt-in; these tuning types and helpers do not change base scene
defaults.

| Type or helper | One-line purpose |
| --- | --- |
| `s1_level_set_tuning` | Defines generated level spacing and companion-layer level and registration offsets. |
| `s1_source_tuning` | Defines seed-driven source radius, strength, aspect, and angle relationships. |
| `s1_surface_tuning` | Defines the S1 surface exponent and boundary attraction. |
| `s1_degeneration_tuning` | Defines mode-specific S1 degeneration amounts. |
| `s1_tuning` | Aggregates all S1 tuning families for a scene. |
| `make_s1_levels` | Generates the geometrically spaced S1 contour-level sequence. |
| `make_s1_source` | Builds one deterministically varied S1 source from a seed and source index. |
| `make_s1_contour_layer` | Builds a primary or companion S1 contour layer. |
| `make_s1_tuned_scene` | Applies S1 surface and degeneration tuning to an explicitly supplied scene. |

### Additive S2 helper set

S2 is opt-in and supplies validated deterministic source-composition
builders.

| Type or helper set | One-line purpose |
| --- | --- |
| `s2_ring_tuning`, `make_s2_ring_sources` | Configure and build a phased ring of related sources. |
| `s2_lattice_tuning`, `make_s2_lattice_sources` | Configure and build a staggered source lattice with bounded variation. |
| `s2_rotating_array_tuning`, `make_s2_rotating_array_sources` | Configure and build a rotating, wave-modulated source array. |
| `s2_object_cluster_tuning`, `make_s2_object_cluster_sources` | Configure and build a core-and-lobe object membrane with bounded jitter. |
| `s2_surface_field_tuning`, `make_s2_surface_field_sources` | Configure and build an anisotropic field sharing tilt, aspect, and a strength envelope. |
| `s2_tuning` | Aggregates all five S2 builder tuning families. |

## Degeneration semantics

For a canvas sample `p`, `sample_position` first subtracts the current layer's
registration, producing `q = p - registration`. In the formulas below, `a` is
`degeneration_amount`, `W` and `H` are the canvas dimensions, and `s` is
`seed`.

- `repeat`: `sample_position` returns `q` without further displacement.
  `sample_field` contains only the source Gaussian sum and optional boundary
  attraction; it adds no degeneration wave or random field term.
- `interfere`: `sample_position` also returns `q` without further
  displacement. `sample_field` adds
  `a * 0.08 * sin(q.x * 0.021 + 0.0002 * s) * cos(q.y * 0.017 - 0.00017 * s)`
  to the source and boundary field.
- `destabilize`: after registration, `sample_position` applies
  `q.x += a * W * 0.035 * sin(q.y * 0.012 + 0.0001 * s)` and then
  `q.y += a * H * 0.02 * cos(q.x * 0.009 - 0.00013 * s)`.
  The second expression uses the already-updated `q.x`. `sample_field`
  evaluates sources and boundary attraction at displaced `q` and adds the
  interfere wave.
- `glitch`: `sample_position` first applies the destabilize displacement,
  then computes `b = max(0, floor(q.y / 64))` and adds
  `a * W * 0.08 * symmetric_random(s ^ (b << 24))` to `q.x`.
  `sample_field` adds the interfere wave plus cell noise
  `a * 0.12 * symmetric_random(s ^ (cell_x << 32) ^ cell_y)`, where each cell
  index is `max(0, floor(q.coordinate / 48))` after all position
  displacement.

Glitch is valid only with `render_profile::experimental`; `accessible_static`
and `motion_safe` reject it with `std::invalid_argument`. All variation is
seed-driven.

## Contour-layer composition

A `scene_spec` supplies one shared source field and an ordered vector of
contour layers. Each layer is sampled and marched independently over the
scene grid.

- Each layer samples the complete source and boundary field after applying
  its own registration and the scene's degeneration mode. One layer does not
  alter the scalar samples, levels, or geometry of another layer.
- For every entry `levels[i]`, the marched and recorded contour level is
  `levels[i] + level_offset`.
- Registration is subtracted during sampling, so the corresponding
  continuous field geometry moves by positive `registration` in canvas
  coordinates. Canvas clipping and marching-grid discretization still apply
  independently per layer.
- `color`, `stroke_width`, `opacity`, and `blend` affect SVG presentation
  only; they do not affect scalar-field sampling or marching-squares
  geometry.
- Multiple layers superimpose as SVG groups in `scene.layers` order. Later
  groups paint after earlier groups, with `source_over`, `multiply`, or
  `screen` mapped to the group's blend style; there is no field feedback
  between groups.
- Resolved contours retain `layer_index`, and scene statistics aggregate
  contours and segments from all layers.

## Tests and examples

- `tests/surface-tension.cc`.
- Package-consumer canonical test (`tests/package-consumer/canonical.cc`).
- Example: `examples/surface-tension-plates.cc`.

## Usage

- Surface-tension proofs are part of the visual experiments method
  (`docs/visual_workflow/visual_experiments_method.md`).

## S1 tuning

S1 is an additive membrane-oriented preset vocabulary. Existing defaults and
the accepted ten-plate generation path remain unchanged until an S1 helper is
used.

### Defaults

| Family | S1 default |
| --- | --- |
| Level sets | first level `0.12`, spacing `0.055`, growth `1.045`, count `18` |
| Companion layer | level offset `0.014`, registration `(3, -2)` |
| Source radius and strength | radius scale `1.08` with spread `0.12`; strength `0.94` with spread `0.14` |
| Source aspect and angle | alternating aspects `1.55` and `0.68`; base `-0.18`, step `0.62`, jitter `0.11` radians |
| Surface | exponent `0.82` |
| Boundary | attraction `0.16` |
| Degeneration | interfere `0.14`, destabilize `0.36`, glitch `0.52` |

### Ranges and rationale

- Keep first levels around `0.06`-`0.30`, spacing around `0.025`-`0.12`,
  growth around `1.0`-`1.1`, and counts around `12`-`28` for dense but
  legible bands.
- Radius scales of `0.8`-`1.35`, strengths of `0.70`-`1.15`, aspects of
  `0.45`-`2.1`, and related angle steps of `0.35`-`1.05` radians support
  elongated folds.
- Surface exponents of `0.65`-`1.25` and boundary attraction of `0`-`0.35`
  move between broad connected membranes and tighter isolated envelopes.
- Interfere amounts of `0.08`-`0.24`, destabilize amounts of `0.22`-`0.48`,
  and glitch amounts of `0.38`-`0.64` retain structure while increasing
  texture.
- All S1 source variation is derived from the scene seed and source index;
  no time, pointer, or environment state participates in parameter variation.
- `surface-tension-plates --s1 OUTPUT-DIRECTORY` selects the additive
  series; `--list` intentionally continues to report only the ten accepted
  v1 names.

## S2 extension

S2 adds source-composition builders without changing `scene_spec`, existing
defaults, the accepted ten plates, or the three S1 plates. Builders return
ordinary `source_spec` vectors and validate their complete tuning input.

### Defaults

| Builder | S2 defaults |
| --- | --- |
| Ring | `10` sources, orbit `310`, radius `150`, phase `0.18`, strength `0.82`, aspect `1.28` |
| Lattice | `4` by `6`, spacing `(240, 250)`, radius `155`, stagger `0.5`, strength `0.74`, aspect `1.35`, tilt `0.28` |
| Rotating array | `13` sources, orbit `300` with wave `90`, radius `145`, step `2.399963229728653`, strength `0.78`, aspect `1.55` |
| Object cluster | `6` lobes plus core, cluster radius `220`, jitter bound `26`, lobe radius `205`, core radius `260` |
| Surface field | `4` by `7`, spacing `(260, 240)`, radius `220`, shared tilt `0.42`, shared aspect `1.85`, edge strength ratio `0.62` |

### Ranges and rationale

- Ring counts of `5`-`24`, orbit radii of `180`-`440`, and phase jitter of
  `0`-`0.16` support circular repetition without requiring identical sources.
- Lattices commonly use `2`-`10` columns and rows, spacing of `140`-`320`,
  stagger phases of `0`-`0.75`, and positional jitter fractions of
  `0`-`0.18`.
- Rotating arrays commonly use `7`-`28` sources, orbit waves below the base
  orbit, rotation steps of `0.8`-`2.8`, and orientation turns of `0.2`-`0.8`.
- Object clusters use `3`-`12` lobes, cluster radii of `120`-`360`, and
  positional jitter of `0`-`48`; jitter is an absolute, deterministic bound.
- Surface fields commonly use shared aspects of `1.2`-`2.6`, shared tilts of
  `-0.9`-`0.9` radians, edge strength ratios of `0.4`-`1`, and strength
  spread below `0.18`. A shared anisotropy makes separate sources read as
  one field.
- Radius, phase, position, strength, and aspect variation uses only the
  scene seed, source index, and fixed stream identifiers. No time, pointer,
  or environment state participates.
- `surface-tension-plates --s2 OUTPUT-DIRECTORY` emits the additive S2
  series. The default path, `--s1`, and the exact ten-name `--list` output
  are unchanged.

## Assessment

Status: `ASSESSED-ENUMERATED`.
Open items: none.
