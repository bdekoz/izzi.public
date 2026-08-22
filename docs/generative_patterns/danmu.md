# Family: danmu (danmaku text overlay)

Header: `src/izzi-svg-text-overlay.h` (345 lines).

## Terminology

- **Canonical term: danmaku** — matches the W3C Bullet Chatting API
  Proposal (https://w3c.github.io/danmaku/api.html) and the `mode` enum in
  the header.
- **Accepted alias: danmu** — used in this family entry and in the example
  file names (`examples/text-danmu-[1-5].cc`).
- **Plain-English descriptor: bullet-chat / bullet chatting.**

All three resolve to the same pattern family; new writing should prefer
**danmaku**.

## API surface

`izzi::text_overlay` is a pure, dependency-free string serializer: it turns
caller-resolved glyph outlines (final pixel placement) into

1. an overlay SVG with a transparent background, one `<g>` per text run and
   one `<path>` per glyph outline; and
2. a manifest JSON carrying the W3C Bullet Chatting lane/timing metadata
   (mode, lane, start, duration, delay, alpha, color, z-order, and
   glyph/font provenance).

Key types and entry points:

- `enum class mode` — `scroll` (left-to-right), `reverse`
  (right-to-left), `top`, `bottom`.
- `struct glyph` — path data in font units plus final `x`/`y`/`scale`
  placement in output pixels.
- `struct run` — full layout and provenance record for one line: id,
  source, mode, lane, start, duration, delay, font_size, width, lane_y,
  fill, fill_opacity, stroke, stroke_width, glyph/font provenance, bbox,
  per-run background box, and the resolved glyph vector.
- `struct document` — viewport, `area_percent` (W3C `area`), `allow_overlap`
  (W3C `allowOverlap`), recorded lane-packing `seed`, generator identity,
  and the runs.
- `emit_overlay_svg(const document&)` and
  `emit_manifest_json(const document&)` — the two serializers.

Izzi performs no font shaping here: per the design rule in
`docs/development/sessions/20260809.md`, resolved outlines
come from a versioned stroke-glyph set or an external shaping adapter, and
every resolved run records its provenance. Because the header is pure
string serialization, native C++ and WebAssembly emit the same normalized
document from the same input.

## Relation to the W3C Bullet Chatting API

The W3C Bullet Chatting Community Group proposal
(https://w3c.github.io/danmaku/api.html) defines a `bulletchatlist`
display area and per-bullet timing/styling. The izzi manifest implements
the same lane/timing metadata and adds deterministic lane packing with a
recorded seed and glyph/font provenance on every run.

| W3C construct | Meaning | izzi mapping |
|---|---|---|
| `bulletchatlist` `area` (0–100) | % of display height reserved for bullets | `document.area_percent` |
| `bulletchatlist` `allowOverlap` | whether bullets may overlap | `document.allow_overlap` |
| `bulletchat` `mode` | scroll / top / bottom / reverse | `run.mode` |
| CSS `bulletchat-duration` | travel speed | `run.duration` |
| CSS `bulletchat-delay` | start offset; negative starts mid-lane | `run.delay` |
| CSS font / color / alpha / shadow | glyph styling | `run.font_size`, `fill`, `fill_opacity`, `stroke`, `stroke_width` |
| Events `bulletchatstart` / `bulletchatend` / `bulletchatcancel` | lifecycle | run `start` / `duration` / `delay` metadata in the manifest |

## Config

Document policy: viewport size, `area_percent` (display-area height
reserved for the danmaku field), `allow_overlap`, and the lane-packing
`seed`. Run timing: mode, lane index, start, duration, and delay (negative
delay starts a run mid-lane, per the W3C semantics). Run styling: font
size, fill and fill opacity, optional stroke, optional rounded background
box, and padding/radius.

## Tests and examples

- `examples/text-overlay.cc` — generic CLI: emits the overlay SVG +
  manifest JSON for a resolved-runs document, with a byte-identical
  determinism self-test on every invocation.
- `examples/text-danmu-[1-5].cc` — danmaku-style briefs registered with
  `make check`, each with its own determinism self-test:

| Example | Brief | Speed / size / color |
|---|---|---|
| `text-danmu-1.cc` | classic scroll wall | mid (~46 px), medium (~10 s), white |
| `text-danmu-2.cc` | dense fast lanes | small (~28 px), fast (~3.5 s), bright white/yellow |
| `text-danmu-3.cc` | large slow titles | large (~72 px), slow (~13 s), saturated colors + stroke |
| `text-danmu-4.cc` | reverse + top/bottom mix | mixed sizes, varied colors |
| `text-danmu-5.cc` | layered composite | `area`/`allow_overlap` policy, deterministic seed and z-order |

## Usage

- Emit a demo: `examples/text-danmu-1.exe --demo --out DIR` writes
  `overlay.svg` and `manifest.json` into `DIR`.
- The ai-time-to-die vertical already uses the overlay surface through
  `scripts/compose-vector-overlay.py` and
  `scripts/render-time-to-die-draft*.py` (T2/T3 bullet text).

## Assessment

Status: `ASSESSED-FIRST-PASS`.
Open items: WASM byte-parity check for the overlay header; an external
shaping adapter and font-asset provenance exercise; z-order policy beyond
run order.
