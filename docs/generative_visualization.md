# Generative visualization families

First-pass index for the generative visualization surface implemented in
`izzi/src/`. This is a pointer index only; the family entry holds the
content. The section mirrors the pattern, color, and movement sections as a
parallel tree.

## docs/generative_visualization index

| File | Content |
| --- | --- |
| `index.md` | Directory entry page mirroring this index |
| `visualization.md` | Family entry: visualization (grids, tables, line/chord/kusama graphs) |
| `kusama-and-chord.md` | Kusama and chord semantics, APIs, metadata mappings, examples, and interpretation guidance |
| `assessment_plan.md` | Assessment plan and per-family entry template |

## Family index (izzi/src)

| Family | Headers | Docs / notes |
| --- | --- | --- |
| visualization | `src/izzi-svg-visualization.h` | `docs/generative_visualization/visualization.md`; Kusama/chord guide `docs/generative_visualization/kusama-and-chord.md`; proposal `docs/development/sessions/20260814.visualization_grammar.md` |

## Visual-tester gallery

| Example | Content | Preview status |
| --- | --- | --- |
| `visualization-alpha60-includes` | line-graph + accessible table re-creating the alpha60-results `_includes` shape | LIVE — [generation-visualization-20260814](https://situationshipin.space/review/generation-visualization-20260814/) |
| `visualization-bar-graph-horizontal` | ordered OTT-vendor assignment comparison from Alpha60 metadata | CURRENT |
| `visualization-bar-graph-vertical` | 20-bin media-object size distribution based on an Alpha60 cache audit | CURRENT |
| `visualization-chord-swarm-record` | one-record age/playing-age chord from alpha60-swarm-metadata v2 | CURRENT |
| `visualization-chord-swarm-aggregate` | full-corpus or selected age/playing-age aggregate from alpha60-swarm-metadata v2 | CURRENT |
| `visualization-radial-kusama-swarm-age` | one-record age-frequency Kusama orbit from normalized swarm metadata | CURRENT |
| `visualization-radial-kusama-swarm-asian-countries` | all 27 `asian-led-global` country-boundary values over current candidate records | CURRENT |
| `visualization-radial-kusama-swarm-black-countries` | all 60 `black-led-global` country-boundary values over current candidate records | CURRENT |
| `visualization-radial-kusama-swarm-latinx-countries` | all 20 `latinx-led-global` country-boundary values over current candidate records | CURRENT |
| `visualization-radial-kusama-swarm-multi-countries` | observed actor-side countries over `multi-led-global` candidate records | CURRENT |
| `visualization-radial-kusama-swarm-white-countries` | observed actor-side countries over `white-led-global` candidate records | CURRENT |
| `visualization-radial-kusama-swarm-ethnicities` | two-panel full principal-cast ethnicity assertions for Ironheart and Altered Carbon | CURRENT |
| `visualization-render` | JSON document render CLI (drives the facade from `izzi.visualization/1`) | CURRENT |

Status: `SECTION-CURRENT-20260814; FACADE-AND-PIPELINE-IMPLEMENTED;
PORTAL-INDEX-LIVE; SHARED-VIA-PUBLIC-MIRROR`
