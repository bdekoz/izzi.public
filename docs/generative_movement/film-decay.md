# Film glitch

For common film/VCR/TV/satellite composition and the current library review,
start with the [screen media facade](screen-glitch.md). This page describes the
film engine and its source-matched material work.

The film API brings six Decasia sources and *The Forbidden Quest* together in
one material program, native SVG renderer and hybrid movie adapter. The
`film-field-v2` route has seven source families, seventeen base variants and
three blister mixtures. It preserves filled/stroked artwork and source clips,
applies density in linear light, and separates material time from source and
presentation time. New variants leave artistic polarity/displacement/stretch
extensions disabled by default.

The historical six-family `film-field-v1` route retains its original recipes,
defaults and rendering. Its minimal polarity/displacement/stretch extensions
remain enabled as before. Both routes use `izzi.video-glitch/2`; each recipe
records its material algorithm explicitly. The older `izzi.video-glitch/1`
`film-damage` effect retains its historical meaning; its controls and vector
study are documented alongside the material engine below. The
[video-glitch guide](video-glitch.md) covers the `/1` tape,
television and satellite effects that can accompany it.

This is procedural artwork, not a reconstruction of a film frame or a chemical
simulation. The [reference analysis and implementation record](../development/sessions/20260929.decasia-nitrate-silent-decay.md)
contains the six source intervals, measured transfer cadence, and visual rationale.
The [production atlas](../development/sessions/20260929.decasia-nitrate-silent-decay.production.svg)
contains one baseline and five isolated parameter changes for each pattern.

The [Decasia 2 plan and delivery record](../development/sessions/20260930.decasia-2.md)
adds source 7, sub-source analysis, controlled/temporal grids, review PDF and
compact MKV chapters. The complete horizontal reel received a human **high
pass** on 2026-09-30; the [review record](../development/sessions/20260930.decasia-2.human-review.json)
identifies the exact file. Separate source-reference and vertical-presentation
reviews remain pending; density flutter's paleness retains its accepted treatment.
The [screen media facade](screen-glitch.md) groups this film work with VCR,
TV-broadcast and satellite effects while keeping the versioned film engine
available directly. Its [design record](../development/sessions/20260930.meta_screen_media_glitch_api.md#why-a-facade-fits-this-project)
explains the composition tradeoffs.

## Named film variants

| Source / public family | Named presets | Reference behavior |
| --- | --- | --- |
| Decasia 1 / `emulsion_bloom` | `pale-cavities`, `gray-membranes`, `cellular-foam`; mixtures `sparse-cavities-membranes`, `membranes-foam-lace`, `full-blister-mixture` | Unequal large layouts, asymmetric rims, picture-bearing membranes and boundary-linked foam/lace. Two-tick holds at 24000/1001. |
| Decasia 2 / `density_tide` | `broad-veil`, `scalloped-density-front` | Translucent source response and localized fronts with boundary activity. Three-tick holds. |
| Decasia 3 / `density_flutter` | `pale-regional-veil`, `relief-trace` | Accepted paleness, regional suppression, source relief and separately persistent fine abrasion. Three-tick holds. |
| Decasia 4 / `emulsion_ribbon` | `abrasion-field`, `thin-tear`, `torn-sheet` | Fine marks, intermittent unequal strips and sheets, independently visible source fragments and quiet states. Alternating four/two-tick holds. |
| Decasia 5 / `survivor_islands` | `connected-survivor-window`, `fragmented-survivor-islands` | Retained picture topology, bridges, holes and bright perimeter structure. Three-tick holds. |
| Decasia 6 / `edge_curtain` | `soft-advancing-front`, `wandering-aperture` | Soft asymmetric fronts, moving openings and closure/reopening. Two-tick holds. |
| Forbidden Quest 7 / `print_erosion` | `fine-flecks`, `regional-fading`, `perforated-picture` | Fine persistent marks, local veil and perforated surviving picture, including their coexistence. Two-tick holds on its own **25 fps** clock. |

Forbidden Quest is not a third-party rendering stack or a renamed generic
film-wear overlay. It shares abrasion, density, boundary cells/lace and indexed
layout components with the Decasia forms. Its full minute contains 1,500
decoded frames and a 1396 × 1080 picture beginning near x266. Strong perforated
loss already occurs in the first few seconds; the forms do not follow a
single increasing-age sequence. The source analysis records five review
intervals, actual PTS and original-film timecodes.

```sh
# Explicit v2 selection; original numeric --preset commands remain v1.
build/film-decay-release/tests/izzi_film_decay_render \
  --variant full-blister-mixture outputs/film/blister-v2.json
build/film-decay-release/tests/izzi_film_decay_render \
  --demo outputs/film/blister-v2.json 196 24000/1001 728 540 stationary outputs/film/blister-v2.svg

# Discover names, then inspect a variant's exact defaults, units and bounds.
build/film-decay-release/tests/izzi_film_decay_render --variants outputs/film/variants.json
build/film-decay-release/tests/izzi_film_decay_render \
  --describe-variant perforated-picture outputs/film/perforated-controls.json
build/film-decay-release/tests/izzi_film_decay_render \
  --variant perforated-picture outputs/film/quest.json
build/film-decay-release/tests/izzi_film_decay_render \
  --demo outputs/film/quest.json 1000 25 728 540 stationary outputs/film/quest.svg
```

For one native example from every source family, use:

```sh
mkdir -p outputs/film/seven
for variant in full-blister-mixture broad-veil pale-regional-veil thin-tear \
  connected-survivor-window soft-advancing-front perforated-picture; do
  build/film-decay-release/tests/izzi_film_decay_render \
    --variant "$variant" "outputs/film/seven/$variant.json"
  build/film-decay-release/tests/izzi_film_decay_render \
    --demo "outputs/film/seven/$variant.json" 23 30 728 540 stationary \
    "outputs/film/seven/$variant.svg"
done
```

`variant_preset(name)` is the C++ equivalent. New parameter types are available
under `svg::video_glitch::film::variants`; `describe(motion)` and
`set(motion, name, value)` provide the same bounds as the CLI. V2 motion JSON
contains `family`, `variant` and the complete `controls` object. Unknown,
missing, duplicate, incompatible or out-of-range values fail. Integer counts
and lifetimes reject fractional inputs.

Mixture weights assign feature populations to the enabled forms after
normalization; they are not three unrelated full-frame opacity overlays.
Setting all three blister weights to zero yields the clean source. Layout
renewal changes indexed count/position/shape/topology over time; seed chooses
a different sequence. `layout_renewal=0` freezes the coarse layout while
independently enabled fine marks can continue. Fine lifetime, feature lifetime
and abrasion lifetime are separate controls. Layout correlation smooths
changes between material epochs; `field.smoothing` additionally interpolates
inside a held image and is an artistic option.

`S` means the short side of the active picture. Feature widths and sizes use
`S`, front positions use active width, angles use degrees, and lifetimes use
material frames. Domain changes preserve those meanings. The old spatial
`field` controls `correlation`, `pitch`, `primary_sources`, `ribbon_angle` and
`ribbon_length` remain at their legacy default values in v2 documents; changing
them is rejected. Use the corresponding named `motion.controls` instead.
Other shared controls such as fine/feature lifetime, variation seed, repetition
and artistic smoothing remain active.

The full review build needs the seven canonical clips at the recorded paths
under `resources.rizal/glitch-nitrate-scratch-fade/`, the release renderer
built below, FFmpeg, Inkscape, Poppler (`pdfunite`, `pdftotext`, `pdftoppm`),
and Python with Pillow, NumPy, OpenCV and CairoSVG. Install Atkinson
Hyperlegible Next Regular/Bold in `~/.fonts/atkinson_hyperlegible.2026/`
and DejaVu Sans/Mono. The delivery manifest records tool and font versions.

Generate all review artifacts, including both movie orientations, with:

```sh
python3 scripts/render-decasia-2.py --stage all --workers 4
python3 scripts/verify-decasia-2.py
```

The output root is `outputs/decasia-2/draft-1/`. It contains **120 controlled
cells, 120 temporal cells, 40 domain proofs, 21 seed samples and twenty
twelve-second chapters**, plus twelve supplementary curtain chronology cells.
The complete H.264 MKV is four minutes / 7,200 frames at 30 fps in each format:

- `deliveries/film-glitch-complete-horizontal.mkv`: 1280 × 720, source and
  effect side by side.
- `deliveries/film-glitch-complete-vertical.mkv`: 720 × 1280, source above
  effect.
- `deliveries/source-1-horizontal.mkv` through `source-7-horizontal.mkv`,
  with matching `-vertical.mkv` versions: fourteen individual source reels.

Both formats preserve the same timing, seeds, frame plans and plate IDs.
Picture panels retain their proportions; labels are arranged for each canvas.
The layout pass reuses the completed diagnostic chapters and adds one H.264
encoding generation. Native geometry, recipes and original chapter masters
remain the reproduction sources. `layouts/manifest.json` records derivation
and hashes; `layouts/verification.json` records complete decoding of all
sixteen reels and encoded preview measurements for all forty chapter layouts.

To format already completed chapters without regenerating film materials:

```sh
python3 scripts/render-decasia-2.py --stage layouts --workers 2
python3 scripts/verify-decasia-2.py
```

`--stage assemble` also restores both formats and their review-index links
when all twenty chapters exist. The 66-page PDF uses the page-748 catalog
format, with unique stable plate IDs, actual page numbers, full legends and
links to source recipes. Reference rasters are identified separately from
native generated geometry.

The sections below document the original v1 controls and shared composition
contract. The v2 control tables at the end list the versioned material controls.

## Build and render

```sh
CCACHE_DISABLE=1 cmake -S . -B build/film-decay-release -DCMAKE_BUILD_TYPE=Release
CCACHE_DISABLE=1 cmake --build build/film-decay-release --parallel 2 \
  --target izzi_film_decay_render izzi_film_decay izzi_film_decay_variants \
    izzi_film_decay_plates izzi_film_decay_study izzi_video_glitch_plates

# Row and column are 1..6; column 1 is the baseline.
build/film-decay-release/tests/izzi_film_decay_render \
  --preset 1 1 outputs/film/blister.json
build/film-decay-release/tests/izzi_film_decay_render \
  --demo outputs/film/blister.json 23 30 728 540 stationary outputs/film/blister.svg

# Import another native SVG, retaining supported fills, strokes and clips.
python3 scripts/render-film-decay-svg.py \
  --native build/film-decay-release/tests/izzi_film_decay_render \
  --input artwork.svg --recipe outputs/film/blister.json \
  --frame 23 --rate 30 --output outputs/film/artwork.svg
```

SVG and movie adapters require Python with NumPy, Pillow, CairoSVG and OpenCV;
movie/reference workflows also require FFmpeg/ffprobe. The geometry header has
no raster dependency. The JSON header uses the same RapidJSON dependency as
the existing `/1` contract.

```sh
# Build/check the shared /1 engine and generate its seven-family review.
make video-glitch-review

# Render the third column of the first film-damage row at frame 23 / 30 fps.
build/cmake/tests/izzi_video_glitch_plates --render \
  outputs/glitch-motion-studies/draft-1/plates/film-damage-r1-c3.json \
  23 30 1 /tmp/film-damage.svg
```

## Installed C++ interface

The `film-damage` family belongs to `izzi.video-glitch/1`, with the C++ type
`svg::video_glitch::film_damage`. Include `izzi-svg-video-glitch.h` for its
vector engine or `izzi-svg-video-glitch-json.h` for JSON recipes. It fades
native strokes, removes polyline segments in procedural erosion regions, and
adds scratches and transient dust. The source-matched materials use the
`izzi.video-glitch/2` interface below. The
[video-glitch API guide](video-glitch.md#using-the-c-api) describes scene
construction, ordered effects, event timing and serialization for `/1`.

Include `izzi-svg-film-decay.h` for geometry and planning, and
`izzi-svg-film-decay-json.h` for recipes. Both use
`svg::video_glitch::film`. Generic region extraction and density arithmetic
remain in `izzi-levels-contours-regions.h`. `izzi-film-decay-fields.h` contains
the shared generator used by both the original study and production; its
`film_decay_detail` namespace is an implementation detail.

```cpp
#include <izzi-svg-film-decay-json.h>
#include <fstream>

int main()
{
  namespace film = svg::video_glitch::film;
  auto recipe = film::preset(0); // Zero-based row; optional second arg is column.
  film::scene source;
  source.width = 800;
  source.height = 600;
  film::source_path shape;
  shape.id = "annulus";
  shape.d = "M50 50H750V550H50ZM250 200V400H550V200Z";
  shape.fill_rule = "evenodd";
  shape.fill = film::color{.8, .65, .4, 1}; // sRGB, straight alpha.
  shape.stroke = film::color{.05, .1, .2, 1};
  shape.width = 6;
  source.paths.push_back(shape);

  const auto frame = film::plan(recipe, {23, {30, 1}, {0, 1}});
  const auto material = film::resolve(recipe, frame, source.width, source.height);
  const auto output = film::render(recipe, material, {source}, "plate-a");
  std::ofstream("film.svg") << output.svg;
  std::ofstream("film.json") << film::emit_json(recipe);
}
```

For the `/1` stroke effect, construct a vector scene and select `film_damage`
in the ordered effect list:

```cpp
#include <izzi-svg-video-glitch-json.h>
#include <fstream>

int main()
{
  namespace vg = svg::video_glitch;
  vg::scene source;
  source.width = 640;
  source.height = 360;
  source.strokes.push_back(vg::line(1, {40, 180}, {600, 180}, "#191f45", 8));
  vg::document recipe;
  vg::film_damage wear;
  wear.wear_pass = 8;
  wear.decay_age = 2; // Independent of wear count and presentation time.
  recipe.effects.push_back({"film-wear", "vector", {}, wear});
  auto frame = vg::sample(recipe, {23, {30, 1}});
  auto damaged = vg::apply_vector(source, frame);
  std::ofstream("film-damage.svg") << vg::render_svg(damaged, recipe.budget);
  std::ofstream("film-damage.json") << vg::emit_json(recipe);
}
```

In `/2`, every source path has an ID, path data, optional fill/stroke RGBA, stroke width,
opacity, fill rule, affine transform, paint role, and a list of ancestor clips.
A clip can contain multiple closed subpaths, including holes. Qualify the final
render prefix for each plate/layer; repeated source copies get unique generated
IDs. Source IDs must be unique inside a scene. SVG strings are XML escaped.

The `/2` SVG importer supports paths (including cubic/quadratic curves and arcs),
rectangles including rounded corners, circles, ellipses, lines, polygons,
polylines, inherited solid paints, affine groups, local `use`, nested viewports,
and single-child compound clips in user space. It rejects external references,
DTD/entities, raster images, text, gradients, filters, masks, dashed strokes,
non-default group opacity, miter limits or paint order, and unresolved
multi-child clip unions. Convert these
features to supported paths/paints before import. It never silently loses them.
The C++ scene interface can be populated directly without importing SVG.

## Three clocks and source construction

`frame_context` contains an integer frame, a rational output rate, and a rational
presentation offset. `plan()` computes exact presentation time and reference
ticks with checked integer arithmetic, resolves the hold pattern and its phase,
and returns **source samples before the caller constructs the scene**.

| Mode | Source sample | Material field |
| --- | --- | --- |
| `damage_only` | Presentation time, including the caller's offset. | Held according to reference ticks and `holds`. |
| `whole_print` | `source_offset + material_frame × source_step`. | Held with the same material frame. |
| Global strength zero or inactive event | Original presentation time; one source sample. | Bypassed, including gate motion, echoes and retiming. |

The reference rate defaults to **24000/1001**. Hold values count ticks of that
clock, not output frames. `phase` is a tick offset within the entire hold cycle.
`[4,2]` means two distinct images held for four and two ticks. `source_step` is
an explicit playback choice, not an estimate of a historical camera rate.
The presets use the mean hold duration; the review's artistic sections halve it.

A moving-source caller uses the returned times:

```cpp
// make_scene(t) is the caller's native source generator.
auto frame = film::plan(recipe, context);
std::vector<film::scene> samples;
for (const auto& request : frame.samples)
  samples.push_back(make_scene(request.time));
auto material = film::resolve(recipe, frame, samples[0].width, samples[0].height);
auto output = film::render(recipe, material, samples, "moving-plate");
```

All samples must share the active domain. The convenience `render(recipe,
context, source)` overload rejects active whole-print retiming and temporal
exposures: it cannot resample an already constructed scene. A stationary SVG
adapter explicitly supplies the same scene at each requested time.

For `/1` `film_damage`, wear count and decay age are independent controls.
Neither is inferred from elapsed video time. With event envelope `e`, source opacity is multiplied by
`(1 - fade_per_pass * e)^wear_pass`, and the erosion radius is
`min(0.36, erosion * sqrt(decay_age)) * scene.width * e`. Erosion acts on the
resolved polyline segments, so source tessellation affects its smallest marks.
Transient dust uses a fixed 6 Hz noise clock. Setting both `wear_pass` and
`decay_age` to zero bypasses the effect, including dust.

## Six presets and their controls

`S = min(active width, active height)`. Geometry and density controls have the
following units. The original five controls remain the atlas axes.

| Cut / type | Hold | Controls: defaults; allowed bounds |
| --- | --- | --- |
| 1 / `emulsion_bloom` | `[2]` | `scale` .32 [.12,.65] S; `rim` .105 [0,.25] field units; `pores` .75 [0,2]; `lace` .58 [0,2]; `stain` .55 [0,1]. |
| 2 / `density_tide` | `[3]` | `scale` .52 [.15,1] S; `veil` .62 [0,1]; `density` .30 [0,2]; `lace` .48 [0,2]; `feather` .32 [.05,.8] field units. |
| 3 / `density_flutter` | `[3]` | `scale` .55 [.15,1] S; `veil` .82 [0,1]; `contrast` .24 [0,2]; `abrasion` .65 [0,2]; `polarity_boost` 0 [0,1]. |
| 4 / `emulsion_ribbon` | `[4,2]` | `width` .085 [.01,.25] S; `raggedness` .72 [0,2]; integral `tears` 2 [1,4]; `displacement_boost` 0 [0,.2] S; `pitting` .65 [0,2]. |
| 5 / `survivor_islands` | `[3]` | `retention` .58 [.08,.9]; `scale` .34 [.15,.65] S; `bridges` .28 [0,1]; `pores` .55 [0,2]; `stretch_boost` 0 [0,.4]. |
| 6 / `edge_curtain` | `[2]` | `aperture` .62 [.02,.98] width; `wander` .045 [0,.3] S; `feather` .045 [.005,.2] S; `density` 3.6 [0,6]; `pitting` .50 [0,2]. |

Density is logarithmic optical attenuation, veil/polarity are mixing fractions,
and contrast is a linear-light contrast multiplier. Pores, lace, bridges,
abrasion, pitting, stain and raggedness are artistic weights. Blister rim and
tide feather are scalar-field intervals, not constant screen-space thickness.
Retention solves a sampled field quantile; `retained_area` reports the resulting
polygon coverage. It is approximate, not an exact continuous-area guarantee.

Each shared extension has separate `enabled` and `amount` values:

| Extension | Default | Range and effect |
| --- | --- | --- |
| `polarity` | true, .025 | [0,1], local inversion in linear light, including all source fill/stroke paints. |
| `displacement` | true, .004 | [0,.2] S, local horizontal displacement; vertical displacement is one fifth of it. |
| `stretch` | true, .012 | [0,.4], local horizontal scale increase around picture center. |

Off suppresses the corresponding family boost as well as the base amount;
it preserves the saved amount for re-enabling. These source-relative
perturbations are artistic. They are not inferred physical displacements.

For the `/1` `film_damage` type, the controls are:

| Control | Default | Accepted range | Meaning |
| --- | ---: | --- | --- |
| `wear_pass` | 4 | Integer 0–`budget.passes` | Wear count controlling fade and scratch population. |
| `scratch_density` | 2 | 0–20 | Scratch marks per wear count; the resulting count is truncated to an integer. |
| `scratch_width` | 0.7 | 0–20 | Scene units. |
| `decay_age` | 1 | 0–32 | Artistic age proxy controlling erosion extent. |
| `erosion` | 0.08 | 0–1 | Erosion extent coefficient, relative to scene width. |
| `fade_per_pass` | 0.018 | 0–0.5 | Fractional stroke-opacity loss per wear count. |
| `dust` | 0.4 | 0–10 | Dust population weight. |

## Field identity, features, and artistic events

Fields use `splitmix64-counter-v1` and **`film-field-v1`**, keyed by document
seed, effect ID, variation seed and material identity. Seeking and evaluation
order do not affect the result.

| Control | Units, defaults, limits |
| --- | --- |
| `field.correlation`, `pitch` | Coarse strip persistence: 1 [1,64], advancement .041 [0,1] per material frame. Higher correlation slows coarse change. |
| `field.fine_lifetime` | Positive material frames [1,10000], default 1, for fine-field/flutter state renewal. |
| `field.feature_lifetime` | Positive material frames [1,10000], default 6, for pore/grain feature birth intervals. |
| `field.smoothing` | [0,1], default 0. Artistic evolution inside a held interval; explicitly distinguish it from reference holds. |
| `field.repeat_material_frames` | 0 disables repetition; positive values up to 1e9 repeat the material sequence. `variation_seed` selects its organization. |
| `field.primary_sources` | 1–32, default 7; coarse bloom/island sources. |
| `field.ribbon_angle`, `ribbon_length` | −180°–180° relative to the canonical diagonal; default 0°. Finite length .3–4 S, default 1.5 S. |
| `event` | Rational start/duration; attack and release in seconds. Ramps may not overlap. Default event is [0,12) seconds. |
| `keyframes` | Unique motion control names and strictly increasing rational presentation times; smooth interpolation, rounded integral tear counts. |
| `elastic` | `x/y` displacement in S, each −.2–.2; `max_gradient` .001–.75 (default .35); curve tolerance .001–4 scene units (default .15). |
| `gate` | `x/y`, `judder`, `frame_slip`: 0–.2 active width/height; rotation 0–15°; frequency 0–100 Hz; defaults zero amplitudes. `debris`: 0–1024. |
| `wear` | 0–4096 scratches and dust features; lifetime 1–10000 material frames; width 0–.02 S; opacity [0,1]; drift 0–.1 S; full-height/broken switches. Defaults: zero counts. |
| `exposure` | 0–8 earlier source samples; positive rational `step`; gain [0,1] per echo; x displacement −.2–.2 width. Default zero echoes. Negative requested head times clamp to the first source sample. |
| `exposure_pump`, `solarize` | Pump density [0,2], default 0; thresholded inversion amount [0,1], threshold [0,1], defaults 0 and .5. Both are artistic. |
| `border` and `paint` | Border 0–.2 S, default 0; explicit sRGB RGBA substrate, light-material and dark-material roles. Defaults white/white/black. |

The smooth elastic pull vanishes at the viewport boundary. Its amplitude is
reduced when necessary to meet the declared gradient bound; the bound remains
below one. Curves and source clips are adaptively subdivided after transforms,
with quarter-point checks, maximum segment length and an explicit point budget.
Both backends preserve native region boundaries. The hybrid adapter uses the
inverse of that bounded field to sample pixels; its interpolation is explicit.

Ribbon displacement keyframes provide peel/recoil. Aperture/density keyframes
provide eclipse/recovery. Repeat plus variation seed provides reel memory.
Temporal echo uses actual separately requested source samples. Named material
paints provide tints; the monochrome references do not calibrate them.

## Loss, alpha, and ordering

`loss="paint"` retains the layered pale/dark material interpretation.
`loss="transparent"` removes the resolved loss regions, exposing the layer
below. `loss="substrate"` removes those regions over a declared substrate.
Substrate is placed beneath the print, so an opaque substrate stays opaque at
antialiased hole edges. Density never changes source alpha.

RGB paint is decoded from sRGB, then follows this order: optional artistic
solarization → local polarity → contrast → optical attenuation → white veil →
density amount → sRGB encoding. Material palette interpolation is explicit.
Density has 28 region bands; fine banding can remain at large magnification.
Source-over composition uses premultiplied sRGB, not an implied spectral model.

The physical composition order is source sampling/exposures → film-attached
wear → local source perturbation and bounded emulsion deformation →
density/loss regions → border → rigid gate transform → gate-attached debris.
Deformation carries attached regions and marks; gate motion carries the whole
print. `ordering="detached_material"` keeps the material regions stationary
while pulling the source as an explicitly artistic alternative.

A `/1` scene can be evaluated first and passed to `film::import_legacy(scene)`.
The importer preserves each stroke's resolved color, width, opacity and echo
role and qualifies its ID. Apply film processing after that import. Applying a
stroke-only legacy operator to a filled/clipped film result is unsupported;
there is no silent conversion that discards fills or clips. The `/1` JSON parser
and family variant remain unchanged.

| Capability | Native SVG | Hybrid movie |
| --- | --- | --- |
| Filled/stroked paths and compound clips | Preserved | Native-generated boundaries rasterized by CairoSVG. |
| Density, polarity, solarization | Per source paint, linear light | Per unpremultiplied pixel, linear light. |
| Elastic pull | Adaptive vector subdivision, retained clips | Inverse bounded remap with linear interpolation. |
| Transparent loss | Alpha mask with compound regions | The same region alpha mask. |
| Retiming and temporal echoes | Caller supplies planned scenes | Caller decodes planned source timestamps. |
| Flattened line-only export after film | Rejected/unsupported | Not applicable. |
| Source after legacy `/1` evaluation | Explicit painted-stroke import | Existing legacy movie route may run first. |

Native paint evaluation and hybrid pixel evaluation can differ at already
antialiased source edges. The verification receipt measures those differences;
zero-strength identity is exact before encoding. FFV1 preserves RGB movie bytes,
while H.264 is a lossy preview.

## Movie adapter and timing policy

Set `backend` to `hybrid_movie` in a generated recipe. Use an explicit crop to
exclude inserted bars from the material domain; padding is preserved bytewise.

```sh
python3 scripts/render-film-decay-movie.py \
  --native build/film-decay-release/tests/izzi_film_decay_render \
  --input source.mkv --recipe film-movie.json \
  --crop 232,0,1456,1080 --rate 24000/1001 \
  --duration 12 --audio preserve_timeline --output outputs/film/movie.mkv
```

The adapter has a separate `/2` route. `video_glitch_movie.transform_film()`
also exposes it to an existing movie caller without changing the seven `/1`
families. That boundary accepts BGR/BGRA, matching its existing movie callers;
the standalone film adapter uses RGB/RGBA internally. Movie duration defaults to source video duration in `damage_only`.
`whole_print` requires explicit `--duration` and `--audio` choices. Audio may be
dropped or kept on presentation time; it is never implicitly stretched to
match held images or exposure echoes. The preserved audio window begins at
the first source video PTS. Missing tail audio is padded to the requested
presentation duration.

Source samples select the nearest decoded PTS relative to the first video PTS,
with ties selecting the earlier frame. This handles container timestamp
quantization without duplicating frames in an identity render. Traces record
both requested times and actual decoded indices/PTS. Outside-source sampling
fails unless `--end-policy hold_last` is explicitly selected.

Crop padding uses presentation time and has a separate
`--padding-end-policy error|hold_last` choice. Its default is `error`. With an
explicit hold policy, a slowed active picture can outlast its original movie
while the surrounding padding holds the final decoded frame. That choice does
not authorize an out-of-range sample requested by the film planner; the
`--end-policy` check still applies. A full-picture composition has no padding
lookup and reads only its planned source samples.

The following fixture renders two seconds from a one-second diagnostic source
for all seven families. It deliberately uses a common slowed demonstration
clock; it is not the canonical per-source timing used by the review reels.
Run the native loop above first to create the seven recipes.

```sh
ffmpeg -v error -y -f lavfi -i testsrc2=size=160x120:rate=24000/1001 \
  -frames:v 24 -an -c:v ffv1 outputs/film/seven/one-second.mkv
python3 - <<'PY'
import json
import subprocess
from pathlib import Path

root = Path('outputs/film/seven')
for path in sorted(root.glob('*.json')):
    if path.name.endswith(('.movie.json', '.trace.json', '.program.json')):
        continue
    recipe = json.loads(path.read_text())
    if recipe.get('schema') != 'izzi.video-glitch/2':
        continue
    recipe['backend'] = 'hybrid_movie'
    recipe['clock'].update(reference_rate=[24000, 1001], holds=[2], phase=0,
                           sampling='whole_print', source_step=[1001, 24000])
    recipe['event'].update(duration=[2, 1], attack=0, release=0)
    recipe['budget']['columns'] = 96  # Explicit proxy quality.
    movie_recipe = path.with_suffix('.movie.json')
    movie_recipe.write_text(json.dumps(recipe, indent=2) + '\n')
    subprocess.run([
        'python3', 'scripts/render-film-decay-movie.py',
        '--native', 'build/film-decay-release/tests/izzi_film_decay_render',
        '--input', str(root / 'one-second.mkv'), '--recipe', str(movie_recipe),
        '--duration', '2', '--rate', '30', '--audio', 'drop',
        '--output', str(path.with_suffix('.mkv')),
    ], check=True)
PY
```

Each result contains sixty frames, with its final planned sample inside the
source. The adapter's MKV files are lossless FFV1 fixtures. The review builder
separately uses H.264 at CRF 18 in MKV for compact delivery.

The adapter decodes to a disk-backed RGBA store with an explicit source-byte
budget (default 8 GiB), caps output at 100,000 frames, and caches two prepared
material programs. Output frame count is `ceil(duration × rate)`; the receipt
records both requested and encoded duration. MKV uses lossless FFV1; MP4 uses
H.264. Movie alpha is composited over the recipe's declared substrate before
encoding; native SVG and in-memory RGBA callers can retain transparency.

## Resource budgets and reproducibility

The historical `/1` vector engine defaults to 32 passes, 60,000 paths,
2,000,000 points and 64 MiB SVG. Exceeding a budget raises an error. Its output
contains strokes; the `/2` material engine preserves filled/stroked source
artwork and source clips.

The `/2` default limits are 300,000 paths, 4,000,000 points, 96 MiB SVG/program bytes,
128 operations, and 192 scalar columns. Counts include repeated source and clip
instances, not only unique fields. Oversized grids, aggregate geometry, source
copies, curves that cannot meet tolerance, and final SVG bytes fail explicitly.
No budget silently reduces detail or substitutes a raster renderer.

Nonsquare pictures sample in short-side units with an aspect-preserving grid.
Still/proxy quality can differ and is recorded. Cache keys include the full
recipe, domain, quality, native binary hash, field/material state, resolved
controls and density amount; smoothing adds hold phase. The C++ API itself is
stateless. Frame traces include rational source/presentation time, held material
identity/phase, seed, resolved controls, gate/exposure instructions and samples.
Program receipts add region/point/feature counts and retained area. Manifest
files pin algorithms, native binary, source files and raster/encoder versions.

## Verification and review artifacts

```sh
CCACHE_DISABLE=1 ctest --test-dir build/film-decay-release --output-on-failure \
  -R '^film-decay[.]'
python3 scripts/analyze-decasia-references.py
python3 scripts/render-film-decay-production.py --workers 3 \
  --publish-plate docs/development/sessions/20260929.decasia-nitrate-silent-decay.production.svg
python3 scripts/verify-film-decay-production.py
```

The reference inventory is `outputs/decasia-production/references/manifest.json`.
It reads the six CUT declarations without executing the cut script, hashes all
six normalized and original clips, decodes every frame of the six normalized
comparison copies, records PTS/crop
and threshold-dependent cadence, and produces overview/adjacent contacts.

Production review output is under `outputs/decasia-production/review/`:

- `decasia-production-atlas.svg`/`.png`: the 36 native comparisons and recipes.
- `domains/comparison.png`: all six patterns on wide and tall sources, plus
  their full-resolution SVG/PNG files.
- `decasia-production-72s.mp4`: six 12-second chapters. Each shows four seconds
  of a stationary source, four seconds of moving source with held damage, and
  four seconds of whole-print retiming with explicitly labeled artistic controls.
- `chapters/01`–`06`: resolved recipes, programs, full per-frame traces,
  source/effect contact sheets, chapter movies, and receipts.
- `manifest.json`: hashes and reproduction data for the complete delivery.

Tests cover rational cadence/phase/offset/overflow, seeking, JSON rejection,
zero identity, source clips and wide strokes, loss alpha, linear ramps, bounded
pulls, resource limits, and native/hybrid agreement. Integration checks include
lossless pixel/audio identity and an explicitly retimed, cropped silent movie.
The independent artifact verifier checks all 2,160 frame plans, 1,072 material
programs, eighteen shuffled seeks and the complete movie decode. The saved
[verification record](../development/sessions/20260929.decasia-nitrate-silent-decay.verification.json)
captures this delivery's results. Visual review used native plates and chapter
contacts; real-time audiovisual playback and final user artistic approval are
not asserted.

`examples/video-glitch-plates.cc` includes the film-damage plate in its
seven-family review. The twelve cells hold source time at 23/30 second and
seed at 20260926. Six columns use wear counts 0, 1, 2, 4, 8 and 16, with
`decay_age = wear_pass / 4` explicitly coupled for this study. Two rows use
erosion 0.06 and 0.14. Both first-column cells are clean references; the other
cells use dust 0.4. This coupling is a study choice, not an API constraint.

The source is a native guilloche scene with three rosettes, circles, a grid
and a dark diagnostic stroke. The plate uses 10% black (`#E6E6E6`) backing,
separate from the transparent core output. The
seven-page vector PDF
includes this plate; its SVG/PNG and complete cell recipes are in
`outputs/glitch-motion-studies/draft-1/plates/`.

The [video-glitch review commands](video-glitch.md#serialization-and-commands)
cover plate-only generation, the seven-chapter reel, movie composition and
verification. The [original study](../development/sessions/20260926.glitch-motion-studies.md)
and [implementation record](../development/sessions/20260926.glitch-motion-studies_praxis.md)
document the historical film/VHS references and review results.

## V2 material controls

These tables are emitted from the native control descriptions. Bounds are
inclusive. “Integer” means fractional inputs are rejected. Defaults belong to
the named representative preset; other presets deliberately choose different
values. Use `--describe-variant NAME FILE.json` for their exact values.
`S` is the active picture’s short side; “weight” is a normalized artistic
control, not a measured chemical concentration or calibrated physical unit.

The three shared controls below occur in every v2 family. The erosion presets
use `layout_correlation = 2`; the other representatives use `1`. Layout
renewal blends indexed coarse layouts; correlation spans material epochs, and
the shared `field.smoothing` separately interpolates within a held image.

| Shared control | Default | Bounds | Units | Integer |
| --- | ---: | --- | --- | --- |
| `layout_lifetime` | 1 | 1–10000 | material frames | yes |
| `layout_renewal` | 1 | 0–1 | weight | no |
| `layout_correlation` | 1 | 1–64 | epochs | yes |

### Blister forms and mixtures

Representative preset: `pale-cavities`.

| Control | Default | Bounds | Units | Integer |
| --- | ---: | --- | --- | --- |
| `cavities` | 1 | 0–8 | weight | no |
| `membranes` | 0 | 0–8 | weight | no |
| `foam` | 0 | 0–8 | weight | no |
| `population` | 9 | 0–64 | features | yes |
| `size` | 0.28 | 0.025–0.8 | S | no |
| `size_spread` | 0.86 | 0–1 | weight | no |
| `aspect` | 0.76 | 0.2–2 | ratio | no |
| `orientation` | 0 | -180–180 | degrees | no |
| `clustering` | 0.25 | 0–1 | weight | no |
| `rim_width` | 0.022 | 0–0.08 | S | no |
| `rim_variation` | 0.85 | 0–1.5 | weight | no |
| `softness` | 0.012 | 0.0005–0.1 | S | no |
| `dark_rim` | 0.9 | 0–1 | weight | no |
| `light_rim` | 0.25 | 0–1 | weight | no |
| `source_retention` | 0.18 | 0–1 | weight | no |
| `veil` | 0.42 | 0–1 | weight | no |
| `density` | 0.12 | 0–3 | optical density | no |
| `stain` | 0.35 | 0–1 | weight | no |
| `perforation` | 0.28 | 0–1 | weight | no |
| `cell_population` | 3000 | 0–12000 | candidates | yes |
| `cell_size` | 0.014 | 0.001–0.06 | S | no |
| `cell_spread` | 0.9 | 0–1 | weight | no |
| `boundary_attachment` | 0.85 | 0–1 | weight | no |
| `cluster_width` | 0.065 | 0.003–0.3 | S | no |
| `spacing` | 0.75 | 0.15–2 | ratio | no |
| `satellites` | 0.08 | 0–1 | weight | no |
| `lace` | 0.65 | 0–2 | weight | no |
| `lace_width` | 0.002 | 0.0002–0.03 | S | no |

### Density tide

Representative preset: `broad-veil`.

| Control | Default | Bounds | Units | Integer |
| --- | ---: | --- | --- | --- |
| `extent` | 0.54 | 0.08–1.5 | S | no |
| `density` | 0.3 | 0–3 | optical density | no |
| `veil` | 0.38 | 0–1 | weight | no |
| `softness` | 0.18 | 0.01–0.8 | S | no |
| `margin_lesions` | 0.25 | 0–2 | weight | no |
| `front_count` | 3 | 1–12 | fronts | yes |
| `front_position` | 0.5 | -0.5–1.5 | active width | no |

### Density flutter

Representative preset: `pale-regional-veil`.

| Control | Default | Bounds | Units | Integer |
| --- | ---: | --- | --- | --- |
| `extent` | 0.55 | 0.08–1.5 | S | no |
| `veil` | 0.82 | 0–1 | weight | no |
| `contrast` | 0.24 | 0–2 | weight | no |
| `abrasion_density` | 140 | 0–2000 | marks | yes |
| `abrasion_width` | 0.0008 | 0.0001–0.015 | S | no |
| `abrasion_width_spread` | 0.8 | 0–1 | weight | no |
| `abrasion_lifetime` | 8 | 1–10000 | material frames | yes |
| `relief` | 0.25 | 0–1 | weight | no |

### Abrasion, tears and sheets

Representative preset: `thin-tear`.

| Control | Default | Bounds | Units | Integer |
| --- | ---: | --- | --- | --- |
| `population` | 3 | 0–16 | tears | yes |
| `width` | 0.026 | 0.003–0.5 | S | no |
| `width_spread` | 0.9 | 0–1 | weight | no |
| `raggedness` | 0.75 | 0–2 | weight | no |
| `orientation` | -12 | -180–180 | degrees | no |
| `length` | 1.5 | 0.1–4 | S | no |
| `source_retention` | 0.15 | 0–1 | weight | no |
| `pitting` | 0.5 | 0–2 | weight | no |
| `occupancy` | 0.75 | 0–1 | weight | no |
| `tear_lifetime` | 1 | 1–10000 | material frames | yes |
| `abrasion_density` | 200 | 0–2000 | marks | yes |
| `abrasion_width` | 0.0008 | 0.0001–0.015 | S | no |
| `abrasion_width_spread` | 0.85 | 0–1 | weight | no |
| `broken_fraction` | 0.65 | 0–1 | weight | no |
| `opacity` | 0.25 | 0–1 | weight | no |
| `clustering` | 0.5 | 0–1 | weight | no |
| `abrasion_lifetime` | 8 | 1–10000 | material frames | yes |

### Surviving picture

Representative preset: `connected-survivor-window`.

| Control | Default | Bounds | Units | Integer |
| --- | ---: | --- | --- | --- |
| `retained_coverage` | 0.58 | 0.02–0.98 | area fraction | no |
| `size` | 0.35 | 0.03–0.8 | S | no |
| `size_spread` | 0.8 | 0–1 | weight | no |
| `bridge_width` | 0.055 | 0–0.3 | S | no |
| `population` | 7 | 1–64 | components | yes |
| `holes` | 0.25 | 0–1 | weight | no |
| `perimeter_lace` | 0.8 | 0–2 | weight | no |
| `rim_width` | 0.009 | 0–0.06 | S | no |

### Curtain and aperture

Representative preset: `soft-advancing-front`.

| Control | Default | Bounds | Units | Integer |
| --- | ---: | --- | --- | --- |
| `position` | 0.18 | -0.5–1.5 | active width | no |
| `aperture` | 0.67 | 0.01–1.5 | active width | no |
| `wander` | 0.18 | 0–0.6 | S | no |
| `softness` | 0.17 | 0.005–0.65 | S | no |
| `extinction` | 3.8 | 0–6 | optical density | no |
| `edge_pitting` | 0.3 | 0–2 | weight | no |
| `curvature` | 0.16 | 0–0.6 | S | no |
| `closure` | 0.96 | 0–1 | weight | no |
| `cycle` | 96 | 8–10000 | material frames | yes |
| `front_count` | 1 | 1–4 | fronts | yes |

### Forbidden Quest print erosion

Representative preset: `perforated-picture`.

| Control | Default | Bounds | Units | Integer |
| --- | ---: | --- | --- | --- |
| `flecks` | 450 | 0–4000 | marks | yes |
| `fleck_size` | 0.0025 | 0.0003–0.025 | S | no |
| `fleck_spread` | 0.85 | 0–1 | weight | no |
| `fleck_opacity` | 0.5 | 0–1 | weight | no |
| `abrasion_density` | 70 | 0–2000 | marks | yes |
| `abrasion_width` | 0.0005 | 0.0001–0.015 | S | no |
| `mark_lifetime` | 4 | 1–10000 | material frames | yes |
| `coverage` | 0.48 | 0–0.95 | area fraction | no |
| `extent` | 0.27 | 0.06–1.5 | S | no |
| `softness` | 0.045 | 0.005–0.3 | S | no |
| `veil` | 0.75 | 0–1 | weight | no |
| `source_retention` | 0.3 | 0–1 | weight | no |
| `edge_bias` | 0.1 | 0–1 | weight | no |
| `perforation` | 1.1 | 0–2 | weight | no |
| `lace` | 1.1 | 0–2 | weight | no |

Blister populations, coarse size/spread and clustering govern large structures;
rim controls govern their boundaries. Cell population/size/spread, spacing,
attachment, satellites and lace govern smaller boundary structures. The three
form weights select cavities, membranes and foam within that shared field.

Tide controls separate broad density from scalloped-front extent and placement.
Flutter keeps paleness distinct from source relief and abrasion. Tear controls
separate fine abrasion from the count, width, length, duty and lifetime of
larger losses. Survivor controls separate retained area/topology from edge
activity. Curtain controls separate opening/closure from boundary softness
and wandering. Erosion controls separate small marks from regional loss, veil,
source retention and perforated/laced boundaries. Coverage and retained-area
targets are evaluated on the declared scalar grid; receipts record the resolved
geometry rather than claiming an exact source-image segmentation.
