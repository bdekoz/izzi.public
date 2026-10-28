# Family: visualization (grids, tables, graphs)

Header: `src/izzi-svg-visualization.h`.

See [Kusama and chord graphs](kusama-and-chord.md) for the distinction between
the generic facade and the typed swarm-metadata renderers, their data
encodings, runnable examples, and interpretation guidance.

## Terminology

- **Visualization document** — one `izzi.visualization/1` JSON: kind,
  title, description, source, stem, viewport, points, bars, bins, headers,
  rows.
- **Kinds** — grid, table, line-graph, bar-graph, bar-histogram,
  chord-graph, kusama-graph.
- **Accessibility contract** — every emitted artifact carries `<title>`,
  `<desc>`, and `role="img"`; tables use an sr-only caption + sortable
  markup.

## API surface

- `enum class visualization_kind` and `struct visualization_document`.
- `emit_line_graph(doc, ofile)` — make_line_graph pipeline (axes/labels);
  reference example: [Andor line graph](https://alpha60-devops.github.io/alpha60-results-star-wars-universe/docs/andor.html).
- `emit_bar_graph(doc, ofile)` — horizontal categorical bars from ordered
  label/value pairs.
- `emit_bar_histogram(doc, ofile)` — vertical distribution bars from resolved
  numeric interval/count bins.
- `emit_grid(doc, ofile)` — deterministic N-column rect grid.
- `emit_table_html(doc)` — accessible HTML table string.
- `emit_chord_graph(doc, ofile)` — generic weighted circular chord layout.
- `load_swarm_metadata_chord_data(metadata_directory, collection_keys)` —
  reads current alpha60-swarm-metadata v2 records and aggregates
  `identity.cast_leads[].ages` without the Alpha60 C++ runtime.
- `h_chord_graph(data, aggregate_name)` — flattened horizontal birth-age to
  playing-age chord graph over the typed swarm-metadata aggregate.
- `swarm_metadata_kusama_values(counts)` — converts normalized metadata
  counts into the existing radial Kusama value map.
- `draw_swarm_metadata_kusama(...)` — draws a reusable Kusama orbit into an
  existing SVG, allowing single-graph and multi-panel metadata views.
- `load_swarm_metadata_slice_candidate_keys(report, slice_key)` — reads the
  current H15 candidate report without depending on Alpha60 C++ code.
- `load_swarm_metadata_cast_breakdown(repository, collection_key)` — joins a
  work's complete principal-credit list to confirmed `people/*.json`
  ethnicity assertions and preserves missing assertions as an explicit bin.
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
- `examples/visualization-bar-graph-horizontal.cc` — categorical OTT-vendor
  ranking based on Alpha60's metadata assignment document.
- `examples/visualization-bar-graph-vertical.cc` — compact 20-bin version of
  an Alpha60 cache-audit media-object size histogram.
- `examples/visualization-chord-swarm-record.cc` — one canonical metadata v2
  record, defaulting to `game-of-thrones-610`.
- `examples/visualization-chord-swarm-aggregate.cc` — full-corpus metadata v2
  aggregate by default, or a selected aggregate when keys are supplied.
- `examples/visualization-radial-kusama-swarm-age.cc` — one-record age
  frequency orbit, defaulting to `succession-301`.
- `examples/visualization-radial-kusama-swarm-asian-countries.cc` — all 27
  specific countries in the `asian-led-global` boundary, including visible
  zero-observation countries; actor nationality metadata is descriptive and
  is not treated as slice-qualification evidence.
- `examples/visualization-radial-kusama-swarm-black-countries.cc` and
  `visualization-radial-kusama-swarm-latinx-countries.cc` — the same complete
  boundary-country view for the 60-country `black-led-global` and 20-country
  `latinx-led-global` definitions.
- `examples/visualization-radial-kusama-swarm-multi-countries.cc` and
  `visualization-radial-kusama-swarm-white-countries.cc` — observed actor-side
  countries for slices that do not define a finite country boundary.
- `examples/visualization-radial-kusama-swarm-ethnicities.cc` — side-by-side
  full principal-cast ethnicity assertions for `ironheart-01` and
  `altered-carbon-02`, with missing confirmed assertions kept visible.
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
- ~~Wire `h_chord_graph`~~ — implemented as a separate flattened age-chord
  surface over `izzi-swarm-metadata.h`; the generic `emit_chord_graph` remains
  the weighted circular, deterministic, WASM-safe layout.
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
