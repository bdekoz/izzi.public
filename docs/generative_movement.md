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
| [screen-glitch.md](generative_movement/screen-glitch.md) | Ordered film/VCR/TV/satellite composition, controls, JSON, source history and review |
| [film-decay.md](generative_movement/film-decay.md) | Decasia and Forbidden Quest materials, film-damage vectors, clocks, controls and review |
| [video-glitch.md](generative_movement/video-glitch.md) | Tape/TV/satellite vector effects, recursive copy passes, parameter plates and review video |
| `assessment_plan.md` | Assessment plan and per-family entry template |

## Family index (izzi/src)

| Family | Headers | Docs / notes |
| --- | --- | --- |
| movement | `src/izzi-svg-movement.h` (uses `izzi-svg-curves-roulette.h`) | `docs/generative_movement/movement.md`; proposal `docs/development/sessions/20260814.movement.md` |
| screen media glitch | `izzi-svg-screen-glitch.h`, `izzi-svg-screen-glitch-json.h`, `izzi-screen-receiver.h` | [Composition API and examples](generative_movement/screen-glitch.md) |
| film decay | `izzi-svg-film-decay.h`, `izzi-svg-film-decay-json.h`; `izzi-svg-video-glitch.h` for `/1` film damage | [Film APIs, controls and studies](generative_movement/film-decay.md) |
| video glitch | `izzi-svg-video-glitch.h`, `izzi-svg-video-glitch-json.h` | [Signal API and parameter plates](generative_movement/video-glitch.md) |

## Visual-tester gallery

| Example | Content | Preview status |
| --- | --- | --- |
| `movement-asama-loops` | camera track along a roulette trochoid + ripple/raindrop/wave layers; JSON + SVG path preview | LIVE — [generation-movement-20260814](https://situationshipin.space/review/generation-movement-20260814/) |

Index updated 2026-10-01. Local documentation and deployed public-site state
are verified separately.
