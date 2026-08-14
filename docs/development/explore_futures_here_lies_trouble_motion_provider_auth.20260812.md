# Here Lies Trouble Frogtown v2 motion-canary provider authorization

Recorded: 2026-08-12 America/Los_Angeles
Status: `DRAFT-AWAITING-APPROVAL; PROVIDER-SUBMISSION-NOT-AUTHORIZED;
ZERO-SPEND; NOT-SHARED`

This document is the requested authorization proposal for generating the three
Here Lies Trouble Episode 1 Frogtown v2 silent motion canaries through the
configured provider. Approving it authorizes a bounded, sequential, five-second
image-to-video run from the accepted Frogtown keyframe using the motion
aesthetics derived from the Blade Runner seed clips. It does not authorize
anything beyond the exact scope below.

## Provider boundary

- Provider: `seedance2ai.io` only. The only configured local credential and the
  only previously tested adapter are bound to this domain
  (`build/private/seedance2-submit-one.sh`,
  `https://www.seedance2ai.io/api/v1/video/seedance2`).
- `seedance2ai.ai` is a distinct service and is never used.
- Operator preflight (no spend, no account change) before any submission:
  1. confirm the paid account is on `seedance2ai.io`;
  2. record the authenticated plan name, credit balance, model menu, and exact
     pre-submit quote for a 5-second 720p image-to-video job;
  3. confirm the local key file exists at
     `${IZZI_PRIVATE_DIR:-$HOME/.config/izzi/private}/seedance2.key` without
     reading or printing its value;
  4. stop and report if the quote differs from the estimate below or exposes a
     different model or resolution spelling.

## Work item and inputs

- Work item: three sequential silent five-second motion canaries, one per
  specification:
  1. `examples/seedance2/here-lies-trouble-episode-01-frogtown.v2.motion-canary-01.json`
     (GROUP-WALK);
  2. `...-02.json` (CONVERSATION-AND-LISTENER-REACTIONS);
  3. `...-03.json` (LOCATION-EXPLORATION).
- Start image: the accepted Frogtown keyframe, hash-bound in all three specs:
  `outputs/review/feedback/visual/here-lies-trouble/neon-addict-stage-01/style-frames/sample-01-frogtown-source.png`
  (941x1672, SHA-256
  `78b79a6f1cb67162293c7ff901ca53d90a4d7c44d179b649d4d9103b95b86d42`).
- Motion aesthetic seeds: the five Blade Runner clips in
  `resources/blade-runner/motion-aesthetic-seeds.json` remain local-only. Only
  the derived `motion_grammar` text compiled from the three canary
  specifications is eligible for prompts. Blade Runner frames, clips, titles,
  and names are never sent to the provider; prompts must not name the film.
- Audio: disabled for all three jobs. No audio bytes, transcripts, voice
  mapping, or speaker diarization data are transferred.

## Requested service contract

Each request follows the previously tested adapter contract
(`mode: image-to-video`, `quality_tier: pro`, `channel: standard`,
`duration: "5"`, `aspect_ratio: "9:16"`, `resolution: "720p"`,
`generate_audio: false`) with deterministic per-canary seed values recorded in
the request JSON and later in each spec's `provider_adapter`.

| Canary | Lens policy | Prompt motion emphasis |
| --- | --- | --- |
| 01 GROUP-WALK | `fixed_lens: false`, restrained lateral drift only | unhurried weighty stride, coherent limb cycles, steady build; camera never the primary mover |
| 02 CONVERSATION-REACTIONS | `fixed_lens: true` | one deliberate gesture per beat, distinct listener reactions, held stillness between beats |
| 03 LOCATION-EXPLORATION | `fixed_lens: false`, mostly fixed, small reframes only after actions | advance, pause, look; rhythmic action-pause-action; plant and water motion during holds |

The compiled prompt is derived from each spec's `prompt`, `scene`, and
`motion_grammar` fields exactly; no field is relaxed at submission time.

## Cost gate

- Prior observed cost on this provider: 120 credits for one 5-second pro 1080p
  job. For 720p the proposal estimate is 60 credits per job.
- Hard stop per job: a job that reports or quotes more than 90 credits.
- Hard stop total: no submission begins if the three-job total would exceed
  270 credits.
- If the pre-submit quote does not match the estimate, stop and report before
  submitting anything.
- No automatic retry and no second creative attempt per job.

## Publication gate

Approval includes two narrow, explicit publications:

1. Stage exactly one accepted keyframe copy for provider fetching, hash-bound
   under `https://situationshipin.space/review/provider-inputs/here-lies-trouble/frogtown-v2/`,
   matching the prior provider-input staging pattern.
2. After the three canaries return, publish ten-frame filmstrip review pages
   and bounded MP4 review proxies (360x640 or equivalent browser derivative)
   for human review on situationshipin.space, exactly as done for the Frogtown
   audio canary.

Canonical MKVs, source audio, Blade Runner media, credentials, and transcripts
are never published. No other media is published.

## Verification and human gate

- Preserve each provider original locally and record SHA-256, duration, codec,
  and exact dimensions.
- Require `ffprobe` to report exactly 720x1280 for the portrait canary before
  any next submission; treat any other resolution as a failed delivery even if
  the creative result is useful.
- Generate the ten-frame filmstrip and confirm source/proxy/frame hash lineage.
- Provider completion is not aesthetic acceptance: the three clips remain
  `HUMAN-REVIEW-PENDING` until reviewed, and no baseline promotion is inferred.

## Authority boundary

Approval grants, for this work item only:

- `provider_submission_authorized: true` (three sequential 5-second 720p jobs);
- `motion_generation_authorized: true`;
- `additional_spending_authorized: true` at the cost gate above;
- narrow publication of the one keyframe copy and the three review packages
  described above.

Approval does not grant: audio transfer, canonical media publication,
baseline promotion, training-conversion transfer, additional episodes, or any
job beyond the three specified canaries.

Companion implementation after approval: each canary spec's `provider_adapter`
and `authority` fields flip to the authorized state with the recorded provider,
model, seed, and ceiling; the motion-canary schema's status contract and the
independent checker are updated in the same change so validation remains
consistent and no authority escalation is hidden.

To approve this exact scope and the proposed cost gate, reply:

`AUTHORIZE HLT FROGTOWN V2 MOTION CANARIES`

or reply with the same phrase plus a different cost ceiling.
