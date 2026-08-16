# Izzi futures — vector text path overlay (proposal)

Status: `APPROVED 2026-08-14 (user); PHASES A-D IMPLEMENTED (DRAFT-4 THROUGH DRAFT-7); WASM PARITY NOT-YET-VERIFIED`

## Decision summary

Goal (user direction, 2026-08-14): use izzi to produce vector text paths as
an overlay that can then be composited with transparency onto whatever raster
image is the source corpus. This proposal defines the izzi emission model —
resolved glyph outlines written as SVG `<path>` layers plus a lane/timing
manifest — maps the W3C Bullet Chatting (danmaku) model onto that manifest,
and shows how the same overlay feeds the ai-time-to-die T2/T3 text channels in
the next draft. No code changes this round; this document is the plan.

The key move is that text stops being baked into the raster during the video
render. izzi emits geometry only (vector paths + metadata); a generic
compositor rasterizes that geometry at any target resolution with per-run
alpha and composites it over any source-corpus frame. Proxy and canonical
renders then share one vector layer instead of two independent `drawtext`
passes.

## Why vector, why overlay

Current ai-time-to-die draft-3 text (T2/T3 terminal + OCR lines) is written by
ffmpeg `drawtext` directly into the 1080×1920 raster. Consequences:

- The proxy (360×640) and canonical renders cannot share text: each is a
  separate baked pass with its own font metrics, so legibility findings in one
  resolution do not transfer to the other.
- Alpha, color, blur, and placement are entangled with the corpus pixels;
  fixing OCR legibility or mask/overlay placement means re-rendering the whole
  segment.
- Nothing is reusable: a line shown in one segment cannot be re-composited
  over a different corpus frame without a full re-render.

A vector overlay fixes all three: resolution independence, per-run alpha and
styling, and re-composition over any raster frame from the same SVG/manifest
without re-shaping or re-rendering the segment.

## Current-state audit (what already exists)

- izzi is a header-only C++ SVG emission library. Text surfaces today are
  `text_element` (`<text>`) and `text_path_element` (`<textPath>`) in
  `src/izzi-svg-elements.h`; both defer to the consuming renderer's font
  handling and emit no glyph outlines.
- No font parsing/shaping exists in core: `rg` finds no freetype, fontconfig,
  or harfbuzz usage under `src/` or `examples/`.
- The design language already requires resolved outlines for inscriptions
  (`docs/development/sessions/explore_futures.20260809.md`): "a deterministic
  inscription uses either a small versioned Izzi stroke-glyph set or
  caller-supplied resolved glyph outlines. External shaping adapters may
  produce those outlines, but the resolved plan records the original UTF-8
  string, glyph IDs, outline hash, font-asset hash and license, shaping engine
  and version, writing direction, and layout transform. Missing glyphs and
  native/WebAssembly outline mismatches are errors, not font substitutions."
  This proposal implements that rule as a general text-overlay surface.
- ai-time-to-die draft-3 renderer (`scripts/render-time-to-die-draft3.py`)
  renders terminal text with ffmpeg `drawtext` (DejaVu Sans Mono, bottom-to-top
  at one line / 0.3 s, colors from the frame palette) — a chatroom pattern,
  not a danmaku mode.

## W3C Bullet Chatting mapping

Reference: https://w3c.github.io/danmaku/ (index) and
https://w3c.github.io/danmaku/api.html (API). The proposal defines a
`bulletchatlist` display area and `bulletchat` items; the lane model is exactly
the deterministic no-overlap rule a vector overlay needs.

| W3C concept | Meaning | izzi overlay mapping |
|---|---|---|
| `bulletchatlist.area` | Percentage of display height reserved (scroll/reverse/top measured from top; bottom measured from bottom) | Frame-height reservation; lanes are packed inside it |
| `bulletchatlist.allowOverlap` | `false` = no overlapping bullets beyond the display area | Lane-based no-overlap packing; the determinism core |
| `bulletchat.mode` | `scroll` (default, left→right), `reverse` (right→left), `top`, `bottom` | Manifest mode enum for each run |
| `bulletchat-duration` | Traversal time; controls scroll speed | Uniform speed = (lane width + run width) / duration |
| `bulletchat-delay` | Start offset; negative starts mid-screen | Start-time offset in the manifest |
| `bulletchat-play-state` | `running` \| `paused`, inheritable | Compositor state; pause = hold lane position (as the API does with a zero-duration transition) |
| start/end/cancel events | Animation lifecycle | Timeline cut points for the compositor |

Notable divergence recorded here: the draft-3 bottom-up terminal scroll is a
chatroom pattern; the izzi-native generator adopts the W3C modes
(`scroll`/`reverse`/`top`/`bottom`) so text behavior is specified, uniform,
and deterministic.

## Target design

### A. Outline resolution (two routes)

1. **Versioned stroke-glyph set (built-in).** A small deterministic izzi glyph
   set for short decorative runs (title cards, labels). Missing glyph is a
   hard error, per the design rule.
2. **Caller-supplied resolved outlines.** An external shaping adapter (e.g.
   fontTools, installed 4.61.0; or FreeType/harfbuzz at build time) extracts
   glyph outlines from a licensed font (DejaVu Sans Mono for OCR/terminal
   text). Each resolved run records UTF-8 source, glyph IDs, outline hash,
   font-asset hash + license, shaper + version, direction, and layout
   transform.

### B. Emission (new izzi surface)

New `src/izzi-svg-text-overlay.h` (optionally extending the guilloche
inscription writer) consumes resolved outlines plus layout (mode, lane, start,
duration, alpha curve, color, z-order) and emits:

- **Overlay SVG**: transparent background, `viewBox` matching the frame (e.g.
  1080×1920), one `<g>` per run with `transform`, `fill`, `fill-opacity`,
  `stroke`/`stroke-width`; each glyph is a real `<path d="...">` outline, not a
  `<text>` element. The overlay is therefore resolution-independent and its
  only non-transparent pixels are the text geometry.
- **Manifest JSON**: per-run id, source string, mode, lane, start/duration/
  delay, alpha curve, color, z-order, glyph-set hash, outline hash, and the
  lane-packing seed.

Native C++ and WebAssembly must produce the same normalized document, matching
the existing requirement that both resolve identical SVG.

### C. Compositing (new script)

`scripts/compose-vector-overlay.py`: rasterize the overlay SVG at the target
resolution (cairosvg installed; inkscape and headless Chrome as fallbacks),
then composite with Pillow `alpha_composite` over any raster corpus frame, or
via ffmpeg `overlay` for motion. Scrolling runs use a path atlas (run-local
coordinates) plus manifest; the compositor applies translation and alpha per
frame, so izzi stays pure geometry. One vector layer serves both the 360×640
proxy and the 1080×1920 canonical render.

### D. Verification gates

- Determinism: same corpus + seed → byte-identical SVG and manifest.
- No-overlap: lane/time audit over the manifest.
- Transparency: outside glyph bounds alpha == 0; per-run `fill-opacity`
  respected; composite diff check.
- Legibility: contrast check of text against the underlying corpus frame at
  both resolutions.
- Missing glyph or shaper mismatch: hard error naming the run and character.

## ai-time-to-die integration (draft-4 candidate, gated on G4)

- T3: `danmu-text-dyads` lines become W3C-mode lanes (`scroll`/`reverse`/`top`/
  `bottom`) as a vector overlay over the video frames; OCR dyad lines use
  `bottom`/`top` or `reverse` lanes.
- T2: terminal lines become vector overlay runs from DejaVu outlines through
  the same compositor.
- Publication flow is unchanged (filmstrip portal, validator).
- No provider spend; this proposal is the input to the draft-4 plan once the
  G4 human review of draft 3 returns a decision.

## Staging (when approved)

### Phase A — Tooling audit and route choice

- Record versions: fontTools 4.61.0, cairosvg, Pillow 11.3.0, inkscape
  present; `rsvg-convert`/`resvg` absent (fallbacks noted).
- Choose outline route per channel: DejaVu outlines via fontTools for
  OCR/terminal; built-in stroke set for short decorative runs.

### Phase B — Emission unit

- Add `src/izzi-svg-text-overlay.h` and `examples/text-overlay.cc` emitting
  overlay SVG + manifest; determinism test (byte-identical output).

### Phase C — Compositor and proof

- Add `scripts/compose-vector-overlay.py`; produce a sample proof: one corpus
  frame + eight danmaku runs composited at 360×640 and 1080×1920, with
  alpha/no-overlap checks passing.

### Phase D — Draft-4 integration decision

- After G4 returns, decide whether draft 4 uses the overlay for T2/T3; then
  write the draft-4 plan against this proposal.

## Files touched (proposed)

- New: `docs/development/vector-text-overlay-proposal.md` (this file),
  `src/izzi-svg-text-overlay.h`, `examples/text-overlay.cc`,
  `scripts/compose-vector-overlay.py`.
- Proof outputs under `outputs/review/feedback/visual/` (gitignored).

## Implementation record (2026-08-14, phases A-C)

User accepted the plan and directed the outline source to be
**AtkinsonHyperlegibleMono-SemiBold.otf** instead of DejaVu Sans Mono.  The
implemented surface:

- `src/izzi-svg-text-overlay.h`: dependency-free serializer emitting the
  transparent overlay SVG (one `<g>` per run with W3C lane/timing `data-*`
  provenance, one `<path>` per glyph outline) and the manifest JSON.  No font
  shaping in core; resolved outlines come from the external adapter, per the
  design rule.
- `examples/text-overlay.cc`: CLI (`--json RESOLVED.json --out DIR`,
  `--demo`), rapidjson parsing of the resolved-runs document, and an
  always-on determinism self-test (byte-identical re-emission).
- `scripts/compose-vector-overlay.py`: fontTools shaping adapter +
  lane packer + cairosvg/Pillow compositor.  Glyph outlines are baked into
  final pixel coordinates with a `TransformPen`, so stroke widths remain
  exact overlay pixels (a stroke on a scaled `<path>` would otherwise scale
  with the glyph).  The same vector layer is rasterized at 1080x1920 and
  360x640 and alpha-composited over the source-corpus frame.
- Font: `/home/bkoz/.fonts/atkinson_hyperlegible.2026/AtkinsonHyperlegibleMono-SemiBold.otf`
  (sha256 `8ac9187458bef3e69b6700446ffc2721c44f7143ee1b08648fa2456075750dff`),
  license recorded from the font name table (Braille Institute, free use, no
  attribution required).

Proof run (`outputs/review/feedback/visual/ai-time-to-die/vector-text-overlay-proof/`):
12 runs (3 danmaku `scroll`/`reverse`, 9 OCR `top`/`bottom`) over a Blade
Runner cut-4 corpus frame, seeded and lane-packed with no overlap; all five
verification gates PASS in `verify.json` (determinism, no-overlap,
transparency, legibility, missing-glyph).  Artifact hashes in
`render-notes.json`.

Deferred:
- WASM byte-parity is `NOT-YET-VERIFIED`; the header is dependency-free and
  trivially portable, and parity should be checked via `compile-wasm.sh` when
  the WASM surface lands.

## Draft-4 implementation record (2026-08-14)

Phase D landed in the ai-time-to-die draft-4 attempted render
(`scripts/render-time-to-die-draft4.py`):

- T2/T3: the codex-card text channels are now izzi vector overlays —
  Atkinson Hyperlegible Mono SemiBold outlines (fontTools adapter), W3C
  Bullet Chatting modes (`scroll`/`reverse`/`top`/`bottom`), lane-packed
  no-overlap, one transparent overlay rasterized per frame and composited
  over the cards.  This replaces the draft-3 ffmpeg `drawtext` chatroom
  scroll.
- T5: the proposed 1080x1440 tiling scheme
  (`docs/development/seedance/visual-composition-proposed-tiles.svg`)
  is implemented as two provider frames rotated 90 degrees, cover-fitted
  into 1080x720 top/bottom halves with no stretch or distortion, letterboxed
  240 px top/bottom in the 1080x1920 page.
- T1: detector swap to MediaPipe FaceLandmarker (Tasks API, 478 landmarks)
  with OpenCV Haar per-second fallback; method recorded in
  `docs/visual_workflow/facial_landmarking_method.md`.
- Canonical: `outputs/review/feedback/visual/ai-time-to-die/draft-4/ai-time-to-die-draft-4-attempted-render.mkv`
  (57.0 s, 1080x1920@24), proxy, SHA256SUMS, and render-notes.json; verified
  bars/halves/text-presence programmatically.
- Dependencies recorded in `docs/dependencies.md` (mediapipe 1.0.0 +
  `face_landmarker.task` asset, Atkinson font, fontTools/cairosvg stack).

## Draft-5 implementation record (2026-08-14)

Per review issue #22 and user direction, the overlay layer gained per-run
**background boxes** (opaque white, rounded) so bullet text reads as red
type on white regardless of the underlying raster:

- The run model now carries `bbox`, `background`, `background_opacity`,
  `padding`, and `radius`; the emitter writes a `<rect>` behind the glyphs
  (`src/izzi-svg-text-overlay.h`, parsed by `examples/text-overlay.cc`).
- Draft-5 bullet text: red `#E60000` type on `#FFFFFF` boxes at 20 pt
  (26.67 px @96 dpi), every line scrolling (W3C scroll/reverse) at
  0.2 s stagger / 2.5 s duration for constant obvious motion.
- Draft-5 review artifact is a **full-motion video proxy** (like draft-1)
  rather than a filmstrip; the portal payload budget was raised 48 -> 64 MiB
  to host playable video reviews.
- Renderer: `scripts/render-time-to-die-draft5.py`; canonical under
  `outputs/review/feedback/visual/ai-time-to-die/draft-5/`.

## Draft-6 implementation record (2026-08-14)

Per review issue #23 KEEP-PARTS and the authorized DTLA 1080p render
(`sd2_BqG5Ao2vDEwX`, 240 credits, 10 s sample):

- Bullet text corpus: seeded-random page in [16,57] of the snapshot-dyads
  PDF, OCR'd; the section block from `BOC-???? CANONICAL-MARKER` to the rule
  or block end feeds the izzi red-on-white 20 pt scrolling bullets
  (`scripts/render-time-to-die-draft6.py`).
- T5: tiling abandoned.  The provider returned 1280x720 landscape (not true
  1920x1080), so one frame is contain-fitted and centered on pure black in
  the 1080x1920 page, per issue #23's fallback.
- Kept from draft-5: mask + audit PASS items, exact landmark transform,
  wild 50% opacity masks, A/V state machine, full-motion review video.
- Canonical under `outputs/review/feedback/visual/ai-time-to-die/draft-6/`.

## Draft-7 implementation record (2026-08-14)

Per review issue #24 REVISE:

- **Orientation fix (critical):** the glyph transform in
  `scripts/compose-vector-overlay.py` now negates the vertical scale so font
  y-up outlines render upright in SVG y-down space (the draft-4..6 text was
  upside-down/reversed).  Verified with tesseract reading the overlay text
  upright.
- Bullet lanes extend the full 1920 px height (`area_percent = 100`), and
  the T2 snapshot background is vertically fitted (height-fit, horizontal
  clipping allowed) per issue #24.
- T5 returns to exactly what draft-1 did: the here-lies-trouble pilot
  entry/hold/exit stand-in (`hlt-episode-01-pilot.mkv`, ss 20/23/25).
- Kept: wild 50% opacity masks, audit usage, red-on-white 20 pt bullets from
  the BOC CANONICAL-MARKER PDF block, A/V state machine, full-motion review.
- Renderer: `scripts/render-time-to-die-draft7.py`; canonical under
  `outputs/review/feedback/visual/ai-time-to-die/draft-7/`.
