# Visual workflow — no-removal docs reorganization proposal

Recorded: 2026-08-12 America/Los_Angeles
Status: `APPROVED-2026-08-12; FIRST-PASS-SORT-EXECUTED; NO-REMOVAL; NOT-SHARED`

This proposal organizes the visual workflow documentation the same way the
audio workflow was organized: a pointer index plus per-bucket files, first-pass
sort only, nothing removed or rewritten. No moves have been executed for this
proposal; the one file already inside `docs/visual_workflow/` is
`camera_tracking_routes.md`.

## Proposed structure

| File | Bucket |
| --- | --- |
| `docs/visual_workflow.md` | Index and map (analogous to `docs/audio_workflow.md`) |
| `docs/visual_workflow/method.md` | Visual method passes, experiments method, episode visual tests |
| `docs/visual_workflow/style_packs.md` | Style packs, style frames, aesthetic proofs |
| `docs/visual_workflow/motion_grammar.md` | Motion seeds, canary grammar, camera tracks |
| `docs/visual_workflow/camera_tracking_routes.md` | Route waypoints and derived camera tracks (exists) |
| `docs/visual_workflow/tool_seedance.md` | Provider image-to-video adapter, delivery contract, cost gate |
| `docs/visual_workflow/tool_guilloche.md` | Guilloche v4 renderer and title cards |
| `docs/visual_workflow/etl.md` | Keyframes, filmstrips, proxies, hashing, episode assembly |
| `docs/visual_workflow/proofs.md` | Review surface pattern (filmstrip + proxy pages) |
| `docs/visual_workflow/visual_workflow_experiments.md` | First-pass unclassified and experiments |

## First-pass classification

Existing material maps into the buckets as follows (pointer references only):

| Existing | Proposed bucket |
| --- | --- |
| `docs/development/explore_futures_seedance_visual_method.md` | method.md |
| `docs/visual_workflow/visual_experiments_method.md` | method.md |
| `docs/development/explore_futures_here_lies_trouble_visual_test.md` | method.md |
| `docs/development/explore_futures_here_lies_trouble_episode_01_v2.md` | method.md (+ motion_grammar.md) |
| `docs/development/explore_futures_blade_runner_motion_seeds.20260812.md` | motion_grammar.md |
| `docs/visual_workflow/camera_tracking_routes.md` | camera_tracking_routes.md |
| `scripts/render-guilloche-v4*.py`, `check/finalize-guilloche-v4.py`, `guilloche_v4_common.py`, `guilloche_v4_contours.py` | tool_guilloche.md |
| `scripts/render-interference-motion-tests.py`, `check-interference-motion-tests.py`, `check-expanded-interference-motion.py` | method.md |
| `scripts/render-here-lies-trouble-local-episode.py`, `check-visual-review.py` | etl.md |
| `examples/seedance2/*.motion-canary*.json`, provider adapter scripts | tool_seedance.md |
| `outputs/…/style-frames/*`, `…/review/*.jpg`, filmstrip + proxy artifacts | etl.md / proofs.md |
| `scripts/render-hlt-title-card-madredeus.py` | tool_guilloche.md |

## Explicitly out of scope (evidence, stays put)

- `docs/training-conversion/dyads/`, `receipts/`, `failure-loops/`,
  `training-conversion-*.jsonl` — immutable evidence, never moved by this
  reorganization.
- `docs/development/explore_futures_*.md` dated logs remain where they are; the index
  points to them until a move is individually approved.

## Approval gate

This is a proposal only. Executing a move (e.g., creating `method.md` or
`tool_seedance.md` and relocating the listed material) requires approval; the
first-pass sort and any new pointer index can be created without moving files.
