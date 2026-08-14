# Here Lies Trouble: Neon Addict visual test plan

## Status and boundary

The local still-image test for *Here Lies Trouble* passed human review on
2026-08-11. The user explicitly accepted the albatross, the three Los Angeles
locations, and the restrained animal-pattern direction, then authorized one
complete local Episode 1 engineering trial with the existing random-default
transformed audio. Human review rejected that trial's motion form, generic
filename, and mono dialogue-only mix: camera drift over one still does not show
the animals talking, walking, and exploring Frogtown. The correction plan is
`docs/development/explore_futures_here_lies_trouble_episode_01_v2.md`, with an editable
boundary at
`examples/seedance2/here-lies-trouble-episode-01-frogtown.v2.json`. This does
not assign recorded voices to animals, authorize a provider job or publication,
promote a production baseline, or accept a corrected episode that does not yet
exist.

The source export
`resources/izzi-review-izzi-review-2026-08-11.6.json` is preserved unchanged.
Its SHA-256 is
`4bd63f46ca176cba26b75d9371400c3ef1c5f562cd61f6ed47e87ece1bf332a4`,
and its canonical review identifier validates. The export contains 62 records:
59 Neon Addict decisions plus three Noir decisions carried over because the
form was not reset. Those three records are excluded from this evaluation.

A provenance-preserving filtered view is available at
`outputs/review/feedback/style-processing/neon-addict/neon_addict_review_2026_08_11_6.filtered.json`.
It has SHA-256
`448d1692e3b72aed4b7fda8634bfb3386a4d4baa32621b4b1e5df16840fd3150`
and records the three exclusions without rewriting the original export. All 59
retained artifact identifiers and hashes match the review portal catalog.

## What the human review says

The valid Neon Addict subset contains:

| Decision | Count |
| --- | ---: |
| Positive | 32 |
| Negative | 9 |
| Exclude | 16 |
| Observation only | 2 |

Only assets 001–060 were touched, with 018 absent from the export. The remaining
Neon Addict collection is unreviewed; silence is not a decision.

The strongest positive signals are:

- bold cyan, chartreuse, magenta, safety-yellow, coral, black, and off-white;
- sparse compositions with large negative fields;
- clear geometric counterweights: circles, spikes, squares, rectangles,
  lightning forms, waves, and scribble fields;
- tiny conversational figures or creatures beneath oversized environmental
  forms;
- mushrooms, insects, speculative plants, shelter, rain, caves, and friendship;
- strong center weight or deliberate asymmetry rather than a crowded collage;
- tactile paper, ink, scale, and screenprint-like texture.

The explicit negative signals are equally useful:

- embedded typography is repeatedly negative;
- brown and muddy palette drift are negative;
- `EXCLUDE` is not a weak positive and must never enter an anchor set;
- `OBSERVATION-ONLY` is descriptive evidence, not approval.

Several positive references contain printed words even though the reviewer
selected only their composition, color, atmosphere, or subject. Generated work
must therefore preserve the reviewed visual property while removing all source
text. Captions and titles remain exact local Izzi layers.

## Here Lies Trouble contract being tested

The test combines the valid Neon Addict evidence with already established
project constraints:

- cast: one original unicorn, albatross, dog, and cat;
- character language: economical contour, readable silhouettes, expressive
  posture, and no copied proprietary character design;
- composition: oblique geometry, angular shadow, theatrical negative space,
  and bold contrasting texture;
- motion language: patient organic movement, listening reactions, and coherent
  speculative ecology rather than exact mouth-sync;
- episode world: one distinct Los Angeles setting per approved audio cut;
- audio rule: every audible source voice is transformed locally before any
  review or release mix; no raw audio or transcript is sent to an image or
  motion provider;
- casting rule: speaker-to-animal mapping remains pending corrected diarization
  and explicit human selection.

The ten-cut candidate document and its timeline are already hash-bound to human
approval:

- candidate SHA-256:
  `3bea01076d471e8e2e04801d86abee2bbbab14dd805940f42e55f0e7a0cce8fc`;
- timeline SHA-256:
  `5f26d36a99500702fbf3e40d17e669b51c216e8fa33936485d43dcf32831c20e`.

The first three approved cuts are 165, 146, and 158 seconds. Their working
visual assignments are Frogtown beside the Los Angeles River, the Arts District
at the river edge, and Griffith Park. The existing transformed-default audio
episodes remain useful engineering references, but their neutral voice codes do
not imply animal casting.

The earlier Here Lies Trouble provider canary remains engineering evidence. It
proved that the four-animal ensemble and a vertical neon world can survive a
motion request, but its generic neon-library treatment is not the new aesthetic
baseline.

## Initial 720p still proofs

Three original 720 × 1280 portrait keyframes now test cast continuity,
location differentiation, the reviewed color/composition language, and a calm
caption-safe lower field:

| Sample | Approved cut / working location | Local artifact | SHA-256 |
| --- | --- | --- | --- |
| 01 | Episode 1 / Frogtown, Los Angeles River | `outputs/review/feedback/visual/here-lies-trouble/neon-addict-stage-01/style-frames/sample-01-frogtown-720.png` | `1b90af5c2d9aaf5425c04b5fb2a7b92e8559ef23d37c5ab973770b23c3319c15` |
| 02 | Episode 2 / Arts District river edge | `outputs/review/feedback/visual/here-lies-trouble/neon-addict-stage-01/style-frames/sample-02-arts-district-720.png` | `52e7934b7f2048fcaf59e645f3b226f30a22e824e815ed47d2aa1fb123cb5077` |
| 03 | Episode 3 / Griffith Park | `outputs/review/feedback/visual/here-lies-trouble/neon-addict-stage-01/style-frames/sample-03-griffith-park-720.png` | `16948596203c93b419b68bdc744a086ca1c6ca0ad510db54702b7186fa4ab98f` |

The contact sheet is
`outputs/review/feedback/visual/here-lies-trouble/neon-addict-stage-01/here-lies-trouble-neon-addict-samples-01-03.png`.

The three images form an accepted first pass: character markings are stable,
the off-white/black/neon grammar is recognizable, the Los Angeles environments
remain distinct, no source typography survives, and the bottom field is usable
for captions. Human feedback specifically resolves the earlier albatross,
location-specificity, and pattern-density questions in favor of the current
rendering. A deliberate model sheet is still useful before scaling beyond the
trial, but it is no longer a blocker for the requested local Episode 1
engineering assembly.

## Test sequence

### Gate 1: still review

Status: `USER-ACCEPTED` on 2026-08-11 for all three proofs, including the
albatross, Los Angeles locations, and restrained animal patterns.

Review each sample independently as `ACCEPT`, `REVISE`, or `REJECT` for:

1. Neon Addict aesthetic fit;
2. animal silhouette and cast continuity;
3. Los Angeles setting recognition without landmark clutter;
4. compositional energy and amount of negative space;
5. caption-safe space and non-color identity cues;
6. originality and absence of embedded text.

Also choose whether the cast should remain mostly black/white with neon markings
or become more fully patterned. A decision on this single variable prevents
later motion tests from comparing several style changes at once.

### Gate 2: cast freeze

After at least one accepted still, create one 720p model sheet with front,
three-quarter, profile, rear, and expressive listening poses for all four
animals. Assign stable non-color identifiers—horn shape, wing edge, dog ear,
cat flank circle, proportions, and recurring accessories—so accessibility does
not depend on hue. Refine the albatross until it reads clearly at thumbnail
size. Do not map a participant or voice to an animal during this gate.

### Gate 3: three short motion canaries

Animate only one controlled property in each accepted frame:

- Frogtown: five seconds of lateral travel, water glint, and wheel arcs;
- Arts District: five seconds of listening posture plus sliding angular shadow;
- Griffith Park: five seconds of plant breathing, distant haze, and one wing
  adjustment.

Start locally with shallow parallax and Izzi overlays. If provider motion is
later authorized, submit silent 720p requests sequentially with a separate cost
ceiling and no automatic retry. The most recent provider evidence returned
496 × 864 after an explicit 720p request, so every result must be probed and
reported at its actual dimensions.

### Gate 4: audio-linked episode prototype

Use one approved 1–3 minute cut and the existing locally transformed-default
audio as a functional prototype. Extend accepted motion locally, use generic
speaker codes for active-listener emphasis, render exact captions as a separate
track, and retain the approved title-card system. Do not infer a final voice,
animal, or performer assignment from the random defaults.

The user explicitly advanced this local gate on 2026-08-11. Episode 1 used the
approved Frogtown frame and all 165 seconds of its existing local random-default
audio. The review master retained the accepted three-second numbered title card,
so its verified total duration is 168 seconds. Human review then rejected the
motion form: restrained deterministic camera drift supplied no character or
scene-content motion. The corrected form must show all four animals conversing,
listening, walking, and exploring Frogtown. It must also carry the location and
version in the filename and replace the mono dialogue-only presentation with a
speaker-balanced stereo mix over the user-selected river ambience. The current
MKV and filmstrip remain rejected-motion engineering evidence, not a baseline.

### Gate 5: three-episode review, then ten

Assemble episodes 1–3 only after the still, cast, motion, and transformed-audio
gates pass. Review continuity across the three settings before expanding to ten.
The ten-episode run remains a separate human and budget decision.

## Immediate human decision

The still decision is complete and the first combined package has disposition
`REVISE`. The user confirmed
`docs/development/explore_futures_here_lies_trouble_episode_01_v2.md` and its JSON defaults.
The provisional stereo river-ambience audio canary is technically verified and
ready for human listening. Three five-second silent motion-canary
specifications are schema-validated, but no motion output was generated. The
immediate decisions are whether the audio balance and ambience should pass or
be revised, and whether the group-walk, conversation-reaction, and Frogtown-
exploration motion specifications express the intended test. Provider
selection, spending, corrected diarization, final speaker-to-animal casting,
caption acceptance, publication, and production expansion remain separately
gated.
