# Audio-first Seedance vertical-installment proposal

- Status: compact author schema, deterministic energy-only candidate pilot,
  and negotiation contract implemented; the real workflow remains
  `CANDIDATES-READY`
- Scope: one local recording, ten candidate coverage bands, an eventual ten
  user-selected ten-second excerpts, ten portrait installments, and one local
  review composite
- Privacy default: offline analysis only; provider transfer is disabled

This is a narrower companion to the
[Seedance account and delivery proposal](explore_futures_seedance.md). The user
provides one editorial `vertical_name` (the series name), one `visual_style`, and
one local audio path. Izzi proposes acoustic moments; the user reviews them, and
only explicit user-sourced decisions can lock exactly ten distinct ten-second
excerpts. Each excerpt becomes one 9:16 installment: `scene-NN` and
`installment-NN` are the same selected ten-second unit in this profile.

The ten excerpts are **selected moments, not a summary of the complete
recording**. Acoustic coverage and variety can make a useful series, but they do
not prove topical, narrative, or statistical representativeness.

## Concrete pilot and privacy boundary

Use this immutable local source for the first implementation test:

```text
path: resources/here-lies-trouble/seed-audio-corpus/here-lies-trouble-1.20260415.mp3
sha256: 32e984630b27abbe1acb505002e36059047a3a671de5526f872cffd24cb30d1e
codec: MP3
channels: 1 (mono)
sample rate: 16000 Hz
duration: 1805.832 seconds
```

The analyzer works locally and offline. Preserve the original MP3 unchanged,
verify its hash before every resumed run, and derive a hash-addressed PCM or
FLAC analysis master with a pinned decoder. Do not upload the recording, an
excerpt, a prompt, or a generated reference; do not call a video provider;
and do not publish any output without a separate, bounded authorization.

A transcript is optional enrichment and is never a prerequisite for candidate
selection, negotiation, local visualization, or the review composite. Candidate
selection uses audio signals only. If a separately authorized transcript is
available, the resolver may use short paraphrased labels or reviewed visual
prompts. Manifests and receipts must not contain raw personal transcript
excerpts. Protected caption files, if later authorized, are referenced by path
and hash rather than copied into logs.

### Implemented pilot state

The first local run measured ten coverage bands and retained three pairwise
non-overlapping ten-second alternatives within each band. Its ten rank-one
proposal windows are also globally non-overlapping. The content-addressed
package manifest is
`manifest.json`,
SHA-256
`a3cd27642c777f62415a8fd562b152f08a0735f001c4bd920ba9c41b2c4fa141`.

The implemented profile is `izzi-audio-candidates-energy-1`. A pinned local
VAD was unavailable, so the profile records VAD as `UNAVAILABLE` with zero
weight; it is not represented as the fuller future profile. The transcript was
not read, temporary decoded PCM was deleted, and a second complete analysis
package was byte-identical. All rank-one choices remain
`PROPOSED-NOT-LOCKED`.

The real negotiation skeleton has zero selections and deterministically derives
`CANDIDATES-READY`. Implementing and testing the negotiation checker did not
advance the real workflow, record a user decision, create a locked manifest,
or authorize provider activity.

## Minimal author interface

The authoring contract is intentionally smaller than the general
[ten-scene authoring skeleton](../../examples/seedance2/ten-scene-authoring.10x10s.skeleton.json).
Version 1 is a closed JSON object with four required fields:

- `schema_version`: integer constant `1`;
- `vertical_name`: 1–120 characters, used on every title card;
- `visual_style`: 20–1000 characters of declarative creative direction, never
  executable code;
- `audio_path`: a repository-relative path with no `..` segment.

Validation also resolves symlinks and requires the final audio path to remain
inside the repository's authorized resource root; lexical validation alone is
not a filesystem authority check.

An optional `title_card_seconds` is a number from 0.5 through 5.0 and defaults
to 2.0. Scene count, excerpt duration, output geometry, and frame rate are fixed
by the profile rather than repeated in user input. Credentials, account IDs,
provider URLs, prompts, transcripts, and timestamps are not accepted here.

Minimal input:

```json
{
  "schema_version": 1,
  "vertical_name": "Here Lies Trouble",
  "visual_style": "Circa-1913 scientific phantasmagoria: restrained white linework, deep black fields, cyan accents, and calm geometric motion.",
  "audio_path": "resources/here-lies-trouble/seed-audio-corpus/here-lies-trouble-1.20260415.mp3"
}
```

The implemented Draft 2020-12 schema is
[`audio-vertical-authoring.proposed.schema.json`](../../examples/seedance2/audio-vertical-authoring.proposed.schema.json).
Its normative content is reproduced here for review:

```json
{
  "$schema": "https://json-schema.org/draft/2020-12/schema",
  "$id": "https://bdekoz.github.io/izzi.public/examples/seedance2/audio-vertical-authoring.proposed.schema.json",
  "title": "Izzi audio-first vertical authoring input",
  "type": "object",
  "additionalProperties": false,
  "required": [
    "schema_version",
    "vertical_name",
    "visual_style",
    "audio_path"
  ],
  "properties": {
    "schema_version": {
      "const": 1
    },
    "vertical_name": {
      "type": "string",
      "minLength": 1,
      "maxLength": 120
    },
    "visual_style": {
      "type": "string",
      "minLength": 20,
      "maxLength": 1000
    },
    "audio_path": {
      "type": "string",
      "pattern": "^(?!/)(?!.*(?:^|/)\\.\\.(?:/|$))[^\\u0000]+$"
    },
    "title_card_seconds": {
      "type": "number",
      "minimum": 0.5,
      "maximum": 5.0,
      "multipleOf": 0.1,
      "default": 2.0
    }
  }
}
```

The resolver adds the verified hash, probed media facts, fixed output contract,
candidate evidence, revisions, exact sample ranges, visual presets, and output
paths. This preserves a simple human interface while keeping execution explicit.

## Deterministic audio-only candidate selection

The target full profile remains `izzi-audio-candidates-1`. The implemented
pilot uses the separately named `izzi-audio-candidates-energy-1` profile
because no pinned local VAD was available. A future full-profile implementation
would:

1. Verify the source hash and probe duration, channel count, and sample rate.
2. Decode the entire source once to mono 16 kHz PCM with a pinned decoder.
   Record the decoder version, arguments, decoded sample count, and hash.
3. Analyze fixed 20 ms frames on a 10 ms hop. Retain RMS energy, peak and
   clipping rate, silence runs, spectral centroid, spectral flux, and a local
   voice-activity detector (VAD) result. Pin the VAD implementation/model and
   hash; if it is unavailable, stop or use a separately named energy-only
   profile rather than silently changing algorithms.
4. Enumerate ten-second windows on a 100 ms grid. Prefer starts and ends near
   silence boundaries, active-but-not-clipped energy, spectral change, and
   coherent VAD runs. Reject windows outside the source and exact overlaps.
5. Divide the source timeline into ten equal coverage bands and retain several
   high-scoring candidates per band. Coverage is a diversity aid, not a claim
   that each tenth has equal meaning.
6. Select a proposed set with deterministic maximum-marginal-relevance scoring:
   30% VAD/activity, 20% energy change, 20% spectral flux, 15% silence-boundary
   quality, 10% timeline coverage, and 5% feature-space diversity. Normalize
   each feature by documented fixed rules. Break every tie by lower start sample,
   then candidate ID.
7. Derive each candidate ID from the source hash, analysis-profile hash, start
   sample, end sample, and feature-vector hash. Identical inputs and toolchain
   must produce identical candidates and ordering.

The analyzer should return three candidates per installment slot, plus feature
summaries and small waveform/spectrogram snapshots. It must not invent dialogue,
topics, speakers, or emotional meaning from acoustic measurements. Without a
transcript, visual prompts derive only from `visual_style`, installment number,
and normalized audio features.

## Negotiation and revision workflow

Use an append-only decision log and these resumable states:

```text
INPUT-VALIDATED -> CANDIDATES-READY -> NEGOTIATING -> TEN-LOCKED
TEN-LOCKED -> LOCAL-PREFLIGHTED -> AWAITING-PROVIDER-AUTHORITY
AWAITING-PROVIDER-AUTHORITY -> PROVIDER-AUTHORIZED -> GENERATING
GENERATING -> VISUAL-REVIEW -> ACCEPTED -> ASSEMBLED
```

The current real state is still `CANDIDATES-READY`. The implemented checker
proves how a visible decision document would resolve; it does not supply user
decisions or advance the state. `NEGOTIATING` begins only when the user provides
an explicit action.

The provider branch remains at `AWAITING-PROVIDER-AUTHORITY` unless the user
explicitly authorizes the named ten excerpts, destination, purpose, and bounded
run. A local Izzi-only render does not require provider authority.

During `NEGOTIATING`, the user may `ACCEPT`, `REJECT`, `REQUEST-ALTERNATE`, or
`ADJUST-BOUNDARY`. The agent may explain acoustic evidence and propose changes,
but cannot accept on the user's behalf. A boundary adjustment creates a new
candidate ID. Every decision records the candidate ID, monotonic revision,
actor, visible reason, and timestamp with timezone. Rejection never deletes the
earlier proposal. Silence never means acceptance.

`TEN-LOCKED` requires all of the following:

- exactly ten accepted candidate IDs numbered 01 through 10;
- each range is exactly 160,000 decoded samples (10 seconds at 16 kHz);
- no two ranges overlap and every range is within the source;
- the locked manifest and decision log have stable SHA-256 values;
- the user has affirmed that these are ten chosen excerpts, not a summary of the
  whole recording.

Any changed source hash, analysis profile, visual style, excerpt boundary, or
ordering increments the revision and returns to negotiation. Resume refuses to
mix artifacts from different locked-manifest hashes.

## Resolved-manifest example

The offsets below demonstrate the machine format only. They are evenly spaced
illustrative values, **not findings from analysis of the pilot**, not user
acceptance, and not execution authority. A real run replaces them with locally
measured candidates and an affirmative decision record.

```json
{
  "manifest_version": 1,
  "evidence_status": "EXAMPLE-NOT-EXECUTED",
  "state": "TEN-LOCKED",
  "revision": 2,
  "provider_transfer_authorized": false,
  "input": {
    "vertical_name": "Here Lies Trouble",
    "visual_style": "Circa-1913 scientific phantasmagoria: restrained white linework, deep black fields, cyan accents, and calm geometric motion.",
    "audio": {
      "path": "resources/here-lies-trouble/seed-audio-corpus/here-lies-trouble-1.20260415.mp3",
      "sha256": "32e984630b27abbe1acb505002e36059047a3a671de5526f872cffd24cb30d1e",
      "codec": "mp3",
      "channels": 1,
      "sample_rate_hz": 16000,
      "duration_seconds": 1805.832
    }
  },
  "analysis": {
    "profile": "izzi-audio-candidates-1",
    "profile_sha256": "1111111111111111111111111111111111111111111111111111111111111111",
    "transcript_enrichment": {
      "state": "NOT-PROVIDED",
      "required_for_selection": false
    },
    "features": [
      "silence",
      "rms-energy",
      "spectral-centroid",
      "spectral-flux",
      "vad",
      "timeline-coverage",
      "feature-diversity"
    ],
    "tie_break": "start-sample-ascending-then-candidate-id"
  },
  "output_contract": {
    "width_px": 1080,
    "height_px": 1920,
    "frame_rate": {
      "numerator": 30,
      "denominator": 1
    },
    "installment_seconds": 10,
    "installment_frames": 300,
    "title_card_seconds": 2,
    "title_card_frames": 60,
    "test_composite_seconds": 120,
    "test_composite_frames": 3600,
    "container": "mkv",
    "video_codec": "h264",
    "pixel_format": "yuv420p",
    "audio_codec": "flac",
    "audio_policy": "replace-generated-audio-with-exact-source-excerpt"
  },
  "installments": [
    {
      "installment_id": "installment-01",
      "number": 1,
      "candidate_id": "example-candidate-01",
      "start_sample": 1920000,
      "end_sample": 2080000,
      "start_seconds": 120,
      "end_seconds": 130,
      "title_card_text": "HERE LIES TROUBLE — INSTALLMENT 01 OF 10",
      "visual_preset": "hamonshu"
    },
    {
      "installment_id": "installment-02",
      "number": 2,
      "candidate_id": "example-candidate-02",
      "start_sample": 4656000,
      "end_sample": 4816000,
      "start_seconds": 291,
      "end_seconds": 301,
      "title_card_text": "HERE LIES TROUBLE — INSTALLMENT 02 OF 10",
      "visual_preset": "roulette"
    },
    {
      "installment_id": "installment-03",
      "number": 3,
      "candidate_id": "example-candidate-03",
      "start_sample": 7392000,
      "end_sample": 7552000,
      "start_seconds": 462,
      "end_seconds": 472,
      "title_card_text": "HERE LIES TROUBLE — INSTALLMENT 03 OF 10",
      "visual_preset": "hamonshu"
    },
    {
      "installment_id": "installment-04",
      "number": 4,
      "candidate_id": "example-candidate-04",
      "start_sample": 10128000,
      "end_sample": 10288000,
      "start_seconds": 633,
      "end_seconds": 643,
      "title_card_text": "HERE LIES TROUBLE — INSTALLMENT 04 OF 10",
      "visual_preset": "roulette"
    },
    {
      "installment_id": "installment-05",
      "number": 5,
      "candidate_id": "example-candidate-05",
      "start_sample": 12864000,
      "end_sample": 13024000,
      "start_seconds": 804,
      "end_seconds": 814,
      "title_card_text": "HERE LIES TROUBLE — INSTALLMENT 05 OF 10",
      "visual_preset": "hamonshu"
    },
    {
      "installment_id": "installment-06",
      "number": 6,
      "candidate_id": "example-candidate-06",
      "start_sample": 15600000,
      "end_sample": 15760000,
      "start_seconds": 975,
      "end_seconds": 985,
      "title_card_text": "HERE LIES TROUBLE — INSTALLMENT 06 OF 10",
      "visual_preset": "roulette"
    },
    {
      "installment_id": "installment-07",
      "number": 7,
      "candidate_id": "example-candidate-07",
      "start_sample": 18336000,
      "end_sample": 18496000,
      "start_seconds": 1146,
      "end_seconds": 1156,
      "title_card_text": "HERE LIES TROUBLE — INSTALLMENT 07 OF 10",
      "visual_preset": "hamonshu"
    },
    {
      "installment_id": "installment-08",
      "number": 8,
      "candidate_id": "example-candidate-08",
      "start_sample": 21072000,
      "end_sample": 21232000,
      "start_seconds": 1317,
      "end_seconds": 1327,
      "title_card_text": "HERE LIES TROUBLE — INSTALLMENT 08 OF 10",
      "visual_preset": "roulette"
    },
    {
      "installment_id": "installment-09",
      "number": 9,
      "candidate_id": "example-candidate-09",
      "start_sample": 23808000,
      "end_sample": 23968000,
      "start_seconds": 1488,
      "end_seconds": 1498,
      "title_card_text": "HERE LIES TROUBLE — INSTALLMENT 09 OF 10",
      "visual_preset": "hamonshu"
    },
    {
      "installment_id": "installment-10",
      "number": 10,
      "candidate_id": "example-candidate-10",
      "start_sample": 26544000,
      "end_sample": 26704000,
      "start_seconds": 1659,
      "end_seconds": 1669,
      "title_card_text": "HERE LIES TROUBLE — INSTALLMENT 10 OF 10",
      "visual_preset": "roulette"
    }
  ],
  "outputs": {
    "installment_pattern": "build/here-lies-trouble/installment-{number}-10s.mkv",
    "snapshot_pattern": "build/here-lies-trouble/installment-{number}-review.png",
    "test_composite": "build/here-lies-trouble/here-lies-trouble-review-120s.mkv"
  },
  "human_visual_gate": "REQUIRED"
}
```

## Visual and media contract

Each released installment is exactly 1080×1920 pixels, 30/1 fps, 300 frames,
and 10 seconds. Use H.264/yuv420p in Matroska for the local master. Provider
video is an input to post-production, not duration or text authority. Normalize
or reject it against the
[existing MKV output contract](explore_futures_seedance.md#mkv-output-contract),
using its 9:16 vertical-master profile.

Preserve the original MP3 by hash. Cut each excerpt from the decoded analysis
master by exact sample indices, retain 160,000 samples, and mux it losslessly by
default as FLAC in Matroska. Do not denoise, normalize loudness, time-stretch,
synthesize speech, or mix provider-generated audio unless a named policy is
separately approved. If a provider returns audio, discard it and restore the
selected source excerpt. A web derivative may transcode audio, but remains a
separately hashed derivative.

Compile `visual_style` into a reviewed allowlisted profile:

- map RMS envelope to Hamonshu amplitude and line density;
- map spectral centroid to bounded palette/lightness choices;
- map spectral flux to bounded roulette phase or angular motion;
- use VAD only for pacing or visual density, never inferred words or identity;
- keep fade as the reduced-motion alternative; avoid blink/wink defaults and
  reject unsafe flashing.

The implementation belongs beside the deterministic frame vocabulary in
[`izzi-svg-sequences.h`](../html/izzi-svg-sequences_8h.html), while encoding remains an
external tool contract. Transition frames consume part of each installment's
fixed 300-frame budget; they never extend or silently shorten it.

## MiL compatibility and sequence handoff

Two historical scripts in the local MiL repository define useful compatibility
evidence:

- `/home/bkoz/src/MiL.git/scripts/ffmpeg-supercut-123-dupe.sh` cuts a named
  interval, emits Matroska, and scales with contain-and-pad behavior;
- `/home/bkoz/src/MiL.git/scripts/ffmpeg-from-png-and-wav-to-mkv.sh` combines a
  still image and an audio source into a bounded video.

The future adapter around [`izzi-svg-sequences.h`](../html/izzi-svg-sequences_8h.html)
should preserve those capabilities behind a validated manifest rather than
require callers to construct positional shell commands. For each locked
installment it emits the source start sample, exact 160,000-sample audio cut,
duration, numbered SVG/PNG frame pattern or still poster, contain-and-pad policy,
vertical dimensions, output MKV path, and expected hashes. A maintained encoder
then consumes only that allowlisted manifest, uses quoted paths and fail-fast
arguments, and verifies the result with `ffprobe`.

The current [`build-clips.sh`](../../examples/seedance2/build-clips.sh) already
demonstrates the complementary numbered-SVG-to-Matroska path. The new handoff
unifies frame-sequence, still-image-plus-audio, and exact supercut inputs without
making FFmpeg or a shell process part of the header-only C++ public API.

## Circa-1913 title-card review composite

The required test deliverable concatenates ten blocks. Each block contains:

1. a two-second silent title card (60 frames); then
2. the corresponding ten-second installment (300 frames with its selected
   source audio).

The default composite is therefore exactly 120 seconds and 3,600 frames:

```text
10 × (2-second card + 10-second installment) = 120 seconds
10 × (60 frames + 300 frames) = 3600 frames at 30 fps
```

`title_card_seconds` may be configured from 0.5 through 5.0 in 0.1-second
increments. It is always outside the installment's fixed ten seconds; the
manifest recomputes card frames and composite duration exactly and rejects
fractional-frame results.

Render the circa-1913 card locally with a static black field, high-contrast
white serif text, restrained border ornament, and no simulated projector flash.
Every card contains the exact `vertical_name` plus `INSTALLMENT NN OF 10`. The
card is silent; the continuous composite audio track contains matching silence
before each exact excerpt. Keep text inside a 10% title-safe area (at least 108
pixels left/right and 192 pixels top/bottom on 1080×1920).

## Accessibility and human review

The title card must have exact visible text, a semantic title in companion
metadata, sufficient contrast, and a static reduced-motion equivalent. Do not
encode meaning only through color, Hamonshu shape, or roulette movement. Check
full-frame luminance changes and saturated red; reject unsafe flashes. Keep
captions inside the portrait title-safe region without obscuring essential
visual content.

Audio-only selection does not require a transcript. If a released excerpt
contains speech or meaningful sound, a WCAG-targeted publication still needs
accurate reviewed captions and an appropriate transcript or media alternative.
That accessibility artifact may be prepared manually or through separately
authorized transcription; its sensitive text stays in a protected sidecar.
Without it, document the limitation and do not claim WCAG 2.2 conformance.

Before accepting an installment, create a local contact sheet containing its
first, middle, and last frame, title card, feature plot, and caption-safe overlay.
The user must affirm the visual result. Provider success, an unchanged file, or
silence in chat is not visual acceptance.

## Receipts, snapshots, and resume

Retain local, hash-addressed receipts for:

- source probe and immutable MP3 hash;
- decoder/tool versions, arguments, analysis-master hash, and sample count;
- analysis profile and candidate manifest;
- append-only negotiation decisions and every superseded revision;
- the locked ten-installment manifest;
- title cards, prompts or transcript-derived paraphrases, and review snapshots;
- provider-transfer notice and authorization, if later granted;
- provider request/result records, downloads, and redactions, if later used;
- per-installment and composite `ffprobe` JSON, hashes, frame counts, audio sample
  counts, and the affirmative human visual decision.

Resume keys are the source hash, analysis-profile hash, author-input hash,
locked-manifest hash, and work-item ID. A mismatch stops before rendering or
transfer. Local analysis may resume without network access. Provider generation
remains disabled until the separate Seedance account workflow has a current
authorization and canary gate.

## Implementation sequence

1. **Completed:** closed four-field author schema, offline fixture validator,
   canonical path confinement, and minimal example.
2. **Completed for the energy-only pilot:** deterministic decode, feature
   extraction, candidate ranking, and feature snapshots. The full VAD profile
   remains deferred.
3. **Completed for offline resolution:** closed negotiation schema, exact
   candidate/hash binding, and authority-aware resolver/checker. Append-only
   real decision persistence and locked-manifest materialization remain open.
4. **Not started:** vertical Izzi previews, title cards, Hamonshu/roulette
   mappings, source-audio muxing, and the exact 120-second review composite.
5. **Completed for candidate resume and fixtures:** deterministic regeneration,
   path/invariant attacks, and negotiation fixtures. Media resume and
   affirmative human visual acceptance remain open.
6. **Not started and separately authorized:** provider adapter described in the
   account proposal. Local-only rendering must remain fully usable without it.
