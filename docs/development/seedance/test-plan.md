# Audio-to-vertical Seedance test plan

- Date: 2026-08-09
- Projects: Duotone Café and Here Lies Trouble
- Status: local audit complete enough for two bounded provider canaries; unattended execution authorized
- Account context: user reports an Annual Standard plan and prefers native 1080p
- Form authority: accepted synthetic form check 02

## Decision summary

The same ten-episode vertical form can test two substantially different ways of
turning recorded conversation into visual narrative.

| Option | Editorial organizing principle | Visual experiment | Best test of | Primary risk |
| --- | --- | --- | --- | --- |
| Duotone Café | A spoken `prompt` starts a reality break that lasts through the episode end | Textless high-contrast noir gives way to Tokyo/psychedelic AI-image fantasy | World building, visual contrast, and transitions between realities | ASR markers require human listening; the style workbook contains competing constraints |
| Here Lies Trouble | Timestamped conversational and argumentative arcs | Animated animals consistently embody the voices | Character continuity, multi-voice dialogue, captions, and long-form editorial rhythm | Recording consent does not by itself establish provider, depiction, voice-use, or publication authority |

Duotone is the stronger visual-world test. Here Lies Trouble is the stronger
dialogue and character-system test. The recommended sequence is to prepare both
locally, make one short provider canary for each, then choose one project for
the complete ten-episode test. The separate transfer and spending approval for
exactly these two canaries was recorded on 2026-08-09; it does not authorize a
complete series or a paid retry. Do not pay for two full series merely to decide
which visual premise is working.

## Shared ten-episode contract

Both options inherit the accepted synthetic-form-check structure:

- ten ordered vertical episodes;
- one three-second black title card before every episode;
- white devastation-pacific-house-style typography;
- the episode number is the only title-card text;
- 60–180 seconds of source-audio content per episode;
- 1,080 by 1,920 output at 30/1 fps;
- H.264/yuv420p Matroska review masters;
- original source audio retained locally as private lineage evidence;
- every audible Here Lies Trouble voice transformed locally before inclusion in
  a review or release episode;
- no provider-generated speech, voice cloning, or imitation of a target person;
- static episode posters and a ten-episode filmstrip;
- one merged MKV for form review; and
- exact source hashes, cuts, provider anchors, local transforms, and output
  probes in a resolved manifest.

The account description and 1080p preference are user-supplied facts, not an
inspection of the provider dashboard. Public Seedance2AI pages observed on
2026-08-09 describe Annual Standard as a 1× pricing account and list Fast-class
text-to-video without video input at 9 credits per second for 720p and 19
credits per second for 1080p. The selected two-job 1080p test therefore expects
190 credits per ten-second canary and 380 credits total, compared with 180
credits total at 720p. The API calls the selected tier `standard`, while the
public application calls the priced class Fast; that mapping is an operating
inference and must be checked against each actual response rather than presented
as a provider guarantee.

Submit Here Lies Trouble first. If its response reports any value other than
190 credits used, stop before Duotone and report the discrepancy. There are no
automatic retries. If the provider returns less than the requested resolution,
preserve that source and report the mismatch; do not describe an upscale as a
native 1080p result.

Current public references:

- <https://www.seedance2ai.io/pricing>
- <https://www.seedance2ai.io/app/video/seedance2pro>
- <https://www.seedance2ai.io/developers>

The first synthesis pass should use one silent ten-second visual anchor per
episode. Izzi and the local video assembler extend that anchor across the
60–180-second audio segment using reviewed loops, alternate crops, held frames,
layered SVG marks, and deterministic transitions. Generating continuous unique
provider footage for every second would require tens or hundreds of paid jobs
and is not appropriate for the form test.

## Option 1: Duotone Café

### Observed source state

The local Duotone corpus now contains four 16 kHz mono PCM recordings. The
first three have complete machine-transcription drafts; the longer map
experiment has a deliberately bounded VAD-assisted draft for its first
approximately two minutes.

| Audio source | SHA-256 | Duration | Local ASR state |
| --- | --- | ---: | --- |
| `20251114_duotone_cafe_1.wav` | `8fedf2d0110763ca36670a41bd52f55328b63327e39f7c741a44ec49429510d0` | 16:56.520 | complete draft; 3,067 words |
| `20251114_duotone_cafe_2.wav` | `c61c204367c5653fcdcb471521af51a0024926446e7848aedd378cb9907106dd` | 04:52.160 | complete draft; 1,041 words |
| `20251114_duotone_cafe_3.wav` | `347cd73be744502306487b6b3eff3675a814d255bfcc9ee514f7803aa1e08432` | 18:40.760 | complete draft; 3,201 words |
| `20251207_duotone_map_experiment_one.wav` | `78d48e0b6ef0d517a6f3158249634b7f62e4d89de053539c2e08a64f05d00473` | 1:32:39.780 | partial VAD draft through about 02:01.840; 316 words |

The retained drafts total 7,625 words. They are ignored local evidence in TXT,
VTT, SRT, and JSON forms; none is provider input. The complete JSON draft
hashes are `391209e36fb44ebb72ba4342d16fbac33aaefda4f9deea8346ae7629a969dae9`,
`44a383ebf297b3e0ec88e9d645f1256840f5b858522c6a78a215af7e651a0db2`,
and `9236b156dd6527bc931924fd8fcef992d9e21573aaec1fdb03a5863a3f4fc7a2`.
The partial map-experiment JSON hash is
`218d2465b3a9f5799d7dda6aa0556736c63af38029903f52245c86c16ab246a5`.

Style workbook:

```text
path: resources.rizal/duotone-111/kw-seedance-inputs.20260808.xlsx
sha256: c3f1e2813f6d34eedb6049cd0a965720a9f8ac651d9c92216d0ebc11056c3146
worksheet: Sheet1
creative rows: 2
titles: The Setup; The Suspicious Partner
```

The local transcription backend is `whisper.cpp` commit
`592feef04a1802b18cbeffd0fd0eb5d02570c2ec` with the quantized large-v3-turbo
model. The unfiltered pass over the 92-minute map experiment produced repeated
phrases in long non-speech regions and was discarded. Silero VAD identified 402
speech segments, reduced the candidate audio by about 37%, and exposed a
verified approximately 4:12 non-speech interval around 40:41.66–44:53.22. At
the user's direction, that source remains a bounded partial transcript: silence
and glitch are active editorial material, not gaps to fill with invented text.

The first café recording alone can support ten non-overlapping installments:
an exhaustive equal division averages 101.652 seconds per episode, safely
inside the 60–180 second contract. The wider corpus supplies additional prompt
markers and material for later exploration of the proposed 60–90 episode form.
Marker-led selection may omit connective material or split a long fantasy
passage.

### First audit: local transcription and prompt markers

1. Hash and probe the source again immediately before transcription.
2. Transcribe locally with word timestamps and speaker turns. Store the raw ASR
   transcript under an ignored private build path; do not upload the recording
   merely to obtain a transcript.
3. Search case-insensitively for a standalone spoken `prompt` token. Preserve
   at least 30 seconds of context on each side for human listening.
4. For this test pass, the first relevant `prompt` marker inside an episode is
   the transition threshold. The fantasy/psychedelic visual mode continues
   from that threshold through the selected episode end.
5. Do not infer, require, or act on an `end prompt` marker in this pass. A person
   must listen to and confirm the start threshold and the episode end.
6. If the material after the prompt is shorter than 60 seconds, move the
   episode start earlier while keeping the prompt threshold fixed. If the full
   episode would exceed 180 seconds, select a coherent sentence or thought
   boundary for the episode end; the fantasy mode still continues to that end.
7. Rank prompt-led episodes by visual distinctness, conversational coherence,
   clean episode boundaries, and coverage. If there are fewer than ten useful
   prompt markers, use connective noir episodes rather than fabricating marker
   events.
8. Emit a local audit JSON containing source hash, transcript-engine identity,
   marker word times, episode boundaries, confidence, speaker IDs, and human
   disposition. Do not make a provider prompt during this phase.

Silence alone is not a useful segmenter for this café recording. At -35 dB with
a 0.45-second minimum, only one quiet interval was found, at approximately
06:45.302–06:45.767. Café ambience and overlapping conversation require speech
activity, speaker turns, and sentence boundaries rather than silence cuts.

### Three alternative cutting methods

1. **Idea-arc cuts.** Segment the transcript into question, elaboration,
   escalation, and landing beats. Select ten self-contained ideas, then snap
   their boundaries to complete speaker turns. This is the best alternative if
   `prompt` is used inconsistently.
2. **Acoustic dramaturgy.** Use voice activity, speaker changes, overlaps,
   laughter, energy peaks, and the sparse quiet intervals to identify changes
   in conversational intensity. Use acoustic boundaries only as candidates;
   the transcript decides whether the thought is complete.
3. **Coverage grid with sentence snapping.** Start with ten contiguous windows
   of approximately 101.652 seconds and move each boundary by at most 20 seconds
   to the nearest complete sentence or speaker turn. This guarantees broad
   coverage and exactly ten valid durations, but treats fantasy markers as
   annotations rather than episode boundaries.

A useful comparison package renders all four cut maps—prompt-led plus these
three alternatives—as horizontal timelines with the same transcript density,
speaker colors, marker positions, and episode-duration warnings. The user can
then choose a segmentation grammar before any visual generation.

### Distilled visual language

The workbook contains two narrative variants but one repeated visual language.
It should be distilled rather than copied wholesale into every provider prompt.

Noir baseline:

- present-day, nameless urban interiors and exterior alleys;
- gritty cinematic hyperrealism with cool saturated shadows and restrained
  warm practical highlights;
- rain, fog, wet pavement, geometric fluorescent light, deep perspective, and
  carefully framed negative space;
- realistic object scale and sparse lived-in ephemera;
- the detective represented through off-screen point of view, traces, or at
  most a distant obscured figure;
- no legible writing, numbers, logos, branded objects, or named buildings; and
- no smoking, alcohol, drugs, firearms, or borrowed intellectual property.

Prompt excursion:

- a recognizable break into Tokyo-like urban density and psychedelic
  AI-image fantasy;
- chromatic duplication, impossible reflections, liquid geometry, folding
  depth, orbital light, and surreal transitions;
- continuity through the noir palette, rain, practical light sources, and
  realistic camera movement; and
- the same prohibitions on legible text, logos, unsafe content, and unlicensed
  characters.

The workbook contains contradictions: a detective subject versus repeated
instructions not to show humans; written evidence versus a universe without
writing; and a ban on living creatures alongside narrow exceptions for cats,
rats, and pigeons. Normalize those into explicit precedence before generation:

1. safety, rights, and no legible text or branding;
2. no close or identifiable human figure;
3. distant silhouette or point-of-view evidence may imply the detective;
4. context-specific sparse animals are optional, never mandatory; and
5. fantasy may bend color, geometry, and physics without changing the first
   four rules.

### Duotone episode pattern

Each episode should use one of two structures selected by the cut audit:

```text
3-second numbered card
  -> noir reality
  -> spoken prompt threshold
  -> Tokyo/psychedelic excursion
  -> episode end
```

or, for a connective episode without a complete marker arc:

```text
3-second numbered card
  -> sustained noir observation
  -> one visual anomaly derived from the episode's central idea
  -> unresolved noir landing
```

The transition at a verified prompt boundary is a strong candidate for Izzi's
Hamonshu or roulette motion language. The generated fantasy/psychedelic mode
then remains active through the episode end.

### Duotone silence and glitch language

Silence is usable material, not dead time. Reviewed non-speech or room-tone
passages may hold on a noir composition, reduce motion to breath-like drift,
open large fields of black, expose paper or film texture, or create an
intentional pause before the next idea. Do not manufacture dialogue from ASR
hallucinations in these regions; captions remain empty unless human listening
confirms speech.

A complete 60–180-second Duotone episode may be `silence-led` or
`glitch-led`; these are first-class editorial kinds alongside `noir-dialogue`
and `prompt-fantasy`. A silence-led episode uses a verified source interval and
intentional room tone or quiet, not a transcription gap mislabeled as art. A
glitch-led episode states which source event, idea, or visual discontinuity it
expresses. The ten-episode pilot may include candidates of either kind, but the
count remains a human editorial decision rather than a generated quota.

Glitch can mark a prompt threshold, a shift in certainty, a broken recollection,
or the passage between conversational reality and fantasy. Candidate Izzi
treatments include registration slip, scan drift, missing-frame cadence,
misaligned color planes, interrupted orbit traces, fragmented type-free SVG
geometry, and controlled Hamonshu or roulette disruption. Preserve a clean
audio reference locally. Audio glitch is optional and, when used, must retain
dialogue intelligibility and caption synchronization.

Glitch never excuses inaccessible flashing. Measure luminance transitions,
avoid repeated high-contrast flashes, keep captions and their backplates stable,
and export a reduced-motion/flicker-safe treatment from the same episode plan.
The local transcription audit records likely non-speech and low-confidence
regions separately from spoken evidence so creative use does not corrupt source
lineage.

## Option 2: Here Lies Trouble

### Observed source state

Audio:

```text
path: resources.rizal/here-lies-trouble/seed-audio-corpus/here-lies-trouble-1.20260415.mp3
sha256: 32e984630b27abbe1acb505002e36059047a3a671de5526f872cffd24cb30d1e
duration: 1805.832 seconds (30:05.832)
format: MP3, 16 kHz, mono
```

Transcript:

```text
path: resources.rizal/here-lies-trouble/seed-audio-corpus/here-lies-trouble-1.20260415.txt
sha256: 7ed51330acad89f4e769d47907fc4a8a3fb2e1f11e31d67a494b06da8ed5d7c9
extent: 487 lines, 5,095 words
structure: timestamped Speaker 1–5 and Unknown Speaker turns
```

Ten exhaustive equal cuts would average 180.583 seconds, just over the maximum.
Removing the 31-second recording-consent prelude is enough to make an exhaustive
partition possible. The editorial draft below is more selective: it removes
the consent prelude, some service/food interruptions, and an incomplete tail.

### Draft ten-episode cut map

These are transcript-boundary candidates, not final edit decisions. Listen to
each boundary against the decoded audio before creating sample files.

| Episode | Start–end | Duration | Internal editorial label | Primary relationship or question |
| --- | --- | ---: | --- | --- |
| 1 | 00:31–03:16 | 2:45 | The DJ Becomes the MC | How a changed performance frame alters admiration and rejection |
| 2 | 03:16–05:42 | 2:26 | The Glorification Turn | Why the political turn, music, costumes, and shoes fail to counter the glamorous framing |
| 3 | 05:42–08:20 | 2:38 | Outside the Solar System | Imelda's historical self-invention, strategic partnership, and grave-marker love story |
| 4 | 08:23–11:23 | 3:00 | A Conversation Starter | Shallow research, the Filipino consultant, and an opening-night disclaimer |
| 5 | 11:47–14:33 | 2:46 | Who Gets the Spotlight? | Oppressor-centered storytelling, audience fandom, boos, applause, and surprise |
| 6 | 14:33–17:22 | 2:49 | Celebrity Before History | Diaspora representation, inherited fandom, and the difference between a musical and a soundtrack |
| 7 | 17:40–20:32 | 2:52 | Silent-Film Intertitles | Projected exposition, attempts to rescue the production, weak songs, and costume choices |
| 8 | 21:09–23:40 | 2:31 | The Dance Pit and the Thrust | Audience participation, Broadway staging, sightlines, and who the staging serves |
| 9 | 23:40–26:15 | 2:35 | A View of Everyone's Back | Failed thrust blocking, inaccessible scenic detail, and the return of the shoes |
| 10 | 26:31–29:16 | 2:45 | The Shoes Are Terrible | Performer testimony, political color, the princess opening, and a villain-origin reading |

The labels are editorial metadata. The accepted test-form title cards still
show only `1` through `10` unless the user separately approves named cards.

### Animal-voice animation language

Each reliably diarized speaker receives one stable animal character across all
ten episodes. Species, color, scale, clothing, camera height, and gesture rules
belong in a cast sheet. Do not assign species from ethnicity, nationality,
gender presentation, or stereotype. Prefer contributor choice or vocal and
conversational qualities, then obtain explicit approval.

The four-character cast inventory is fixed:

- unicorn;
- albatross;
- dog; and
- cat.

The inventory does not yet assign a particular speaker to a particular animal.
Corrected diarization must either resolve the provisional labels to four actual
voices or flag a cast-count mismatch for human decision. It must not silently
discard, merge, or leave an additional audible participant unmodified.

Until diarization is corrected:

- Speaker 1–4 remain distinct provisional character IDs;
- Speaker 5 and Unknown Speaker remain uncast rather than being silently merged;
- an uncertain turn may use an off-screen voice or neutral listening reaction;
- no real person's facial likeness is requested or inferred; and
- every audible voice is assigned a stable, non-identifying local transform
  before it enters a review or release mix.

Voice modification is mandatory for every audible participant, including brief
interjections and uncertain speakers. Use a deterministic combination of pitch,
formant, spectral-envelope, and timing-preserving timbre changes, keyed by
provisional speaker ID. Do not train on a participant, clone a participant, or
target the recognizable voice of another real person. Keep a private local
mapping from source speaker to transform settings so revisions remain stable.
Human review must confirm both de-identification and intelligibility; captions
remain required and do not excuse an unintelligible mix.

The animation can use a stylized café-table ensemble with restrained motion:
one active speaker, legible listening reactions, occasional visual metaphors,
and a stable spatial arrangement. Content-specific historical or theatrical
imagery appears as stage miniatures, lighting changes, silhouettes, shoes,
curtains, projected shapes, and abstract set models—not as fake documentary
footage or impersonations of living speakers.

For the Here Lies Love discussion, use bold hues and visibly contrasting
textures. Composition draws from German Expressionist film language: oblique
angles, compressed or distorted perspective, angular shadow fields, theatrical
negative space, exaggerated silhouettes, and forceful asymmetry. This is a
compositional vocabulary, not a request to reproduce a particular film frame.
Color and texture must never carry meaning alone; caption backplates retain
reviewed contrast, important figures keep a distinguishable silhouette, and
rapid texture or luminance changes remain below the accessibility flicker gate.

The visual reference synthesis adds:

- Hergé's clear-line readability, especially the lively economy of the Snowy
  dog character, translated into an original dog design rather than a replica;
- the quiet, tactile organic motion and patient visual rhythm of *The Red
  Turtle* (2016);
- the windswept ecological scale, air, and nature/technology tension associated
  with *Nausicaä of the Valley of the Wind*; and
- the strange but causally coherent speculative plant behavior of *Scavengers
  Reign*.

Use a clear hierarchy to make the combination coherent: clear-line economy for
the animal silhouettes and gestures; German Expressionist geometry for framing,
shadow, and spatial pressure; bold hue and contrasting texture for material
separation; and original ecological systems for plants and environmental
motion. Provider-facing prompts should translate the references into those
observable attributes. They should not request copied characters, recognizable
frames, proprietary logos, or a direct reproduction of any named production's
designs.

Seedance should generate silent character/world anchors from abstracted visual
prompts, not from the raw audio or transcript. Local assembly uses the approved
voice-transformed mix and an Izzi active-speaker layer driven by corrected
timestamps. The untouched recording remains a private local lineage source and
is never muxed into a review or release episode. This avoids asking a short
provider clip to lip-sync a three-minute conversation and keeps identifiable
voices out of the provider request.

Captions derive from a corrected transcript and are delivered as a separate
WebVTT or subtitle track. They are not entrusted to generated imagery. Episode
summaries identify the discussion as criticism and distinguish participant
claims from verified historical statements.

### Here Lies Trouble Los Angeles setting palette

The explicit series-level direction is a stable animal ensemble moving through
a different Los Angeles setting in every episode. The locations are not generic
establishing shots: Los Angeles is a recurring third character whose concrete
river channels, coastal paths, dry hills, rooflines, garden rooms, marine haze,
and changing light give each conversation a distinct register. Animal
silhouettes, markings, scale, clothing, and recurring accessories remain stable
while the environment, weather, time of day, and palette change around them.

This is a working ten-location map for visual development, not a locked
production order. It deliberately treats Malibu as its own coastal setting
rather than inaccurately presenting it as part of the Marvin Braude route.

| Episode | Working Los Angeles setting | Environmental and color direction |
| --- | --- | --- |
| 1 — The DJ Becomes the MC | Frogtown, beside the Los Angeles River bike corridor | Cool morning concrete, river green, steel bridges, bike wheels, and long lateral movement introduce the cast while traveling together. |
| 2 — The Glorification Turn | Arts District at the river edge | Rust, original mural-scale color blocks, rail and warehouse geometry, and hard late-afternoon shadows make glamour and industrial reality occupy the same frame. |
| 3 — Outside the Solar System | Griffith Park | Sage, ochre, eucalyptus shadow, an opening dawn sky, and distant basin or observatory glimpses give this episode the broadest scale. |
| 4 — A Conversation Starter | Culver City and the Ballona Creek corridor | Pale concrete, teal infrastructure, overpasses, and office-industrial edges support a walking or riding conversational rhythm rather than a landmark montage. |
| 5 — Who Gets the Spotlight? | Marvin Braude Coastal Bike Trail, Venice–Marina del Rey | Sun-bleached sand, marine blue, palms, skaters, and cyclists reduced to soft background motion contrast public spectacle with the speakers' critique. |
| 6 — Celebrity Before History | Long Beach waterfront and bikeway | Cobalt water, faded maritime red, port geometry, and marine haze hold celebrity imagery against a working coastal landscape. |
| 7 — Silent-Film Intertitles | Malibu bluff or beach access | Marine layer lifting into coral and gold, wind-shaped plants, and a long horizon create quieter blocking for projected shapes and intertitle-like metaphors. |
| 8 — The Dance Pit and the Thrust | DTLA roof deck | An amber-to-cyan night transition, skyline planes, vents, railings, and a visibly awkward floor plan make staging and sightline arguments legible. |
| 9 — A View of Everyone's Back | Runyon Canyon | Dry switchbacks, dusty ochre, dog-walker silhouettes, and a hazy basin view let foreground bodies intentionally occlude the vista without obscuring captions. |
| 10 — The Shoes Are Terrible | The Woods, West Hollywood | Deep garden green, plum shadow, and warm practical lights create an intimate cannabis-café ensemble finale; exact logos, décor, trade dress, and interior access remain separate clearance questions. |

The earlier planning palette—Central Library, the Geffen Contemporary, Hauser
& Wirth, the Hammer, a Sunset Boulevard exterior, and Los Angeles River
kayaking—remains available as alternates rather than being silently discarded.
Substituting one requires a visible location-map revision so two episodes never
accidentally converge on the same visual world.

The numbered title cards retain the already approved circa-1913 white-on-black
aesthetic and do not acquire place names without a later title-card decision. A
small cyan signal from that system may recur inside every episode—a bike light,
trail stripe, skyline edge, glass reflection, or garden practical—to unify the
geographically varied settings without making the episode worlds monochrome.

Use graphic 2D or shallow 2.5D animation with tactile linework, simplified
planes, and selective texture. The cast should read as conversational animals,
not photoreal wildlife, plush mascots, or human caricatures in animal masks.
Animate dialogue through posture, ear and tail response, paws, eye line,
breathing, and a small reviewed mouth-shape set; exact photoreal lip sync is
neither required nor desirable. Listening reactions matter as much as the
active speaker.

Favor shared movement—walking, coasting, pausing at a rail, or sitting at a
table—over ten static café tableaux. Bicycles and trail traffic establish place
without turning the conversation into a chase or sports sequence. Compose the
portrait frame in depth: environmental identity and sky in the upper third,
animal faces and gestures near the middle, and uncluttered subtitle-safe space
below. Translate wide Los Angeles views into layered vertical reveals rather
than squeezed panoramas.

Give each location two or three low-amplitude ambient loops such as water
glint, grass or palm movement, passing shadows, distant wheels, marine haze,
skyline lights, or garden foliage. Use one clear establishing reveal per
episode, followed by medium ensemble views and selective close reactions;
avoid landmark stacking, constant drone-like movement, and unsafe flashing.

These are setting references, not claims of affiliation, interior access,
filming permission, endorsement, or documentary capture. Recognizable public
geometry can anchor the world, but provider prompts exclude private faces,
readable personal data, real murals, logos, and exact branded interiors unless
separately reviewed and cleared. The Woods may begin as a lush, clearly West
Hollywood garden-café interpretation; exact branded depiction is a later
rights and access decision. Any real river, coastal-trail, road, bluff, or
rooftop capture requires a separately verified access and safety plan;
otherwise use animation or licensed reference material.

## Provider test sequence

### Stage A: local editorial package

For both options, produce without network access:

- source probe and hash receipt;
- transcript audit and corrected speaker/marker timeline;
- ten proposed cuts with duration checks;
- ten visual briefs and negative constraints;
- one cast or world-continuity sheet;
- ten numbered title cards;
- local placeholder episodes using Asama or static Izzi compositions;
- one merged review MKV and episode filmstrip; and
- a human disposition for every cut and visual brief.

### Stage B: two canaries

The user authorized the following unattended sequence on 2026-08-09:

1. Generate one silent 9:16, requested-1080p, ten-second Here Lies Trouble
   animal-ensemble anchor. Expect 190 credits and stop if the provider reports
   a different charge.
2. Only after that gate passes, generate one silent 9:16, requested-1080p,
   ten-second Duotone prompt-excursion anchor. The total expected ceiling is 380
   credits.
3. Preserve both provider originals, probe native dimensions and frame rates,
   create posters and contact strips locally, and retain exact hashes and
   bounded receipts.
4. Stop. Do not infer a winner, submit revisions, or begin ten episodes from
   provider success alone.

The provider request contains the visual brief and negative constraints only.
It excludes audio, transcript text, speaker names, account metadata, private
paths, and the training-conversion corpus.

### Stage C: selected ten-episode anchors

After the user chooses one option and authorizes a bounded budget, create at
most one initial provider anchor per episode. Submit sequentially, checkpoint
each job, and stop after the first rejected result for a policy decision. There
are no automatic paid creative retries.

For each accepted anchor:

- preserve the provider original;
- normalize to the 1,080-by-1,920 master without claiming native resolution;
- produce a loop-seam and motion-consistency review;
- extend it locally over the source-audio cut;
- apply deterministic Izzi transitions and speaker/marker overlays;
- mux only the approved locally transformed voice mix;
- generate a poster and contact strip; and
- verify exact duration, frames, streams, and hashes.

### Stage D: full-form review

Merge the ten numbered episodes in order. Produce:

```text
outputs/ad-hoc/here-lies-trouble/canary-1080p-01/
outputs/ad-hoc/here-lies-trouble/episodes/episode-01.mkv ... episode-10.mkv
outputs/ad-hoc/here-lies-trouble/here-lies-trouble-ten-episode-test.mkv
outputs/ad-hoc/here-lies-trouble/here-lies-trouble-episode-filmstrip.png
outputs/ad-hoc/here-lies-trouble/here-lies-trouble-resolved-plan.json
outputs/ad-hoc/duotone-111/canary-1080p-01/
outputs/ad-hoc/duotone-111/episodes/episode-01.mkv ... episode-10.mkv
outputs/ad-hoc/duotone-111/duotone-111-ten-episode-test.mkv
outputs/ad-hoc/duotone-111/duotone-111-episode-filmstrip.png
outputs/ad-hoc/duotone-111/duotone-111-resolved-plan.json
```

Human review separately scores segment coherence, visual continuity, aesthetic
fit, relationship clarity, text/logo artifacts, animal-cast legibility,
transition quality, loop fatigue, captions, and audio integrity.

## Scale-up gates: 10, 30, then 60 episodes

The two canaries prove only that the provider path and visual premises can be
reviewed. They do not authorize the next paid stage. Scale in independently
reversible blocks of ten episodes, with a separate human and credit decision at
every gate.

### Credit models

The preflight planning rate was 190 credits for one ten-second 1080p Standard
text-to-video anchor. The first 2026-08-09 API submission instead reported 145
credits used. That mismatch activated the contractual stop before the second
canary. Until the provider or account UI explains the difference, retain both
rates as separate evidence rather than silently rewriting the estimate.

| Episode count | One anchor per episode at 145 observed credits | One anchor per episode at 190 preflight credits | 1 anchor per 3 episodes at 145 | 1 anchor per ten-episode block at 145 |
| ---: | ---: | ---: | ---: | ---: |
| 10 | 1,450 | 1,900 | 580 (4 anchors) | 145 (1 anchor) |
| 30 | 4,350 | 5,700 | 1,450 (10 anchors) | 435 (3 anchors) |
| 60 | 8,700 | 11,400 | 2,900 (20 anchors) | 870 (6 anchors) |

These are generation-only estimates with zero retries. Accepted canaries may be
reused as their episode anchor, reducing incremental spend by one anchor. An
Annual Standard allocation is publicly described as 2,500 credits released
monthly; credits rather than nominal annual totals are therefore the scheduling
constraint. A 60-episode one-anchor-per-episode run would span at least four
monthly releases at the observed 145-credit response or five at the preflight
190-credit rate. Top-ups, plan changes, retries, and unused-credit rollover are
not assumed.

The one-anchor-per-ten-block model is a motion-language test, not a claim that
one clip can carry ten distinct episodes without fatigue. It relies on local
Asama loops, Izzi graph/orbit layers, deterministic Hamonshu and roulette
transitions, alternate crops, held frames, texture passes, and episode-specific
SVG compositions. The one-per-three model is the preferred cost/variety
comparison for the ten-episode pilot.

### Gate 10: production-form pilot

Purpose: prove that a reviewed transcript cut can become a complete, accessible
60–180-second installment rather than merely a striking ten-second clip.

Before paid anchors:

- lock ten source ranges by decoded sample and complete speaker/thought
  boundary;
- resolve every Duotone prompt threshold or Here Lies Trouble speaker/cast ID;
- approve one continuity sheet, one color/texture key, and a reduced-motion key;
- build all ten episodes with local placeholders first;
- choose one-, four-, or ten-anchor budget explicitly; and
- preserve a no-provider fallback using Asama and Izzi only.

Acceptance requires ten numbered episodes, a merged review MKV, caption and
voice-treatment verification where applicable, native-resolution reporting,
flicker screening, a ten-poster filmstrip, and affirmative human visual review.
Failure in any episode pauses the block; it does not trigger a paid retry.

### Gate 30: three ten-episode blocks

Purpose: test whether the visual language survives multiple narrative arcs and
whether review remains manageable.

- Treat episodes 1–10, 11–20, and 21–30 as separately releasable blocks.
- Lock a versioned continuity bible: cast geometry, locations, palettes,
  caption zones, motion vocabulary, prompt templates, negative constraints,
  seeds, and provider settings.
- Maintain a scene ledger linking source range, transcript confidence, visual
  brief, anchor reuse, local transforms, accessibility checks, and human
  disposition.
- Freeze each completed block before authoring the next; changes to the bible
  require an explicit migration plan and a regression filmstrip.
- Review every episode automatically, every poster visually, and every tenth
  episode as a full start-to-finish human playback sample; increase sampling
  after any failure.

Proceed beyond episode 30 only if the three blocks show consistent character or
world identity, sustainable caption/voice handling, acceptable loop fatigue,
bounded review time, and a measured credit burn that agrees with receipts.

### Gate 60: six-block production system

Purpose: validate the lower end of the intended 60–90 episode vertical form.

- Schedule six ten-episode blocks against monthly credit releases; never queue
  future blocks merely because prompts are ready.
- Separate immutable source/cut evidence from revisable visual plans and from
  provider artifacts.
- Add resumable job state, idempotency records, per-block spend ceilings, and a
  circuit breaker for cost, resolution, provider, privacy, or accessibility
  mismatches.
- Generate deterministic local masters, posters, filmstrips, probes, hashes,
  captions, audio-transform receipts, and a corpus-level index.
- Use regression grids across all prior blocks for cast silhouette, palette,
  subtitle-safe space, orbit/Kusama graph language, and transition fidelity.
- Require a human checkpoint after each ten episodes and a cross-block review
  at episodes 30 and 60.

Only after Gate 60 is accepted should the same machinery be evaluated for
episodes 61–90. That extension needs its own editorial inventory, calendar and
credit schedule, participant/publication authority review, and affirmative
budget approval.

## Simplified authoring JSON

One schema can carry both options while keeping layout-specific details in a
discriminated visual treatment:

```json
{
  "schema_version": "izzi.audio-vertical-test/1",
  "project_id": "duotone-or-here-lies-trouble",
  "form": {
    "episode_count": 10,
    "content_seconds": { "minimum": 60, "maximum": 180 },
    "title_card_seconds": 3,
    "width": 1080,
    "height": 1920,
    "frame_rate": { "numerator": 30, "denominator": 1 }
  },
  "source": {
    "audio_path": "REPLACE_WITH_ONE_APPROVED_LOCAL_PATH",
    "audio_sha256": "REPLACE_WITH_64_LOWERCASE_HEXADECIMAL_CHARACTERS",
    "transcript_path": "REPLACE_OR_LEAVE_UNAVAILABLE"
  },
  "segmentation": {
    "method": "prompt-arc-or-transcript-topic",
    "human_boundary_review": "pending",
    "episodes": []
  },
  "visual_treatment": {
    "kind": "duotone-reality-break-or-animal-dialogue",
    "continuity": {},
    "negative_constraints": []
  },
  "provider": {
    "adapter": "seedance2ai-v1",
    "account_plan": "annual-standard-user-asserted",
    "quality_tier": "standard",
    "resolution": "1080p",
    "duration": "10",
    "generate_audio": false,
    "maximum_jobs": 2,
    "expected_credits_per_job": 190,
    "maximum_expected_credits": 380,
    "automatic_retries": 0,
    "provider_transfer_authorized": true,
    "provider_spending_authorized": true
  },
  "review": {
    "cut_map": "pending",
    "canary": "not-started",
    "ten_episode_form": "not-started"
  }
}
```

The resolved plan must replace every placeholder, record exact timestamps in
integer decoded samples and frames, materialize every default, and reject a
changed source hash.

## Consent and privacy gates

The Here Lies Trouble transcript begins with consent to record. That is evidence
for recording only. Before either project leaves local planning, identify the
required authority for:

- provider processing of derived content;
- animal embodiment of recognizable voices;
- use of transformed source voices in a rendered work;
- distribution to collaborators;
- public release; and
- retention of transcripts, captions, and provider artifacts.

Do not upload source audio by default. If a later workflow genuinely requires
audio-conditioned generation, it needs a new transfer notice naming the exact
audio excerpt, provider, purpose, retention uncertainty, job count, and budget.

## Acceptance conditions

- All ten cuts are 60–180 seconds, start and end at reviewed thought or speaker
  boundaries, and preserve exact decoded-sample references.
- Consent preambles and unrelated private material are excluded from released
  episodes unless explicitly required and authorized.
- Each Duotone prompt-led cut has a human-reviewed `prompt` threshold, and its
  fantasy/psychedelic visual mode continues from that threshold through the
  reviewed episode end.
- Duotone prompts follow one normalized rule hierarchy rather than copying the
  contradictory workbook verbatim.
- Here Lies Trouble uses corrected diarization and stable, approved animal
  casting without voice cloning or stereotyped species assignment.
- Every audible Here Lies Trouble participant has a stable, reviewed,
  non-identifying voice transform; no unchanged source voice is present in a
  review or release mix.
- Here Lies Trouble assigns one distinct, reviewed Los Angeles location ID to
  every episode; the location map, time-of-day key, palette, and cast continuity
  sheet are locked before production rendering.
- Location art preserves subtitle-safe portrait composition and does not copy
  real murals, logos, bystander likenesses, branded interiors, or private trade
  dress without separate review and clearance.
- Provider requests contain no raw audio, transcript, names, credentials, or
  private filesystem paths.
- Every provider result is silent, independently hashed, probed, and visually
  reviewed before use.
- Every final episode is 1,080 by 1,920 at 30 fps, carries only its approved
  local audio treatment, and has a separate corrected caption track.
- The merged test retains the accepted three-second numbered title cards and a
  human-review filmstrip.
- No provider retry, second-stage generation, publication, or baseline
  promotion is inferred from silence or technical success.

## Immediate next action

Execute the separately authorized two-canary Stage B contract. Submit Here Lies
Trouble first, verify the reported 190-credit charge and requested native
dimensions, and stop on any mismatch. If it passes, submit Duotone. Preserve the
two provider originals and generate local posters, contact strips, probes,
hashes, manifests, and training-conversion receipts. Human visual selection and
any production render remain later decisions.
