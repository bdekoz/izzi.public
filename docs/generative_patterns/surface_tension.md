# Family: surface tension

Header: `src/izzi-svg-surface-tension.h` (799 lines).

## API surface

- `svg::surface_tension::point`, `segment`, `api_version =
  "izzi-surface-tension-1"`, `geometry_provenance`.
- Enums: `render_profile`, `degeneration`, `blend_mode`,
  `diagnostic_severity`; `diagnostic`.
- Specs: `source_spec`, `contour_layer_spec`, `resource_budget`,
  `scene_spec`.

## Config

Source and contour-layer specs with degeneration handling and budgets.

## Tests and examples

- `tests/surface-tension.cc`.
- Package-consumer canonical test (`tests/package-consumer/canonical.cc`).
- Example: `examples/surface-tension-plates.cc`.

## Usage

- Surface-tension proofs are part of the visual experiments method
  (`docs/visual_workflow/visual_experiments_method.md`).

## Assessment

Status: `ASSESSED-FIRST-PASS`.
Open items: document the `degeneration` semantics and how contour layers
compose into a `scene_spec`.
