# Visual experiments method

Date: 2026-08-10

Status: working method for local generation and human review; not a baseline,
publication, accessibility-conformance, provider-transfer, or release approval

## Purpose

Izzi visual experiments follow a form-first, feedback-driven method. The first
responsibility of the system is to make a complete artifact from the available
inputs. Humans should be able to see or play the actual form before spending
time fine-tuning it. Later passes broaden the choice field, combine visual
grammars, and refine selected candidates.

This method applies to guilloche, moire, surface tension, title cards, wild
motion, vector synthesis, and the related audio-review workflow. It keeps four
ideas separate:

- a **generation pass** is one reproducible execution;
- a **feedback round** is one cycle of human response and revised intent;
- **current** identifies what humans should inspect now;
- a **baseline** is an explicitly accepted artifact.

Current does not mean accepted. A technically successful artifact does not
become a baseline merely because it rendered.

## Generation passes and feedback rounds

A generation pass has a bounded contract:

```text
declared source + declared inputs + feedback contract + output/tool contract
  -> generated artifacts
  -> technical verification
  -> review package
```

The pass records exact hashes for the relevant source files, input resources,
feedback contract, configuration, tools, and outputs. Repeating the same pass
with the same contract should reproduce the same package or explain any
declared nondeterminism.

A feedback round is larger than a generation pass. It begins when human
feedback materially changes what should be explored, compared, retained, or
rejected. A round may need several corrective generation passes before it is
ready for inspection. Those reruns remain part of the same round when they only
repair or faithfully complete the existing feedback contract.

Open a new numbered round when human direction changes an artistic or review
requirement, for example:

- expanding three motion canaries to ten or twenty useful alternatives;
- requiring composition, color, texture, density, and speed variation;
- separating text-free wild motion from semantic title cards;
- changing the canonical text corpus;
- introducing cross-family vector synthesis;
- retaining a rejected candidate as evidence while asking for a materially
  different construction.

Names such as `guilloche.v1` through `guilloche.v4` are historical feedback
rounds. They are not promises of semantic-version compatibility and should not
be used as public C++ API versions.

## Pass sequence

### Pass 0: complete-form proof

Pass 0 answers: **does the workflow work with the supplied inputs?**

Use deterministic or explicitly recorded provisional defaults. Produce a
complete artifact at the intended form and delivery orientation. Verify its
dimensions, duration, frame count, ordering, source lineage, checksums, and
review packaging. A Pass 0 result is labeled `STAGE-0-PROVISIONAL` or an
equivalent engineering-review state.

Pass 0 does not ask humans to debug plumbing or make every aesthetic decision.
It also does not infer approval of provisional voices, fonts, colors, edits, or
compositions.

Examples in the current workspace include:

- three complete random-default audio episodes for `duotone-111`;
- three complete random-default audio episodes for `here-lies-trouble`;
- one complete 60-second wild-motion episode;
- the local noir portrait inventory, frame, and motion-preview package.

### Pass 1: expanded choice field

Pass 1 answers: **does the review set contain enough meaningful variation for
a human to discover a direction?**

Expand only real axes. For static work these include composition, symmetry,
grouping, palette, background, density, texture, registration, and semantic
content. Motion work also varies speed, speed progression, phase, direction,
loop behavior, and transition mechanism. A filename or seed change alone is
not a useful variant.

Ten candidates are a practical complete first field. A mature experimental
family may expand to twenty when the additional candidates explore declared
axes rather than padding the set. Contact sheets and filmstrips must make those
differences comparable.

The existing moire and surface-tension packages demonstrate why source hashes
alone are insufficient. Their static generator binaries still match their
manifests, but their motion packages contain only three wild canaries. The
later feedback contract asks for an expanded wild field, so those packages are
reproducible but no longer current for human review.

### Pass 2: `3d-synthesis`

Pass 2 asks: **what happens when the geometry carrying the composition and the
geometry constructing the glyph come from different visual languages?**

`3d-synthesis` is a top-level output category. Its three independent dimensions
are:

1. curve or field grammar: roulette, moire, or surface tension;
2. glyph grammar: roulette, moire, or surface tension;
3. content kind: number, word, or word-number combination.

The complete first matrix therefore contains 27 portrait vector specimens:

```text
3 field grammars × 3 glyph grammars × 3 content kinds = 27 SVG/PNG proofs
```

The field grammar controls the surrounding or traversing paths. The glyph
grammar controls paths clipped to, traced around, or used to construct the
letter and number contours. These axes remain independently identifiable in
the SVG and manifest. The experiment must use vector paths rather than hiding
a raster image inside an SVG.

The first matrix should use a stable comparison corpus—such as `111`,
`duotone`, and `duotone 111`—so humans can compare geometry rather than decode
changing words. Later feedback rounds may draw from the wider canonical text
corpus.

Number roles need a recognizable reference treatment. In current static and
title-card comparison sets, render every reference number with Atkinson
Hyperlegible Next ExtraBold. Decorative font contours remain useful for word
roles and may be explored as a separately labeled experimental number axis;
they must not silently replace the recognizable reference proof.

### Pass 3: `4d-synthesis`

Pass 3 asks: **how does a selected vector relationship behave as a complete,
episode-scale moving composition?**

`4d-synthesis` retains the three axes of `3d-synthesis` and adds time as a
fourth axis. Each of the three visual passes—guilloche, moiré, and surface
tension—owns ten independent portrait episodes. Each episode is 60–240 seconds
at 1080×1920 and 30 fps. Across each ten-episode family, the pass varies three
explicit wildness dimensions:

1. motion: phase, direction, rate, pulses, scans, orbit, strobe, and controlled
   glitch;
2. color: palette, saturation, contrast, tint, shade, and registration
   relationships;
3. composition: center, edge, bilateral, quadrilateral, banded, split,
   layered, and negative-space structures.

The editable contract is
`examples/generative/4d-synthesis.round-01.json`, validated by
`schemas/izzi-4d-synthesis.schema.json`. Its small motif vocabulary was derived
from observable composition, motion, and structural relationships in the local
user-authored Asama loops. The encoded Asama videos remain design evidence;
they are not copied into the 4D frames. The resolver records every selected
source proof, segment, motif, speed, palette, duration, and seed before
rendering.

A complete first pass contains 30 silent MKVs, three decoded review frames and
one portrait filmstrip per episode, one ten-episode grid per visual pass,
ffprobe receipts, source hashes, checksums, and separate human, accessibility,
promotion, publication, and transfer states. Rendering occurs in a temporary
package and cuts over atomically only after every episode succeeds. A matching
four-factor fingerprint and checksum inventory is a cache hit; it does not
re-encode unchanged media.

### Pass 4: human refinement

Pass 3 begins only after the complete form and useful choice field are visible.
Humans may:

- accept a candidate as a keeper;
- retain only named parts or relationships;
- reject a candidate while preserving it as evidence;
- request more variants along a specific axis;
- combine selected mechanisms;
- keep a provisional default;
- or decline to make a selection.

Refinement should be inexpensive to express. A useful minimum response is
`KEEP`, `KEEP-PARTS`, `MORE-LIKE`, or `REJECT`, plus a short note. Human silence
is not acceptance.

For audio, the same pass means listening to complete episodes before choosing
or changing provisional voices. Participants can accept the generated option,
explore alternatives, request another pass, or decline. The working artifact
comes first; consent and selection remain explicit but do not become an
obstacle to proving that the machinery works.

### Pass 5: promotion and delivery

Only selected work enters promotion checks. Depending on the artifact, these
include:

- resolved title legibility;
- temporal-contrast, flash, and loop-seam review;
- accessible-static and reduced-motion companions;
- WCAG-oriented semantic, contrast, keyboard, and non-color checks;
- actual-size SVG, PNG, MKV, PDF, and WebAssembly testing;
- source and third-party provenance review;
- explicit baseline, publication, and transfer decisions.

Experimental work may deliberately relax legibility or temporal intensity
during local exploration. It must not be labeled WCAG-conformant unless the
separate accessible profile passes its applicable review.

## Freshness and optimized regeneration

Before rendering, compute four fingerprints:

1. **source fingerprint** — generators, public headers, schemas, and adapters;
2. **input fingerprint** — fonts, images, audio intervals, prior accepted
   artifacts, and other declared resources;
3. **feedback-contract fingerprint** — candidate counts, comparison axes,
   content corpus, dimensions, duration, and requested output categories;
4. **output/tool fingerprint** — renderer, compiler, font engine, FFmpeg,
   Inkscape, codec, and serialization contract.

A review package is current only when all four fingerprints match and its
independent checker passes.

```text
all fingerprints match + checker passes -> reuse
source or input changed                 -> regenerate affected categories
feedback contract changed               -> generate the missing review field
tool/output contract changed            -> regenerate affected deliverables
artifact missing or corrupt              -> repair and verify the same round
```

This is performance analysis as well as provenance. It avoids spending minutes
or hours regenerating hundreds of video frames that are already exact, while
preventing a technically reproducible but underspecified package from being
mistaken for the current human-review set.

The audio packages illustrate a valid cache hit: three complete episodes per
project already satisfy the current Stage 0 depth contract, so they should be
reused until their source, cut, transformation, packaging, or feedback contract
changes.

## Human-facing output organization

Humans should not need to navigate implementation history to find the current
work. Visual review is family-first so every visual language exposes the same
five generative classes. Audio remains project-first on its own path:

```text
outputs/review/
  current/
    visual/
      guilloche/
        static/
        title-card-transitions/
        wild-motion/
        3d-synthesis/
        4d-synthesis/
      moire/
        static/
        title-card-transitions/
        wild-motion/
        3d-synthesis/
        4d-synthesis/
      surface-tension/
        static/
        title-card-transitions/
        wild-motion/
        3d-synthesis/
        4d-synthesis/
    audio/
      duotone-111/
      here-lies-trouble/
  feedback/
    visual/<family>/round-NN/
    audio/<project>/round-NN/
  README.md
  manifest.json
```

The `current` entries resolve to one declared feedback round. The `feedback`
tree preserves numbered human-response history. Live guilloche generation and
verification cut over directly to its current review round; they do not
recreate `outputs/ad-hoc/guilloche.v4`. Unique legacy packages now live under
`outputs/ad-hoc/` as historical evidence. The current moiré, surface-tension,
and audio views may resolve there until a later generation pass writes their
replacement packages directly under `outputs/review/`.

Canonical category spellings use lowercase kebab-case:

- `static`;
- `title-card-transitions`;
- `wild-motion`;
- `3d-synthesis`;
- `4d-synthesis`;
- `audio`.

Repository-authored Markdown filenames use lowercase words separated with
underscores, for example `visual_experiments_method.md`. This filename rule is
independent of the output-category vocabulary above. Contract-pinned
training-conversion artifacts retain their exact normative paths, and generated
documentation retains the names emitted by its generator.

## Review-package requirements

Every current visual category should provide the smallest useful combination
of:

- SVG sources with semantic metadata;
- portrait PNG proofs;
- MKV artifacts for motion;
- decoded start, midpoint, and end frames;
- vertical filmstrips or contact sheets;
- exact checksums and source lineage;
- declared variation axes;
- machine-readable technical and human-review states.

The review index should lead with images and playable media. Detailed manifests
remain available for agents and reproducibility, but humans should not have to
read JSON to decide what they see.

## Independent state model

Record these states independently:

| State | Meaning |
|---|---|
| Local generation | The artifact was produced locally. |
| Technical verification | Format, dimensions, duration, hashes, and declared contracts passed. |
| Human feedback | A person reviewed the actual artifact and supplied a bounded response. |
| Human acceptance | A person explicitly accepted the relevant visual or audio result. |
| Accessibility review | The applicable static, semantic, motion, and interaction checks passed. |
| Baseline promotion | The accepted artifact became a comparison baseline. |
| Publication | The artifact may be made public. |
| External transfer | The artifact may leave the local repository boundary. |

No state implies another. In particular, local technical success does not
authorize publication, provider submission, or external transfer.

## Current assessment

As of 2026-08-10:

- **Guilloche:** `outputs/review/feedback/visual/guilloche/round-05` contains
  30 portrait static proofs, ten wild-motion loops, and ten title-card
  transitions. Its current-content checker passes. A corrective rerender moved
  all number roles to Atkinson Hyperlegible while retaining decorative contours
  for word roles. Human visual review remains pending.
- **Moiré:** the technically reproducible static evidence is reused. The new
  `round-02` motion package contains ten ten-second portrait loops varying
  composition, field, intensity, direction, and speed. Its independent checker
  passes; the deliberately dark, fine-line filmstrip needs human inspection.
- **Surface tension:** the independent static evidence is reused. Its new
  `round-02` package contains ten ten-second portrait loops with the same
  declared comparison axes and family-specific curved geometry. Its checker
  passes; human inspection remains pending.
- **`3d-synthesis`:** `round-01` contains the complete 27-specimen vector
  matrix, partitioned as nine proofs for each visual pass, plus four whole-field
  and three pass-specific portrait comparison grids. It contains vector paths
  rather than embedded raster or SVG text shortcuts, and its checker passes.
  Human review remains pending.
- **`4d-synthesis`:** `round-01` contains 30 portrait H.264 episodes—ten per
  visual pass—covering 4,500 seconds and 135,000 exact frames. Every family
  uses all nine of its 3D source proofs and spans ten distinct motion, color,
  and composition wildness levels. Checksums, live packet counts, decoded
  frames, filmstrips, and family grids pass the independent checker; a second
  invocation is a verified cache hit. Human visual and accessibility review
  remain pending.
- **Audio:** three complete engineering-review episodes exist for each project
  and pass their technical checker. Human listening, voice refinement,
  participant selection, and final acceptance remain pending.

`outputs/review/current-review-index.png` is the human entry point for this
generation pass. `outputs/review/current/` presents the family-first visual and
project-first audio view and reuses the already verified audio packages by
exact lineage rather than rerendering them. All human gates remain explicitly
pending; none of these artifacts is an accepted baseline.

The current review exposes 17 entries across six categories: five visual
classes for each of three families plus two audio projects. The immediate human
gate is inspection of the consolidated current review
package. Only after that inspection should selected mechanisms become stable
public APIs, accepted baselines, accessible delivery companions, or production
assets.
