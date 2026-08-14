# Izzi video and graph API evolution

- Date: 2026-08-09
- Status: derived proposal; implementation not yet authorized by this document
- Stage gate: commit this plan before beginning Stage 1, **Freeze evidence**
- Scope: sequence artifacts, vertical-series assembly, radial and orbit graph
  evolution, roulette-to-guilloche pattern systems, canonical header naming,
  and build/install migration
- Predecessors: `docs/development/archive/explore_futures.md` and
  `docs/development/sessions/explore_futures.20260808.md`, preserved unchanged

## Outcome

Izzi should treat video and graphs as reproducible assemblies of typed visual
artifacts, not as side effects hidden in shell commands or procedural functions
with ambient state. The Asama loops in `resources/asama-loops` are the concrete
bridge for video. The user identifies the collection as their original work
made by the source machinery in `/home/bkoz/src/MiL.git/src`. Their public
portfolio independently documents **Asama Loops** as a 2015 two-channel video
installation and describes its combinatoric source and temporal language.
Izzi's current
`src/a60-svg-sequences.h` is a reduced, poorly adapted descendant of that
machinery. The loops now serve as local inputs to the confirmed vertical-series
form and as historical oracles for repairing the sequence API.

The MMRL artifacts in `resources/mmrl` are the corresponding bridge for data
visualization. The user confirms that the Izzi Kusama and radial graph sources
were used to create the top-level artifacts and that
`src/a60-svg-graphs-chord.h` produced the matched files under `image.chord`.
They demonstrate expressive orbit and flattened-chord languages across
large-format SVG, PDF, PNG, and encoded video. Those languages must be retained.
The opportunity is to simplify how data, layout, styling, accessibility,
provenance, and export are expressed around them.

The evolved API should connect this chain without putting FFmpeg, networking,
or process execution into the header-only C++ library:

```text
Izzi sequence specification
  -> deterministic SVG frames
  -> encoded sequence artifact + provenance manifest
  -> episode slot + fit/loop policy
  -> vertical-series assembly manifest
  -> external encoder adapter
  -> MKV/PDF/PNG/WebAssembly consumers
```

The graph path should use the same artifact discipline:

```text
versioned JSON or typed C++ graph data
  -> validated radial/orbit specification
  -> deterministic semantic scene
  -> SVG + accessible data alternative
  -> PNG/PDF/WebAssembly/video render profiles
  -> provenance manifest + visual-review receipt
```

An optional provider result enters at the same encoded-artifact boundary as an
Asama loop. It must conform to an already accepted local form. The provider
does not define timing, typography, aspect handling, accessibility, or
editorial structure.

## New confirmed requirements

1. The production vertical series contains 60–90 episodes.
2. Each episode contains 60–180 seconds of visual content.
3. Title-card time is explicit and outside the content slot.
4. The first synthetic form check contains ten episodes. Each begins with a
   three-second numbered card and then uses one randomly selected Asama loop,
   repeated or trimmed to fill one random 60–180-second slot.
5. Random test choices are reproducible from a recorded seed.
6. `resources/asama-loops/*.mkv` are first-class Izzi-generated sequence
   artifacts, not generic stock footage.
7. Canonical public filenames change from `src/a60-*` to `src/izzi-*`, with
   matching `IZZI_*` include guards and coordinated Make/CMake/build updates.
8. Provider calls remain outside form discovery and require a separate later
   authorization.
9. `resources/mmrl` is evidence of production use of the Izzi radial and Kusama
   sources across static, large-format, multipage, and time-based artifacts.
10. The orbit/Kusama visual language remains a first-class graph style; API
    simplification must not replace it with a conventional chart grammar.
11. The nine matched SVG/PNG pairs in `resources/mmrl/image.chord` were produced
    by `src/a60-svg-graphs-chord.h` and preserve a distinct flattened-chord
    visual language.
12. The user explicitly passed `outputs/synthetic/synthetic-form-check-02.mkv` and
    `outputs/synthetic/synthetic-form-check-02-episode-filmstrip.png`. That approval is
    limited to those two artifacts and does not include the Izzi example
    fidelity grids.
13. Roulette curves should evolve into a complete guilloche composition
    language informed by historical French banknote pattern craft and Dutch
    banknote design, without reproducing currency or claiming security value.
14. Static and animated moiré, interference, registration-slip, and
    deliberately wild compound patterns are first-class visual-exploration
    modes. Accessible and public profiles constrain their presentation; they do
    not remove the experimental profile.
15. Guilloche motion should be available to the Duotone visual language as a
    locally authored bridge between noir, silence, glitch, and psychedelic
    fantasy.
16. Set-level Asama provenance is documented: the user states that the eleven
    local loop artifacts are their own work, and the Benjamin De Kosnik public
    portfolio documents the named 2015 **Asama Loops** installation. This
    resolves authorship at the collection level; exact per-file recipes, seeds,
    source-image rights, and publication scope remain separately evidenced.
17. Words and numbers are valid guilloche source geometry. They may be outlined,
    braided, repeated, used as centerlines, cut out as negative space, or filled
    with related path families, provided their source string, outline/font
    provenance, semantics, and resource bounds remain explicit.
18. The user authorizes redistribution of all non-secret files under
    `resources/` as project resources. Preserve attribution and embedded license
    notices. Credentials, especially `resources/.private/SEEDANCE_KEY`, remain
    excluded; provider transfer, participant voice treatment/casting, baseline
    promotion, and publication of derived media retain their separate gates.
19. The guilloche public namespace is `izzi::guilloche`.
20. Default radial behavior matches the supplied MMRL samples. Stage 1 must
    characterize and freeze those corpus-defined values; alternative modern or
    simplified defaults enter only as explicitly named presets.
21. Animated experimental artifacts and all wild visual elements are always
    allowed for local generation, export, retention, and human review. Local
    accessibility and flash diagnostics describe the artifact but do not block
    it or silently simplify it. Public or claimed-conformant output remains a
    separate gate.

The detailed duration, episode-count, layout, runtime, and file-size variants
are in `docs/development/vertical-form.md`.

## Evidence from the current sources

The current `src/a60-svg-sequences.h` is included by the public umbrella header
and supplies fade, blink, wink, vertical-sync roll, optical-dot, and swipe
generators. These functions return eager `std::vector<std::string>` collections
of SVG fragments. Durations are accepted as floating-point seconds, frame
counts are generally derived through multiplication and truncation, and
`dot_grid_seq` owns a hidden `std::random_device`-seeded generator.

That design was productive: the Asama artifacts demonstrate that sequence
fragments can become substantial encoded visual works. It also exposes the
next API boundary. The header does not describe a complete frame, time base,
seed, source hash, artifact identity, output profile, or encoder handoff.

The historical MiL scripts demonstrate both required external operations:

- `ffmpeg-supercut-123-dupe.sh` cuts and conforms encoded sources; and
- `ffmpeg-from-png-and-wav-to-mkv.sh` combines still imagery and audio.

They also show what should not become the public contract: positional shell
arguments, hard-coded output directories, ambient codec defaults, and results
whose exact inputs and acceptance conditions are not represented by a manifest.

### MiL/Asama lineage and adaptation debt

#### Authorship and public provenance

The user explicitly identifies the eleven files under
`resources/asama-loops/*.mkv` as their own work. This is authoritative
collection-level authorship evidence for the current planning corpus. The
public Benjamin De Kosnik portfolio corroborates the work's title, artist,
date, installation form, 1080p loop practice, and visual grammar:

- <https://benjamin.dekosnik.com/video> (user-supplied portfolio route); and
- <https://benjamin.dekosnik.com/tag/video-art/> (public page observed on
  2026-08-09, including the **Asama Loops** entry).

This closes the earlier *unknown external stock* concern and is sufficient to
treat the local set as user-authored Izzi production evidence. The user's
subsequent resource-wide direction also authorizes redistribution of these
non-secret local artifacts as project resources. It does not
reconstruct unavailable historical commands, seeds, intermediate frames,
third-party source-image permissions, or the mapping from every local filename
to a displayed installation channel. Redistribution authority does not itself
promote every file to a canonical baseline, authorize provider processing, or
approve a newly derived episode; those remain explicit output decisions.

The resource-wide grant never includes a credential or other secret merely
because it is stored beneath `resources/`. In particular,
`resources/.private/SEEDANCE_KEY` remains ignored, untracked, unhashable in
retained evidence, and forbidden from redistribution. Existing attribution,
license, privacy, transformed-voice, and participant-selection requirements
continue to accompany applicable resource files and derived works.

MiL's `/home/bkoz/src/MiL.git/src/mil-svg-inscribe.h` is the originating source
for fade, blink, wink, vertical-roll, dot-grid, and optical-sound-dot effects,
as well as Asama recipes that compose those temporal effects with image layers.
Izzi imported that source in commit `9411e57ca014ae1120580ff6fb52ae98de0f72a0`
and substantially reduced it in commit
`b36c1438e110e0d48e63f92d331bba92b09f025f`, the direct ancestor of the
current sequence header.

The reduction introduced observable semantic losses:

- `fade_from_color_seq` reverses a fade whose default maximum opacity is zero,
  so its default result never becomes visible;
- `vertical_sync_roll_seq` calculates per-frame displacement but serializes
  unchanged rectangle coordinates; and
- `optical_sound_dots_seq` removed duplicated pattern frames while retaining
  the original 25-frame cadence, producing 21 fragments per nominal cycle.

`examples/sequences.cc` constructs an empty SVG and exercises none of these
behaviors. Before adding Hamonshu or roulette transitions, characterize the
current results against the MiL source, preserve known failures as explicit
tests, and repair each behavior in work separate from the canonical file
rename.

The MiL recipes also expose the right architectural boundary: temporal effect
generation is a track; image/effect layering is composition; frame naming,
SVG/PNG writing, and MKV encoding are external assembly. Complex recipes build
works from forward, reversed, repeated, and offset tracks. The evolved API
therefore needs typed track, layer, and clip composition rather than additional
monolithic functions returning raw SVG strings.

MiL remains historical/oracle evidence, not code to copy. Its recipes include
absolute paths, fixed 1920-by-1080 dimensions, ambient output directories,
static randomness, and direct file writing. Asama filenames correlate with
recipe families, but individual artifact-to-recipe mappings remain `INFERRED`
until a manifest or reproducible receipt proves them.

The current header inventory contains 29 `src/a60-*.h` public headers plus the
`a60-svg-curves-hamonshu-v2.inc` implementation fragment. Guards currently mix
`MiL_*`, `a60_*`, and `izzi_*` forms, and
`a60-svg-radial-fill-hexagon.h` has no guard. This is a concrete correctness and
identity problem, not merely cosmetic renaming.

### MMRL radial/Kusama production evidence

The user confirms that the Izzi radial and Kusama sources were used to produce
the artifacts now retained in `resources/mmrl`. This is authoritative lineage
for the source family. The exact historical checkout, input dataset, command
line, fonts, external glyph files, and per-output recipe remain `UNAVAILABLE`;
they must not be reconstructed from filenames or timestamps.

| Artifact | Observed evidence | Reproducibility status |
| --- | --- | --- |
| `mmrl-kusama-v3.svg` | SHA-256 `1723c960c6801b94f91314f321cdd2a18cd22183bccf98700742dddf2089b539`; XML-valid 22-by-17-inch SVG; 1,650 text elements, 890 circles, 334 paths, 216 radial-gradient references, and 42 image elements | User-confirmed Izzi radial/Kusama lineage; exact recipe unavailable |
| `media-metadata-media-objects-a-z.20211001.pdf` | SHA-256 `26caf2b9a93002678c3cc0e4490fc3759d723c6d7fcfce33db6926f8fd736350`; 330 pages at 621 by 630 points | User-confirmed source-family use; page-to-routine mapping unavailable |
| `de-kosnik-404-ages-v38-0.11.mp4` | SHA-256 `30a61e121a087693cce6f30d412ea7ed8983d187560f85e11ce53a89c01b5c58`; 1,920 by 1,080, 50 fps H.264 plus AAC, 23.86 seconds | User-confirmed source-family use; frame recipe unavailable |
| `de-kosnik-404-ages-v38-0.11.mkv` | SHA-256 `d6068d59747fae9badb1ca4e3d5845f774e604dbfedf79712058a1e64458a8b0`; 1,920 by 1,080, 50 fps H.264 plus AAC, 23.88 seconds; probe identifies an ISO Base Media/MP4 container despite the `.mkv` suffix | User-confirmed source-family use; container naming must be validated rather than trusted |
| `image.chord/{svg.fixed,png}` | Nine XML-valid SVGs paired stem-for-stem with nine 1,920-by-1,080 PNGs; sorted path-and-content inventory digest `aebbafa8170b5e9b443dfc2983e0e8eb7111f44751cb55babfa8cb0ad8ac7602` | User-confirmed output of `a60-svg-graphs-chord.h`; original data, generator command, and meaning of the retained `svg.fixed` stage unavailable |

The SVG visibly preserves the vocabulary worth extending: central anchors,
concentric rings and halos, value rays, satellite glyphs, clustered labels,
directional marks, and repeated small-multiple compositions. It also exposes
concrete portability and accessibility debt. Its 42 image elements refer to
two legacy `alpha60-data/identity` files, 21 times each; neither relative target
resolves from this corpus. A local Inkscape render therefore completes with
missing-asset diagnostics. The document has 80 symbol-level `aria-label`
attributes, but no document-level SVG `title` or `desc`, no focus order, and no
retained structured-data alternative.

The current repository contains three compact Kusama examples and seven radial
text examples, all already included in the 106-example visual-fidelity gate.
The Kusama examples call `kusama_ids_per_uvalue_on_arc`, which mutates an
`svg_element` while also returning it by value. Their behavior depends on
process-wide mutable settings for radial range, label spacing, minimum ring
size, minimum satellite distance, and identifier render-state caches. The
public call ends in three adjacent booleans for weighting, collision handling,
and label sorting. Collision thresholds and high-orbit displacement are
embedded heuristics keyed to value maxima and fixed radius multiples.

Those facts argue for a compatibility-preserving redesign. They do not argue
for discarding the visual language that made the artifacts distinctive.

The chord corpus adds a second production idiom. Its flattened diagrams place
ordered age categories on a horizontal axis, birth-age bars above the axis,
playing-age bars below it, and colored relationship lines between them. Across
the nine SVGs there are 34–55 lines, 8–32 rectangles, and 43–76 text elements
per composition. Each has a filename-derived SVG `title`, but none has a
`desc`, ARIA structure, keyboard semantics, or an attached relation table.

The generating header is more domain-specific than its name suggests. It
imports `a60-metadata.h`, installs `using namespace a60::metadata` at header
scope, hard-codes age bands and the “playing” grammar, discovers JSON files,
caches and serializes metadata, computes aggregates, lays out marks, writes SVG,
and prints diagnostics from one header. It also uses namespace-scope definitions
that are unsafe across translation units, assumes expected cumulative-data
indices exist, and can divide by zero when both displayed totals are empty.
There is no chord example or focused test in this checkout. The retained MMRL
pairs are therefore essential production evidence, but not yet reproducible
baselines.

## Layered video model

### 1. Timeline primitives

Introduce small value types with checked construction:

```cpp
namespace svg::video {

struct frame_rate {
  std::uint32_t numerator;
  std::uint32_t denominator;
};

struct frame_extent {
  std::uint32_t width;
  std::uint32_t height;
};

struct frame_span {
  std::uint64_t first;
  std::uint64_t count;
};

} // namespace svg::video
```

All resolved timing uses integer frames and a rational rate. Authoring JSON may
use seconds, but resolution must reject values that do not land on frame
boundaries. The core never infers a frame rate from an encoder or container.

### 2. Lazy SVG sequence

Replace eager fragment vectors as the new primary interface with a lightweight
sequence descriptor and frame renderer:

```cpp
struct sequence_descriptor {
  std::string_view effect_id;
  frame_extent canvas;
  frame_rate rate;
  std::uint64_t frame_count;
  std::uint64_t seed;
};

class frame_sequence {
public:
  const sequence_descriptor& descriptor() const noexcept;
  std::string render_fragment(std::uint64_t frame) const;
};
```

The exact representation may use a C++20 range or callback rather than this
class shape. The invariant matters: callers can render frame `n` without
materializing every prior string, and the descriptor is sufficient to validate
the requested frame.

Existing fade, blink, wink, vertical-sync, optical-dot, and swipe behavior
should receive typed configuration objects. Hamonshu and roulette should enter
through the same effect registry. No generator may own hidden randomness;
random engines or recorded seeds are explicit inputs.

Legacy functions may forward to the new implementation during migration, but
their eager allocation and historical rounding behavior must be documented.

### 3. Sequence artifact

An encoded Asama loop is not a frame generator; it is a materialized sequence
artifact. Represent it with its lineage:

```cpp
struct encoded_sequence_artifact {
  std::filesystem::path path;
  sha256_digest content_hash;
  frame_extent encoded_extent;
  frame_rate encoded_rate;
  std::uint64_t frame_count;
  std::string generator_id;
  std::string generator_version;
  std::optional<sha256_digest> generator_manifest_hash;
};
```

The C++ type describes and validates metadata; it does not probe files itself
or link FFmpeg. A CLI or application adapter supplies observed probe facts, and
the library checks them against the manifest.

The Asama collection should receive one provenance manifest per artifact with:

- exact MKV hash and media probe;
- generating sequence/effect identity where recoverable;
- canvas, frame rate, frame count, duration, palette, and seed status;
- source image or data hashes where available;
- `OBSERVED`, `USER-ASSERTED`, or `UNAVAILABLE` evidence labels; and
- retained posters/contact sheets and accessibility-review status.

Do not invent missing historical seeds or generation commands. The present
user-supplied lineage is authoritative context; exact historical parameters
remain unavailable until separately evidenced.

### 4. Episode and series assembly

The form belongs above individual sources:

```cpp
enum class fit_policy { contain, cover, designed_extension };
enum class fill_policy { trim, loop_then_trim, freeze_last_frame };

struct episode_spec {
  std::uint32_t number;
  std::uint64_t title_frames;
  std::uint64_t content_frames;
  std::string source_id;
  std::uint64_t source_first_frame;
  fit_policy fit;
  fill_policy fill;
};

struct series_spec {
  frame_extent canvas;
  frame_rate rate;
  std::vector<episode_spec> episodes;
};
```

Validation for the confirmed production profile requires 60–90 episodes and
60–180 seconds of content per episode. A separately named synthetic profile may
use ten episodes while retaining the same per-episode timing and title-card
semantics. Test-only exceptions must be explicit profile data, not relaxed
global validation.

An episode references one source in synthetic check 01. A future multi-source
episode is a new structure, not an overloaded interpretation of the duration
range.

### 5. Command-neutral render plan

Resolution produces a complete, immutable plan containing:

- canonical project and episode identifiers;
- exact input paths and hashes;
- frame-based source offsets and durations;
- title-card SVG/PNG identity and accessible name;
- canvas, rate, pixel format, fit and fill policies;
- transition identifiers, versions, parameters, and seeds;
- expected output frames, duration, streams, and path;
- static poster/contact-sheet requests; and
- provenance and privacy labels.

The plan contains no API key, authorization header, signed URL, account
identity, or provider-specific secret. It is safe to validate without network
access and refuses an input hash mismatch before invoking an adapter.

### 6. External adapters

Keep process and provider integrations outside the header-only library:

```text
izzi core                 validates JSON and resolves frame-exact plans
izzi-sequence-render      emits SVG/PNG frames and generation manifests
izzi-video-assemble       maps a validated plan to FFmpeg/mkvmerge
izzi-provider-*           optionally materializes an encoded source artifact
```

The assembler should replace the useful behavior of the MiL scripts with
quoted arguments, allowlisted paths, explicit codecs, deterministic container
metadata where supported, resumable episode outputs, and post-encode probes.
It should emit command receipts without embedding secrets or private signed
URLs.

A provider adapter may create a candidate artifact, but every provider result
must pass the same probe, conform, snapshot, accessibility, and human-review
gates as a local Asama artifact. Rejection of a provider image must not require
changing the series API.

## Guilloche futures: roulette as a complete pattern language

### Intent

The current roulette header is a sound atomic beginning. It validates and
samples a trochoid, epitrochoid, or hypotrochoid and serializes one SVG path.
Full guilloche is not another formula beside those functions. It is a
composition system that generates related path families, modulates them,
repeats them into rosettes or bands, combines them into fields and ink layers,
and can resolve the same design for SVG, print, WebAssembly, or frame
sequences.

Keep the existing curve functions small and independently useful. Build
guilloche above them:

    roulette or harmonic centerline
      -> deterministic modulation stack
      -> related path family
      -> rosette, band, corner, field, or mask
      -> styled plate and semantic scene
      -> SVG, PDF, PNG, WebAssembly, or video frames

This preserves the elegance of curves-roulette while making it possible to
create dense historical, modernist, data-bearing, and intentionally
unrestrained pattern systems.

### Historical reference without imitation

The French reference should begin with the
[Banque de France historical archive](https://archives-historiques.banque-france.fr/fr),
which exposes a general catalogue and chronology of issued notes, and the
Banque de France/Citéco
[banknote-design resource](https://www.citeco.fr/dessine-moi-un-billet-0).
Citéco describes banknote design as a synthesis of a clear visual message,
cultural identity, legal constraints, security features, and creative
ingenuity. Izzi should study the visible historical vocabulary—fine engraved
line families, rosettes, medallions, frames, nested symmetry, tonal fields,
negative-space reserves, and multi-ink registration—as compositional evidence.
It should not copy a particular note or purport to implement its security
features.

The Dutch reference should use De Nederlandsche Bank's studies
[Designing Banknote Identity](https://www.dnb.nl/en/publications/research-publications/occasional-study/nr-3-2012-designing-banknote-identity/)
and
[Banknote Design for Retailers and Public](https://www.dnb.nl/en/publications/research-publications/occasional-study/nr-4-2010-banknote-design-for-retailers-and-public/).
The first explicitly discusses the design freedom given to Dutch designers
Robert Deodaat Emile Oxenaar and Jaap Drupsteen; the second treats public and
retailer recognition as design concerns. The proposal draws an interpretive
design lesson from their work rather than claiming a DNB taxonomy: combine
memorable abstract geometry, assertive color fields, asymmetric placement,
unconventional information hierarchy, and public legibility with the
line-generating precision of guilloche.

The useful synthesis is not “make a fake old note.” It is:

- French line craft for density, layering, framing, and visual tactility;
- Dutch design freedom for color, scale, asymmetry, and identity;
- Izzi orbit/Kusama language for exploratory relationships and eccentricity;
  and
- contemporary accessibility and provenance for human use.

Historical images remain research references. They should not enter test
fixtures until their copyright, reproduction, attribution, and redistribution
status is reviewed.

### Geometry and composition model

Separate seven concepts so that neither style nor animation leaks into the
underlying curve mathematics:

1. **Centerline.** A validated roulette, cycloid, rose, harmonic, Lissajous, or
   caller-supplied sampled path with explicit closure behavior.
2. **Modulation.** Phase, radius, amplitude, frequency, normal-offset, taper,
   envelope, coordinate warp, and controlled discontinuity functions applied
   in a declared order.
3. **Family.** Deterministic related paths produced by phase stepping, normal
   offsets, scale stepping, rotation, reflection, or parameter interpolation.
4. **Composition.** Rosettes, medallions, borders, corner turns, ribbons,
   interference fields, clipping windows, masks, and repeated tiles.
5. **Inscription.** A word, number, or mixed string plus a deterministic glyph
   source, resolved outline or stroke geometry, layout mode, semantic role, and
   provenance. It may become a centerline, mask, reserve, fill region, or path
   family without losing the original string.
6. **Plate.** A named visual layer with stroke, width, opacity, blend policy,
   clip, ink or display color, z-order, and accessible purpose.
7. **Scene.** A resolved, immutable collection of plates, stable identifiers,
   bounds, diagnostics, semantics, animation channels, and provenance.

A possible C++20 surface is:

    namespace izzi::guilloche {

    struct path_spec;
    struct modulation_spec;
    struct family_spec;
    struct rosette_spec;
    struct band_spec;
    struct field_spec;
    struct inscription_spec;
    struct plate_spec;
    struct scene_spec;

    resolved_scene resolve(const scene_spec&);
    std::string render_svg(const resolved_scene&);

    } // namespace izzi::guilloche

The concrete representation may use variants and spans, but it should remain
header-only, value-oriented, deterministic, and free of ambient I/O. Resolution
returns structured diagnostics. It rejects non-finite geometry, impossible
closure, invalid clipping, and unbounded work before allocation.

Retain the current exact rational-closure behavior for centered roulettes.
Add an explicit open-cycle mode for experimental irrational or deliberately
near-commensurate ratios. Fixed sampling remains available for compatibility
and deterministic animation; adaptive subdivision may be selected for print
quality using declared geometric-error and point-count limits. Neither mode may
silently cross the existing resource ceiling.

### Pattern vocabulary

The first complete vocabulary should include:

- single and nested rosettes with radial, elliptical, and warped envelopes;
- medallions with cut-out centers and caller-supplied semantic content;
- straight, circular, and path-following bands;
- corner continuations that preserve phase through a frame;
- nested orbit nets and braided line families;
- line-density fields that create engraved tonal regions without raster fill;
- two-plate and multi-plate registration studies;
- positive and negative masks for reveals, irises, windows, and graph regions;
- outlined words, digits, and mixed inscriptions woven from related line
  families or retained as negative-space reserves;
- path-following repeated inscriptions whose spacing, direction, and legibility
  are explicit rather than incidental;
- moiré and beat-frequency fields from close but distinct path families;
- explicit overprint, multiply, screen, and source-over layer policies;
- seamless rectangular, polar, and caller-defined tiling; and
- data-driven variants whose parameter mapping and legend are retained.

Moiré is not merely an error state. It is a useful way to make relative phase,
frequency, drift, and interference perceptible. The system should therefore
support both designed moiré and accidental-moiré diagnostics.

### Words and numbers as guilloche geometry

Guilloche is allowed to form words, numbers, and mixed inscriptions. Text is
not limited to a label placed above a pattern. The source glyph geometry may:

- provide centerlines for braided or phase-stepped curve families;
- clip dense fields so the inscription appears as engraved linework;
- reserve unprinted negative space inside a multi-plate composition;
- repeat along a band, orbit, spiral, or caller-supplied path;
- distort through a bounded warp while retaining the unwarped source string;
  or
- map data to individual glyph density, phase, plate, or interference.

Arbitrary font shaping is not silently embedded in the header-only core. A
deterministic inscription uses either a small versioned Izzi stroke-glyph set
or caller-supplied resolved glyph outlines. External shaping adapters may
produce those outlines, but the resolved plan records the original UTF-8
string, glyph IDs, outline hash, font-asset hash and license, shaping engine and
version, writing direction, and layout transform. Missing glyphs and
native/WebAssembly outline mismatches are errors, not font substitutions.

Decorative inscriptions may expose the source string only in provenance and
remain absent from the reading order. Informative words or numbers retain an
accessible live-text equivalent, deterministic reading order, and structured
data where applicable; thousands of decorative paths must not be announced as
thousands of glyphs. Warped text that is intentionally difficult to read is
labeled as visual texture and cannot serve as the only presentation of required
information.

### Experimental and “crazy patterns” profile

Provide a named experimental profile rather than hiding expressive behavior
behind undocumented parameter combinations. It may combine:

- close-frequency layers that form large moving interference envelopes;
- multiple centers of rotation and nonconcentric coordinate systems;
- phase discontinuities, registration slips, missing-line intervals, and
  glitch fractures;
- explicit-seed jitter, parameter mutation, and bounded stochastic omission;
- polar, sinusoidal, lens, vortex, and caller-supplied coordinate warps;
- very dense line families, extreme aspect ratios, and asymmetric cropping;
- multiple incompatible symmetry orders in one composition; and
- slowly changing blend, phase, and color-plate relationships.

“Experimental” is permission to be visually unruly, not permission for hidden
randomness, unbounded allocation, invalid coordinates, or irreproducible
results. Every stochastic choice uses a recorded seed. Every resolved scene
records point, path, layer, and estimated SVG-byte budgets. A budget overflow
produces a useful diagnostic and a lower-density preview option; it does not
silently simplify the artwork.

Three render profiles keep exploration and accessibility from being confused:

- **accessible-static:** a nonanimated, reduced-density composition with
  checked mark contrast and an equivalent description or data view;
- **motion-safe:** animated, pausable, reduced-motion-aware, and accepted only
  after temporal-contrast and flash analysis; and
- **experimental:** full static or animated moiré and wild composition,
  always permitted for local render, export, retention, and artistic review,
  even when it is not eligible for a WCAG claim.

The profiles share geometry and provenance. They differ in output policy, so a
safe alternative is a faithful view of the same idea rather than an unrelated
fallback. A local experimental artifact is not rejected or automatically
simplified because it exceeds a public motion, flash, contrast, or complexity
gate. It retains those diagnostics and an accurate `LOCAL-EXPERIMENTAL` label;
promotion to a public or claimed-conformant profile remains separately gated.

### Duotone, video transitions, and temporal form

Guilloche should enter the sequence registry as resolved geometry with
frame-addressable parameters. Useful transition families include:

- rosette iris and medallion aperture;
- line-family draw-on and erase;
- phase drift into a moiré beat;
- registration slip between color plates;
- orbit-to-guilloche and graph-to-guilloche morphs;
- band wipe and engraved-field dissolve; and
- glitch fracture followed by deterministic geometric repair.

For Duotone 111, this provides a locally authored transition grammar. A noir
scene can lose selected line intervals, enter silence, slip out of registration,
and let the same orbit or radial structure bloom into dense psychedelic
guilloche. Because identity-bearing geometry persists through the rupture, the
fantasy reads as a transformation of the recorded scene rather than generic
provider neon. Some episodes may remain silent or hold on a static high-density
field; motion is not mandatory.

Animation interpolates a resolved parameter channel, not independently sampled
SVG strings. Phase uses unwrapped values; closure, frame count, seed, and
sampling policy are fixed before rendering. This permits exact reverse, loop,
hold, and ping-pong tracks and makes a transition independently testable from
FFmpeg assembly.

Animated moiré may create apparent flicker even when individual strokes change
slowly. Public web and WCAG-oriented outputs must therefore provide pause,
stop, or hide controls, honor reduced motion, and pass the applicable
[WCAG 2.2 motion and flash criteria](https://www.w3.org/TR/WCAG22/#three-flashes-or-below-threshold).
A failed flash analysis blocks only the motion-safe or claimed-conformant
artifact. It does not erase the experimental source or prevent a static review
render.

### Data visualization and semantics

Guilloche becomes useful for visual thinking when its parameters carry
traceable meaning. A graph or scientific-notation binding may map:

- value to radius, line displacement, or band width;
- category to plate, dash grammar, or symmetry family;
- uncertainty to envelope width or phase spread;
- time to phase or registration offset;
- relationship strength to braid count or line density; and
- anomaly state to a bounded discontinuity or missing-line interval.

These mappings belong in the resolved plan and its legend. Pattern density or
color alone must never be the only carrier of required information. Dense
informative scenes receive a structured table or long description; purely
decorative plates are marked as decorative and omitted from the semantic
reading order. Interactive WebAssembly views expose keyboard-addressable
controls for density, phase, plate visibility, motion, and reduced complexity.

### JSON interface

The JSON form should describe intent and parameters, not serialized SVG:

    {
      "$schema": "https://izzi.example/schemas/guilloche-v1.schema.json",
      "version": 1,
      "kind": "guilloche",
      "profile": "experimental",
      "seed": 20260809,
      "canvas": { "width": 1080, "height": 1920 },
      "composition": {
        "kind": "moire-field",
        "centerline": {
          "kind": "hypotrochoid",
          "fixed_radius": 11,
          "rolling_radius": 7,
          "point_distance": 9.5
        },
        "family": {
          "count": 48,
          "phase_step_degrees": 2.25,
          "normal_offset_step": 0.7
        },
        "modulators": [
          {
            "kind": "sinusoidal-radius",
            "amplitude": 0.08,
            "frequency": 13
          }
        ]
      },
      "inscriptions": [
        {
          "id": "episode-mark",
          "content": "DUOTONE 111",
          "geometry": "outline-family",
          "glyph_source": {
            "kind": "resolved-outline-set",
            "asset_id": "house-display-uppercase-v1",
            "sha256": "<required-sha256>"
          },
          "semantic_role": "label"
        }
      ],
      "plates": [
        { "id": "ink-a", "color": "#123C69", "stroke_width": 0.8 },
        { "id": "ink-b", "color": "#E64A2E", "phase_offset": 0.035 }
      ],
      "motion": {
        "kind": "phase-drift",
        "seconds": 10,
        "reduced_motion": "static-midpoint"
      },
      "accessibility": {
        "purpose": "decorative-transition",
        "flash_policy": "analyze-and-block-public-output"
      }
    }

Schema validation must distinguish unknown fields, unsupported combinations,
resource-limit failures, and accessibility-policy failures. Native C++ and
WebAssembly must resolve the same normalized document and stable scene IDs.

### Reproduction and safety boundary

This feature is for decorative art, scientific illustration, data
visualization, and temporal composition. It does not implement or certify
banknote security. Examples must avoid denominations, currency names, central
bank marks, seals, signatures, serial numbers, portraits from real notes,
current-note color/layout combinations, and note-like dimensions.

This boundary does not prohibit user-supplied words or numbers. It separates a
general inscription engine from shipped examples that could be mistaken for
currency or official documents. Izzi may demonstrate numeric and textual
guilloche with unmistakably artistic, scientific, graph, title-card, or
episode-identification content.

The
[European Central Bank reproduction rules](https://www.ecb.europa.eu/euro/banknotes/images/html/index.en.html)
apply even to individual design elements when they appear on a banknote-like
background. Izzi's examples should instead use synthetic data, obviously
noncurrency text, art/graph/video aspect ratios, and clearly non-note
compositions. The project should record this boundary in example manifests and
documentation without claiming that a software guard can determine legal
compliance.

### Implementation and verification

Develop the feature as reviewable layers:

1. Freeze exact output and error behavior for the existing roulette primitives.
2. Extract deterministic sampled geometry from SVG path serialization.
3. Add modulation and related-path families without styling or animation.
4. Add rosette and band composition with exact closure tests.
5. Add deterministic word/number inscription geometry, outline provenance,
   negative-space reserves, and accessible source-string retention.
6. Add multi-plate scenes, clipping, masks, and print-aware stroke policies.
7. Add moiré fields and the explicit experimental profile with resource
   budgets.
8. Add versioned JSON and native/WebAssembly normalization parity.
9. Add frame-addressable motion channels and sequence-registry adapters.
10. Add data bindings, legends, semantic alternatives, and reduced-complexity
   views.
11. Add SVG, PNG, PDF, and ten-second video review packages before baseline
    promotion.

Required examples should include a French-lineage rosette study, a
Dutch-modernist asymmetric plate study, a monochrome print band, a data-bound
orbit guilloche, a WCAG-oriented static alternative, a motion-safe transition,
an outlined word-and-number guilloche sheet, and an unapologetically dense
set of two or three moiré/crazy-pattern wild plates. Names describe
inspiration and behavior; they must not reproduce or imply endorsement by a
central bank.

**Post-anchor requirement, 2026-08-09 (OBSERVED user direction).** Build an
expanded guilloche candidate grid covering every implemented path family,
rosette, band, plate, inscription, data-bound form, and motion transition. Each
applicable row must include the compatibility, accessible-static, motion-safe,
and unrestricted local `experimental`/wild profiles rather than using one wild
showcase as a proxy for the other variants. Label every cell with its stable
variant ID, profile, deterministic seed, and compact parameter summary. Retain
the source SVG, a raster review tile, and hashes. Animated variants also require
a frame-addressed filmstrip with the start, intermediate extrema, loop seam,
and end represented. Keep this expanded grid as a candidate until a human pass
explicitly promotes it; diagnostics may annotate wild cells but must not remove,
simplify, or substitute them locally.

**Post-anchor review-notification requirement, 2026-08-09 (OBSERVED user
direction).** After the guilloche implementation and its automated tests pass,
notify the user in the active work thread with two or three distinct wild
plates ready for visual review. The plates must exercise materially different
wild compositions rather than presenting parameter-near duplicates. At least
one plate must use the number `111` and the exact letters `duotone` as guilloche
source geometry, retaining both source strings in its accessible description
and provenance. It must remain unmistakably experimental artwork rather than
currency imitation. Do not send a completion notice while implementation or
tests are incomplete, and do not treat delivery of the plates as human
acceptance or baseline promotion.

**Art-preservation invariant, 2026-08-09 (OBSERVED user direction).** Always
allow the art in locally authorized experimental work. Accessibility, motion,
contrast, flash, print-survival, and publication diagnostics may annotate a
wild artifact, require an alternative presentation, or block a conformance or
release claim; they must never delete, sanitize, simplify, substitute, or make
the original local artwork unavailable. Hard failures are limited to finite
bounds, declared resource ceilings, invalid geometry, and host safety—not
aesthetic acceptability.

Verification covers mathematical closure, finite bounds, deterministic IDs and
bytes, declared resource ceilings, empty and extreme inputs, raster
cross-rendering, print line survival, reduced-motion behavior, pause controls,
flash analysis, semantic alternatives, and human visual review. Candidate
grids belong in the existing production render-fidelity system and remain
separate from accepted baselines until explicitly passed.

## Graph API: retain the orbit language, simplify the machinery

### Design boundary

The graph implementation should become four explicit layers:

1. **Data:** stable identifiers, labels, numeric measures, groups, directed or
   undirected relationships, annotations, and source provenance.
2. **Specification:** a typed layout strategy plus its scale, ordering,
   collision, label, visual-preset, accessibility, and output policies.
3. **Layout:** a deterministic, renderer-neutral scene of positioned nodes,
   relationships, marks, label boxes, reading order, and diagnostics.
4. **Rendering:** SVG, printable PDF/SVG, PNG, WebAssembly DOM, or frame
   sequences produced from that scene.

Layout must not read files, mutate a global cache, write SVG, or depend on
ambient randomness. Rendering must not decide the data domain or silently move
colliding values. This separation lets one resolved scene drive a large-format
print, a static accessible web graphic, and an animated transition without
three interpretations of the data.

### Reconciliation without conflation

Radial/Kusama and flattened chord should not share one geometry algorithm or a
bag of optional parameters. They answer different questions:

- **Radial/orbit** emphasizes distribution, magnitude, clustering, satellites,
  and exceptional values around a center.
- **Flattened chord** emphasizes directed relationships between two positions
  in an ordered categorical domain, while retaining marginal totals.

They can be reconciled above and below layout. Above layout they share document
identity, source data, stable entity IDs, measures, annotations, style tokens,
accessibility intent, and provenance. Below layout they share semantic scene
primitives, rendering, interaction, export, hashing, and verification. At the
layout boundary they remain different discriminated types with different
validation rules.

The same relational dataset may support two explicit projections. For example,
an age-to-playing-age relation can become a flattened chord view of pairwise
flows and a radial/Kusama view of marginal totals and clustered identities.
Those views are complementary, not interchangeable. The resolved document must
record the projection that derived each view, and linked selection uses stable
IDs to highlight corresponding evidence across both.

This avoids both failure modes: two disconnected bespoke systems, and one
“universal graph” abstraction that erases what each visual form is good at.

### Experimentation is the primary interaction

These forms are a visual language for experimenting with data so relationships
become easier for humans to understand. Publication is one outcome, not the
starting assumption. The system should support a recorded exploration loop:

```text
source data + question
  -> two or more explicit projections
  -> small visual candidates + semantic summaries
  -> human comparison, emphasis, and correction
  -> resolved view set
  -> accessible web, print, or motion artifacts
```

Changing order, scale, radial range, collision policy, label density, grouping,
or visual emphasis creates a new derived specification; it never mutates the
source data. Side-by-side and small-multiple views should make those choices
comparable. Agents may propose variants and explain what each encoding reveals,
but must preserve the question, data lineage, and user selection rather than
silently choosing the most conventional chart.

### Contemporary C++20 shape

A small typed interface can replace mutable getters and boolean-heavy free
functions while remaining header-only:

```cpp
namespace svg::graph {

struct orbit_item {
  std::string id;
  std::string label;
  double value;
  std::vector<std::string> satellites;
  std::optional<std::string> description;
};

struct radial_dataset {
  std::string id;
  std::vector<orbit_item> items;
};

struct relation_node {
  std::string id;
  std::string label;
};

struct weighted_relation {
  std::string source_id;
  std::string target_id;
  double value;
  std::optional<std::string> description;
};

struct flow_dataset {
  std::string id;
  std::vector<relation_node> nodes;
  std::vector<weighted_relation> relations;
};

using dataset = std::variant<radial_dataset, flow_dataset>;

struct angular_domain {
  double start_degrees;
  double end_degrees;
};

enum class satellite_scale { constant, by_value };
enum class collision_policy { none, outward_orbit, compact_splay };
enum class label_order { input, identifier, shortest_first };

struct radial_orbit_spec {
  angular_domain angles;
  double base_radius;
  double ring_spacing;
  double minimum_satellite_radius;
  double minimum_satellite_gap;
  satellite_scale sizing;
  collision_policy collisions;
  label_order labels;
  std::string visual_preset;

  static radial_orbit_spec mmrl_defaults();
};

struct linear_chord_spec {
  std::vector<std::string> category_order;
  double vertical_scale{0.7};
  double bar_width{10.0};
  bool show_marginal_totals{true};
  std::string visual_preset{"mmrl-linear-chord"};
};

using layout_spec = std::variant<radial_orbit_spec, linear_chord_spec>;

struct graph_document {
  std::string id;
  dataset data;
  layout_spec layout;
  accessibility_spec accessibility;
};

struct layout_result {
  graph_scene scene;
  std::vector<diagnostic> diagnostics;
};

layout_result layout(const graph_document&);
layout_result layout_radial_orbit(const radial_dataset&,
                                  const radial_orbit_spec&);
layout_result layout_linear_chord(const flow_dataset&,
                                  const linear_chord_spec&);
void render(svg_element&, const graph_scene&, const graph_theme&);

} // namespace svg::graph
```

The precise names may change during API review, but the boundaries should not.
The `kusama` and `mmrl-linear-chord` presets remain public and named.
`radial-orbit` and `linear-chord` are the generic layout vocabularies beneath
them, not replacement aesthetics. Compatibility adapters can translate the
current `id_value_umap` and `vumids` plus settings into typed documents and then
render the resulting scenes into the caller's `svg_element`.

### Corpus-defined radial defaults

The default radial/orbit specification is the supplied MMRL visual language,
not a new set of aesthetically tidy constants. An omitted radial layout in the
authoring API resolves to `radial_orbit_spec::mmrl_defaults()` and the public
`kusama` preset. Stage 1 characterizes the relevant current source behavior and
the retained MMRL samples, then freezes the exact angular domain, scale,
radius/ring relationships, satellite sizing, collision behavior, label order,
and dense-label fallback in a normalized resolved fixture.

Until that characterization is complete, numeric values in explanatory JSON
are explicit example overrides and must not be mistaken for the accepted
defaults. The resolved JSON always expands every corpus-derived value. A future
cleaner, responsive, print-specific, or WCAG-oriented policy receives a named
preset and a comparison grid; it cannot silently change what a default render
means. The user-authorized redistribution of non-secret `resources/` files
allows the MMRL samples needed for this compatibility fixture to ship with
their attribution and retained provenance.

The new code should also:

- accept `std::span` views where ownership is unnecessary;
- use strong angle, radius, and extent values at validation boundaries;
- return structured diagnostics instead of writing zero-value omissions to
  `std::clog`;
- reject an empty dataset, non-finite value, invalid domain, zero maximum, or
  impossible geometry before indexing or dividing;
- preserve input IDs and deterministic tie-breaking in every scene;
- reject a dataset/layout mismatch unless an explicit projection converts it;
- make heuristic choices visible in the resolved plan; and
- avoid process-wide mutable state so concurrent and WebAssembly renders do not
  contaminate one another.

### Extensions that belong in the model

The existing language can grow without adding one-off rendering functions:

- multiple concentric measures with a shared angular domain;
- nested satellite groups and explicit relationship edges;
- ordered linear-chord domains with directed, weighted, or bidirectional flows;
- linked radial and chord projections with synchronized selection;
- small multiples with one comparable scale and synchronized highlighting;
- uncertainty bands, ranges, thresholds, and annotated reference arcs;
- categorical sectors combined with continuous radial position;
- stable transitions between orbit layouts for temporal data;
- data-driven entry, exit, pulse, blink, wink, Hamonshu, and roulette motion;
- responsive web views that preserve a separately designed print composition;
- retained candidate sets that explain which relationship each view emphasizes;
  and
- linked detail panels, tables, source notes, and downloadable JSON/CSV.

Animation operates on stable scene IDs. It interpolates two resolved scenes or
applies a named transition to one scene; it does not rerun collision heuristics
independently on every frame. This makes graph video deterministic and gives
`prefers-reduced-motion` a direct static-scene fallback.

### Accessible graph contract

The visual preset and the semantic representation are parallel outputs. A
WCAG-oriented graph profile should require:

- a concise title and purpose-specific description at the SVG root;
- a deterministic linear reading order independent of radial screen position;
- one accessible group per datum or relation with label, value, units,
  endpoints, relationships, and annotation text;
- an adjacent structured table or long-description target for dense graphs;
- keyboard focus and the same disclosure available on focus as on hover;
- non-color encodings for states and relationships;
- checked text/background and mark/background contrast tokens;
- minimum interactive target spacing in interactive profiles;
- no mandatory motion and a reduced-motion rendering path; and
- retained source, units, definitions, and update date.

Automated checks can verify structure, contrast inputs, stable names, keyboard
bindings, data equivalence, and reduced-motion variants. They cannot by
themselves certify WCAG 2.2 conformance; representative assistive-technology
and human review remain explicit gates.

### Graph JSON interface

The authoring form should be compact enough for a person or AI agent, while a
resolved form records every default and heuristic. A single radial view remains
straightforward:

```json
{
  "schema_version": "izzi.graph/1",
  "graph_id": "mmrl-age-distribution",
  "kind": "radial-orbit",
  "data": {
    "value_label": "age band count",
    "items": [
      {
        "id": "age-30-39",
        "label": "30–39",
        "value": 71,
        "satellites": ["source-a", "source-b"]
      }
    ]
  },
  "layout": {
    "angles_degrees": [10, 350],
    "base_radius": 80,
    "ring_spacing": 16,
    "satellite_scale": "by-value",
    "collision_policy": "outward-orbit",
    "label_order": "identifier"
  },
  "style": {
    "visual_preset": "kusama",
    "accessibility_profile": "wcag-2.2"
  },
  "accessibility": {
    "title": "Age distribution",
    "summary": "Radial distribution of age-band counts with source satellites",
    "details_ref": "#age-distribution-table"
  },
  "outputs": [
    { "format": "svg", "profile": "large-format-22x17" },
    { "format": "svg", "profile": "responsive-web" }
  ]
}
```

A reconciled exploration document references shared data and declares distinct
views and projections instead of forcing both layouts through one option set:

```json
{
  "schema_version": "izzi.graph-workspace/1",
  "workspace_id": "mmrl-age-relationships",
  "question": "How do recorded and playing ages relate?",
  "datasets": [
    {
      "dataset_id": "age-flow",
      "kind": "directed-flow",
      "nodes": [
        { "id": "age-30-39", "label": "30–39" },
        { "id": "age-50-59", "label": "50–59" }
      ],
      "relations": [
        {
          "source_id": "age-30-39",
          "target_id": "age-50-59",
          "value": 7
        }
      ]
    }
  ],
  "views": [
    {
      "view_id": "pairwise-flow",
      "dataset_id": "age-flow",
      "kind": "linear-chord",
      "visual_preset": "mmrl-linear-chord",
      "projection": { "measure": "relation-value" }
    },
    {
      "view_id": "target-distribution",
      "dataset_id": "age-flow",
      "kind": "radial-orbit",
      "visual_preset": "kusama",
      "projection": {
        "value": "target-total",
        "satellites": "incoming-source-ids"
      }
    }
  ],
  "linking": {
    "selection_key": "node-id",
    "synchronize_focus": true
  }
}
```

The flow-to-radial projection is named, validated, and retained in the resolved
document. It may be replaced with another explicit projection during
exploration; neither view silently changes the other's meaning.

Schema errors should contain a stable diagnostic code, JSON Pointer, expected
shape, observed value, and suggested correction. Defaults must be materialized
in a normalized resolved JSON document with input and schema hashes. That makes
the interface useful to AI agents without requiring them to infer overloaded
function arguments or hidden process state.

### Portable assets and provenance

Every font, glyph, logo, source image, and external data file must be either
embedded where licensing permits or represented by a declared asset with an
exact hash and resolution policy. A render that cannot resolve a required asset
fails or emits an explicit placeholder report; it may not silently produce a
nominally complete baseline. The MMRL SVG's unresolved identity references are
the regression case for this rule.

A graph artifact manifest should retain the input data hash, normalized spec
hash, Izzi revision, scene hash, output hash, output dimensions, font and asset
resolution, accessibility checks, render diagnostics, and human-review state.
Historical MMRL artifacts can enter as provenance fixtures with unavailable
fields; regenerated candidates must satisfy the full contract.

### Migration and test strategy

Keep `kusama_ids_per_uvalue_on_arc`, `h_chord_graph`, and the two
`analyze_metadata_aggregate_chord` overloads temporarily as deprecated
adapters. First freeze the three Kusama example outputs, the MMRL reference
render, and all nine matched chord pairs. Then add pure-layout tests for angles,
orbit spacing, collision promotion, flow endpoints, marginal totals, stable
ordering, empty and invalid data, and viewport bounds. Add semantic tests that
compare every rendered datum and relation with the structured alternative,
plus keyboard and reduced-motion browser tests for WebAssembly output.

The production visual-fidelity grid remains the broad regression gate. Add
separate MMRL large-format and chord-corpus candidates that record unresolved
historical assets and postprocessing stages instead of hiding them. Promotion
requires both machine comparison and explicit human approval; a pixel pass
cannot excuse missing labels, relations, reading order, or a mismatched data
table.

## Video JSON interface

Use standard JSON for authoring and resolved plans. A compact authoring example:

```json
{
  "schema_version": "izzi.video-project/1",
  "profile": "vertical-series",
  "canvas": {
    "width": 1080,
    "height": 1920,
    "frame_rate": { "numerator": 30, "denominator": 1 }
  },
  "series": {
    "episode_count": 75,
    "content_duration_seconds": { "minimum": 60, "maximum": 180 },
    "title_card_seconds": 3
  },
  "source_pool": {
    "selection": "editorial",
    "sources": [
      {
        "source_id": "asama-blue-mushroom",
        "kind": "encoded-izzi-sequence",
        "path": "resources/asama-loops/asama-oo-ee-oo-blue-2-mushroom.mkv",
        "sha256": "<64 lowercase hexadecimal characters>"
      }
    ]
  },
  "output": {
    "container": "matroska",
    "video_codec": "h264",
    "pixel_format": "yuv420p",
    "audio": "none"
  }
}
```

The resolved form expands every episode to exact frames and one immutable
source reference. Randomized tests record their algorithm, seed, ordered input
set, resolved draw, and source hashes. Production authoring should prefer
explicit editorial choices over ambient randomness.

## Canonical header rename

### Required mapping

Perform an atomic canonical rename:

```text
src/a60-svg.h                         -> src/izzi-svg.h
src/a60-svg-sequences.h               -> src/izzi-svg-sequences.h
src/a60-svg-<component>.h             -> src/izzi-svg-<component>.h
src/a60-svg-curves-hamonshu-v2.inc    -> src/izzi-svg-curves-hamonshu-v2.inc
```

This covers all 29 `a60-*.h` headers and the one `.inc` fragment. Internal
includes, umbrella includes, examples, tests, install manifests, build scripts,
documentation links, and live downstream consumers must change in the same
work item or in explicitly ordered commits that always build.

Historical receipts and frozen baseline manifests keep their original paths.
They are evidence of what was tested and must not be mechanically rewritten.

### Include guards

Canonical guards use uppercase `IZZI_`, the normalized canonical basename, and
the extension:

```cpp
#ifndef IZZI_SVG_SEQUENCES_H
#define IZZI_SVG_SEQUENCES_H 1
// ...
#endif // IZZI_SVG_SEQUENCES_H
```

Examples:

```text
izzi-svg.h                         -> IZZI_SVG_H
izzi-svg-graphs-line.h             -> IZZI_SVG_GRAPHS_LINE_H
izzi-svg-radial-fill-hexagon.h     -> IZZI_SVG_RADIAL_FILL_HEXAGON_H
```

Add the missing guard to radial-fill-hexagon and replace all surviving `MiL_*`,
`a60_*`, and lowercase `izzi_*` guards. A generated check should prove that
every canonical public header has exactly one expected guard and that guard
names are unique.

Do not combine this work with a namespace rename. The global `svg` namespace
deserves its own compatibility design and downstream review; coupling it to
filesystem identity would make regressions needlessly difficult to isolate.

### Compatibility policy

The canonical `src` tree should contain only `izzi-*` names. If alpha60,
cartofreako, or another downstream checkout needs a transition window, generate
thin deprecated `a60-*` forwarding headers into a separate compatibility
include tree. They include the matching `izzi-*` header and contain no copied
implementation.

Make compatibility installation opt-in and time-bounded, for example
`IZZI_INSTALL_A60_COMPAT_HEADERS`. Test both canonical-only and compatibility
configurations. New examples and documentation must use only canonical names.

## CMake, Make, packaging, and build changes

The current CMake target is already a C++20 `INTERFACE` library, but it has no
install/export contract. The rename should add:

- an explicit canonical public-header list or CMake header file set;
- installation under `include/` with `izzi-svg.h` as the umbrella;
- `izziTargets.cmake` export and `izzi::izzi` imported target;
- generated package version/configuration files;
- optional installation of generated compatibility headers;
- build-tree and install-tree consumer tests; and
- a failure when an unexpected `src/a60-*` file or legacy guard remains.

Using CMake `FILE_SET HEADERS` would make the public surface precise and may
justify raising the current minimum from 3.20 to 3.23. If the project retains
CMake 3.20, maintain an explicit header list and test it against the filesystem
so new headers cannot be silently omitted from installation.

Extend the Make façade with focused targets:

```text
make check-prefixes       no canonical a60 filenames/includes/guards remain
make check-headers        every izzi header is self-contained under GCC/Clang
make check-install        configure, install, and compile an external consumer
make synthetic-form-check build and verify the local ten-episode MKV fixture
make check-video          validate plans, probes, hashes, frames, and snapshots
make check-graphs         validate graph schemas, layouts, semantics, and SVG
make visual-fidelity-mmrl build a candidate MMRL large-format review package
```

`make all`, `make check`, `make check-gcc`, `make check-clang`, and
`make check-compilers` must continue to work. The migration gate includes Debug
and Release, GCC and Clang, single-header compilation, two-translation-unit ODR
tests, the 106-example matrix, and WebAssembly checks where applicable.

Do not promote new visual baselines merely because the rename is mechanically
expected to preserve output. Regenerate into a candidate directory, compare
hashes and pixels, and require the existing human visual gate.

### Production render-fidelity gate

The historical visual test—compile every top-level `examples/*.cc` program,
run it, inspect its SVG output, and arrange the results on one grid—is now a
formal conversion gate rather than an ad hoc ritual. The harness must:

- discover all 106 top-level example sources without a hand-maintained allowlist;
- compile each as C++20 in an isolated directory;
- execute every binary twice to expose hidden variation;
- preserve compile failures, runtime failures, nonvisual results, and changing
  outputs instead of excluding them from the grid;
- validate every emitted SVG as XML;
- render normalized previews with retained external-resource diagnostics;
- create content-addressed SVG, PNG, and large-format PDF grids;
- compare exact SVG bytes and decoded RGB pixels against a pinned baseline;
- emit changed-preview and whole-grid difference sheets; and
- require explicit human acceptance before baseline promotion.

`make visual-fidelity` runs the resumable candidate capture and bounded
comparison. Pinned hashes make accidental baseline mutation a hard failure, and
the comparator proves that neither the baseline nor candidate changed while it
was being read. A passing comparator means the evidence package is internally
valid; it does not silently accept a visual change.

The current conversion review pair is retained at
`outputs/izzi-example-fidelity-pre.png` and
`outputs/izzi-example-fidelity-post.png`. Both are 4,320 by 5,988 pixels and
contain the same 106-tile inventory. The first bounded comparison reports four
pixel-changing previews: three known variable
`color-band-expand-to-larger` outputs and the intentionally stabilized
`markers-1` output. Human acceptance for these two fidelity grids remains
pending. The user's 2026-08-09 human pass was explicitly limited to synthetic
form check 02 and does not close this gate.

## Asama regression corpus

Set-level authorship and public-work provenance are now documented. The Asama
loop set should become a video-API conformance corpus while per-artifact
technical manifests preserve what is observed, user-asserted, inferred, and
unavailable. Tests should cover:

- native landscape and portrait sources;
- 25-to-30 and 30-to-30 fps conformance;
- contain, reviewed crop, and designed-extension policies;
- loop seams and source offsets;
- exact title-card and content frame budgets;
- straight cuts plus fade, blink, wink, Hamonshu, and roulette transitions;
- guilloche rosette, moiré, registration-slip, and experimental pattern
  transitions under explicit motion profiles;
- silent and separately authorized audio-bearing assemblies;
- deterministic plan resolution and stable source hashes;
- contact sheets, frame-difference screens, and flash diagnostics; and
- interruption/resume without resubmitting or re-encoding verified episodes.

The corpus does not need to be bit-identical across every encoder build. The
contract should separate deterministic Izzi plans and SVG frames from
environment-sensitive encoded bytes, recording tool versions and validating
semantic media facts in both cases.

## Proposed implementation order

This order has a commit gate. Before Stage 1 changes an inventory, receipt,
source, build file, test, or generated baseline, commit this proposal and record
that commit as the stage-zero planning anchor. Stage 1 must cite that anchor;
later changes must not rewrite it as though every derived detail were original
user authority.

1. **Freeze evidence.** Inventory and hash the 29 headers, one include fragment,
   current guards, internal include graph, CMake/Make files, examples, tests,
   downstream references, Asama media facts, and the 22-file MMRL corpus.
2. **Characterize the sequence debt.** Record the current Izzi behavior and MiL
   oracle for every retained effect, including the known fade, roll, and cadence
   losses. Keep expected failures distinct from the mechanical rename.
3. **Characterize the graph debt.** Freeze the three Kusama examples, radial
   text examples, MMRL reference render, nine chord pairs, unresolved-asset and
   postprocessing diagnostics, global settings, collision heuristics, domain
   coupling, and available accessibility structure.
4. **Add migration gates.** Teach tests to detect legacy canonical filenames,
   wrong/missing/duplicate guards, stale includes, and incomplete install lists.
5. **Rename atomically.** Use Git-aware moves, update canonical guards and all
   live repository references, and leave historical receipts unchanged.
6. **Repair build/install.** Export `izzi::izzi`, install canonical headers,
   test a clean external consumer, and add the optional generated compatibility
   tree only if downstream scheduling requires it.
7. **Introduce the graph data/spec/scene boundary.** Add versioned JSON, typed
   radial and flow datasets, discriminated layout specifications, deterministic
   scenes, structured diagnostics, and no ambient state or file I/O.
8. **Adapt orbit/Kusama and chord.** Preserve both named visual presets, move
   their heuristics into explicit policies, extract metadata analysis from
   layout, and keep the old top-level functions as tested compatibility
   adapters.
9. **Add semantic graph outputs.** Generate accessible SVG structure, an
   equivalent table or long description, keyboard interaction, reduced-motion
   behavior, and large-format/WebAssembly profiles from one resolved scene.
10. **Repair existing effects.** Correct characterized semantic losses in
   separately reviewable changes with exact frame and visual tests.
11. **Introduce timeline primitives.** Add rational frame rates, exact frame
   spans, checked canvas types, diagnostics, and JSON conversion.
12. **Refactor sequence generators.** Add lazy frame access, typed effect
   configuration, explicit seeds, and adapters for the legacy functions.
13. **Build guilloche composition.** Preserve the atomic roulette output, then
   add sampled geometry, modulation, related-path families, rosettes, bands,
   plates, moiré fields, wild-pattern profiles, JSON, and frame-addressable
   transitions as separately reviewable layers.
14. **Define artifact and series manifests.** Represent graph scenes, SVG
   sequences, encoded Izzi artifacts, episode slots, source fitting, title
   cards, accessibility, outputs, and receipts.
15. **Build the external assembler.** Replace positional MiL shell behavior with
   manifest-driven FFmpeg/mkvmerge execution and exact postconditions.
16. **Review the artifact corpora.** Submit MMRL, Asama, and guilloche
   candidates to their
   separate machine, accessibility, provenance, and human-review gates before
   accepting any of them as a regression baseline.
17. **Add optional providers last.** Materialize provider output into the same
   encoded-artifact interface only after the local form is accepted.

## Acceptance conditions

- No canonical file under `src/` begins with `a60-`.
- Every canonical public header uses its exact unique `IZZI_*` guard.
- `#include <izzi-svg.h>` and component includes work from both build and
  install trees.
- GCC and Clang pass Debug/Release umbrella, self-contained-header, ODR, and
  example gates.
- Existing accepted SVG/PNG behavior is unchanged or any intentional visual
  change receives a separate candidate and human approval.
- A JSON plan can represent an Izzi-generated SVG sequence, an encoded Asama
  artifact, and a later provider artifact without changing episode semantics.
- The ten-episode synthetic test resolves to exactly its recorded sources,
  durations, 34,260 frames, and 19:02 output without provider interaction.
- The production profile rejects fewer than 60 or more than 90 episodes and
  content slots outside 60–180 seconds.
- Every video artifact retains probe facts, hashes, source lineage, title-card
  metadata, static review imagery, and accessibility-review state.
- A versioned graph JSON document resolves to the same normalized specification
  and semantic scene in native and WebAssembly builds.
- Omitted radial layout values resolve to the corpus-characterized MMRL defaults
  and reproduce the accepted MMRL compatibility fixtures; alternative defaults
  are named presets with separate review artifacts.
- The `kusama` preset preserves orbit, satellite, ray, ring, label, and
  small-multiple composition without relying on mutable global settings.
- The `mmrl-linear-chord` preset preserves ordered anchors, paired marginal
  bars, relationship lines, totals, and labels without depending on the MMRL
  metadata filesystem or “playing” grammar.
- One shared relational dataset can produce explicitly projected, linked
  radial and chord views without treating their geometries as interchangeable.
- Empty, non-finite, zero-domain, collision-heavy, and missing-asset inputs
  produce stable structured diagnostics without undefined behavior.
- Every informative graph has a root title and description, deterministic
  reading order, equivalent structured data, non-color state encoding, and
  keyboard/reduced-motion behavior where interactive.
- A required external asset can never disappear silently from a passing visual
  baseline.
- Existing roulette primitive output and validation behavior remain frozen
  until an intentional, separately reviewed compatibility change is accepted.
- Guilloche scenes resolve deterministically from typed C++ and versioned JSON,
  enforce declared path/point/byte ceilings, and expose stable layer IDs.
- The guilloche public API is rooted at `izzi::guilloche`; resolved word and
  number inscriptions retain their source string, deterministic glyph geometry,
  outline/font provenance, and an accessible equivalent when informative.
- The accessible-static and motion-safe profiles provide semantic or static
  alternatives and pass their contrast, reduced-motion, control, and flash
  gates before any WCAG claim.
- The experimental profile may retain dense moiré and crazy patterns that fail
  a public accessibility gate. Local static and animated exports remain
  available, retain accurate diagnostics, and are never silently simplified or
  substituted for the accessible alternative.
- Every locally authorized wild artifact remains renderable and retained even
  when it requires a separate accessible alternative or cannot carry a public
  conformance claim; diagnostics never become aesthetic censorship.
- The expanded guilloche candidate grid includes every implemented variant and
  its applicable wild profile, labels deterministic parameters, retains SVG and
  raster hashes, and includes frame-addressed filmstrips for motion variants;
  baseline promotion remains an explicit human decision.
- After implementation and automated verification pass, two or three materially
  distinct wild plates are delivered together for human visual review and the
  user is notified in the active work thread; delivery is not acceptance.
- At least one delivered wild plate uses `111` and the exact letters `duotone`
  as deterministic guilloche geometry, preserves both strings as accessible
  source metadata, and cannot reasonably be mistaken for currency.
- No shipped guilloche example reproduces a real note, uses current currency
  identifiers, claims security value, or adopts a banknote-like composition.
- The MMRL candidate preserves the accepted orbit/Kusama visual language across
  large-format SVG/PDF and responsive web profiles; intentional changes require
  a separately hashed human pass.

## Decisions still requiring explicit approval

- whether to ship temporary `a60-*` compatibility headers and for how long;
- whether CMake may advance from 3.20 to 3.23 for header file sets;
- the final public C++ namespace migration, if any;
- which redistributable Asama artifacts should be promoted to canonical
  baselines versus retained as optional fixtures;
- which redistributable MMRL artifacts should be promoted to regression
  fixtures and what attribution accompanies the `kusama` preset;
- whether required graph glyphs and logos are embedded, packaged as hashed
  assets, or replaced with library-owned equivalents;
- the final vocabulary for `izzi::guilloche` path families, plates, fields,
  inscriptions, and profiles;
- default resource ceilings and whether adaptive sampling is enabled for print
  profiles;
- which historical French and Dutch reference images may be retained in local
  review packages, with what attribution and redistribution status;
- whether animated experimental artifacts may also be distributed through an
  explicit expert-mode WebAssembly control; local generation, export,
  retention, and review are already allowed;
- the production source-fit policy for landscape imagery;
- the production episode-count and duration distribution inside the confirmed
  ranges; and
- any future provider transfer, budget, or publication authority.

The prefix migration, graph API, video API, corpus promotion, and provider
adapter should remain separate reviewable work items even when this document
explains their relationship.
