# Generative visualization families — assessment plan

Recorded: 2026-08-14 America/Los_Angeles
Status: `FIRST-PASS; FACADE-AND-PIPELINE-IMPLEMENTED; NO-REMOVAL; NOT-SHARED`

This plan assesses the generative visualization surface implemented in
`izzi/src/` and records the assessment as the family entry under
`docs/generative_visualization/`. It mirrors the pattern, color, and
movement assessment plans.

## Families

| Family | Headers | Entry |
| --- | --- | --- |
| visualization | `izzi-svg-visualization.h` | `visualization.md` |

## Assessment results (first pass, 2026-08-14)

- **API**: `visualization_kind`, `visualization_document`,
  `emit_line_graph` (make_line_graph wired), `emit_grid`,
  `emit_table_html` (accessible), `emit_chord_graph`, `emit_kusama_graph`,
  `emit_visualization_json` (schema `izzi.visualization/1`), and the
  accessibility contract (title/desc/role).
- **Data shapes**: mmrl-metadata `json/` supplies cast-lead attribute arrays
  (kusama radial dot-fields; chord = derived media↔attribute pairs);
  alpha60-results `data/*.json` + `*.geojson` supply tables/grids/line
  series (554 documents generated).
- **Pipeline**: `build-alpha60-visualizations.py` → `izzi.visualization/1`
  documents → `izzi_visualization_render` CLI → `_includes`; sha-256 skip
  and byte determinism verified (second run: 0 generated / 554 skipped).
- **Improvements approved**: accessibility contract, WASM byte-parity,
  payload bounds + sha-256 skip (visualization_grammar Response 2).
- **Automation**: alpha60-results-animation push-path workflow (script-based
  invocation) staged; izzi check-in/push and portal index gated.

Status: `FIRST-PASS-COMPLETE; PIPELINE-VERIFIED; OPEN-ITEMS-RECORDED`
