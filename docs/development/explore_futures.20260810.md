# Izzi futures: next-stage ideas

Date: 2026-08-10  
Status: locally authorized implementation plan; human acceptance, baseline,
publication, and transfer gates remain pending

## Purpose and boundary

This document sketches work following the historical `guilloche.v3`
text, number, and portrait-transition exploration. The current local feedback
round is `outputs/review/feedback/visual/guilloche/round-05`. It supplements rather than
replaces:

- `docs/development/explore_futures.md`, the authoritative user-supplied source plan;
- `docs/development/explore_futures.20260808.md`, the initial audit and evolution plan;
- `docs/development/explore_futures.20260809.md`, the active conversion, video, graph, and
  guilloche proposal;
- `docs/visual_workflow/visual_experiments_method.md`, the accepted living method for generation
  passes, feedback rounds, freshness, review packaging, and promotion gates;
- the local training-conversion lineage, receipts, and failure-loop evidence.

The v3 work remains historical review evidence. The later current review pass
is also only a review candidate. Authorization to generate and test either
locally is not the same as human visual acceptance, baseline promotion,
publication, provider submission, or acceptance of every idea below.

## Governing two-pass workflow

Every audio, visual, and generative-form workflow follows the same order:

1. **First pass (Stage 0): prove the complete form.** Starting with the inputs
   already supplied, apply deterministic or clearly recorded provisional
   defaults and produce a complete artifact that a human can play, view, or
   inspect end to end. Verify its format, duration, dimensions, ordering,
   provenance, and review packaging. This pass answers whether the machinery
   and the form work; it does not claim aesthetic approval or consent to a
   provisional choice.
2. **Second pass (Stage 1): refine with humans.** Begin from the visible Stage 0
   artifact. Humans may keep a default, choose alternatives, reject candidates,
   tune composition, color, speed, voice, timing, typography, and other artistic
   decisions, or decline. Preserve the Stage 0 evidence so refinement does not
   obscure whether the underlying workflow worked.

Do not make human fine-tuning a prerequisite for obtaining the first complete
artifact. Do not mistake a technically successful first pass for human
acceptance, a promoted baseline, publication authority, or provider-transfer
authority.

The generation-pass vocabulary, family-first visual review tree, four-factor
freshness rule, and `3d-synthesis` matrix in
`docs/visual_workflow/visual_experiments_method.md` govern the implementation details below.
Keep that method document current whenever a later human correction changes a
generation pass, feedback round, comparison axis, or review-package gate.

## Stage X1: converge the guilloche visual language

First, complete a human review of the current portrait guilloche package while
retaining `guilloche.v3` and `guilloche.v4` as historical evidence:

- ten digit studies;
- ten word studies;
- ten word-number studies;
- ten wild portrait transitions;
- category grids and decoded transition filmstrips.

The review should answer a small set of concrete questions:

1. Does the central glyph remain legible at phone, desktop, and print-proof
   sizes?
2. Do floral clusters feel intrinsic to the glyph rather than pasted on top?
3. Do cycloid contours read as expressive engraving rather than a pixel-font
   outline?
4. Which symmetry families belong to the Izzi visual language: bilateral,
   fourfold, radial, orbital, or deliberate asymmetry?
5. Which candidates are keepers, which contain useful parts, and which should
   remain rejected evidence?

Promote only explicitly selected keepers. Preserve rejected candidates and
their manifests so later work does not accidentally rediscover the same dead
ends.

Human feedback on 2026-08-10 affirms that this concrete generate, review,
preserve, and refine loop is working. Increase useful choice breadth without
changing that loop. A mature exploratory family may contain up to 20
candidates, but each candidate must vary one or more declared visual axes:
composition, symmetry, grouping, palette, background, density, texture, or
registration. MKV candidates must also vary motion speed or speed progression.
Changing only a seed, filename, label, or source string is not sufficient
variation. Manifests should make the axes comparable across the review set.

The strongest evidence so far is the wild composition and symmetry work,
especially the registration-slip field and left-expression layering. Continue
that family as abstract geometry: no glyphs, words, numbers, captions, or title
cards inside the motion frames. Prefer denser translucent line bundles,
bilateral and radial counterflow, longer curves entering from the frame edge,
and deliberate registration displacement. Keep its proof identity separate
from title-card transitions.

## Stage X2: make guilloche typography a stable library surface

After visual convergence, turn the accepted mechanisms into a small,
source-compatible public API:

- retain woven rows as a compatibility mode;
- expose cycloid-outline, floral-cell, and floral-cycloid treatments;
- support linear, radial, orbital, and bilateral layouts;
- keep deterministic seeded grouping as the default;
- separate glyph content, geometric treatment, layout, palette, and motion;
- retain the exact source string in SVG accessibility metadata even when the
  visible result is entirely path geometry;
- expose resource estimates before allocating or rendering a dense scene;
- make currency-like and extreme moiré output possible locally while clearly
  identifying experimental review requirements.

The next API review will replace the prototype 5×7 mask with an
optional contour provider. A useful progression would be:

1. dependency-free built-in stroke masks;
2. user-supplied normalized vector contours;
3. optional font-outline adapters kept outside the core header;
4. path effects that operate identically on glyphs, scientific notation, graph
   labels, and arbitrary user geometry.

This keeps the header-only core small while allowing high-quality typography
without embedding or redistributing fonts.

To model this, use the font resources selected for the guilloche typography
baselines in `resources/typeFONTBAT/`.

Inspect these files and use them as specified contours. Derive each stable
selector by stripping the three-digit numeric prefix and underscore from its
nested directory name; for example, `011_rns_bobo_dylan` becomes
`rns_bobo_dylan`.

Treat the selected contours and the independently implemented ideas documented
by the moiré and surface-tension evidence archives as inputs to a shared family
of repetition and interference styling modes. Expose
a deterministic degeneration axis such as `repeat`, `interfere`, `destabilize`,
and `glitch`. Early states preserve recognizable source geometry; later states
may accumulate phase error, registration displacement, omission, duplication,
tearing, and dense interference until the result becomes deliberately wild.
Preserve the exact source string and the selected contour identifier in
metadata even when a local experimental rendering becomes difficult to read.
This shared operator model does not collapse the three review packages: each
keeps its own visual lineage, manifest, human-review state, and promotion gate.

Use this canonical text-pattern corpus for the next guilloche typography and
motion passes:

- `duotone`;
- `111`;
- `devastation pacific`;
- `alpha60`;
- `cartofreako`;
- `here lies trouble`;
- `stargirl & cosmic ray situationshipin.space`;
- `reef`;
- `talafreako`;
- `LA->SF`;
- `-[{0-0}]-`.

Vary between these options rather than placing the entire corpus into every
candidate. Distribute every source string across a full review set and record
the exact selected string per candidate. Typography treatment remains an
independent axis: fill, outline tracing, cycloid tracing, floral grouping,
radial/orbital placement, and deliberate interference should be comparable
without conflating them with the content choice. XML serialization must escape
characters such as `&` while retaining the exact semantic source value in
metadata.

## Stage X3: converge the moiré visual language

Inspect `resources/moire-index.tar.xz` and propose a plan to integrate its
archive files into the Izzi visual language and C++ API.

Then, complete a human review of the portrait `moire.v1` package:

- ten digit studies;
- ten word studies;
- ten word-number studies;
- ten wild portrait transitions;
- category grids and decoded transition filmstrips.

Implementation boundary, 2026-08-09: the license bundled with
`resources/moire-index.tar.xz` permits published uses but restricts alteration,
derivative software, and making the source software available for download.
Keep that archive immutable and out of generated output. Do not copy, edit,
embed, or render its SVG members. `izzi::moire` must use independently authored
mathematical interference fields; the archive's broad category structure is
historical vocabulary only. The first implementation package is therefore a
ten-plate portrait geometry and review-infrastructure starter. It does not
silently satisfy the later digit, word, word-number, or transition review gate.

Motion-test checkpoint, 2026-08-10: three semantic canaries now exercise
`ripple`, `falling`, and controlled `glitch` with a number-first reading order,
and three text-free canaries exercise `repeat`, `interfere`, and `glitch` as
closed wild loops. They are ten-second, 1080-by-1920, 30 fps H.264 MKVs under
`outputs/ad-hoc/moire.v1/motion-tests/`. The package includes decoded start/midpoint/end
frames, vertical filmstrips, ffprobe evidence, temporal measurements, exact
checksums, and a byte-identical second regeneration. This is a six-canary
technical checkpoint, not completion or human acceptance of the planned ten
wild transitions, typography studies, or accessible alternatives.

Expanded-motion checkpoint, 2026-08-10: the category-first review pass adds ten
ten-second portrait wild-motion loops under
`outputs/review/feedback/visual/moire/round-02`. The set varies composition,
field construction, intensity, direction, and speed and passes its independent
technical checker. It completes the requested first ten-candidate motion field,
but its dark fine-line filmstrip still requires human inspection and is not an
accepted baseline.


## Stage X4: converge the surface-tension visual language

Inspect `resources/edge-tension-please-01.tar.xz` and propose a plan to
integrate its archive files into the Izzi visual language and C++ API.

The archive name and `edge-tension` phrase remain preserved as source lineage;
the user-facing module and package name are `surface-tension`.

Then, complete a human review of the portrait `surface-tension.v1` package:

- ten digit studies;
- ten word studies;
- ten word-number studies;
- ten wild portrait transitions;
- category grids and decoded transition filmstrips.

Implementation boundary, 2026-08-09: read-only inspection found no bundled
license or rights grant. Embedded Illustrator metadata names Patricia Braun,
which is attribution evidence rather than reuse permission. Keep the archive
immutable; do not extract it into tracked/output directories or derive code or
proof art from its paths. Start `izzi::surface_tension` with independently
authored scalar fields, level-set contours, boundary attraction, deterministic
instability, and the shared degeneration vocabulary. Any future archive-derived
use requires a separately documented rights decision.

Motion-test checkpoint, 2026-08-10: the same six-canary contract is implemented
under `outputs/ad-hoc/surface-tension.v1/motion-tests/`, but the independently authored
geometry uses long cubic membrane sweeps, curved nested cells, and moving
level-set-like groupings rather than moiré line fields. Number contours use the
Atkinson Hyperlegible Next ExtraBold face, title content is retained exactly in
sidecar metadata, and wild frames are contractually text-free. All six MKVs are
ten-second portrait 1080p artifacts with decoded filmstrips, ffprobe and
temporal evidence, checksums, and byte-identical regeneration. Human motion,
legibility, accessibility, and baseline-promotion reviews remain pending; the
larger ten-per-category package remains incomplete.

Expanded-motion checkpoint, 2026-08-10: the category-first review pass adds ten
ten-second portrait wild-motion loops under
`outputs/review/feedback/visual/surface-tension/round-02`. The independently
authored set varies composition, curved field construction, intensity,
direction, and speed and passes its checker. Human visual review, accessibility
review, and baseline promotion remain pending.

## Stage C: define a standard JSON scene interface

Add a versioned JSON schema that maps cleanly onto the C++ value types. Keep
content separate from treatment and layout. A conceptual object is:

```json
{
  "$schema": "../../schemas/izzi.guilloche-scene-1.schema.json",
  "schema_version": "izzi.guilloche-scene/1",
  "scene_id": "duotone-111-radial-stage0",
  "canvas": {
    "width": 1080,
    "height": 1920,
    "background": "#02030A"
  },
  "content": {
    "kind": "word-number",
    "word": "duotone",
    "number": "111",
    "accessible_label": "duotone 111"
  },
  "treatment": {
    "kind": "floral-cycloid",
    "seed": 111,
    "floral_group_range": [1, 3],
    "petal_range": [5, 11],
    "cycloid_amplitude": 12,
    "density": "experimental"
  },
  "layout": {
    "kind": "radial",
    "symmetry_order": 6,
    "center": [540, 960],
    "radius": 360
  },
  "review": {
    "profile": "experimental",
    "human_acceptance": "PENDING"
  }
}
```

The production schema should reject unknown versions, non-finite geometry,
invalid colors, unbounded path budgets, and ambiguous output dimensions. It
should preserve unknown user metadata only in a clearly namespaced extension
object.

Implementation checkpoint, 2026-08-10: Stage C now has a strict Draft 2020-12
authoring schema, a dependency-minimal public C++20 JSON adapter, a portrait
`duotone`/`111` fixture, and a canonical resolved-manifest baseline. Resolution
is in-memory and deterministic, preserves exact semantic source strings, binds
input and schema hashes, applies explicit path, point, and SVG-byte budgets,
and reports structured diagnostics at JSON Pointer paths. Eight schema-negative
cases and ten native-negative cases cover unknown members, versions, types,
colors, content mismatches, resource caps, namespaced extensions, geometry, and
hash context. GCC and Clang compile, ODR, XML, schema, deterministic-resolution,
and relocated-package checks pass. The resolved document keeps human acceptance
and baseline promotion `PENDING` and accessibility conformance `NOT-CLAIMED`;
no visual or accessibility acceptance is inferred from these mechanical tests.

## Stage D: make portrait motion a first-class Izzi workflow

Generalize the current ad hoc video experiments into a frame-addressed motion
API. A transition should declare:

- start, optional midpoint, and end states;
- duration, frame rate, dimensions, and loop behavior;
- geometric interpolation rather than raster interpolation when possible;
- tint, shade, registration slip, dot, strobe, blink, wink, floral aperture,
  cycloid sweep, and radial/orbital reveal treatments;
- exact deterministic frame addressing;
- an optional audio clock without making audio mandatory;
- an explicit reduced-motion alternative.

Portrait 1080×1920 should be the standard proof format for the vertical-series
work. Landscape, square, print, and arbitrary scientific-document dimensions
should remain supported output profiles rather than implicit conversions.

Every motion package should include:

- MKV output;
- an ffprobe receipt;
- decoded start, midpoint, and end frames;
- a portrait filmstrip;
- checksum and source-lineage manifests;
- temporal-contrast, flash, and loop-seam measurements;
- human visual acceptance and baseline-promotion states represented
  separately.

### Separate static, wild motion, episode motion, and title-card transitions

Four visually and semantically different output types are required:

1. `static` produces portrait SVG and PNG review plates with no temporal
   behavior.
2. `title-card-transitions` presents semantic content: first a number, then an
   optional title. It must be good at both glyph geometry and the surrounding
   image rather than treating text as a texture pasted onto a raster reveal.
3. `wild-motion` explores composition, symmetry, line density, moiré,
   registration slip, and expressive curves. Its video frames contain only
   patterns, lines, and motion. Stable proof names should be
   `wild-variation-01.mkv` through `wild-variation-10.mkv`.
4. `wild-motion-episodes` composes wild-motion sources into one through ten
   complete portrait episodes. Each episode is 60–180 seconds at 1080×1920,
   text-free by default, and has a deterministic edit manifest. Episode sets
   must vary composition, palette, background, texture, density, source order,
   and motion speed or speed progression. Emit individual MKVs, ffprobe data,
   checksums, decoded review frames, filmstrips, and a project review index.

`wild-motion-episodes` is an episode-scale compositor, not a longer filename
for a loop. It should select, transform, time-warp, layer, and sequence accepted
wild-motion generators while preserving source lineage and exact frame counts.
It may use the canonical text corpus only when a separately declared semantic
title-card layer is requested; the wild episode frames remain text-free by
default.

Stage 0 implementation checkpoint, 2026-08-10: the output type now has a Draft
2020-12 JSON Schema, one hash-bound example edit, a renderer, and an independent
checker. The example produces one mechanically resolved 60-second portrait
episode from six ten-second segments spanning guilloche v4, moiré v1, and
surface-tension v1. It exercises six primary speed values, identity and mirror
transforms, identity/monochrome/warm/cool treatments, and four overlays using
screen, addition, and difference blending. The result is exactly 1,800 silent
H.264 frames at 1080×1920 and 30 fps. Its start/midpoint/end proof is a vertical
1080×1920 filmstrip. Seven invalid configuration variants are rejected, and a
second full generation is byte-identical.

This checkpoint proves only the complete output form. The episode is
`STAGE-0-PROVISIONAL`; its mixed-family composition, palette, layering, source
order, speeds, 417 MiB encoded size, and transitions have not been accepted.
Human refinement should decide whether to retain one family per episode,
cross-family layering, longer holds, local transitions, or a lighter delivery
encode before expanding the example to ten episodes.

### Add episode-scale `4d-synthesis`

Treat `4d-synthesis` as the temporal continuation of the 27-cell vector
matrix, not as an alias for `wild-motion-episodes`. The latter may edit accepted
loops across families; `4d-synthesis` keeps each visual language independently
reviewable while adding episode-scale motion, color, and composition variance
to its nine vector source relationships.

The first complete review field is intentionally large:

```text
3 visual passes × 10 episodes × 60–240 seconds = 30 portrait MKVs
```

Each of guilloche, moiré, and surface tension must therefore expose the same
five classes: `static`, `title-card-transitions`, `wild-motion`,
`3d-synthesis`, and `4d-synthesis`. Put those below
`outputs/review/current/visual/<family>/`; keep audio separately below
`outputs/review/current/audio/<project>/`.

Use the observable Asama-loop vocabulary for structure and behavior—center and
edge compositions, bands, bilateral and radial counterflow, registration
layers, holds and reveals, swipe, scan, pulse, orbit, drift, dot expansion,
strobe, and controlled glitch—without copying the encoded Asama imagery into
these frames. Represent that vocabulary in a short versioned JSON document so
artists and agents can change durations, wildness ranges, seeds, families, and
allowed motifs without editing FFmpeg commands.

The production pass must resolve every random choice before rendering, bind
all nine 3D source proofs per family by hash, emit exact 30 fps H.264 portrait
video, retain three decoded frames and a filmstrip per episode, and build one
ten-episode grid per family. Generate into a temporary directory and expose it
to `current` only after the independent checker passes. Reuse an exact package
on a four-factor fingerprint match; do not perform an expensive byte-for-byte
rerender merely to rediscover that no input changed.

The first title-card motion canaries should test three mechanisms:

- `ripple`: propagate a bounded geometric wave through the ornamented number
  and surrounding field, then resolve to a stable readable card;
- `falling`: let path components descend, accumulate, or settle into the final
  number and title without losing the intended reading order;
- `glitch`: use controlled registration displacement, omission, duplication,
  and recovery while bounding flash and preserving a clean resolved state.

Test each mechanism first with a number alone and then with a number plus a
short title. During exploration, legibility is a review signal rather than a
hard rejection gate: variants may fragment, obscure, invert, duplicate, or
unexpectedly re-form their content. Preserve the intended source string in SVG
metadata and label the output experimental, but allow the image to surprise
us. Require a readable resolved hold only when promoting a variant to a
delivery candidate. Reduced-motion alternatives and separate reviews of motion
interest and title legibility still belong at that later promotion gate. Simple
crossfades, wipes, and masks between pre-rendered text plates remain useful API
exercises but are not evidence that the title-card visual language has
succeeded.

## Generation-pass checkpoint: current review package

The approved 2026-08-10 generation pass is locally complete and organized at
`outputs/review/`. The current human-facing index is
`outputs/review/current-review-index.png`; category entries under
`outputs/review/current/` resolve to preserved feedback rounds rather than
duplicating media.

The pass contains:

- a fresh guilloche round with 30 static proofs, ten wild-motion loops, and ten
  title-card transitions;
- ten expanded wild-motion loops each for moiré and surface tension;
- the complete 27-cell `3d-synthesis` matrix, partitioned as nine proofs per
  visual pass, with four whole-field and three pass-specific portrait
  comparison grids;
- 30 `4d-synthesis` episodes, ten for each visual pass, spanning 4,500 seconds
  of exact 1080×1920, 30 fps H.264 motion with three decoded review frames,
  one filmstrip per episode, and one ten-episode grid per family;
- exact reuse of three complete Stage 0 audio episodes for each project.

All category-specific checkers pass. During visual inspection, decorative
number contours failed the recognizable-reference role, so static and
title-card numbers were rerendered with Atkinson Hyperlegible Next ExtraBold
while word roles retained the selected decorative contours. This was a
corrective pass inside the same feedback round, not a new aesthetic direction.

`4d-synthesis` independently verifies all 30 videos, 135,000 frames, all nine
3D sources per family, ten distinct levels on every wildness axis, exact
checksums, live ffprobe evidence, and seven rejected invalid configurations.
A second invocation is a cache hit rather than re-encoding 75 minutes of
content. The
family-first current view now contains 17 entries across six categories: five
visual classes for each of three families and two separately organized audio
projects.

The package is ready for human inspection. It is not a promoted baseline and
does not imply accessibility conformance, publication, provider transfer, or
human acceptance. The generation-pass and feedback-round contract is
maintained in `docs/visual_workflow/visual_experiments_method.md`.

## Stage E: accessible and deliberately wild variants

Izzi should make accessible defaults easy without disabling experimentation.
Each accepted scene could produce paired profiles:

- `accessible-static`: semantics, tested contrast, non-color cues, print-safe
  line weights, and meaningful descriptions;
- `motion-safe`: reduced temporal contrast, bounded flash frequency, pauseable
  playback, and a reduced-motion substitute;
- `experimental`: moiré, registration slip, dense interference, strobe, and
  other locally authorized wild elements.

The experimental profile must not be mislabeled as WCAG-conformant. The same
underlying data and accessible label should be reusable across profiles so a
wild artifact can coexist with an understandable static alternative.

Implementation checkpoint, 2026-08-10: one valid guilloche scene now resolves
through `resolve_guilloche_scene_profile_bundle` into a fixed three-member
bundle: `accessible-static`, `motion-safe`, and `experimental`. All three retain
the same scene ID, accessible label, exact word/number source strings, and
geometry provenance. The accessible-static variant is motionless, lowers
sampling and weave density, removes jitter, uses thicker fully opaque source-
over strokes, and preserves a mandatory human contrast-and-semantics warning.
The motion-safe variant is a lower-density static substitute with an explicit
temporal-review diagnostic. The experimental variant retains the authored wild
geometry. The bundle is canonical and deterministic and keeps human,
contrast, temporal-safety, baseline-promotion, and WCAG-conformance states
separate and pending. These are useful mechanical companions, not evidence of
human legibility or WCAG 2.2 conformance.

## Stage F: connect guilloche, radial, chord, and graph work

Treat ornamented glyphs as one more visual mapping surface rather than a
separate decorative subsystem. Candidate integrations include:

- use floral count, petal order, orbit radius, line density, and color as data
  channels;
- place radial-graph nodes on the same orbit model used by radial text;
- route chord edges through shared cycloid or roulette path effects;
- allow a central word-number glyph to summarize a radial or chord graph;
- expose the MMRL orbit/Kusama language as presets rather than hard-coded
  examples;
- use deterministic seeds derived from stable record identifiers, not record
  order;
- provide legends and tabular alternatives for every nontrivial data mapping.

A reconciled radial/chord model could use one semantic graph with two geometry
views:

1. the radial view emphasizes nodes, groups, magnitude, and orbit;
2. the chord view emphasizes pairwise relationships and flow;
3. shared IDs, colors, filters, labels, and accessibility descriptions keep the
   views synchronized;
4. guilloche path effects decorate or encode selected relationships without
   replacing the underlying graph semantics.

This is directly relevant to `alpha60-results`, its slice repositories,
Cartofreako pages, MMRL-derived experiments, and scientific documentation.

Implementation checkpoint, 2026-08-10: the first reconciled proof now uses one
validated typed flow dataset as the semantic authority for both views. Node
marginal totals deterministically project into an MMRL-default-derived
radial/Kusama orbit; the original directed relations resolve into the linear
chord view. Both retain the same three stable IDs, labels, descriptions,
accessibility title, summary, and table reference. Shape families and dash
patterns provide non-color cues. A central `reef`/`111` guilloche summary
retains exact source strings without replacing graph semantics. The same
resolution emits a portrait semantic SVG, a two-table HTML alternative, and a
machine-readable manifest. The local review candidate is under
`outputs/review/feedback/visual/graph-synthesis/round-01/` with deterministic
SVG and stripped PNG, checksums, and all human, accessibility, and promotion
states pending. It proves that radial and chord can be synchronized through
one data model; it does not establish that this particular composition is a
visual keeper or WCAG 2.2 conformant.

## Stage G: audio, scene cutting, and provider-assisted video

Keep the local-first visual method:

1. derive or negotiate scene cuts from recorded audio;
2. generate local deterministic placeholders and title cards;
3. assemble the full vertical form locally;
4. conduct human review of cuts, voices, captions, and style packs;
5. submit only specifically approved provider canaries;
6. verify returned duration, resolution, codec, and visual correspondence;
7. replace or reject provider clips without losing the local edit decision
   list.

For `duotone-111` and `here-lies-trouble`, human-curated seed-image packs should
remain the primary artistic control. Provider prompts should reference explicit
style-pack assets and shot constraints rather than attempting to reconstruct a
visual language from prose alone.

The local Stage 0 voice gate should provide one through three complete approved
episode intervals per project before asking humans to judge the workflow. The
current target is three episodes for `duotone-111` and three for
`here-lies-trouble`, with ordered playlists, voice-assignment timelines, exact
hashes, and project-level listening notes. Random neutral targets remain
provisional. Human playback does not imply voice selection, character casting,
or consent; those remain the low-friction Pass 2 interaction described in
`docs/audio_workflow/human_voice_selection.md`.

## Stage H: production outputs and integration surfaces

Exercise the same source scene through all intended delivery paths:

- SVG for inspection and the web;
- PNG sequences and MKV assembly for video;
- PDF for large-format inkjet output;
- WebAssembly for interactive exploration;
- GitHub Pages packages for Cartofreako, Devastation Pacific house styles, and
  the alpha60-results family.

For web delivery, add keyboard navigation, visible focus, non-color state cues,
semantic summaries, data tables, reduced-motion behavior, and automated WCAG
checks. Automated checks are evidence, not a substitute for disabled-user and
expert review.

## Stage I: production-ready verification and release gates

Before calling the subsystem production-ready:

- compile public headers independently and in multiple translation units;
- run GCC and Clang warning-as-error builds;
- retain deterministic SVG and media regeneration checks;
- maintain approved pre/post visual-fidelity grids;
- test PNG, PDF, MKV, and browser/Wasm outputs at their actual delivery size;
- test extreme resource budgets and malformed JSON;
- pin tool versions or record them in manifests;
- separate generated candidates, accepted baselines, and published artifacts;
- document third-party inputs and redistribution authority;
- provide concise agent-facing examples, schemas, error messages, and
  machine-readable test receipts.

An artifact advances only when its relevant gates pass. Local generation,
technical verification, human visual acceptance, accessibility review,
baseline promotion, publication, and external transfer remain distinct states.

## Suggested order after the current generation pass

1. Human-pass the consolidated current grids, filmstrips, motion loops, and
   audio timelines.
2. Select two or three glyph treatments and two or three motion treatments as
   keepers.
3. Simplify and stabilize only the API needed by those keepers.
4. Add the versioned JSON schema and round-trip examples.
5. Produce paired accessible and experimental profiles.
6. Integrate one small radial/chord dataset and one alpha60-results slice.
7. Exercise PNG, MKV, PDF, and Wasm outputs from the same scene description.
8. Promote baselines only after explicit human review.

## Questions reserved for human review

- Which current glyphs remain legible without becoming typographic
  illustrations?
- Should flowers sit inside contours, replace contours, or bloom outside them?
- Should cycloids be visibly cusped or smoothed into roulette-like loops?
- Which combinations should behave as one fused glyph rather than two related
  inscriptions?
- Is two seconds the right transition proof duration, or should the proof
  package standardize on another interval?
- Which experimental effects need an automatic accessible-static companion?
- Which alpha60-results slice is the smallest useful first graph integration?

Until those questions receive explicit answers, the corresponding choices are
proposals rather than accepted requirements.
