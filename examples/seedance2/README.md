# Seedance 2 title-transition studies

This directory contains two offline, deterministic 10-second motion studies:

- `hamonshu-10s.mkv` uses izzi's source-indexed Hamonshu geometry as a wave/mask reveal;
- `roulette-10s.mkv` uses izzi's closed hypotrochoid geometry as an expanding iris.

Both clips are 1280×720, 30 fps, H.264 in a Matroska container, with no audio. The corresponding poster PNG is the fully revealed exact-title frame intended for use as a start-frame reference. The MKV is a transition proof and post-production asset; neither file calls a video-generation service.

The JSON files follow the proposed `title-sequence-1` shape in [the futures report](../../docs/development/explore_futures.20260808.md). The example C++ program performs strict checks for the fields it consumes, but it is not the future full JSON Schema implementation.

Build from any working directory:

```sh
bash examples/seedance2/build-clips.sh
```

Pass a different output directory as the first argument if desired. The build uses a temporary frame directory and leaves these artifacts in the selected output directory:

```text
hamonshu-10s.mkv
hamonshu-10s-poster.png
hamonshu-10s-render-manifest.json
hamonshu-10s-ffprobe.json
roulette-10s.mkv
roulette-10s-poster.png
roulette-10s-render-manifest.json
roulette-10s-ffprobe.json
```

Required tools are a C++20 compiler, RapidJSON headers, `ffmpeg` with librsvg support, `ffprobe`, `jq`, and `mkvmerge`. The final remux uses `mkvmerge --deterministic`, regenerates track UIDs from the configured seed, and omits the container date so repeat builds with the same toolchain are byte-identical.

The delivery blocks in the JSON are inert examples. They contain no API key or public asset URL. A future delivery adapter should publish a validated poster, inject its public HTTPS URL, read credentials outside JSON, and submit only after a redacted dry run.

## Proposed ten-scene project

[`ten-scene-authoring.proposed.schema.json`](ten-scene-authoring.proposed.schema.json) is the concise human-facing Draft 2020-12 contract. It requires exactly ten scenes, with only `scene_id`, `title`, and `description` required per scene. [`ten-scene-authoring.example.json`](ten-scene-authoring.example.json) demonstrates that minimal scene form.

The proposed resolver turns authoring input into [`ten-scene-project.proposed.schema.json`](ten-scene-project.proposed.schema.json), the strict execution-plan contract. [`ten-scene-project.example.json`](ten-scene-project.example.json) supplies all ten resolved scenes, including exact provider prompts, constraints, seeds, title frames, transitions, and provisional accessibility descriptions. Until the resolver is implemented, the two examples are independent valid illustrations rather than a golden input/output pair.

Both representations can be checked with a Draft 2020-12-capable validator:

```sh
jsonschema \
  -i examples/seedance2/ten-scene-authoring.example.json \
  examples/seedance2/ten-scene-authoring.proposed.schema.json

jsonschema \
  -i examples/seedance2/ten-scene-project.example.json \
  examples/seedance2/ten-scene-project.proposed.schema.json
```

The current example generator does not yet implement authoring-to-plan
resolution, provider submission, or synthesis of the ten provider clips. The
futures report specifies the proposed resolution/review, offline render,
explicit submission, resumable collection, exact post-composite, per-scene QA,
and optional 100-second assembly workflow.

## Audio-first vertical profile

The later [audio-first proposal](../../docs/development/explore_futures_seedance_audio.md)
adds a smaller four-field input for recorded conversations. It treats each
provider scene as one 9:16, ten-second vertical installment; acoustic analysis
proposes candidate excerpts, and only ten explicit `USER`-sourced decisions can
lock the selections. The real skeleton currently contains none and remains
`CANDIDATES-READY`. Its test composite places a two-second, black-and-white
circa-1913 title card before every installment, for 120 seconds total at the
default 30 fps.

The detailed schemas and emailed skeleton in this directory remain preserved as
their original hand-authored profile. They are not silently converted to the
audio-first vertical profile; a resolver will produce a new detailed execution
plan only after the ten audio selections are affirmatively locked.

### Compact audio-first author input

[`audio-vertical-authoring.proposed.schema.json`](audio-vertical-authoring.proposed.schema.json)
materializes the proposal's closed Draft 2020-12 input contract.
[`audio-vertical-authoring.minimal.json`](audio-vertical-authoring.minimal.json)
is its minimal valid example: `schema_version`, `vertical_name`, `visual_style`,
and `audio_path` are required. The only optional field is
`title_card_seconds`; it defaults to 2.0 and accepts 0.5 through 5.0 seconds in
0.1-second increments. Scene count, candidate moments, transcripts, provider
details, and mutable run state deliberately do not belong in this input.

Validate the example and all negative fixtures offline with the installed
`jsonschema` 4.23 implementation:

```sh
python3 examples/seedance2/check-audio-vertical-authoring.py
python3 examples/seedance2/check-audio-vertical-authoring.py --check-fixtures
```

The JSON Schema pattern rejects leading `/` paths and `..` segments, but a
regular expression is not filesystem authority. The checker also resolves the
repository, authorized resource root, target, and every symlink to canonical
paths; it accepts the target only when it is a regular file beneath that root.
Its fixture suite includes a temporary symlink-escape regression. Validation
stats the target but does not open, hash, decode, transcribe, or otherwise
inspect the audio. No network or provider operation occurs.

This compact file starts audio-candidate negotiation. It does not replace
[`ten-scene-authoring.10x10s.skeleton.json`](ten-scene-authoring.10x10s.skeleton.json)
or either detailed ten-scene schema/example. Those describe a later resolved
execution plan after exactly ten excerpts have been reviewed and locked.

### Deterministic energy-only candidate pilot

[`audio-candidates-energy-1.profile.json`](audio-candidates-energy-1.profile.json)
defines the fixed `izzi-audio-candidates-energy-1` acoustic profile. It is a
separately named fallback because a pinned local WebRTC VAD is unavailable;
the profile records VAD as `UNAVAILABLE` with zero weight and must not be
represented as the fuller `izzi-audio-candidates-1` profile.

Run the synthetic gate, then the named local pilot:

```sh
python3 examples/seedance2/analyze-audio-candidates-energy.py --self-test
python3 examples/seedance2/analyze-audio-candidates-energy.py
python3 examples/seedance2/analyze-audio-candidates-energy.py
```

The analyzer first verifies the pinned MP3 hash, then uses FFmpeg 7.1.5 to
decode temporary mono 16 kHz signed PCM. It computes fixed energy, silence,
spectral, coverage, and feature-diversity measurements without reading a
transcript. Ten coverage bands retain three ranked, non-overlapping ten-second
alternatives each. Each band's rank-one candidate enters a ten-item system
proposal labeled `PROPOSED-NOT-LOCKED`; selected choices and user acceptance
remain `UNAVAILABLE`.

The first real run writes the content-addressed
`izzi-audio-candidates-energy-1-4ebd1fc24c7fb66d`
package.
A repeated run regenerates everything in a staging directory and succeeds only
when every manifest, negotiation sheet, snapshot, contact-sheet, and checksum
file is byte-identical to the preserved package. Run `sha256sum -c SHA256SUMS`
from the package directory for an independent check.

Only numeric acoustic evidence, waveform/spectrogram PNGs, hashes, and local
toolchain facts are retained. Temporary PCM is deleted. The analyzer creates no
audio excerpt, transcript, title card, video, semantic dialogue label, provider
request, email, network transfer, or user decision. It stops at
`CANDIDATES-READY`; moving to `NEGOTIATING` requires a separate human action.

### Audio-candidate negotiation

[`audio-negotiation-1.schema.json`](audio-negotiation-1.schema.json) is the
closed Draft 2020-12 contract for negotiating the preserved acoustic
candidates. [`audio-negotiation.unavailable.json`](audio-negotiation.unavailable.json)
is the real starting skeleton. It binds the exact `CANDIDATES-READY` manifest
path and SHA-256 and contains all ten bands, but every action, decision, and
decision source is `UNAVAILABLE`. Its claimed state is `CANDIDATES-READY`, its
lock claim is false, and it contains no selected candidate.

Resolve the real skeleton and run the complete offline fixture suite with:

```sh
python3 examples/seedance2/check-audio-negotiation.py
python3 examples/seedance2/check-audio-negotiation.py --check-fixtures
python3 examples/seedance2/check-audio-negotiation.py --check-fixtures
```

The first command validates and resolves the real all-unavailable skeleton. The
two fixture-suite invocations exercise the same complete valid/invalid set
twice so their deterministic output can be compared.

The checker resolves the manifest path canonically beneath the authorized audio
baseline root, rejects traversal and symlink escapes, and verifies the pinned
manifest hash before inspecting candidate IDs. It then checks ten unique bands,
candidate membership, exact 160,000-sample ranges, uniqueness, and non-overlap.
The input's state and lock fields are claims, not authority: the checker derives
the state independently and rejects any mismatch.

Ten `UNAVAILABLE` decisions derive `CANDIDATES-READY`. Once any action is
present, a rejection, reanalysis request, defer action, or incomplete selection
set derives `NEGOTIATING` and cannot lock. Real `TEN-LOCKED` eligibility requires one
set derives `NEGOTIATING` and cannot lock. Real `TEN-LOCKED` eligibility
requires one explicit `USER`-sourced selection from every bound band. Silence,
a system proposal, omitted data, and system-sourced choices never count as user
acceptance.

The fixture named `synthetic-all-selections.json` exercises the ten-selection
branch only. The document, every decision source, and the resolver result label
it as synthetic, non-authoritative test evidence—not user decisions. The
checker reports `synthetic_lock_test: true`, `authoritative_lock: false`, and
`actual_user_acceptance: UNAVAILABLE`; it never writes a real locked manifest.

The resolver reads JSON and the candidate manifest only. It does not read the
audio or transcript, modify the preserved candidate package, create media,
contact a provider, use the network, or persist its derived result. Repeated
fixture-suite output is deterministic and can be compared directly by hash.
