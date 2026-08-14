# Family: moire

Header: `src/izzi-svg-moire.h` (805 lines).

## API surface

- `svg::moire::point`, `path`, `api_version = "izzi-moire-1"`,
  `geometry_provenance`.
- Enums: `render_profile`, `field_kind`, `degeneration`, `blend_mode`,
  `diagnostic_severity`; `diagnostic`.
- Specs: `family_spec`, `layer_spec`, `resource_budget`.

## Config

Field families + layers with blend modes and resource budgets.

## Tests and examples

- `tests/moire.cc`.
- Package-consumer canonical test (`tests/package-consumer/canonical.cc`)
  exercises the family through the canonical include.
- Example: `examples/moire-plates.cc`.

## Usage

- Moire proofs are part of the visual experiments method
  (`docs/visual_workflow/visual_experiments_method.md`).

## Assessment

Status: `ASSESSED-FIRST-PASS`.
Open items: enumerate the supported `field_kind` values and their
parameter surfaces; confirm render-profile coverage in tests.
