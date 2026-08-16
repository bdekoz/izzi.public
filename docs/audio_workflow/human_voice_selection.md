# Human voice selection and redubbing completion

This workflow lets each recorded participant choose—or decline—the local
synthetic voice used to transform their own speech. It also gives humans and AI
agents an unambiguous way to record the decision without treating a successful
render, silence, or a producer's preference as consent.

Voice selection, character casting, provider transfer, and publication are four
separate decisions. None implies another.

## Current audition packages

The current packages contain three approved excerpts and three neutral voice
options per excerpt. Every review WAV is transformed locally with MeanVC2; no
unchanged-source comparison clip is included.

| Project | Listening guide | Waveforms | Manifest and frozen SHA-256 |
|---|---|---|---|
| Duotone 111 | `outputs/ad-hoc/duotone-111/audio-auditions/README.md` | `outputs/ad-hoc/duotone-111/audio-auditions/audio-audition-waveforms.png` | `outputs/ad-hoc/duotone-111/audio-auditions/manifest.json`, `32fc6c513c6adca4ea88d651132bf10c62ca8ceb070251b19311c545a13b89c7` |
| Here Lies Trouble | `outputs/ad-hoc/here-lies-trouble/audio-auditions/README.md` | `outputs/ad-hoc/here-lies-trouble/audio-auditions/audio-audition-waveforms.png` | `outputs/ad-hoc/here-lies-trouble/audio-auditions/manifest.json`, `eebab6d5d067d153e3cfbaac14eb65d9a3389b123b6ddc77c4147f4366af2d21` |

The neutral codes currently identify these exact local target references:

| Code | Kokoro stock source ID | Target-reference SHA-256 | Character assignment |
|---|---|---|---|
| `V01` | `af_river` | `d8797ae92dce760ce1f9d91d16a811bbdaf77dd6fe9b91c501c8e19a6bb82365` | none |
| `V02` | `am_echo` | `740558df7065577c850a1db8840d3cda395a953c6eb92d4721301c0f0161882d` | none |
| `V03` | `bf_emma` | `082a8a0c519bd6885e70508f22821b3da4eb85fe09a7da141f89c382e1221eec` | none |

If an audition manifest or target-reference hash changes, every selection bound
to the prior hash returns to review. A code by itself is not enough.

## Complete-form engineering review sets

Before asking any participant to fine-tune a voice, the local pipeline now
renders three complete episode intervals for each project with deterministic
random defaults. This first pass establishes that source cutting, voice
transformation, whole-episode duration, ordering, packaging, and playback form
work end to end. It does not ask for or record a human voice decision.

| Project | Episodes | Total duration | Playlist | Timeline sheet | Review-set manifest SHA-256 |
|---|---:|---:|---|---|---|
| Duotone 111 | 3 | 305.66 s | `outputs/ad-hoc/duotone-111/audio-random-defaults/review-playlist.m3u` | `outputs/ad-hoc/duotone-111/audio-random-defaults/review-timelines.png` | `ed0c1135429220d69758721984b804034d1cb914679a01e92730d0d1febe2c13` |
| Here Lies Trouble | 3 | 469.00 s | `outputs/ad-hoc/here-lies-trouble/audio-random-defaults/review-playlist.m3u` | `outputs/ad-hoc/here-lies-trouble/audio-random-defaults/review-timelines.png` | `b8baba94297857c4c73d8e9771c297d401ce69cefb2bd2fff2439726854ea134` |

Run the complete functional gate with:

```bash
build/private/voice-audition/runtime/venv/bin/python \
  scripts/check-local-audio-full-episodes.py
```

Only after a complete-form set passes and humans can listen to it does the
second pass begin: keep a generated default, explore alternatives, correct a
cut or assignment, or decline. This boundary keeps consent lightweight because
participants are reviewing a working artifact, not helping debug the pipeline.
The random defaults remain `REVIEW-ONLY-NOT-SELECTED`, and the second pass
retains every existing consent and authority boundary below.

## Two-pass terminal workflow

The offline terminal program is:

```text
./scripts/izzi-human-voice-select.py
```

It intentionally separates technical generation from human consent. A
participant should not have to wait while the producer discovers whether the
pipeline works.

### First pass (Stage 0): engineering preview and complete-form proof

The producer runs the workflow without asking the participant to choose
anything. At project scale, the six complete episodes above prove the full
render and packaging form. For a participant-specific session, the program
then verifies every manifest and transformed WAV hash, randomly assigns one
neutral provisional voice, and creates a private review session. The assignment
is labeled `REVIEW-ONLY-NOT-SELECTED`; it writes no selection ledger event and
conveys no participant consent.

Prepare a guided studio review:

```bash
./scripts/izzi-human-voice-select.py generate-default --profile guided
```

Prepare a solo-explore review:

```bash
./scripts/izzi-human-voice-select.py generate-default --profile solo-explore
```

`prepare-solo` is a shorter alias for the second command. The setup prompts ask
for the local manifest, private participant and speaker codes, speaker-isolated
line IDs, and—only for guided review—a facilitator code and confirmation mode.

Before involving the participant, verify and play only the generated default:

```bash
./scripts/izzi-human-voice-select.py preview-default --session SOLO-0123456789ABCDEF
```

This producer preview is the “does the machinery actually work?” gate. It never
writes a participant decision. `--no-audio` verifies the complete path and
hash contract without invoking audio hardware.

### Second pass (Stage 1): human review, refinement, and consent

After the Stage 0 first pass succeeds, start the profile recorded in the
session:

```bash
./scripts/izzi-human-voice-select.py guided --session GUIDED-0123456789ABCDEF
./scripts/izzi-human-voice-select.py solo --session SOLO-0123456789ABCDEF
```

The participant first receives the randomly generated option. They may:

- approve that generated option;
- explore all neutral alternatives and select one;
- request another round;
- decline use of their recorded speech; or
- leave without any recorded decision.

After a voice choice, the program shows its exact local project scope and asks
one plain-language yes-or-no question. That single affirmative response records
that the participant reviewed their own speaker-isolated transformed voice and
approves the selected option for local redubbing in this project only. There
are no magic phrases or repeated consent confirmations. A default answer is
never “yes.”

The interface deliberately records no replay counts, response timing,
keystrokes, hesitation, rankings, or other behavioral telemetry. Provider
transfer, publication, character casting, and training-conversion transfer are
hard-coded `false` in every terminal decision.

### Guided profile

The participant and facilitator are in the studio together. The facilitator
may explain or operate the playback controls but may not recommend or choose a
voice. The ledger distinguishes participant-at-keyboard confirmation from a
visible verbal participant confirmation and stores only a private facilitator
code.

### Solo-explore profile

The producer prepares and previews the review session, provides headphones,
starts the participant-specific terminal interface, and leaves the participant
alone. The participant sees neutral letter labels rather than stock voice
names. Exiting writes nothing; completing a decision locks that one-time
session and displays a redacted receipt ID.

The terminal exposes no shell escape or network operation. Playback uses a
local `mpv`, `ffplay`, `pw-play`, `paplay`, or `aplay` process without invoking
a shell.

Verify the terminal, current audition packages, and private ledger state with:

```bash
make voice-selection-check
make voice-selection-verify
```

## Who may choose

Each participant chooses the transformed voice for their own recorded speech.
Another participant, producer, collaborator, or AI agent may prepare and explain
the options but may not make that choice for them. A participant may:

- select one offered voice;
- request a different set of options;
- decline transformation and use of their recorded speech; or
- revoke or replace an earlier choice.

The current scene-level clips are palette canaries. They qualify as a
participant's consent audition only when that person can identify and review
their own transformed line. If a clip contains multiple speakers, prepare a
speaker-isolated audition line before registering a participant selection.

## Listening procedure

1. Verify the package and every transformed WAV hash before Pass 1.
2. Generate and preview one random provisional option without recording a
   participant selection.
3. In Pass 2, let the participant hear the generated option first. Describe it
   as provisional, not recommended.
4. If they choose to explore, provide every neutral version of the same
   one-speaker line under letter labels.
5. Use headphones or a quiet room and play each option at the same playback
   volume. The files share a `-1 dBFS` peak ceiling, but perceived loudness may
   still differ.
6. Ask the participant to judge intelligibility, comfort, adequate separation
   from their source timbre, and fit with the project. The waveform sheet is for
   navigation, not for choosing a voice by appearance.
7. Let the participant replay, pause, request a new round, decline, or leave
   undecided without
   penalty.
8. After a choice, ask one clear yes-or-no question covering ownership of the
   reviewed line and local project scope. Do not infer consent from silence,
   laughter, continued collaboration, or prior recording consent.
9. Record the exact manifest hash, target-reference hash, scope, and decision in
   the private append-only ledger described below.

For Here Lies Trouble, choosing a voice does not assign the unicorn, albatross,
dog, or cat. Character casting is a later creative decision with a separate
record.

## Selection states

Use only these machine-readable states:

| State | Meaning |
|---|---|
| `NOT-ASKED` | No participant audition has occurred. |
| `AUDITION-PREPARED` | A participant-specific set exists but has not been delivered. |
| `AUDITION-DELIVERED` | The participant received or heard the exact hash-bound set. |
| `SELECTED` | The participant explicitly selected one exact voice reference for the stated scope. |
| `REQUEST-NEW-ROUND` | None of the offered options was accepted; prepare new choices. |
| `DECLINED` | The participant declined use of their recorded speech under this workflow. |
| `REVOKED` | A prior selection is withdrawn. Stop new use and evaluate already-produced artifacts under the stated retention policy. |
| `SUPERSEDED` | A newer explicit selection replaces the referenced event. |

`SELECTED` authorizes local redubbing only when
`local_redubbing_authorized` is also `true`. It does not authorize a Seedance
submission, another hosted service, publication, training-conversion transfer,
or use in a different project.

## Private append-only selection ledger

Store selection events locally at:

```text
resources.static/.private/human-voice-selections.jsonl
```

The `.private` directory is ignored by Git. Keep the file mode restricted to the
local user, do not paste participant names or email addresses into public
manifests, and never copy the ledger into `outputs/`. Use stable private codes
such as `P01` and `SPEAKER-01`; keep any identity-to-code mapping in a separate
private consent record.

Write one complete JSON object per line. Never edit an old decision in place.
Revocation and reselection are new events that reference the event they replace.

Example `SELECTED` event:

```json
{
  "schema_version": "izzi-human-voice-selection-v1",
  "event_id": "VOICE-SELECTION-000001",
  "recorded_at": "2026-08-09T12:00:00-07:00",
  "actor": "participant",
  "participant_code": "P01",
  "project": "here-lies-trouble",
  "scope": {
    "speaker_track_id": "SPEAKER-01",
    "scene_ids": ["hlt1-episode-01-proposed"],
    "use": "local-redubbing-for-this-project-only"
  },
  "audition": {
    "manifest_path": "outputs/ad-hoc/here-lies-trouble/audio-auditions/manifest.json",
    "manifest_sha256": "2b0e487a10240ec554260a4df996d691e72ba5db73a5842603aa97fe65befea6",
    "speaker_isolated": true,
    "own_transformed_line_reviewed": true
  },
  "decision": {
    "state": "SELECTED",
    "voice_code": "V02",
    "target_reference_sha256": "740558df7065577c850a1db8840d3cda395a953c6eb92d4721301c0f0161882d",
    "selection_origin": "GENERATED-OPTION-OK",
    "evidence_status": "EXPLICIT-PARTICIPANT-CONFIRMATION"
  },
  "interaction": {
    "profile": "SOLO-EXPLORE",
    "facilitator_code": null,
    "participant_at_keyboard": true,
    "confirmation_mode": "participant-at-keyboard",
    "session_id": "SOLO-0123456789ABCDEF",
    "behavioral_telemetry_recorded": false
  },
  "authority": {
    "local_redubbing_authorized": true,
    "provider_transfer_authorized": false,
    "publication_authorized": false,
    "training_conversion_transfer_authorized": false,
    "revocable": true
  },
  "supersedes_event_id": null,
  "privacy_redaction_status": "PRIVATE CODED PARTICIPANT DECISION; no public identity mapping"
}
```

For `DECLINED` or `REQUEST-NEW-ROUND`, set `voice_code` and
`target_reference_sha256` to `null`. For `REVOKED` or `SUPERSEDED`, set
`supersedes_event_id` to the exact earlier event. Never delete the earlier line
to make the history appear cleaner.

## Registering a decision in this Codex thread

A participant may provide this bounded instruction directly, or a producer may
relay it only with a separate, visible participant-confirmation receipt:

```text
REGISTER LOCAL VOICE SELECTION
participant_code: P01
project: here-lies-trouble
speaker_track_id: SPEAKER-01
scene_ids: hlt1-episode-01-proposed
audition_manifest_sha256: 2b0e487a10240ec554260a4df996d691e72ba5db73a5842603aa97fe65befea6
decision: SELECTED
voice_code: V02
target_reference_sha256: 740558df7065577c850a1db8840d3cda395a953c6eb92d4721301c0f0161882d
local_redubbing_authorized: true
provider_transfer_authorized: false
publication_authorized: false
```

Codex should then:

1. verify the manifest and target-reference hashes;
2. verify that the participant reviewed their own transformed line;
3. append one private ledger event with a monotonic event ID;
4. write a public trace event containing only the participant code, bounded
   scope, decision state, and artifact hashes—never an identity mapping;
5. report that voice selection is registered without implying character casting
   or publication consent.

An AI agent must reject ambiguous instructions such as “use whichever sounds
best,” a selection made by someone else, a voice code without its hash, or a
choice against a regenerated manifest.

## Separate character-casting record

After participant selections are valid, record animal casting separately in a
private append-only file such as:

```text
resources.static/.private/here-lies-trouble-character-casting.jsonl
```

Each casting event should reference the participant code, valid selection event
ID, character code, project scope, and explicit creative approval. It must not
change the selected target voice or broaden consent.

## Redubbing completion gate

The audio-redubbing task is complete only when all of the following are true:

- every audible participant and uncertain speaker turn has a stable private
  speaker code and a reviewed line inventory;
- every used participant has a current `SELECTED` event for the exact manifest,
  target-reference hash, and project scope;
- every `DECLINED` or unselected source voice is removed, re-recorded, or recast
  without cloning that person;
- dialogue is separated by speaker before transformation, including overlaps;
- every audible source turn receives the correct selected local transform;
- no unchanged source voice leaks into the review or release mix;
- outputs decode, contain finite samples, meet the delivery format, and avoid
  clipping;
- each participant reviews their own completed transformed passages for
  intelligibility and comfort;
- the project owner separately accepts the assembled mix and character casting;
- provider transfer and publication are separately authorized if those actions
  are desired; and
- a final receipt records hashes, tests, corrections, unresolved limitations,
  and explicit human outcomes.

Current status: the six-scene, eighteen-clip local audition matrix is
technically verified, but participant-specific selection events are
`NOT-ASKED`. Human listening is the next gate.
