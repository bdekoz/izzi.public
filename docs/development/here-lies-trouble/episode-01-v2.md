# Here Lies Trouble Episode 1: Frogtown v2 feedback plan

## Decision requested

This is the correction plan for the first complete *Here Lies Trouble* episode.
It integrates the 2026-08-11 human review and waits for confirmation before any
audio render, motion generation, provider submission, spending, publication,
or baseline promotion.

The editable production boundary is
`examples/seedance2/here-lies-trouble-episode-01-frogtown.v2.json`; its validation
contract is `schemas/izzi.here-lies-trouble-episode-v2.schema.json`. The JSON is
deliberately usable with proposed defaults while leaving genuine human choices
visible as `NEEDS-HUMAN` or `null`.

The requested review-master identity is:

`here-lies-trouble-episode-01-frogtown.v2.mkv`

The existing rejected-motion artifact remains evidence and is not overwritten.
The user-requested v1 identity is
`here-lies-trouble-episode-01-frogtown.v1.mkv`; applying that rename is deferred
until work resumes so the current evidence path and manifest are not silently
changed during planning.

## Feedback incorporated

The Frogtown still, albatross, Los Angeles location language, and restrained
animal patterns remain accepted. The following properties of the first
full-episode assembly are rejected or require revision:

- camera drift across one still is not character or scene motion;
- the unicorn, albatross, dog, and cat must converse, listen, walk, and explore;
- Frogtown and the Los Angeles River must be evident in the scene and filename;
- the mono dialogue-only mix must become a spatial review mix;
- dialogue levels must be balanced by actual speaker for the production mix;
- the selected river recording must supply a quiet outdoor ambience bed.

The existing audio manifest states `speaker_diarization: NOT-PERFORMED`. Its 11
approximately 14–16 second windows carry randomly assigned engineering voice
codes and may cross real speaker changes. A quick pass can therefore normalize
those windows for an engineering canary, but it cannot honestly claim
per-speaker normalization. Corrected diarization and stable speaker turns are a
production prerequisite.

## Episode contract

The v2 review master remains a 720 × 1280, 30 fps preview: three seconds of the
accepted black-and-white title card followed by 165 seconds of episode content,
for 168 seconds and 5,040 frames total. The first production-quality promotion
may move to 1080 × 1920, but preview geometry and provider-returned geometry must
always be reported separately.

The visual acceptance contract is:

1. Exactly one unicorn, one albatross, one dog, and one cat persist throughout.
2. The group visibly talks and listens through turn-aware gesture and reaction.
3. They walk through and investigate Frogtown rather than posing in a tableau.
4. Legs, bodies, plants, water, and environmental details move; camera motion
   may support this but cannot substitute for it.
5. The Los Angeles River edge, bicycle-path geometry, and urban/riparian
   mixture establish the location without generic skyline shorthand.
6. The accepted restrained Neon Addict patterns and readable silhouettes remain
   stable; species swaps, duplication, extra limbs, disappearing characters,
   generated text, and identity drift fail the shot.
7. Exact lip sync is not required for v2. Active-speaker gesture, head and body
   orientation, and listener reactions are the proposed readable language.

## Editable choices and usable defaults

| Boundary | Proposed default | Human input needed now? |
| --- | --- | --- |
| Route | Bike-path entrance → river overlook → bridge shadow → riparian edge → final river view | Optional; the five beats can be rewritten in JSON |
| Conversation | Turn-aware gesture, no exact lip sync | Confirm or edit |
| Animal movement | Four distinct proposed movement personalities in JSON | Confirm or edit |
| Speaker-to-animal casting | All mappings remain `null` | No; defer until corrected diarization and voice review |
| Location references | Accepted Frogtown still plus explicit location anchors | Optional; add repository-relative image paths and hashes if more specificity is wanted |
| Captions | Exact local overlay or subtitle track, never provider-generated | Defer until transcript/caption review |
| Motion provider | Unselected; external submission is false | Required before any paid motion canary |
| Preview quality | 720p requested, actual returned geometry disclosed | Confirmed working preference |
| Ambience | User-selected river recording, beginning after its opening silence | Already supplied |
| Audio targets | -16 LUFS-I delivery, ≤ -1.5 dBTP, LRA ≤ 12 LU; ambience near -32 LUFS-I with 5 dB speech ducking | Proposed engineering defaults; approve by listening |

The only motion input that materially improves the first canaries is a more
specific Frogtown route or extra user-selected reference images. Neither is a
blocker: the JSON contains a five-beat route and the accepted still. The
speaker-to-animal mapping should remain empty rather than be guessed.

## Production sequence

### Stage 0: freeze identity and evidence

- validate the editable JSON against its schema;
- retain the current artifact and hashes as rejected-motion engineering
  evidence;
- apply the requested Frogtown v1 filename and update all corresponding local
  manifest references without overwriting media;
- record pre-change hashes, intended outputs, checks, and acceptance conditions.

### Stage 1A: provisional audio canary

This pass answers “does the corrected mix shape work?” before doing expensive
speaker correction.

1. Decode `Sounds of Nature - The River.mp3` once to a verified 48 kHz stereo
   lossless intermediate. The MP3 is 191.778 seconds, so a bounded 165-second
   region beginning after the opening silence does not require looping.
2. Measure and normalize each of the 11 existing engineering windows to the
   proposed canary target. Label the result
   `PROVISIONAL-WINDOW-NORMALIZED-REVIEW-MIX`, never “speaker-normalized.”
3. Place dialogue in a centered stereo image. Mix river ambience at the proposed
   low level and duck it about 5 dB during speech.
4. Deliver a lossless audio canary and a clearly labeled audio-review MKV. The
   rejected still may be used only as a listening placeholder, not as v2 motion.
5. Measure integrated loudness, true peak, loudness range, channel layout,
   duration, ambience presence, and each window’s level spread; then obtain a
   human listen/revise/accept decision.

### Stage 1B: production dialogue correction

1. Run local diarization over the bounded 165-second source interval.
2. Review and correct its speaker boundaries; assign stable anonymous speaker
   codes without mapping people to animals.
3. Rebuild or segment the transformed dialogue by actual turn.
4. Normalize by stable speaker and confirm intelligibility before ambience is
   restored.
5. Preserve the random defaults as first-pass evidence; do not represent them as
   human-selected voices.

### Stage 1C: three motion canaries

Generate three silent, sequential, five-second 720p canaries only after the
motion method and any provider cost ceiling receive explicit approval:

1. a coherent four-animal group walk;
2. one speaking gesture with three distinct listener reactions;
3. location exploration at the river edge with a visible Frogtown anchor.

Each canary is inspected for real body and environmental motion, cast identity,
location specificity, composition, and mutation failures. A failure stops the
sequence; there is no automatic retry or full-episode batch.

### Stage 2: complete local animatic

Build the full 165-second content timeline from approved keyframes, accepted
motion canaries, deliberate holds, reaction shots, and location cutaways against
the corrected review mix. Eleven narrative beats can initially follow the
existing audio-window lengths, while corrected speaker turns determine gesture
cues. This proves route, rhythm, shot order, title timing, and audio/visual form
before purchasing or rendering final motion.

The animatic is a human gate, not a production baseline.

### Stage 3: full motion package

After animatic acceptance, make a shot manifest with source hashes, prompt or
local-animation parameters, seed, requested and actual geometry, duration,
provider cost if any, and continuity checks. A five-second-only provider would
require roughly 33 content slots for 165 seconds, so full submission is never
inferred from approval of three canaries. Reusable walking cycles, reaction
shots, local Izzi overlays, and deliberate scenic cutaways may reduce provider
work without reverting to a single drifting still.

### Stage 4: final review assembly

- combine the accepted motion package with the speaker-corrected stereo mix;
- keep the three-second title silent unless explicitly revised;
- encode the exact Frogtown v2 basename and create a ten-frame filmstrip;
- verify 168 seconds, 5,040 frames, 720 × 1280, 30 fps, yuv420p, 48 kHz stereo,
  loudness, true peak, and hash-bound lineage;
- watch and listen to the entire episode, then record explicit `ACCEPT`,
  `REVISE`, or `REJECT`. Silence is not acceptance.

## Confirmation boundary

The user confirmed the editable choices and usable defaults on 2026-08-11.
That confirmation authorized only Stage 0, Stage 1A, and preparation of the
three silent motion-canary specifications. Provider use remains separately
closed.

## Local canary implementation checkpoint

Status: `TECHNICALLY-VERIFIED-HUMAN-AUDIO-REVIEW-PENDING`.

The rejected-motion artifact now has its required evidence identity:

`outputs/review/feedback/visual/here-lies-trouble/neon-addict-stage-01/episode-01-local-trial/here-lies-trouble-episode-01-frogtown.v1.mkv`

It was renamed without re-encoding. Its SHA-256 remains
`a6f754c62de7ab194e1ff76241a45af33826a34052d812bdfebcd4ca5ac72718`,
and its manifest records `REVISE` and the rejected motion form.

The provisional listening package is:

- audio-only WAV:
  `outputs/review/feedback/visual/here-lies-trouble/neon-addict-stage-01/episode-01-local-trial/audio/here-lies-trouble-episode-01-frogtown.v2.audio-canary.wav`;
- listening MKV:
  `outputs/review/feedback/visual/here-lies-trouble/neon-addict-stage-01/episode-01-local-trial/here-lies-trouble-episode-01-frogtown.v2.audio-canary.mkv`;
- manifest:
  `outputs/review/feedback/visual/here-lies-trouble/neon-addict-stage-01/episode-01-local-trial/audio/here-lies-trouble-episode-01-frogtown.v2.audio-canary.manifest.json`.

The canary measures -16.03 LUFS-I, -1.49 dBTP, and 4.3 LU loudness range.
It is exactly 165 seconds at 48 kHz stereo. The listening MKV is exactly 168
seconds, retains the 720 × 1280 / 30 fps v1 video as an explicitly rejected-
motion listening placeholder, and has a silent three-second title interval.
Its post-title decoded PCM matches the WAV exactly.

All 11 engineering windows land within 0.2 LU of the -18 LUFS-I target and at
or below the -1.49 dBTP measured ceiling. This is still not per-speaker
normalization: corrected diarization remains required before Stage 1B.

The three five-second motion specifications are:

1. `examples/seedance2/here-lies-trouble-episode-01-frogtown.v2.motion-canary-01.json` — group walking;
2. `examples/seedance2/here-lies-trouble-episode-01-frogtown.v2.motion-canary-02.json` — speaking gesture and three listener reactions;
3. `examples/seedance2/here-lies-trouble-episode-01-frogtown.v2.motion-canary-03.json` — Frogtown river-edge exploration.

They validate against
`schemas/izzi.here-lies-trouble-motion-canary.schema.json`. Provider, model,
seed, and cost remain unset; motion generation and submission are false. No
motion artifact exists yet.

Each specification also references hash-bound Blade Runner motion-aesthetic
seeds (`resources/blade-runner/motion-aesthetic-seeds.json`) and carries a
derived `motion_grammar` block; only the derived descriptor text is eligible
for prompts, and the film clips stay local-only. See
`docs/development/blade-runner/motion-seeds.md`.

The next gates are human listening of the audio canary and human review of the
three motion specifications. Neither technical verification nor silence is
acceptance.

To accept the plan as written, reply:

`CONFIRM HERE LIES TROUBLE FROGTOWN V2 PLAN`

That confirmation has been received. To revise later stages, edit
`examples/seedance2/here-lies-trouble-episode-01-frogtown.v2.json` and identify
the fields changed. Provider submission and a cost ceiling still require a
separate explicit instruction.
