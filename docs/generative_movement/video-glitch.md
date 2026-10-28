# Video glitch: native vector signal studies

Headers: `src/izzi-svg-video-glitch.h` and
`src/izzi-svg-video-glitch-json.h`.

Namespace: `svg::video_glitch`. Contract: **`izzi.video-glitch/1`**.

Film effects are documented in [Film decay](film-decay.md). This page covers
the `/1` tape, television and satellite engine recipes and vector studies.
Mixed media composition uses the [screen media facade](screen-glitch.md),
whose guide explains stage ordering, source history and capabilities.

This API turns native stroke geometry into adjustable tape, television,
and satellite artifacts. Output remains SVG paths. The renderer rejects
embedded images, raster filters, and foreign objects in the vector studies.
The resulting SVG can be composited over a canonical raster movie frame.

## Signal parameter plates

`examples/video-glitch-plates.cc` generates **one plate for each family**. Every
plate holds its source scene, source time (23/30 second), and seed (20260926)
fixed. Six columns vary the main control; two rows vary a secondary control.
The first column is a clean comparison in both rows. All 84 cells have complete
JSON recipes. The six signal plates are described below; the remaining plate
is documented in the [film review guide](film-decay.md#verification-and-review-artifacts).

| Family / plate | Columns | Rows |
| --- | --- | --- |
| `vhs-generation` | recursive copy generations 0, 1, 2, 4, 8, 16 | segment dropout probability per copy 0.012 / 0.040 |
| `vhs-tracking` | tracking offset 0, 0.2, 0.4, 0.6, 0.8, 1 | band height 12% / 35% |
| `sync-loss` | frequency error 0, 0.13, 0.26, 0.39, 0.52, 0.65 Hz | line pairing 0% / 0.3%; clean column bypasses pairing |
| `ghost` | 0–5 echo paths | horizontal delay 2.5% / 6.5% of width |
| `composite-color` | transfer generations 0, 1, 2, 4, 8, 16 | decoder leakage proxy 0.25 / 0.80 |
| `satellite-threshold` | degradation proxy 0, 0.2, 0.4, 0.6, 0.8, 1 | maximum impulse density 120 / 340 |

The native source contains three algorithmic guilloche rosettes, circles, a
grid, and a dark stroke for observing bright impulses. The examples use 10%
black (`#E6E6E6`) backing. That presentation backing is separate from the
transparent output of the core API.

Gallery: seven-page vector PDF.
Individual SVGs/PNGs and cell recipes are in
`outputs/glitch-motion-studies/draft-1/plates/`.

## Copy generation is a process count

Generation zero returns the original geometry. Each subsequent call to
`copy_pass()` transforms the actual previous result. Registration error,
time-base displacement, missing segments, reduced paint opacity, and echo
spread therefore accumulate through the copy history.

The seed, effect ID, and pass number identify that pass's error. Increasing the
generation count preserves preceding passes. The test suite verifies that
generation 8 rendered directly matches generation 3 followed by passes 4–8.

Timeline position is independent of generation. The review reel repeats the
same two-second native animation at each setting, with a 60-frame hold per
setting. A numeric generation is never inferred from elapsed video time.

## Using the C++ API

```cpp
#include <izzi-svg-video-glitch.h>

namespace vg = svg::video_glitch;

vg::document doc;
doc.seed = 20260926;
vg::vhs_generation copies;
copies.generation = 8;
copies.dropout = 0.012;
doc.effects.push_back({"copy-history", "vector", {}, copies});

vg::frame_context frame;
frame.frame = 23;
frame.rate = {30, 1};

// `native_scene` is an izzi::guilloche::resolved_scene from a native recipe.
auto clean = vg::from_guilloche_lines(native_scene);
auto plan = vg::sample(doc, frame);
auto distorted = vg::apply_vector(clean, plan);
auto svg = vg::render_svg(distorted, doc.budget);
```

`scene` is also directly constructible from strokes and point arrays, so other
native families can supply their resolved paths. The guilloche adapter imports
line geometry, converts filled primitives to outlines, samples circles with a
declared chord error (default 0.15 scene units), and excludes reference
backgrounds and inscriptions.

Apply multiple `effect_spec` entries in list order. For example, tracking
followed by four copy passes records that tracking damage into the copy input.
The reverse order places a new playback error over an already copied scene.
Call the API on the selected native layer before composing it with other
layers; the current backend accepts the `vector` target.

### Timing, geometry, and paint

- `frame_context` carries an absolute frame, positive rational output rate,
  and rational source-time offset. Rates such as 24000/1001, 25, 30000/1001, and
  30 are supported without traversal-dependent random state.
- Each event has rational start/duration and optional smooth attack/release
  ramps in seconds. The interval is start-inclusive and end-exclusive.
- Scene coordinates and stroke widths use scene units. Registration, jitter,
  echo delay/spread, and tearing use fractions of scene width; band size and
  line pairing use fractions of height. Frequencies use cycles per second.
- Place the resulting scene with a uniform scale factor. Deliberate signal
  warps are explicit operations on geometry. Tracking splits at exact
  horizontal band crossings, preventing false diagonals across tears.
- SVG colors are sRGB paints; stroke opacity is in [0,1]. Cairo supplies
  premultiplied alpha for movie composition. At alpha zero the source movie
  pixels are unchanged. The core itself performs no movie color conversion.
- Event timing and seeds are independent of render order. Counter noise uses
  the named `splitmix64-counter-v1` algorithm and stable FNV-1a effect IDs.
  Pin the compiler/native binary and renderer versions for byte-level replay;
  platform math/rasterizer differences can affect floating-point serialization
  or antialiasing.

### Main controls and validation

| Parameter type | Additional controls / accepted ranges |
| --- | --- |
| `vhs_generation` | generation 0–budget; registration/jitter 0–0.05 width; dropout 0–0.5 per segment/pass; echo spread 0–0.03 width; paint loss 0–0.5 per pass |
| `vhs_tracking` | signed tracking offset −1–1; band height 0.001–1; travel −10–10 Hz; tear 0–0.5 width; noise density 0–1000 |
| `sync_loss` | frequency error −10–10 Hz; phase −10–10 cycles; blanking 0–0.5 height; line pairing 0–0.02 height |
| `ghost` | 0–8 echoes; signed delay −0.3–0.3 width; gain 0–1; drift 0–10 Hz |
| `composite_color` | generation 0–budget; registration 0–0.05 width; leakage 0–1; phase 0–30 Hz; dot pitch 0.002–0.1 width |
| `satellite_threshold` | degradation proxy 0–1 (`link_margin_proxy`); density 0–1000; streak length 0–0.3 width; recovery 0–10 Hz |

The default budget is 32 passes, 60,000 paths, 2,000,000 points, and 64 MiB SVG.
Exceeding a budget raises an error. Unsupported backends and targets are
rejected; the engine never substitutes a raster implementation.

## Serialization and commands

`emit_json(document)` writes all resolved parameter values, event timing, seed,
noise version, backend, targets, and limits. `parse_json()` rejects unknown
schemas, keys, duplicate keys, fractional generation counts, nonfinite values,
unsupported operations, and invalid ranges. Start from an emitted recipe when
editing JSON; fields are explicit and required.

```sh
make video-glitch-check
make video-glitch-review

# Just the seven parameter plates and their SVG/PDF/PNG previews:
python3 scripts/render-video-glitch.py --plates-only

# Render a transparent SVG from an edited recipe at frame 23 / 30 fps:
build/cmake/tests/izzi_video_glitch_plates --render \
  outputs/glitch-motion-studies/draft-1/plates/vhs-generation-r1-c3.json \
  23 30 1 /tmp/glitch-copy.svg

# Include a separate six-second movie/vector composition example:
python3 scripts/render-video-glitch.py \
  --integration-source outputs/ai-runner/visual/arx1-sdr-1920x800.mkv

# Inventory and measure the three supplied video references:
python3 scripts/analyze-glitch-references.py
```

The renderer produces seven 12-second chapters and an **84-second Matroska
`.mkv`, 1920×1080 progressive, 30 fps, FFV1 RGB** review master, plus a compact
H.264 MP4 proxy. The synthetic reel is silent. The optional integration example
maps existing source audio to FLAC if present, with no added sound; the current
AI Runner picture-only comparison input has no audio stream. It fits the movie
proportionally, samples it at 30 fps, and applies only the native vector overlay.
The centered 1080-square vector viewport deliberately spans the full canvas
height, including movie padding. Its alpha is composed in encoded RGB byte
space; the adapter does not perform linear-light blending.

Receipts record exact cell recipes, per-frame SVG hashes, output/input hashes,
the repeated source frame index, seed, parameter holds, geometry checks, and
decoded-pixel hashes. Family files are reused only when native binary, renderer,
recipe, and encoded-file identities match. Partial encodes do not count as
completed chapters.

Run `python3 scripts/verify-video-glitch.py` for the final technical verification.
After technical and static review are recorded as `verification.json`
and `static-review.json`, commit the implementation and run
`python3 scripts/record-video-glitch-production.py`. This verifies the recorded
artifact hashes and committed sources, writes every recipe's ordered copy-pass
history, and updates the production manifest and checksums. It retains an
explicit pending status for human playback review.

## Fidelity and current limits

These are adjustable vector interpretations of visible artifacts. Copy history
is recursive, while its echo spread uses a declared three-tap geometric
approximation to cumulative variance. Dropout selection acts on the
resolved polyline segments; source tessellation affects its smallest marks.
Tracking uses 120 normalized bands and line pairing uses 240. Noise clocks are
fixed at 30 Hz for tracking/satellite marks.

The backend does not reconstruct a physical PAL/NTSC/RF signal, implement
field-order decoding, or perform photographic pixel-channel convolution.
Composite color leakage is expressed through native paint/geometry, and
satellite link margin is an artistic proxy. White streaks appear first, with
mixed-polarity dots at higher degradation; their visibility over dark geometry
comes from the composition rather than a measured RF/luminance model.

Full signal reconstruction remains a separately scoped backend. Public movie
decoding and raster manipulation do not belong in the core header.

## References and implementation record

The [approved study and historical sources](../development/sessions/20260926.glitch-motion-studies.md)
cover the supplied VHS material, the 1954 sync discussion, 1978 multipath
report, BBC color-decoding work, and NASA's satellite threshold observations.
The local reference manifest
records file hashes and temporal measurements. The supplied VHS clip's absolute
copy numbers remain uncalibrated; the native study labels its own counts.

Build and review results are recorded in the
[praxis](../development/sessions/20260926.glitch-motion-studies_praxis.md).

## Moiré movie pass — 2026-09-29

The [moiré production record](../development/sessions/20260929.moire_glitch_motion.md)
applies the same seven-family moving-pixel adapter to all 23 plates of the
moiré/visual-illusion companion. It trims the four-second opening and
produces a 3:50 compact vertical MKV plus a clockwise horizontal MKV.
The local review and delivery index
links both files and the encoded comparisons.

`scripts/video_glitch_movie.py` accepts optional BGR paint roles in
`transform()` and `copy_pass()`, plus `recovery_frame` and `seed` in
`parameters()`. Defaults preserve the earlier adapter's pixels and controls.
The moiré pass uses named Izzi paints and recovery at local frame 289 so
the source's last eleven clear frames remain unobstructed. Channel
registration, interpolation and compression still produce derived pixel
colors. The renderer records all 6,900 frame controls and checks every
ending in both orientations. Rebuild commands, palette roles and technical
versus human-review status are in the production record.
