# Family: guilloche

Headers: `src/izzi-svg-guilloche.h` (3017 lines),
`src/izzi-svg-guilloche-json.h` (1828 lines),
`src/izzi-svg-graph-guilloche.h` (923 lines).

## API surface

- Core namespace `izzi::guilloche`: `point`, `path`, `api_version`;
  enums `render_profile`, `closure_mode`, `roulette_kind`,
  `composition_kind`, `pattern_class`, `pattern_family`, `blend_mode`,
  `semantic_role`, `glyph_ornament_kind`, `diagnostic_severity`;
  `diagnostic`; `resolved_shape_kind`.
- Centerline geometry: `roulette_spec`, `rose_spec`, `lissajous_spec`,
  `wave_spec`, `radial_ray_spec`, `harmonic_wave_spec`.
- Field/frame/overlay specs: `fish_scale_spec`, `barleycorn_spec`,
  `vignette_spec`, `medallion_spec`, `multicolor_line_spec`,
  `flinque_overlay_spec`.
- Composition machinery: `centerline_spec`, `family_spec`,
  `composition_spec`, `scene_spec` (with `nested_scenes` and
  `maximum_nesting_depth`), plates, inscriptions, motion, endpoint tone.
- JSON authoring: scene authoring/resolved/profile-bundle schemas plus an
  optional top-level `pattern {class, family}` enum object.
- Graph synthesis: `izzi::graph::dual_view_style/spec/result`,
  `graph_composite_spec`, and `resolve_dual_view_guilloche(...)` with
  optional composite backgrounds for every pattern family.

## Config

Scene JSON authoring and resolved schemas; profiles and density controls.

## Tests and examples

- Tests: `tests/guilloche.cc`, `tests/guilloche-scene-json.cc`,
  `tests/guilloche-profile-companions.cc`, `tests/guilloche-odr/*`,
  `tests/graph-guilloche-synthesis.cc`.
- Examples: `examples/guilloche-plates.cc`,
  `examples/guilloche-glyph-v3.cc`,
  `examples/guilloche-transition-frames.cc`,
  `examples/guilloche-capability-plates.cc` (nine categories, three
  parameter variations each).

## Usage

- Guilloche v4 title cards and proofs
  (`docs/visual_workflow/tool_guilloche.md`).
- Portal review: [Guilloche Capability 20260816](https://situationshipin.space/review/media/generation-guilloche-20260816/generation-guilloche-20260816-index.index.html).

## Assessment

Status: `ASSESSED-FIRST-PASS` (post-pilot).
Open items: full JSON authoring of the new composition geometries beyond
the `pattern` enum metadata; latent-image and lenticular patterns remain
deferred.
