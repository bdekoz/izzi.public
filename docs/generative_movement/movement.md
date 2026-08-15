# Family: movement (paths, camera, serialization)

Header: `src/izzi-svg-movement.h`.

## Terminology

- **Movement document** — one animation: viewport, total duration, a camera
  segment, and layered segments (ripple/raindrop/wave).
- **One-path** — an element or camera follows a generated SVG path from a
  start location to an end location over a duration.
- **Multi-path** — a camera track plus layered paths composed over it.
- **Path reference** — family + kind + numeric params; the path is generated
  by a `generation_pattern` family (roulette, surface tension, hamonshu, ...)
  and sampled 0..1 over the segment.

## API surface

- `enum class easing { linear, ease_in, ease_out, ease_in_out }` and
  `apply_easing(t01, ease)`.
- `struct movement_path` — family, kind, params, samples, reverse;
  `param(name, fallback)`.
- `struct movement_segment` — id, path, start, end, duration, easing.
- `struct movement_document` — viewport, duration, camera, layers.
- `generate_path_points(path, origin)` — deterministic sample points
  (roulette trochoid, hamonshu catalogue motifs via `make_motif_path`, and
  surface-tension field contours via `resolve(scene_spec)` wired; unknown
  kinds fall back to a straight line).
- `sample_path(path, start, end, t01)` / `sample_segment(segment, t)` —
  point at time, mapped onto the segment's start/end (endpoints exact).
- `emit_movement_json(doc)` — compact serialization (schema
  `izzi.movement/1`); parsing is staged.

## Config

Path family/kind/params, start/end locations, duration in seconds, easing,
sample count, viewport. Determinism: all sampling is pure; stochastic paths
use seeded generation.

## Tests and examples

- `examples/movement-asama-loops.cc` — re-creates the
  `resources/asama-loops/asama-01-roji-1.0-5px.mkv` alley pan: camera along
  a roulette trochoid (194.733 s) plus ripple/raindrop/wave layers; emits
  `movement-asama-01-roji.json` and a deterministic SVG path preview.
  Registered as `movement.asama-loops.demo` (CTest) and `make
  movement-check`.
- Header self-containment covers `izzi-svg-movement.h`.

## Usage

- Camera tracking descriptions for here-lies-trouble and the time-to-die
  seed corpus (planned `scripts/extract-movement.py`).
- Layered ripple/raindrop/wave movement for future izzi animations.

## Assessment

Status: `ASSESSED-FIRST-PASS`.

Open items (2026-08-14 movement proposal):

- ~~Wire surface-tension and hamonshu point generators~~ — **done**
  (2026-08-15): hamonshu kinds sample the real catalogue motif geometry
  (e.g., `nested-current-scrolls` → 808 points); surface-tension kinds run
  the deterministic field/contour extraction (minimal single-source scene,
  level 0.3). Per-kind scene presets (matching the plate examples) remain a
  follow-up.
- `movement_from_json` parser and SRT/subtitle timing integration.
- `scripts/extract-movement.py` for video-to-camera-movement extraction.
- Portal `generation-movement-20260814` index (gated on portal
  authorization).

## Links

- Proposal: `docs/development/sessions/explore_futures.20260814.movement.md`
- Pattern families: `docs/generative_patterns/`; color: `docs/generative_colors/`
