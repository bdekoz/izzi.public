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
| `assessment_plan.md` | Assessment plan and per-family entry template |

## Family index (izzi/src)

| Family | Headers | Docs / notes |
| --- | --- | --- |
| visualization | `src/izzi-svg-visualization.h` | `docs/generative_visualization/visualization.md`; proposal `docs/development/sessions/20260814.visualization_grammar.md` |

## Visual-tester gallery

| Example | Content | Preview status |
| --- | --- | --- |
| `visualization-alpha60-includes` | line-graph + accessible table re-creating the alpha60-results `_includes` shape | LIVE — [generation-visualization-20260814](https://situationshipin.space/review/media/generation-visualization-20260814/) |
| `visualization-render` | JSON document render CLI (drives the facade from `izzi.visualization/1`) | CURRENT |

Status: `SECTION-CURRENT-20260814; FACADE-AND-PIPELINE-IMPLEMENTED;
PORTAL-INDEX-LIVE; SHARED-VIA-PUBLIC-MIRROR`
