# Duotone visual-convergence method for Seedance

- Status: noir-thriller local Stage 0 is `WORKING-PROVISIONAL`; Noir Vibezz
  evaluation Stage 1 is completed by explicit human disposition; three
  corrected Stage 2 five-second previews are generated and awaiting review
- Active scope: Duotone noir-thriller only
- Deferred scope: Tokyo-psychedelia, until its input collection exists
- Current Stage 2 medium contract: original neutral black-and-white film-noir
  photography, high-contrast chiaroscuro, heavy shadow, rain or smoke, and
  classic 1940s composition; it is a review baseline, not a promoted production
  baseline
- Operating model: Stage 0 proves the method works with the given inputs;
  Stage 1 refines artistic intent
- Provider role: constrained motion rendering after human art direction
- Provider submission authority: not granted by this document

This document captures the visual-convergence workflow that followed the first
Duotone Seedance canaries. The canaries demonstrated that the provider can
return video, but their generated imagery did not express the intended visual
language. They remain diagnostic evidence and must not become an aesthetic
baseline merely because they completed successfully.

The user and collaborator have begun the noir-thriller seed-image collection.
Tokyo-psychedelia remains an empty future lane. This document does not claim
that the noir collection is complete, that any image has been approved, or that
a final style has been accepted.

On 2026-08-11, a partial human classification supported three original portrait
style plates and three separately authorized five-second provider canaries. The
first motion round established that the visual form could pass through the
provider, but human review rejected the hand-inked Expressionist medium while
preserving the three compositions. The same review supplied one exact
photographic film-noir correction for all three scenes. The user then marked
Noir Vibezz evaluation Stage 1 complete and authorized Stage 2.

Stage 2 rebuilt the three frames as original identity-obscured black-and-white
photographs. Guilloche and moire are allowed only as physical elements inside
the photographed world: projected light through smoke, overlapping mesh, and
architectural shadow. They may conceal a face or body, but they must not turn
the image back into an illustration.

The companion documents remain:

- [Seedance account and delivery proposal](explore_futures_seedance.md);
- [audio-first installment workflow](explore_futures_seedance_audio.md);
- [audio authentication and local-key guidance](explore_futures_seedance_auth.md);
- [2026-08-09 Izzi evolution plan](explore_futures.20260809.md).

## Central decision

Do not ask Seedance to invent the Duotone aesthetic and animate it in the same
request. Settle the composition, palette, texture, subjects, and emotional
register with human-authored evidence first. Treat the provider as a motion
renderer operating inside those constraints.

The visual-production sequence is explicitly two-stage. This is the governing
workflow for visual, audio, and generative-form development: first make a
complete default artifact from the available inputs and prove that its form is
correct; only then ask humans to tune its artistic decisions.

```text
Stage 0: given inputs
  -> verify and inventory
  -> deterministic default selection
  -> contact sheets + local portrait previews
  -> provider request dry-run
  -> optional separately authorized provider canary
  -> WORKING-PROVISIONAL or an observable failure

Stage 1: refinement
  -> hero and rejection selections
  -> explicit visual and motion grammar
  -> finished local portrait style frames
  -> controlled short motion canaries
  -> pairwise human review
  -> frozen style-pack candidate
  -> bounded episode trial and local post-production
```

Stage 0 is not a low-quality draft of Stage 1. It is a functional acceptance
gate with intentionally mechanical choices. It answers whether the complete
workflow can ingest, transform, assemble, validate, preserve, and present a
reviewable result. Stage 1 answers whether the result expresses human intent.
These states must remain separate so curation is not used to debug plumbing and
successful plumbing is not mistaken for aesthetic approval.

Titles, exact typography, guilloche, glitch, silence holds, accessibility
alternatives, and final assembly remain local Izzi or post-production work.
Provider-generated text is never authoritative.

## Stage 0: is the method working with the given inputs?

Stage 0 is an engineering proof, not an art review. It must run from minimal
human-supplied image collections and produce something concrete enough to
inspect before asking the humans to make the workflow beautiful.

The minimum input is one readable supported image in the active visual world's
declared source directory; the source need not be copied into a role directory.
A partial `style-pack.json` may be present but is not required to
contain hero selections, rejection reasons, finished prose, or approval. If an
input is missing or unreadable, Stage 0 reports the exact file and stops that
branch; it does not silently substitute provider imagery.

### Deterministic Stage 0 defaults

Stage 0 uses deliberately plain, reproducible defaults:

1. Inventory supported files by stable repository-relative path and SHA-256.
2. Use an explicit valid `default_anchor` when supplied; otherwise select the
   first supported image in UTF-8 bytewise path order.
3. Preserve the complete image inside a 1080×1920 black portrait canvas. Do not
   silently crop, stretch, inpaint, or regenerate it.
4. Generate a labeled contact sheet for every supplied image.
5. Generate one provisional portrait frame per visual world.
6. Generate a short local motion preview using a simple, declared hold,
   crossfade, or bounded pan. This proves the image-to-video assembly path; it
   is not a proposed Duotone transition.
7. Resolve and validate a provider request as a dry-run without reading a key,
   spending credits, or submitting it.
8. Submit one short provider canary only after a separate explicit authority to
   spend and transfer the selected reference.

Every Stage 0 image and title must be marked `STAGE 0 / PROVISIONAL` in its
review context. The source image itself remains unchanged. A good-looking
default is welcome, but Stage 0 passes because the declared pipeline works and
its outputs are valid—not because anyone has approved the aesthetic.

Suggested Stage 0 outputs are:

```text
outputs/ad-hoc/duotone-111/visual-method/stage-0/
├── noir-thriller/
│   ├── input-inventory.json
│   ├── contact-sheet.png
│   ├── default-frame.png
│   ├── local-default-preview.mkv
│   ├── provider-request.dry-run.json
│   └── receipt.json
└── tokyo-psychedelia.NOT-SUPPLIED.json
```

### Separate functional status

Stage 0 answers two questions separately:

- `local_pipeline`: can Izzi ingest the given inputs and produce valid,
  reviewable local artifacts?
- `provider_motion`: can the authorized provider animate the selected artifact
  and return the requested media properties?

The local pipeline may be `WORKING` while provider motion remains `NOT-RUN`.
Conversely, a provider returning a file does not prove the local visual method
or aesthetic is correct. Never collapse these states into one success flag.

Stage 0 completion records `WORKING-PROVISIONAL`, `FAILED`, or `NOT-RUN` for
each branch, along with input and output hashes, exact dimensions, duration,
commands, validation results, and any separately authorized provider cost. It
does not record human approval.

## Stage 1: refine artistic intent

Stage 1 begins from visible Stage 0 artifacts. Humans replace mechanical
defaults with artistic decisions: anchor priority, informative rejections,
visual grammar, finished style frames, transition behavior, and controlled
motion. The three refinement workflows below may iterate, but they never erase
the Stage 0 receipt.

Stage 1 can begin while a provider canary remains `NOT-RUN`; most visual
decisions should be settled locally before another paid request. Its completion
state is `FROZEN-CANDIDATE`, which still requires a bounded episode trial before
production expansion.

## The visual worlds and current scope

The collections represent related but distinct narrative states, not merely two
color presets.

### Noir thriller

This is the grounded or reality-facing visual world. The human reference pack
defines its lighting, texture, framing, locations, subjects, degree of
abstraction, and acceptable motion. Izzi must not infer those decisions from
the phrase `noir thriller` alone.

### Tokyo psychedelia

This is the fantasy or reality-break visual world. The human reference pack
defines how far color, geometry, scale, texture, and botanical or urban forms
may depart from the grounded world. Izzi must not treat `psychedelia` as
permission for generic neon imagery, arbitrary visual noise, or uncontrolled
subject replacement.

Tokyo-psychedelia is currently `NOT-SUPPLIED` and therefore `NOT-RUN`. Its
empty scaffold is preserved for later work, but the current Stage 0, audit, and
production proposal must not block on it or manufacture substitute inputs. The
only active visual-world branch is noir-thriller.

### Transition grammar

The relationship between the two worlds needs its own authored grammar. A
transition is not simply a crossfade between unrelated generations. Candidate
mechanisms include:

- a shared object, silhouette, table edge, reflection, or camera axis that
  survives the change;
- a locally controlled color or texture incursion;
- Izzi roulette, guilloche, moiré, or glitch geometry that reveals the second
  world without asking the provider to invent the bridge;
- a silence hold, hard cut, or deliberately broken audiovisual boundary;
- an audio `prompt` marker as the editorial entrance and an inferred
  `end prompt` or episode ending as the return boundary.

Silence and glitch are active Duotone materials. They may structure an episode
or transition; they are not automatically defects to remove.

## Human source collections

The recommended resource layout is:

```text
resources/duotone-111/seed-images/
├── noir-thriller/
│   ├── README.md
│   ├── style-pack.json
│   ├── anchors/
│   ├── supporting/
│   └── rejects/
└── tokyo-psychedelia/
    ├── README.md
    ├── style-pack.json
    ├── anchors/
    ├── supporting/
    └── rejects/
```

The human-curated source images remain in `resources`. Deterministic contact
sheets, normalized copies, style frames, canaries, review artifacts, and
provider results belong under `outputs/ad-hoc/duotone-111/visual-method/`; generated
derivatives must not be mixed back into the source collection.

The initial image count is deliberately flexible. A useful first pass is 12–24
images per visual world, followed by selection of approximately three hero
anchors and three informative rejections. More images are not automatically
better: an unranked reference heap can give a model contradictory instructions.

### Reference roles

- `anchors/` contains the few images that most completely describe the desired
  result.
- `supporting/` contains narrower evidence for palette, texture, objects,
  lighting, framing, or motion.
- `rejects/` contains images or generated results that demonstrate a specific
  unwanted tendency.

Rejections are first-class evidence. Each one should say what failed: generic
neon, palette drift, plastic texture, unwanted text, incoherent anatomy,
unmotivated camera motion, replacement of a central subject, loss of the
grounded composition, or another directly observed problem. A rejection is not
a general prohibition on experimentation.

Record source and permission provenance even when the repository's existing
resource authority permits local use and redistribution. Provenance makes later
selection, replacement, publication, and credit decisions inspectable.

## Noir input audit, 2026-08-10

The active material is currently stored in `resources/noir-vibezz/`, rather
than copied into the empty role directories. Referencing it in place is the
fastest non-destructive path and avoids duplicating roughly 62 MiB of source
images. The observed pool contains 58 readable landscape PNG frames in six
families:

| Family | Count | Observed contribution |
|---|---:|---|
| `big-combo` | 6 | restrained black-and-white close-ups, bar interiors, telephones, and soft midtone glamour |
| `crime-wave` | 8 | hard silhouettes, stairs, corridors, doorways, and extreme geometric negative space |
| `la-confidential` | 4 | amber, burgundy, red accents, pale fabric, layered foregrounds, and selective-color noir |
| `rio` | 10 | romantic black-and-white group staging, nightclub tables, tropical exteriors, and chain-link texture |
| `saigon` | 7 | vehicle interiors, roads, hotels, restaurants, shadows, and location movement |
| `spidern` | 23 | the broadest system: green institutional corridors, amber rooms, tiled transit halls, silhouettes, lamps, and black-and-white interiors |

The files range from 986×720 to approximately 1998×1080. They are useful as
landscape composition, palette, light, texture, and location evidence, but are
not ready-made portrait frames. Stage 0 must therefore contain each complete
frame inside a black 1080×1920 canvas and disclose the resulting negative
space. Cropping, inpainting, subject replacement, and portrait reframing belong
to Stage 1.

The strongest visible grammar across the collection is not generic “noir.” It
is depth arranged through bright thresholds and dark foregrounds: a figure or
object occupies one plane while doors, corridors, lamps, windows, roads, mesh,
or haze establish another. Black-and-white is the foundation; amber/burgundy
and institutional green are selective alternate registers. Motion should begin
with restrained changes inside that geometry—light, haze, fabric, reflection,
rain, or a bounded camera drift—rather than morphing faces or inventing new
subjects.

For later Stage 1 review, the following are useful candidates, not selections:

- `spidern/...22h07m06s614.png`: green receding corridor and pale foreground
  figure;
- `spidern/...22h08m22s728.png`: amber tiled transit hall, haze, light shafts,
  and a centered silhouette;
- `crime-wave/...21h51m49s010.png`: a nearly black interior resolved by a
  bright doorway and two silhouettes;
- `spidern/...22h12m54s406.png`: black-and-white lounge geometry, hats, lamps,
  and cones of light;
- `spidern/...22h08m04s893.png`, `la-confidential/...21h55m16s271.png`, and
  `saigon/...22h00m44s278.png`: supporting evidence for amber interiors,
  layered darkness, and vehicle-framed movement.

Several frames visibly contain subtitles or a watermark, and at least one pair
appears near-duplicate. Those observations should become explicit Stage 1
rejection or deduplication decisions. They do not remove the files from the
Pass-0 inventory. More importantly, the collection appears to contain screen
captures with recognizable performers and production imagery. Repository
authority permits the local work described here, but direct transfer of those
captures to a provider or publication of them remains a separate rights and
authority decision. The fastest durable production route uses them locally as
visual vocabulary, creates an original project-owned portrait style frame, and
submits only that derived, human-approved frame when provider transfer is
separately authorized.

### Fastest noir production path

1. **Pass 0 / prove the form:** inventory all 58 files in stable path order,
   generate a hash-bound portrait contact sheet, contain the deterministic
   default image in a black 1080×1920 frame, make a short local hold or bounded
   pan, validate it, and record `WORKING-PROVISIONAL`. Do not wait for anchor
   selection and do not submit anything.
2. **Review only what now exists:** confirm that inventory, portrait geometry,
   local playback, duration, and review packaging are correct. If the form is
   wrong, fix the workflow without spending human art-direction effort.
3. **Pass 1 / refine:** use the contact sheet to choose three or four anchors,
   five or six narrow supporting references, and explicit subtitle, watermark,
   duplicate, or aesthetic rejects. Populate the hash-bound style pack only
   from those decisions.
4. **Make one original vertical still:** compose a single 1080×1920 noir frame
   from one dominant composition reference and, at most, one palette or texture
   reference. Preserve deep thresholds, foreground/background separation, and
   intentional black space. Remove source text, logos, and recognizable actor
   identity rather than asking motion generation to repair them.
5. **Animate the accepted still:** after separate transfer and spending
   authority, run one short image-to-video canary at the already confirmed
   `1080p-pro` profile. Ask only for restrained motivated motion; forbid new
   subjects, text, face replacement, palette drift, and camera warping.
6. **Prove one episode before scaling:** compare decoded start, midpoint, and
   end frames with the accepted still, then assemble one complete 1–3 minute
   episode locally with approved audio. Keep title cards, captions, guilloche,
   silence, glitch, and final encoding in Izzi. Only after that episode passes
   should production expand to 10, 30, or 60 episodes.

This route minimizes both provider spend and premature human work. The first
artifact proves the mechanism; the second pass concentrates human judgment on
the few decisions that actually control the visual result.

### Stage 0 execution evidence

The first-pass noir package now exists under
`outputs/ad-hoc/duotone-111/visual-method/stage-0/noir-thriller/`. It was generated
twice with byte-identical artifacts and passes its independent checker:

```bash
python3 scripts/izzi-seed-image-stage0.py
python3 scripts/check-seed-image-stage0.py
```

The package records all 58 inputs and deliberately chooses `N0001`, the first
UTF-8 bytewise path, without claiming it is a good artistic choice. Its review
artifacts are:

- portrait contact sheet: `outputs/ad-hoc/duotone-111/visual-method/stage-0/noir-thriller/contact-sheet.png`,
  2160×3840, SHA-256
  `350ad6b0163f46ea09a1f501087a404e02e15533317614413228dd7485811aa8`;
- provisional portrait frame: `outputs/ad-hoc/duotone-111/visual-method/stage-0/noir-thriller/default-frame.png`,
  1080×1920, SHA-256
  `341adbdc49b7dc37fa6183a8b2ed69cffb6a10b3055f7a6ce80c7b95a1a4ce48`;
- three-second local preview: `outputs/ad-hoc/duotone-111/visual-method/stage-0/noir-thriller/local-default-preview.mkv`,
  H.264 1080×1920 at 30 fps, SHA-256
  `4279888a36fe7a360aed071a27c25daa6422e1d7729520d62b17a993f79df516`;
- receipt: `outputs/ad-hoc/duotone-111/visual-method/stage-0/noir-thriller/receipt.json`,
  SHA-256
  `ed5ea137228e06696c6da0bba9b0a5a76b07a4cdbd15e285329e96e961363f6a`.

The dry-run did not read a credential, contact a provider, spend credits, or
transfer an image. `provider_motion` remains `NOT-RUN`, all aesthetic choices
remain `PENDING`, and the Tokyo lane remains `NOT-SUPPLIED`. The form is now
available for human review before any refinement work begins.

## Proposed style-pack record

`style-pack.json` is a human-editable visual contract. It is not a provider
request and must not contain credentials, account identifiers, task IDs, or
private output URLs. The following is a working skeleton rather than a frozen
JSON Schema:

```json
{
  "schema_version": 1,
  "style_pack_id": "duotone-noir-thriller-v1",
  "functional_status": {
    "local_pipeline": "NOT-RUN",
    "provider_motion": "NOT-RUN"
  },
  "refinement_status": "CURATING",
  "narrative_role": "GROUNDED-REALITY",
  "anchors": [
    {
      "path": "anchors/example.png",
      "sha256": "<sha256>",
      "notes": [
        "Describe exactly what this image establishes."
      ],
      "provenance": {
        "source": "<human-recorded-source>",
        "use_status": "<reviewed-status>"
      }
    }
  ],
  "supporting": [],
  "rejects": [
    {
      "path": "rejects/example.png",
      "sha256": "<sha256>",
      "rejection_reasons": [
        "GENERIC-NEON",
        "COMPOSITION-NOT-PRESERVED"
      ]
    }
  ],
  "visual_grammar": {
    "palette": [],
    "lighting": [],
    "texture": [],
    "composition": [],
    "recurring_forms": []
  },
  "motion_grammar": {
    "camera": [],
    "subject_motion": [],
    "environmental_motion": [],
    "tempo": []
  },
  "must_preserve": [],
  "may_vary": [],
  "must_avoid": [],
  "review": {
    "status": "PENDING",
    "approved_by": [],
    "approved_artifact_sha256": null
  }
}
```

The Tokyo-psychedelia pack uses the same structure with its own `style_pack_id`
and `narrative_role`. Shared transition rules should be referenced by both
packs, not silently copied into diverging versions.

## Stage 1A: curated visual bible and contact-sheet gate

This is the first and least expensive refinement stage after the Stage 0
functional proof.

1. Preserve each source image unchanged and compute its SHA-256.
2. Normalize orientation only as a derived output; do not overwrite the source.
3. Generate a deterministic labeled contact sheet for each visual world.
4. Review the sheets together and choose hero anchors, supporting details, and
   explicit rejections.
5. Write concise observations about visible properties rather than relying on
   genre names or an artist's name as a substitute for direction.
6. Freeze a candidate pack hash for the next stage.

The contact sheet is a review aid, not a style average. One dominant reference
family should govern a shot. Combining every appealing image in one request is
likely to produce reference soup rather than synthesis.

Suggested generated artifacts are:

```text
outputs/ad-hoc/duotone-111/visual-method/
├── contact-sheets/
│   ├── noir-thriller.png
│   └── tokyo-psychedelia.png
├── style-frames/
├── canaries/
├── reviews/
└── receipts/
```

### Contact-sheet readiness gate

The stage is ready for human review when:

- every tile has a stable identifier and source hash;
- source orientation and any derived crop are disclosed;
- provenance or use status is recorded rather than inferred;
- hero and rejection choices remain visibly pending;
- no source image has been changed by the sheet generator.

## Stage 1B: finished style frame before motion

Create a finished 1080×1920 portrait frame locally for each representative
scene. Photography, collage, drawing, paint-over, local image generation, Izzi,
and conventional compositing are all valid. The acceptance question is simple:
would the still be usable if it never moved?

For the first gate, author only three frames:

1. one established noir-thriller frame;
2. one established Tokyo-psychedelia frame;
3. one transition frame that visibly relates them.

The provider brief then becomes narrow and testable:

```text
The supplied frame is the composition and visual authority.

Preserve:
- <subjects, geometry, palette, lighting, and texture>

Animate only:
- <rain, reflection, smoke, fabric, foliage, or restrained camera movement>

Do not introduce:
- <text, logos, new subjects, palette replacement, camera behavior, or other
  observed failure modes>
```

If the still is not accepted, do not spend a provider generation trying to fix
its art direction through motion prompting.

## Stage 1C: short pairwise canary tournament

Use one fixed two-to-three-second scene to compare controlled variations. Hold
the source frame, duration, geometry, and all available deterministic settings
constant. Change one named variable per round:

- composition constraint;
- palette constraint;
- texture preservation;
- camera movement;
- subject movement;
- degree of surreal transformation.

Present candidates as A and B without treating generation order as quality.
The human decision is `A`, `B`, or `NEITHER`, accompanied by one or more concise
reason codes. Silence is not approval. Preserve the losing candidate and its
reason because it is useful negative evidence for the next round.

Two consecutive satisfactory rounds are the default freeze gate, not an
immutable rule. The reviewers may require another round whenever the result is
fragile, internally inconsistent, or only accidentally successful.

### Canary review dimensions

- **Composition:** did the important geometry and framing survive?
- **Palette:** did color remain intentional rather than averaging or drifting?
- **Texture:** did surfaces retain the desired material quality?
- **Continuity:** do subjects and recurring objects remain identifiable?
- **Motion:** is movement motivated, restrained where requested, and free of
  distracting morphs?
- **Originality:** does the result avoid the generic provider tendencies already
  rejected?
- **Transition:** when applicable, is the relationship between the two visual
  worlds legible?

Do not collapse these dimensions into an unexplained numerical score. Pairwise
choice plus visible reasons provides more useful revision evidence.

## Observed canary round 01: engineering result

The first curated Noir motion round tested three deliberately different
compositions: telephone intimacy, a stair/threshold encounter, and a mesh
speakeasy performance. The first photorealistic fictional telephone frame was
rejected by the provider as possibly containing a real person before task
creation and without reported credit use. Recasting all three compositions as
angular hand-inked Expressionist plates removed ambiguous photographic identity
cues while retaining the composition, palette, and motion brief. All three
graphic inputs then passed validation.

Each corrected request used `quality_tier=pro`, `resolution=1080p`, five seconds,
fixed lens, standard channel, and generated audio disabled. Each completed at
120 credits, for 360 credits total. Independent probing found one silent H.264
stream at 24 fps and 5.041667 seconds in every artifact, but every artifact is
720×1280. The workflow must therefore keep requested and observed resolution as
separate fields and must never manufacture a passing result by upscaling.

The local review package is
`outputs/review/feedback/visual/duotone-111/noir-canary-round-01/`. It contains:

- the initial and superseding graphic style frames;
- provider-original MP4 files;
- lossless native-resolution MKV remuxes;
- one ten-frame portrait filmstrip and ten addressable review frames per clip;
- manifests with duration, dimensions, frame rate, media hashes, and review
  state.

The round-01 web derivatives were superseded during the Stage 2 hard cutover.
They remain in situationshipin.space git history at commit
`eaa43a94df5fc2cc40ccc84aec36facfd08b50f4` and in the local package above;
the live portal does not retain duplicate round-01 assets.

Technical inspection confirms visible temporal change and broad composition
retention. It does not substitute for human review of movement, emotional tone,
continuity, or aesthetic fit. No further provider batch should begin until the
three clips receive explicit `ACCEPT`, `REVISE`, or `REJECT` decisions and the
1080p provider mismatch has a deliberate disposition.

## Observed Noir Stage 2: photographic-medium correction

The review export
`resources/izzi-review-izzi-review-2026-08-11.5.json` contains three `REVISE`
decisions. Its canonical review identifier validates as
`sha256:30316e28cd308d1701f914cd1f47c2901bbc449c38b16033c1475118c79a7f75`,
and all three reviewed artifact hashes match portal build `.5`. The exact
shared correction is:

> great use of scene, composition. It's just the style is not black and white
> film noir. Assume black and white photography style, high-contrast lighting,
> heavy shadows (chiaroscuro), moody atmospheric elements like rain or
> cigarette smoke, and classic 1940s compositions

That text is the Stage 2 medium baseline. The controlled scene variations are:

1. telephone: guilloche projected as real light through smoke over the
   shadowed face and receiver;
2. stair threshold: banister and blind shadows form restrained physical moire
   across the wet wall and rear-view coat;
3. mesh speakeasy: two mesh depths form optical moire while a faint guilloche
   light-gobo moves through haze behind the singer.

All three 1080×1920 source frames passed local visual inspection before provider
submission. The complete local package is
`outputs/review/feedback/visual/duotone-111/noir-stage-02/`, including source
frames, the exact stage specification, provider originals, native MKV remuxes,
ten sampled frames per clip, and portrait filmstrips.

Each provider request used Pro, explicit `resolution=720p`, five seconds,
fixed lens, standard channel, and generated audio disabled. Each completed for
55 credits, or 165 credits total. Independent probing found one silent H.264
stream at 24 fps and 5.041667 seconds in every result. The provider nevertheless
returned 496×864 for all three. These are below-720 creative previews, not 720p
delivery evidence. They were not retried or upscaled.

Current compact review derivatives are:

- [telephone style frame](https://situationshipin.space/review/provider-inputs/duotone-111/noir-stage-02/stage-02-canary-01-telephone-film-noir.jpg);
- [telephone filmstrip](https://situationshipin.space/review/izzi/2026-08-11/video/duotone-noir-stage-02-canary-01-telephone/duotone-noir-stage-02-canary-01-telephone.filmstrip.jpg);
- [stair-threshold style frame](https://situationshipin.space/review/provider-inputs/duotone-111/noir-stage-02/stage-02-canary-02-stair-threshold-film-noir.jpg);
- [stair-threshold filmstrip](https://situationshipin.space/review/izzi/2026-08-11/video/duotone-noir-stage-02-canary-02-stair-threshold/duotone-noir-stage-02-canary-02-stair-threshold.filmstrip.jpg);
- [mesh-speakeasy style frame](https://situationshipin.space/review/provider-inputs/duotone-111/noir-stage-02/stage-02-canary-03-mesh-speakeasy-film-noir.jpg);
- [mesh-speakeasy filmstrip](https://situationshipin.space/review/izzi/2026-08-11/video/duotone-noir-stage-02-canary-03-mesh-speakeasy/duotone-noir-stage-02-canary-03-mesh-speakeasy.filmstrip.jpg).

The source MP4 and MKV files remain local. Publication of compact derivatives
does not imply Stage 2 aesthetic acceptance, approval of any individual frame,
resolution acceptance, or baseline promotion.

## Observed Noir Stage 3: animation-medium round

The next controlled round changes one variable: medium. It retains the three
Stage 2 compositions, monochrome 1940s noir lighting, deep spatial staging,
identity concealment, and in-world guilloche or moire, but replaces photography
with clearly two-dimensional cinematic animation.

The first reference-led pass preserved too much photographic microtexture to
make that change unambiguous. A bounded second pass therefore required clean
economical contours, three-to-five grayscale value shapes, broad cel-shadow
boundaries, and painted two-dimensional backgrounds. It explicitly excluded
photorealism, three-dimensional rendering, etched or carved linework, comic-page
treatment, flat poster composition, and color. Both passes remain local so the
correction can be audited.

The three selected 720×1280 proofs and their comparison sheet are in
`outputs/review/feedback/visual/duotone-111/noir-animation-stage-03/`:

- telephone: the original call composition with projected guilloche continuing
  to obscure the foreground face;
- stair threshold: the rear-view figure and distant doorway with physical
  banister and blind shadows forming restrained moire;
- mesh speakeasy: the singer, listeners, microphone, table, and two mesh depths
  translated into a painted cel-animation frame.

The exact selected prompts, input and output hashes, two-pass decision, and
authority state are recorded in `prompt_manifest.json` and
`stage-03-spec.json`. Local inspection confirms that the selected pass reads as
animation and preserves the intended scene family; it does not constitute human
aesthetic acceptance. No video-provider request, credit use, publication, or
baseline promotion occurred in Stage 3.

## Stage 1 completion: style-pack freeze and episode production

A style pack may advance from `CURATING` to `FROZEN-CANDIDATE` only when:

- the Stage 0 local-pipeline receipt is `WORKING-PROVISIONAL` and its exact
  input hashes still match;
- its exact anchor, supporting, and rejection image hashes are recorded;
- its visual, motion, preservation, variation, and avoidance rules are filled;
- the representative portrait style frame has human approval;
- the short canary has human approval at its actual delivered resolution;
- the provider request settings and observed cost are captured separately;
- the approved pack and canary artifacts have immutable hashes;
- approval names the artifact reviewed and does not rely on silence.

`FROZEN-CANDIDATE` means suitable for a bounded episode trial, not universal
approval. A ten-episode test may still reveal continuity failures that require a
new pack version. Never rewrite `v1` in place; derive `v2` with explicit reasons
and lineage.

The first production trial should use one approved style pack, one approved
scene, and one bounded provider request. Validate actual pixel dimensions,
duration, frame rate, audio policy, cost, and content before extending the run.
The local pipeline then applies exact titles, accessibility assets, intentional
glitches, transitions, and episode assembly.

## Roles and authority

| Role | Responsibility |
|---|---|
| Human curators | Collect references; identify anchors and rejections; author or approve style frames; make A/B decisions; approve or reject a frozen candidate |
| Izzi | Verify hashes; preserve provenance; generate contact sheets and receipts; render exact local graphics; assemble and test final media |
| Motion provider | Animate a bounded, already resolved visual frame under explicit constraints |
| Local post-production | Preserve exact typography, guilloche, glitch, silence, audio, transitions, color treatment, and deliverable encoding |

This method does not authorize Codex to populate or revise the human seed
collections, infer collaborator approval, submit a provider request, publish
media, or transfer training-conversion material. Each of those actions retains
its separate authority boundary.

## Failure ceiling and local escape hatch

Do not answer repeated provider aesthetic failure with an indefinitely longer
prompt or escalating spend. If two controlled canary rounds fail the same
must-preserve constraint:

1. stop provider submission;
2. inspect whether the style frame or constraint is ambiguous;
3. revise the local frame or split the requested motion into simpler layers;
4. consider local parallax, masks, Izzi animation, compositing, or an Asama-loop
   treatment instead of provider generation;
5. begin a new canary round with the revision explicitly named.

The provider is replaceable. The human style pack, source lineage, review
evidence, local effects, and final assembly are the durable project assets.

## Next human checkpoint

Noir Vibezz evaluation Stage 1 is complete. Stage 2 photographic motion remains
available as evidence, and Stage 3 now supplies a controlled local
animation-medium alternative. The next bounded checkpoint is comparative human
review, not another provider batch:

1. review each complete local MKV; use its ten-frame web filmstrip to attach
   frame-specific notes, not as a substitute for temporal review;
2. record `ACCEPT`, `REVISE`, or `REJECT` for telephone, stair threshold, and
   mesh speakeasy, with composition, palette, texture, continuity, motion, and
   originality reasons;
3. decide whether projected guilloche and physical moire preserve the intended
   photographic medium while providing useful identity concealment;
4. treat the actual 496×864 results as below-720 previews and decide whether
   provider/account resolution must be settled before any episode trial;
5. compare the three Stage 3 stills against their Stage 2 photographic anchors
   and decide whether animation is the preferred Duotone medium;
6. authorize another paid canary only after naming the retained frame, the one
   variable to change, the duration, and the cost ceiling.

Tokyo-psychedelia and the bridge remain deferred until their inputs exist. They
are not prerequisites for proving or refining the noir branch.

This checkpoint seeks convergence, not volume. A small visual contract that both
reviewers recognize is more valuable than a large provider batch that must be
explained after the fact.
