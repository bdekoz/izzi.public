# Screen media glitch

`svg::screen_glitch` is the common composition API for **film, VCR,
TV-broadcast and satellite** effects. The same native descriptors feed the
library review, its parameter studies, wild compositions and encoded reels.
Include `izzi-svg-screen-glitch.h` for C++, or
`izzi-svg-screen-glitch-json.h` for strict `izzi.screen-glitch/1` documents.

Start with the [runnable example](../../examples/screen-glitch/main.cc), the
[complete JSON recipe](../../examples/screen-glitch/recipe.json), or the
[generated API reference](../html/screen-glitch/index.html).

The facade owns ordered stages, stable stage identities, seeds, clocks,
resource limits and receipts. It delegates film material work to the film
engine, analog transfer work to the video engine, and receiver/display work to
`izzi-screen-receiver.h`. It does not require a separate public renderer for
each media category. Category names describe the origin of a fault; stage
order describes what happens to a particular picture.

## Film and video engine guides

- [Film decay](film-decay.md) covers Decasia and Forbidden Quest material
  variants, film clocks, controls, native SVG rendering and movie adapters.
  It also documents the [historical `film-damage` vector effect](film-decay.md#six-presets-and-their-controls).
- [Video glitch](video-glitch.md) covers tape copying, tracking, sync loss,
  ghosting, composite color and satellite-threshold vector effects, with
  controls, C++ usage, JSON recipes and parameter studies.

For composition here, use a `film-material` stage with a
`svg::video_glitch::film::document`, or an `analog-chain` stage with a
`svg::video_glitch::document`. The film-material engine uses
`izzi.video-glitch/2`; the analog chain uses `izzi.video-glitch/1`, including
the historical `film-damage` effect. Both are nested inside the common
`izzi.screen-glitch/1` recipe.

## Compose native stages

Izzi requires C++20. Both screen headers currently include the engines' JSON
headers, so RapidJSON is required even for the typed composition interface.
From the repository root on a system with RapidJSON in `/usr/include`:

```sh
mkdir -p build/screen-glitch-docs
g++ -std=c++20 -I src -I/usr/include/rapidjson \
  examples/screen-glitch/main.cc -o build/screen-glitch-docs/example
build/screen-glitch-docs/example build/screen-glitch-docs/output
```

The executable saves `still.svg`, `recipe.json`, `still.receipt.json`,
`current.svg`, `moving.svg`, `moving.recipe.json` and `moving.receipt.json`.
Open the SVGs in an SVG-capable viewer. Its optional second argument reads
a complete recipe:

```sh
build/screen-glitch-docs/example build/screen-glitch-docs/from-json \
  examples/screen-glitch/recipe.json
```

For installed consumers, follow [Installing and consuming](../installing.md)
and link `izzi::izzi`; include the screen header explicitly. The minimal
stationary example is:

```cpp
#include <izzi-svg-screen-glitch-json.h>
#include <fstream>

namespace sg = svg::screen_glitch;
int main()
{
  sg::document recipe;
  recipe.seed = 20260930;
  recipe.source_identity = "my-source-content-hash";

  sg::stage reception;
  reception.id = "antenna";
  reception.family = "tv-broadcast";
  reception.operation = "analog-reception";
  reception.evidence = "inferred_mapping";
  reception.profile = "weak-signal-native-v1";
  sg::reception snow;
  snow.amount = .35;
  reception.controls = snow;
  recipe.stages.push_back(reception);

  sg::native_source source;
  source.width = 640;
  source.height = 360;
  source.identity = recipe.source_identity;
  source.svg = R"(<svg xmlns="http://www.w3.org/2000/svg"
    viewBox="0 0 640 360"><rect width="640" height="360"
    fill="#191f45"/><circle cx="320" cy="180" r="95"
    fill="#ff1493" fill-opacity=".7"/></svg>)";

  auto plan = sg::plan(recipe, {90, {30, 1}});
  auto result = sg::render_native(plan, source);
  auto persisted_recipe = sg::emit_json(recipe);
  auto receipt = sg::emit_receipt_json(result);
  std::ofstream("screen.svg") << result.svg;
  std::ofstream("screen.json") << persisted_recipe;
  std::ofstream("screen.receipt.json") << receipt;
}
```

`{90, {30, 1}}` means frame 90 at 30 fps: presentation time is 3 seconds.
`frame_context` also accepts a rational presentation offset as its third
member. Pass the actual width and height to `plan()` when your source differs
from the default 640 × 360; use the same domain for all historical samples.

The recipe vector is the processing order. A receiver followed by film damage
differs from film damage followed by receiver concealment. Seeds derive from
stable stage IDs, so inserting a differently named stage does not renumber
unrelated randomness. Stage clocks map presentation time by an explicit
rational scale and offset. Receipts record local time, planned source samples,
material/checkpoint identities, conversions and resource use.

For moving input, pass a `source_resolver` callback to `render_native` that
returns the source at the requested rational presentation time. Receiver concealment and
player stalls need this history. Earlier stages are evaluated at the requested
historical time before a later stage uses the picture. Receipt source requests
retain local stage/source-clock times and a clock identity; the composition
recipe retains the rational scale and offset mapping. A single supplied SVG
is a stationary input; it cannot establish how an unseen moving source looked.

Set `native_source::stationary = false` for moving artwork. The executable's
`source_at(time)` generates a moving circle. It appends a receiver configured
to hold an earlier valid picture and renders at 2.1 seconds, inside a loss
burst. The callback supplies the requested earlier circle position; preceding
snow is reevaluated at that earlier time as well. The pattern is:

```cpp
// recipe contains the ordered stages; source_at(t) returns moving artwork.
auto frame = sg::plan(recipe, {63, {30, 1}}, 640, 360);
auto current = source_at(frame.presentation);
current.stationary = false;
sg::source_resolver history = [](sg::rational time) { return source_at(time); };
auto output = sg::render_native(frame, current, history);
```

`native_source` can carry complete SVG paint, typed film paths, or typed analog
lines. Complete SVG preserves fills, strokes, alpha and nested definitions.
Multiple film exposure samples can use complete SVGs supplied by the callback.
Operations that require unavailable information fail explicitly: elastic film
path deformation requires suitable typed paths. The renderer preserves filled
drawings without reducing them to centerlines.

Complete-SVG film density uses a bounded 257-sample response table in linear
RGB, with alpha unchanged; typed film paths use the exact scalar engine
response. This bridge retains CSS paint, gradients and earlier filters. The
SVG VCR transfer uses Gaussian spread to keep successive-copy markup bounded.
These are explicit native render interpretations. Geometry-point budgets use
conservative serialized numeric-token estimates, alongside enforced stage and
byte limits; they are not measurements of decoded video memory.

## Categories and controls

The [generated control reference](screen-glitch-controls.md) lists all 38
catalog forms and their 697 controls, defaults, units, bounds and enum
choices. The [JSON catalog](screen-glitch-catalog.json) includes baseline
recipes, study definitions and capabilities. These named presets can have
narrower bounds and different defaults than the typed engine itself.

| Operation | C++ controls type | JSON `parameters` |
| --- | --- | --- |
| `film-material` | `svg::video_glitch::film::document` | Complete `izzi.video-glitch/2` document; see [Film decay](film-decay.md). |
| `analog-chain` | `svg::video_glitch::document` | Complete `izzi.video-glitch/1` document; see [Video glitch](video-glitch.md) and [Film decay](film-decay.md). |
| `analog-reception` | `sg::reception` | Complete reception fields, including `kind`. |
| `native-receiver` | `sg::digital_receiver` | Complete receiver fields, including `profile` and `policy`. |
| `player-buffer` | `sg::player_buffer` | Complete throughput, buffer and event fields. |
| `crt-shutdown` | `sg::crt_shutdown` | Complete decay, persistence and terminal black-hold fields. |
| `codec` | `sg::codec_request` | Unavailable: validation rejects real codec requests. |

Here `sg` aliases `svg::screen_glitch`. Match `operation` to the controls type;
the `family` label alone does not select the operation. Stage defaults select
`film-material`; set both fields when authoring another operation. The
[API reference](../html/screen-glitch/namespacesvg_1_1screen__glitch.html)
shows the native structs and their initializer defaults. The
[composition schema](../../schemas/izzi.screen-glitch-1.schema.json) gives
the individual native parameter bounds.

Composition JSON encodes enums as integers. Catalog overrides use their
declared string choices instead; for example, the receiver form override is
`"concealment": "hold"`, while typed receiver parameters use `"policy": 1`.

| Composition field / C++ enum | JSON integer mapping |
| --- | --- |
| reception `kind` / `reception_kind` | 0 `snow`, 1 `impulses`, 2 `cochannel`, 3 `roll`, 4 `ghost` |
| receiver `profile` / `link_profile` | 0 `continuous`, 1 `weather`, 2 `solar`, 3 `geo_obstruction`, 4 `leo_obstruction`, 5 `transport_burst` |
| receiver `policy` / `concealment` | 0 `stale_regions`, 1 `hold`, 2 `blank`, 3 `automatic` |

| Category | Forms | Important interpretation |
| --- | --- | --- |
| Film | The Decasia and Forbidden Quest material variants, scratches, dust, density response and surviving picture | Existing reference variants and their native material controls are reused. A new review layout has its own pending human review. |
| VCR | Copy generations, tracking, sync, ghost echoes, composite color, FM threshold interpretation | Generation counts describe successive transfer work. Signal-stage names do not imply that every fault originates in the tape. |
| TV-broadcast | Weak signal snow, ignition bursts, co-channel beats, roll/relock, broadcast ghosts, CRT shutdown, with shared digital transmission-error capability | Analog reception and CRT display have their own catalog constructions. Digital receiver stages use the shared TV/satellite transport construction. |
| Satellite | Picture damage, weather fade, solar interference, obstruction, transport recovery and IP player buffer drain | Native concealment is an interpretation, not actual MPEG decoding. IP buffering is distinct from satellite television reception. |

The `satellite/transport-burst-recovery` construction is shared by TV-broadcast
and satellite digital reception. Its descriptor lists both families and the
BBC/ITU/ETSI descriptions behind the mechanism. A typed `native-receiver` stage
can use `family = "tv-broadcast"`; an additional family-label alias is not a
separate example. `satellite/mpeg-picture-damage` keeps continuous impairment,
while transport bursts default to 0.55-second correlated loss windows on a
2-second cadence. `burst_spacing` and `burst_width` now make that timing
explicit. Clean gaps and a final loss window precede recovery. These are
artistic timing controls rather than measured packet or codec calibration.
Each burst uses its immediately preceding picture; earlier stages are
reevaluated at that historical source time.

The shared receiver also supports OTA decoder interpretations without a new
media family. Set `family = "tv-broadcast"`, `operation = "native-receiver"`
and a descriptive profile such as `ota-decoder-breakup-v1`. Existing `/1`
recipes remain readable: newly added receiver fields are optional on input
and default to the earlier model; emitted recipes include resolved values.

- `slice_run_blocks` groups adjacent damaged regions within one row.
- `damage_persistence_frames` sets the damage-mask lifetime; zero uses
  `checkpoint_frames`. These are model frames at the declared `frame_rate`.
- `displacement_x` and `displacement_y` shift historical fragments by fractions
  of picture width and height. `chroma_damage` mixes a cyclic RGB channel
  permutation inside damaged regions, preserving alpha; it is an artistic
  color-corruption proxy.
- Catalog `refresh_mode` maps to typed/JSON `refresh`: 0 `checkpoint`,
  1 `intra-picture` (displacement ramps then resets each prediction group),
  2 `rolling-slice` (rows clear in order during recovery).
- `concealment = "automatic"` starts with stale regions, holds after
  `hold_after` seconds of uninterrupted loss and blanks after `blank_after`.
  Zero disables a transition. `reacquire_delay` holds black at the beginning
  of recovery; after that delay, automatic concealment returns to partial
  picture repair rather than applying the loss-only hold/blank thresholds. These transition controls apply only to the automatic policy.

`burst_width` must not exceed `burst_spacing`; a nonzero `blank_after` must
not precede a nonzero `hold_after`; `reacquire_delay` must not exceed
`recovery`. The C++ parser enforces these relationships in addition to the
individual bounds in the JSON schema. All added controls participate in
recipe and render-cache identities.

Movie decoding stays at the explicit raster boundary in
`screen_receiver_movie.py`.
`ota-movie-plan.cc` obtains the control
values, receiver masks, transitions and source-history times from the native
engine. The adapter applies them to decoded movie pixels with integer
fragment translations and black outside the photographic frame. The native
SVG renderer retains subpixel translations. These are documented render
interpretations, not corrupted-MPEG decoding.

Fine decoder residue can be enabled with `detail_amount`. This revision adds
`detail_block_pixels` (2–64 pixels at a 1080p short side), `detail_chroma`,
`detail_coefficients`, `detail_slices`, and `detail_orientation` (0 horizontal,
1 vertical in the native frame). A portrait frame subsequently rotated to
landscape uses orientation 1 so the output's shards run horizontally.
`damage_persistence_frames` accepts up to 1,000 model frames in the catalog;
longer groups preserve a damaged pattern through the loss interval.

The photographic implementation in
`decoder_detail_movie.py` combines
reference-derived chroma DC distributions, block-local cosine coefficient
residues, sub-block details, displaced historical texture, unequal horizontal
runs, and thin slice fragments. It uses a declared PCG64 stream keyed by the
native seed and checkpoint. Native SVG represents these controls as grouped
colored fragment geometry and bright coefficient marks; it is a distinct
geometry interpretation. Neither backend decodes corrupted MPEG. Color
centroids come from the provided recording; no reference scene pixels are
composited into the pool footage.

The CRT controls now also include `point_duration`, `point_radius`, and
`point_intensity`. A nonzero point duration adds an explicit glowing center
spot after both axes finish collapsing, followed by its fade and black hold.
Zero retains the earlier timing. The revised review uses equal axis decay
so the picture shrinks to a point rather than lingering as a line. This is
an intentional older-CRT interpretation; the Samsung OTA reference's panel
has not been identified. Remaining beam energy after deflection stops is
covered by the [CRT spot-elimination patent](https://patents.google.com/patent/US5677730A/en).

Worker recipe examples:
[shared concealment](../../examples/screen-glitch/ota-concealment.json),
[slice damage](../../examples/screen-glitch/ota-slice-damage.json), and
[loss/reacquisition](../../examples/screen-glitch/ota-loss-reacquisition.json).
They reuse existing satellite-named constructions; typed OTA compositions
can set the stage family to `tv-broadcast`.

See the [OTA production session](../development/sessions/20261011.telemundo_ota_glitch.md)
for three additional review passes and the pool-footage production.
Loss-of-lock/reacquisition notices use Atkinson Hyperlegible Next and are
pass content in both OTA orientations. The
`portrait notice finisher`
adds them to the 4K portrait using exactly the landscape's native frame
intervals. Pass names and parameter labels remain landscape-only; the film
companion's portrait remains free of text.

The session also records two subsequent pool-footage reviews.
`render-telemundo-ota-v3.py` keeps
the 21-pass sequence and refines the nine OTA variations against reference
seconds 8–9, using their weighted color palette, overlapping unequal blocks,
and displaced horizontal bands. Its photographic shader is
`decoder_reference_movie.py`;
this review interpretation adds no catalog controls or algorithms.

`render-film-pool-cyan-review.py`
applies all 20 native film algorithms, three variations each, directly to the
same unlooped pool sequence. A `#00FFFF` multiply layer at 50% opacity follows
material composition: red is halved and green/blue are preserved in encoded
RGB. Each companion has a 4K vertical movie without pass or parameter labels
and a rotated, titled 1080p landscape movie. The current OTA portrait includes
receiver-state notices; the film portrait is free of text. The film companion
lasts 180 seconds; the OTA review lasts 189 seconds. See the session for source
receipts, build prerequisites, verification, and current local delivery links.

The discoverable worker catalog supplies current names, bounds, units,
baseline values and study definitions. `make examples-forms` rebuilds it from
the current implementation; it is not maintained as a second list in Python.
Unknown keys, repeated JSON keys, unsupported versions, invalid numbers,
invalid operation/type combinations and exceeded budgets produce errors.

`native_svg` is the implemented backend. A real codec request produces an
unavailable-capability error. This implementation does not simulate sampled RF,
decode corrupted MPEG transport, or claim a provider-specific decoder match.
The MPEG study implements native concealment and source history; low-bitrate
quantization and ringing are not implemented.
See the [evidence catalog](screen-glitch-evidence.json)
for primary engineering descriptions, provenance limits and missing recordings.
CRT timing and receiver presets are candidates for visual review, not captured
hardware measurements.

## JSON, worker rendering and receipts

The [example recipe](../../examples/screen-glitch/recipe.json) is emitted by
the C++ API. It contains the complete root fields `schema`, `seed`, `backend`,
`source_identity`, `color_policy`, `budget` and `stages`. Each stage requires
`id`, `family`, `operation`, `version`, `clock`, `evidence`, `profile` and
`parameters`. C++ defaults do not make those JSON fields optional.
Use `parse_json(text)` to load a recipe and `emit_json(recipe)` to save it.

Rationals are `[numerator, denominator]` integer pairs with a positive
denominator. Stage-local time is `presentation * clock.scale + clock.offset`;
scale must be positive and resolved stage time must be nonnegative. IDs are
unique, 1–128 characters from letters, digits, `_` and `-`. Supported values
are backend `native_svg`, color policy `preserve_srgb_alpha`, and operation
version `"1"`. Evidence is `recorded_observation`, `historical_description`,
`inferred_mapping` or `pending_reference`; it records provenance, not rendering
quality. `profile` is an authored descriptive string.

Default aggregate budgets are 300,000 paths, 4,000,000 estimated points,
100,663,296 SVG bytes (96 MiB), 64 stages, 1,000 history frames and 32 copy/wear
passes. Positive limits are required except passes, which accepts 0–256.
`checkpoint_frames` cannot exceed the history budget, and total analog
copy/wear passes cannot exceed the pass budget. No limit silently lowers quality.

The installed [JSON Schema](../../schemas/izzi.screen-glitch-1.schema.json)
checks the envelope and all receiver/display fields. It checks nested
film/analog schema tags; their engine parsers validate their full documents.
Native validation also enforces duplicate-key rejection, unique IDs,
cross-field rules and capability limits. A schema-valid recipe still needs
`parse_json()` and `plan()` validation.

The native review worker renders one named form or a complete composition.
Build it and inspect its descriptor without building the full review:

```sh
cmake -S . -B build/cmake
cmake --build build/cmake --target izzi_screen_glitch_native
build/cmake/izzi_screen_glitch_native --catalog > build/screen-glitch-catalog.json

python3 examples/screen-glitch/render-worker.py \
  build/cmake/izzi_screen_glitch_native examples/screen-glitch/worker.json \
  build/screen-glitch-docs/worker.svg --progress 0.175
```

The [worker recipe](../../examples/screen-glitch/worker.json) uses `form_id`
and active `parameters` overrides. To render a composition instead, supply
`"document": { ...complete recipe... }`; do not also supply `form_id` or
`parameters`. Optional worker fields include `seed`, `domain` (`[x,y,w,h]`),
`source_svg`, `source_sequence` and `receipt_path`. A `source_svg` path is a
stationary complete SVG. Omit both source fields to use the moving diagnostic.
Paths are relative to the worker's current directory unless specified below.

For sampled animation, the [source-sequence example](../../examples/screen-glitch/source-sequence.json)
uses `izzi.screen-source-sequence/1`, `selection: "nearest-preceding"`,
`endpoints: "hold"` and a nonempty `samples` array of
`{"time_seconds": number, "svg": "path.svg"}`. Times must be nonnegative and
strictly increasing; the sample count is bounded by `history_frames`.
SVG paths inside the sequence are relative to the sequence file. Sampling
holds the nearest preceding frame and clamps at both endpoints; it does not
interpolate frames. `source_svg` and `source_sequence` are mutually exclusive.

Worker stdin takes normalized progress values from 0 to 1. Its recipe's
`progress` field does not request a frame by itself. The normal timeline is
12 seconds, so 0.175 requests 2.1 seconds. Changing CRT `black_duration` from
its default can extend the timeline to
`max(12, onset + max(decays) + persistence + black_duration)`.
Stdout is a byte-count line followed by exactly that many SVG bytes for each
request; the Python example decodes this protocol. `receipt_path` saves the
last requested frame's receipt, overwriting that file on each request.

`emit_receipt_json(result)` produces `izzi.screen-receipt/1`. Inspect:

- `cache_identity` and `backend` for render identity and implementation version.
- `geometry` for cumulative geometry estimates and final SVG byte count.
- Stage `local_time`, `clock`, `state`, `amount` and `seed` for the evaluated fault.
- `source_requests` for historical times and weights; these are in the stage's
  local clock. The C++ callback receives converted presentation times.
- `material_frame`, `checkpoint`, `history_identity` and `buffer_seconds` for
  the applicable temporal model; unused fields can be zero or empty.
- `conversions` for typed/SVG bridges, stationary source reuse and native
  interpretations. Receipts describe processing; they do not assert visual acceptance.

| Error or symptom | Action |
| --- | --- |
| Missing/unknown/duplicate JSON field | Start from an emitted full recipe. Form overrides belong in the worker wrapper. |
| Operation does not match typed controls | Set the matching operation from the table above. |
| Moving source requires historical resolver | Set `stationary=false` and provide a callback or worker source sequence. |
| Elastic deformation needs typed paths | Supply supported `native_source::paths` via the film interface. |
| Budget exceeded | Inspect geometry/recipe size and choose explicit limits appropriate to your artwork. |
| Negative stage time | Adjust scale/offset or presentation time so the local clock remains nonnegative. |
| Real codec unavailable | Choose the native receiver interpretation explicitly. |
| Worker emits no SVG | Supply progress on stdin or use the Python example. |
| Effect is absent | Check event onset/duration/recovery and zero controls at the requested local time. |

Regenerate the public catalog reference and evidence after descriptor changes:

```sh
python3 scripts/generate-screen-glitch-docs.py --native build/cmake/izzi_screen_glitch_native
python3 scripts/generate-screen-glitch-docs.py --native build/cmake/izzi_screen_glitch_native --check
```

## Why a facade

A facade gives callers one composition and review contract while keeping
specialized engines responsible for their own mechanics. The same full source
picture, clock policy, stage ordering and failure behavior apply across media.
It also makes mixed histories explicit: a stalled receiver can hold the earlier
film-processed frame, including that frame's material state.

Separate top-level APIs would leave callers to reconcile timestamps, histories,
colors and legends. A single flat parameter structure would allow combinations
that do not describe a valid operation and make units ambiguous. A monolithic
renderer would duplicate proven film and analog machinery. Unrestricted
generic filters would hide which operations have real temporal state and which
need unavailable codec or path capabilities. Typed stages behind a shared
facade give these differences a clear place without four review pipelines.

The engine headers are available directly, and the facade composes their
versioned documents. The engine guides above document their individual
controls and rendering interfaces.

## Review and delivery

```sh
make examples-forms-pilot
make examples-forms
make examples-composition
make examples-forms-reels
make examples-composition-reels
```

The [workflow guide](../development/examples-workflows.md) describes discovery,
coverage, legends and cache receipts. Every media family uses that same
presentation. The study book contains static controls and temporal studies.
The composition book contains authored scenes and crossovers. The reel
renderer evaluates actual native frames rather than interpolating the five
proofing samples. Horizontal and vertical reels share their chapter list,
timestamps and frame count. Each native media chapter pairs the current clean
input with the processed picture: side by side horizontally, stacked
vertically. Both use the same camera and native time. The clean panel continues
to show current input while concealment can hold an earlier processed picture;
the frame trace records that historical request. Native time and encoded
presentation time are labeled separately. Technical verification and human
visual acceptance are recorded separately.
`screen-glitch-complete` and the per-media reels contain native diagnostic
comparisons. `examples-study-complete` contains native temporal chapters.
`examples-composition-complete` contains authored wild and crossover chapters.

The 2026-09-30 native edition has 38 media forms and 605 controls. Its 7:36
screen-glitch reels are rendered and verified in horizontal and vertical
orientations, along with separate film, VCR, TV-broadcast and satellite reels.
The current `make examples-forms-reels` command writes
`outputs/examples-study/reels/deliveries/screen-glitch-complete-horizontal.mkv`
and `screen-glitch-complete-vertical.mkv` in the same directory. Generated
review files are local artifacts and are excluded from the public docs mirror;
use the review commands above to build them.
The [dated implementation record](../development/sessions/20260930.meta_screen_media_glitch_api.md)
links the verified compact and vector review PDFs, the final technical
verification receipt and the pending human visual review.

The focused API reference is generated from the composition/JSON/receiver
headers. To rebuild it without publishing or committing:

```sh
cd docs
doxygen screen-glitch.doxygen
```
