# Here Lies Trouble Frogtown v2 full-length scene provider authorization

Recorded: 2026-08-12 America/Los_Angeles
Status: `DRAFT-AWAITING-APPROVAL; PROVIDER-SUBMISSION-NOT-AUTHORIZED;
ZERO-SPEND; NOT-SHARED`

This document is the requested authorization for generating one full-length
30-second Frogtown scene with real motion. It follows the three approved
five-second motion canaries and uses the same accepted keyframe, provider,
seed grammar, and review-publication path. The previous approval explicitly
left additional provider jobs unauthorized, so this is a separate, bounded
grant for exactly three chained jobs that assemble into the one scene.

## What "full-length scene" means here

The requested scene length is 30 seconds. The provider adapter's documented
maximum single-job duration is 15 seconds, so the scene is produced as three
chained 10-second image-to-video jobs:

1. **Job A (seconds 0-10):** image-to-video from the accepted Frogtown
   keyframe, seed `2026081204`.
2. **Continuity handoff:** the last provider frame of Job A is staged publicly
   (hash-bound) as the start image for Job B.
3. **Job B (seconds 10-20):** image-to-video from that last frame, seed
   `2026081205`, continuing the same walk-and-converse action.
4. **Job C (seconds 20-30):** the last frame of Job B is staged as the start
   image, seed `2026081206`, continuing the same action.
5. **Assembly:** the three silent provider clips are losslessly concatenated
   into one continuous 30-second MKV and one bounded MP4 review proxy.

A complete 168-second episode is a separate, much larger round and is not part
of this authorization.

## Provider boundary

- Provider: `seedance2ai.io` only; the configured credential and tested adapter
  are domain-bound to this platform. `seedance2ai.ai` is never used.
- Operator preflight (no spend, no account change) before submission:
  1. confirm the paid account is on `seedance2ai.io`;
  2. record the authenticated plan name, credit balance, model menu, and exact
     pre-submit quote for a 10-second 720p image-to-video job;
  3. confirm the local key file exists at
     `${IZZI_PRIVATE_DIR:-$HOME/.config/izzi/private}/seedance2.key` without
     printing its value;
  4. stop and report on any model, price, or resolution-spelling mismatch.

## Work item and input

- Work item: `WI-HLT-FROGTOWN-V2-FULL-LENGTH-SCENE-001`.
- Start image: the accepted Frogtown keyframe, already staged and live:
  `https://situationshipin.space/review/provider-inputs/here-lies-trouble/frogtown-v2/sample-01-frogtown-source.png`
  (SHA-256
  `78b79a6f1cb67162293c7ff901ca53d90a4d7c44d179b649d4d9103b95b86d42`).
- Motion aesthetic seeds: the Blade Runner clips stay local-only; only the
  approved `motion_grammar` text enters the prompt, and the film is never
  named.
- Audio: disabled. No audio bytes, transcripts, or voice mapping are
  transferred.

## Requested service contract

```json
{
  "mode": "image-to-video",
  "quality_tier": "pro",
  "channel": "standard",
  "duration": "10",
  "aspect_ratio": "9:16",
  "resolution": "720p",
  "generate_audio": false,
  "fixed_lens": false,
  "seed": 2026081204,
  "image_url": "https://situationshipin.space/review/provider-inputs/here-lies-trouble/frogtown-v2/sample-01-frogtown-source.png"
}
```

Jobs B and C use the same body with `seed: 2026081205` and `2026081206`, and
`image_url` set to the staged last frame of the preceding job. All three
prompts are compiled from the approved canary prompts and grammar (subject to
exact wording in the request JSONs at submission); continuation jobs
additionally say the scene continues from the supplied frame with no cut, same
camera restraint, same group, and the same Frogtown location.

> Animate the supplied accepted Frogtown keyframe as one continuous original
> two-dimensional scene. Keep exactly one unicorn, one albatross, one dog, and
> one cat with stable silhouettes and restrained neon markings. Over ten
> seconds the four animals walk together along the Frogtown Los Angeles River
> bicycle path with an unhurried, weighty stride, talking as they go: coherent
> leg cycles, shifting body weight, one economical speaking gesture, and
> distinct listener reactions. The albatross walks and makes one low balancing
> hop rather than flying away. The camera drifts laterally with restraint to
> keep the group framed but never becomes the primary source of motion;
> near-static beats are preferred. River glints, riparian plants, and one
> distant bicycle-wheel arc move independently. Preserve bold off-white and
> black fields, cyan, chartreuse, magenta, yellow, and coral accents, tactile
> ink and paper texture, angular shadow, and generous negative space. End with
> all four animals intact and farther along the same recognizable path. No
> text and no audio.

## Cost gate

- Observed rate from this round: 55 credits per five-second pro 720p job.
- Estimate per ten-second job: 110 credits.
- Estimate for the 30-second scene (three jobs): 330 credits.
- Hard stop per job: a quote or reported charge above 150 credits.
- Hard stop total: no next job begins if the three-job total would exceed
  450 credits.
- No automatic retry and no second creative attempt.

## Delivery and publication gates

- Preserve the provider original locally and record SHA-256, duration, codec,
  and exact dimensions.
- If the delivery is 720x1280, label it 720p. Based on this provider's
  behavior, 496x864 is likely; in that case label it a below-720 creative
  preview and never call it 720p. No upscaling, no retry.
- Each job's original is retained locally with SHA-256, duration, codec, and
  exact dimensions recorded; the assembled 30-second scene is verified with
  `ffprobe`.
- After assembly, publish one ten-frame filmstrip review page (sampled across
  the full 20 seconds) and one bounded MP4 review proxy on
  situationshipin.space, mirroring the approved canary packages, and verify
  the exact review URL is live before reporting.
- Provider completion is not aesthetic acceptance; the scene remains
  `HUMAN-REVIEW-PENDING` until reviewed.

## Authority boundary

Approval grants, for this single job:

- `provider_submission_authorized: true` (exactly three chained 10-second 720p
  jobs that assemble into one 30-second scene);
- `motion_generation_authorized: true`;
- `additional_spending_authorized: true` at the cost gate above;
- narrow publication of the filmstrip and MP4 review package described above.

Approval does not grant: audio transfer, canonical media publication, baseline
promotion, training-conversion transfer, retries, upscaling, the remaining
episode scenes, or any additional provider job.

To approve this exact scope and cost gate, reply:

`AUTHORIZE HLT FROGTOWN V2 FULL-LENGTH SCENE`

or reply with the same phrase plus a different per-job ceiling or total
ceiling.
