# Family: visualization (grids, tables, graphs)

Header: `src/izzi-svg-visualization.h`.

## Terminology

- **Visualization document** — one `izzi.visualization/1` JSON: kind,
  title, description, source, stem, viewport, points, headers, rows.
- **Kinds** — grid, table, line-graph, chord-graph, kusama-graph.
- **Accessibility contract** — every emitted artifact carries `<title>`,
  `<desc>`, and `role="img"`; tables use an sr-only caption + sortable
  markup.

## API surface

- `enum class visualization_kind` and `struct visualization_document`.
- `emit_line_graph(doc, ofile)` — make_line_graph pipeline (axes/labels);
  reference example: [Andor line graph](https://alpha60-devops.github.io/alpha60-results-star-wars-universe/docs/andor.html).
- `emit_grid(doc, ofile)` — deterministic N-column rect grid.
- `emit_table_html(doc)` — accessible HTML table string.
- `emit_chord_graph(doc, ofile)` — first-pass pure bipartite layout
  (wiring the alpha60-domain `h_chord_graph` over `vumids` is open).
- `emit_kusama_graph(doc, ofile)` — radial ids via `kusama_ids_orbit_low`.
- `emit_visualization_json(doc)` — pure serialization (WASM byte-parity).
- `examples/visualization-render.cc` — JSON document → SVG/HTML CLI.

## Config

Kind, title/description/source, viewport, points, table headers/rows.
Determinism: all emitters are pure; chord/kusama layouts are seeded-free.

## Data shapes (mmrl-metadata assessment, 2026-08-14)

- `mmrl-metadata/json/<title>.json` — per-title MMRL metadata with
  `cast-lead-N-{ethnicities,nationalities,genders,sexualities,ages,z,names}`
  arrays; `data-version` fields; schemas in `json.schema/base.json.v03-v10`.
- **Kusama input** — attribute buckets/values from the cast-lead arrays;
  existing `visualization/svg/test-ages-*.svg` outputs are circle-based
  radial dot-fields (3840×1080).
- **Chord input** — no native edge lists; derive bipartite pairs
  (media object ↔ attribute value) from the cast-lead arrays.
- Alpha60-results `data/` — `*-btiha-media-objects.json` (tables),
  `*-cumulative.json` (grids), `*-week.json` (line series), `*.geojson`
  (geo grids); 554 documents produced by `build-alpha60-visualizations.py`.

## Tests and examples

- `examples/visualization-alpha60-includes.cc` — re-creates the
  alpha60-results `_includes` line-graph + table shapes;
  `visualization.alpha60-includes.demo` CTest + `make visualization-check`.
- `examples/visualization-render.cc` — JSON render CLI (rapidjson);
  built by `make visualization-check`.
- Determinism: rendering a document twice yields identical bytes; the
  generator's second run skips all 554 unchanged documents.

## Usage

- Alpha60 unique-BTIHA pipeline (`a60-cache-recache-synthesize-uniques.cc`)
  augment/meta renders (migration proposal Appendix: alpha60 → izzi).
- Alpha60-results Jekyll `_includes` regeneration (script-based, GitHub
  Actions push-path workflow).
- mmrl-metadata cast-attribute kusama/chord review surfaces.

## Assessment

Status: `ASSESSED-FIRST-PASS`.

Open items:
- ~~Wire `h_chord_graph`~~ — superseded: `emit_chord_graph` is a weighted
  circular chord layout (deterministic, WASM-safe); alpha60-domain
  `h_chord_graph` stays outside the pure izzi surface.
- ~~WASM-driver path~~ — implemented: `src.wasm/visualization-driver.cc` +
  `scripts/build-visualization-wasm.sh`; native↔WASM byte-parity verified
  **554/554**; `generate-alpha60-includes.mjs --wasm` wired.
- ~~Portal index~~ — `generation-visualization-20260814` published
  (4 members, validator PASS).
- Remaining: alpha60 producer migration (Stages A/F of the migration plan),
  baseline-candidate promotion review.

## Links

- Proposal: `docs/development/sessions/20260814.visualization_grammar.md`
- Migration: `docs/development/sessions/20260814.migration.md`
- Sibling sections: `docs/generative_patterns/`, `docs/generative_colors/`,
  `docs/generative_movement/`
