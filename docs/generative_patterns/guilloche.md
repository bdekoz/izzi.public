# Family: guilloche

Headers: `src/izzi-svg-guilloche.h` (2107 lines),
`src/izzi-svg-guilloche-json.h` (1750 lines),
`src/izzi-svg-graph-guilloche.h` (639 lines).

## API surface

- Core: `svg::guilloche::point`, `path`, `api_version`;
  enums `render_profile`, `closure_mode`, `roulette_kind`,
  `composition_kind`, `blend_mode`, `semantic_role`,
  `glyph_ornament_kind`, `diagnostic_severity`; `diagnostic`.
- Specs: `roulette_spec`, `rose_spec`, `lissajous_spec`, `wave_spec`,
  `centerline_geometry`, scene/spec types for composition.
- JSON authoring: `scene_content_kind`, `scene_density`, `scene_content`,
  `scene_treatment`, `scene_radial_layout`, `scene_palette`, `scene_review`,
  `scene_json_context`, `scene_json_diagnostic`, `scene_json_document`;
  schema versions for authoring/resolved/profile-bundle scenes.
- Graph synthesis: `svg::graph::dual_view_style/spec/result` in
  `graph-guilloche.h`, `synthesis_api_version`.

## Config

Scene JSON authoring and resolved schemas; profiles and density controls.

## Tests and examples

- `tests/guilloche.cc`, `tests/guilloche-scene-json.cc`,
  `tests/guilloche-profile-companions.cc`, `tests/graph-json.cc`,
  `tests/graph-model.cc`, `tests/graph-guilloche-synthesis.cc`,
  `tests/guilloche-odr/{left,right}.cc`,
  `tests/package-consumer/canonical.cc`.
- Examples: `examples/guilloche-plates.cc`, `examples/guilloche-transition-frames.cc`.

## Usage

- Guilloche v4 title cards and proofs
  (`docs/visual_workflow/tool_guilloche.md`), including the madredeus
  Here Lies Trouble title card.

## Assessment

Status: `ASSESSED-FIRST-PASS`.
Open items: map scene-JSON schema versions to the review surfaces; confirm
roulette/rose/lissajous sub-generators each have example coverage.
