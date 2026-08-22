# Generative movement families

First-pass index for the generative movement surface implemented in
`izzi/src/`. This is a pointer index only; the family entry holds the
content. The section mirrors [generative pattern families](generative_patterns.md)
and [generative color families](generative_colors.md) as a parallel tree.

## docs/generative_movement index

| File | Content |
| --- | --- |
| `index.md` | Directory entry page mirroring this index |
| `movement.md` | Family entry: movement (one-path, multi-path, serialization) |
| `assessment_plan.md` | Assessment plan and per-family entry template |

## Family index (izzi/src)

| Family | Headers | Docs / notes |
| --- | --- | --- |
| movement | `src/izzi-svg-movement.h` (uses `izzi-svg-curves-roulette.h`) | `docs/generative_movement/movement.md`; proposal `docs/development/sessions/20260814.movement.md` |

## Visual-tester gallery

| Example | Content | Preview status |
| --- | --- | --- |
| `movement-asama-loops` | camera track along a roulette trochoid + ripple/raindrop/wave layers; JSON + SVG path preview | LIVE — [generation-movement-20260814](https://situationshipin.space/review/media/generation-movement-20260814/) |

Status: `SECTION-CURRENT-20260814; API-EXAMPLE-IMPLEMENTED;
PORTAL-INDEX-LIVE; SHARED-VIA-PUBLIC-MIRROR`
