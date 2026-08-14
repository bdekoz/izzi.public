# Family: radial

Headers: `src/izzi-svg-radial.h` (28 lines, aggregate include),
`src/izzi-svg-radial-base.h` (410 lines), `radial-arc.h` (198),
`radial-direction-arc-glyph.h` (129), `radial-fill-hexagon.h` (167),
`radial-kusama.h` (577).

## API surface

- `svg::radial::detail` utilities: id/value maps (`id_value_umap`,
  `value_id_ummap`), point hashing (`PointHash`, `PointEqual`), arc and
  direction-arc-glyph types, kusama-style fill composition.
- `svg::graph::radial_orbit_spec` / `radial_dataset` consumed by the graph
  module (from `graph-guilloche.h` usage).

## Config

Radial orbit/dataset specs used by graph synthesis; base types for arcs and
glyphs.

## Tests and examples

- Indirect: `tests/graph-guilloche-synthesis.cc`, `tests/graph-model.cc`
  (radial datasets feed graph synthesis), `tests/graph-json.cc`,
  `tests/moire.cc`.
- Examples: `examples/radial-text-3.cc`, `examples/radial-text-4.cc`,
  `examples/arc-labels-3.cc`.

## Usage

- Radial layouts feed the graph/guilloche synthesis path
  (`scene_radial_layout` in guilloche JSON authoring).

## Assessment

Status: `ASSESSED-FIRST-PASS`.
Open items: radial lacks a family-level public facade beyond the aggregate
include; decide whether `radial.h` should expose named entry points.
