# Izzi and Seedance 2 account integration proposal

- Status: historical proposal for the `seedance2ai.io` adapter; later canary
  evidence supersedes its original purchase recommendation
- External-source review: 2026-08-08, with provider-identity correction on
  2026-08-10 (America/Los_Angeles)
- Scope: ten vertical installments, one ten-second provider scene per
  installment, with deterministic izzi titles and post-production

This proposal describes an optional account and delivery adapter for the
independent `seedance2ai.io` service. It does not create an account, buy credits,
publish an asset, submit a generation request, or change the existing Seedance
JSON schemas.

`seedance2ai.ai` is a different independently operated service. Do not replace
the `.io` hostname in this proposal, reuse the `.io` key there, or interpret an
account at one domain as an account at the other. The current comparison and
new-domain preflight are in
[`20260811.md`](../sessions/20260811.md).

The later audio-first clarification is specified in
[`explore_futures_seedance_audio.md`](../seedance/audio.md). In
that workflow, *scene* is the provider/rendering unit and *installment* is the
editorial unit; they map one-to-one. Its compact input describes an overall
vertical style, while the ten source moments and resolved scene details are
negotiated rather than authored up front.

## Recommendation

For the current collaboration, use one designated adult account operator and
keep the creative handoff in the existing izzi authoring JSON. Do not share an
API key with collaborators. Start with one deliberately inexpensive canary scene,
then authorize the remaining nine scenes only after recording its actual credit
charge and reviewing its visual result.

The current best-fit purchase is the **one-time Starter Pack**, not a Team plan
or annual subscription. The public pricing page currently advertises 1,400
credits and up to 70 videos for $39.90, with credits valid for one year. This
comfortably exceeds the advertised count needed for one ten-installment project and
some revisions, without an auto-renewing commitment. This is a capacity
recommendation, not a cost guarantee: the platform says that model and quality
settings consume different amounts, while its API example reports 30 credits
for a five-second standard request. It publishes no quote endpoint or definitive
ten-second price. Record the canary's `credits_used` value before extrapolating.
[Pricing](https://www.seedance2ai.io/pricing)
[API documentation](https://www.seedance2ai.io/developers)

Use the existing provider defaults for the first canary:

```json
{
  "quality_tier": "standard",
  "channel": "standard",
  "aspect_ratio": "9:16",
  "duration": "10",
  "resolution": "720p",
  "generate_audio": false,
  "fixed_lens": true
}
```

These canary settings retain the studies' 30 fps and 300-frame timing while
selecting the clarified vertical output. The existing 1280×720 studies remain
evidence for the compositing mechanism, not the new canvas orientation. A
720p canary avoids paying for resolution that an account-path test does not
need; the accepted local vertical master is still 1080×1920, and any scaling is
named in its receipt. API
`quality_tier: "pro"` and the account subscription named **PRO** are different
concepts. The former is a request setting; the latter is a recurring credit
plan.

## Service and ownership boundary

Two organizations must remain visibly separate in code, documentation, receipts,
and credits:

| Concern | Authority | What izzi may rely on |
|---|---|---|
| Seedance model capabilities | ByteDance Seed | ByteDance's model page and launch material |
| Web account, credits, API, storage, terms, and privacy | SixBryan LLC's independent `seedance2ai.io` platform | That platform's current public documentation and the account operator's observed settings |
| Exact SVG titles, transitions, manifests, MKV normalization, and accessibility records | izzi and its user | Local schemas, tools, artifacts, hashes, and human review |

ByteDance Seed announced Seedance 2.0 on 2026-02-12. Its official material says
the model accepts text, image, audio, and video, can combine up to nine images,
three video clips, and three audio clips, and can produce up to 15-second
multi-shot audio-video output. It also identifies remaining weaknesses,
including text-rendering accuracy. These facts support using the model for scene
motion while retaining izzi-rendered typography as the exact authority.
[ByteDance Seedance 2.0 launch](https://seed.bytedance.com/en/blog/seedance-2-0-official-launch)
[ByteDance Seedance 2.0 model page](https://seed.bytedance.com/en/seedance2_0)

The independent platform's terms say it is operated by SixBryan LLC, a Wyoming
limited liability company, and is not affiliated with or endorsed by ByteDance
or the other model developers it names. Its footer likewise describes the site
as independent. The adapter must therefore be named for the platform, not
presented as a ByteDance API client.
[seedance2ai.io terms](https://www.seedance2ai.io/terms-of-use)

Use a versioned `seedance2ai-v1` adapter. Keep provider behavior out of izzi's
SVG core so another provider or a fully offline MKV workflow can replace it
without changing the creative document.

## Account levels

The platform says that any account with credits can create an API key; API access
has no separate plan gate. Personal web and API generation draw from the same
personal balance. For Team accounts, API calls use each member's personal
balance, not the shared Team pool. A Team plan therefore does not solve API
budgeting for this two-person authoring workflow.
[Developer FAQ](https://www.seedance2ai.io/developers)

The following values were visible on 2026-08-08 and can change. “Up to” counts
are platform marketing estimates, not per-request guarantees.

| Offering | Current published allowance | Appropriate use | Recommendation now |
|---|---:|---|---|
| Trial Pack, one time | $2.99; 200 credits; up to 5 videos | One to five canary or API-integration tests | Useful before the full run, but insufficient for ten advertised outputs |
| Starter Pack, one time | $39.90; 1,400 credits; up to 70 videos; one-year credit validity | One bounded project with revisions | **Recommended** |
| BASIC, annual new-member offer | $179/year; 1,200 credits released monthly; up to 60 videos/month | Recurring monthly projects | Choose only after the first project demonstrates continuing demand |
| STANDARD, annual new-member offer | $299/year; 2,500 credits/month; up to 125 videos/month | Frequent iteration or several projects per month | Not justified by the present ten-installment workload |
| PRO, annual new-member offer | $599/year; 6,000 credits/month; up to 300 videos/month | Sustained production volume and currently advertised higher-tier benefits | Defer until measured use warrants it |
| MAX, annual new-member offer | $1,199/year; 13,000 credits/month; up to 650 videos/month | High-volume production | Do not select for the present work |

Annual offers renew and release credits monthly. The pricing page labels its
displayed annual discount as a new-member rate. Recheck the live checkout total,
renewal terms, commercial-license indication, and credit release schedule before
purchase. Do not turn a displayed promotion into a hard-coded budget.
[Current pricing](https://www.seedance2ai.io/pricing)
[Refund policy](https://www.seedance2ai.io/refund-policy)

### Credit uncertainty and the canary rule

The platform's terms say different models and quality settings consume different
amounts of credits. The API response exposes `credits_used` and
`credits_refunded`, but the public API documentation does not describe a quote
or balance endpoint. Consequently:

1. Dry-run and hash all ten requests locally without submitting them.
2. Submit only `scene-01` with a stable idempotency key.
3. Persist the returned `credits_used`, output, and visual snapshot.
4. Stop for cost and visual review.
5. Set a numeric project credit ceiling based on observed evidence.
6. Submit scenes 02–10 sequentially or with bounded concurrency.
7. Stop before a new paid attempt whenever the ceiling or retry count would be
   exceeded.

No estimate should silently become authorization to spend. A failed generation
may be refunded according to the terms, but izzi should record the returned
`credits_refunded` value rather than assume a refund occurred.

## Account setup and secret handling

The account operator performs these steps outside izzi:

1. Review the current pricing, terms, privacy policy, commercial-use status, and
   refund terms; create or select the adult account that will own the jobs.
2. Purchase the chosen credits only after an explicit budget decision.
3. Create an API key in the account dashboard. The developer page says the key
   is displayed once.
4. Store the key in an operating-system credential store or inject it into the
   process as `SEEDANCE2AI_API_KEY` at execution time. This checkout's
   on-disk fallback is outside the repository tree at
   `~/.config/izzi/private/seedance2.key` (mode 600; `IZZI_PRIVATE_DIR`
   overrides the directory); see `docs/development/archive/seedance-auth.md`.
5. Revoke and replace the key immediately if it is exposed.

The key must never appear in:

- the authoring or resolved project JSON;
- shell history, command arguments, Git, CI logs, trace JSONL, error messages,
  screenshots, or email;
- the idempotency key, output name, or downloadable artifact metadata;
- a browser bundle or WebAssembly module.

The adapter should accept only a credential reference such as:

```json
{
  "credential": {
    "source": "environment",
    "name": "SEEDANCE2AI_API_KEY"
  }
}
```

It resolves the value at the last possible moment, sends it only in the HTTPS
`Authorization: Bearer …` header, and redacts that header from every receipt.
Provider output URLs may themselves contain temporary access material; private
runtime state may retain them with owner-only permissions until download, while
shareable receipts store a redacted origin and the downloaded file's SHA-256.

## Existing izzi JSON contracts

The integration should preserve the three-document separation already proposed
in `examples/seedance2`:

| Document | Existing artifact | Role |
|---|---|---|
| Human authoring input | [`ten-scene-authoring.proposed.schema.json`](../../../examples/seedance2/ten-scene-authoring.proposed.schema.json) and [`ten-scene-authoring.example.json`](../../../examples/seedance2/ten-scene-authoring.example.json) | Exactly ten scenes; collaborator edits creative intent and optional references, never credentials |
| Resolved execution plan | [`ten-scene-project.proposed.schema.json`](../../../examples/seedance2/ten-scene-project.proposed.schema.json) and [`ten-scene-project.example.json`](../../../examples/seedance2/ten-scene-project.example.json) | Deterministic prompts, seeds, title frames, transitions, delivery defaults, outputs, and accessibility drafts |
| Provider run state | Proposed below; generated locally, not part of either existing schema | Account-neutral execution policy, task checkpoints, receipts, downloads, and acceptance |

The existing authoring schema fixes ten scenes and a ten-second, 30 fps,
300-frame timeline. A scene requires only `scene_id`, `title`, and `description`;
optional fields carry a prompt override, constraints, reference asset IDs, seed,
transition override, and accessibility hint. Assets are relative local paths with
SHA-256 values. This is the correct handoff document for collaborators.

That detailed contract remains useful for a fully hand-authored project. The
audio-first workflow adds a smaller negotiation input; it resolves to this
detailed shape only after the ten excerpts are approved. The exact skeleton
already emailed to collaborators is retained as sent rather than silently
rewritten. A later vertical run must resolve `9:16` explicitly.

The existing resolved schema keeps provider fields under
`defaults.delivery`, including `adapter`, `quality_tier`, `channel`,
`aspect_ratio`, `duration`, `resolution`, `generate_audio`, and `fixed_lens`.
Neither schema should acquire account identity, purchase information, keys,
public asset URLs, live task IDs, or mutable status.

Both existing schemas use JSON Schema Draft 2020-12. New operational schemas
should use the same dialect, close every object with
`additionalProperties: false`, and provide structured validation errors with
JSON Pointer instance locations.
[JSON Schema Draft 2020-12 core](https://json-schema.org/draft/2020-12/json-schema-core)

## Proposed provider-run JSON interface

Add a future `seedance2ai-run-1` schema only when the delivery client is
implemented. This generated file binds one immutable resolved plan to one local
run without putting mutable execution fields into the creative schemas.

```json
{
  "$schema": "https://bdekoz.github.io/izzi.public/examples/seedance2/ten-scene-project.proposed.schema.json",
  "schema_version": 1,
  "run_id": "pacific-instrument-ten-scene-run-001",
  "project": {
    "project_id": "pacific-instrument-ten-scene",
    "authoring_path": "examples/seedance2/ten-scene-authoring.example.json",
    "authoring_sha256": "<64 lowercase hexadecimal characters>",
    "resolved_plan_path": "build/pacific-instrument-ten-scene/resolved-plan.json",
    "resolved_plan_sha256": "<64 lowercase hexadecimal characters>"
  },
  "provider": {
    "adapter": "seedance2ai-v1",
    "base_url": "https://seedance2ai.io",
    "api_profile_checked_on": "2026-08-08",
    "account_offering_expected": "starter-pack",
    "account_offering_observed": "UNAVAILABLE",
    "credential": {
      "source": "environment",
      "name": "SEEDANCE2AI_API_KEY"
    }
  },
  "authorization": {
    "provider_transfer": "not-authorized",
    "authorized_scene_ids": [],
    "canary_scene_id": "scene-01",
    "max_total_credits": null,
    "unknown_cost_action": "stop-after-canary",
    "max_paid_attempts_per_scene": 2,
    "approval_receipt": null
  },
  "publishing": {
    "adapter": "caller-owned",
    "allow_public_https_only": true,
    "require_asset_sha256": true,
    "remove_after_collection": true,
    "public_url_log_policy": "redact-query"
  },
  "submission": {
    "enabled_in_file": false,
    "require_submit_flag": true,
    "queue_policy": "bounded-sequential",
    "max_in_flight": 1,
    "respect_retry_after": true,
    "retry_network_with_same_idempotency_key": true
  },
  "polling": {
    "initial_interval_seconds": 10,
    "maximum_interval_seconds": 60,
    "timeout_seconds": 7200,
    "resume_from_task_id": true
  },
  "normalization": {
    "container": "matroska",
    "extension": ".mkv",
    "width_px": 1280,
    "height_px": 720,
    "frame_rate": {
      "numerator": 30,
      "denominator": 1
    },
    "frame_count": 300,
    "duration_seconds": 10,
    "video_codec": "h264",
    "pixel_format": "yuv420p",
    "fit_policy": "contain-and-pad",
    "audio_policy": "discard"
  },
  "accessibility": {
    "reduced_motion_alternative": "poster",
    "require_flash_analysis": true,
    "require_actual_clip_description_review": true,
    "caption_policy": "required-when-speech-or-meaningful-audio",
    "transcript_policy": "required-when-meaningful-audio"
  },
  "state": {
    "checkpoint": "build/pacific-instrument-ten-scene/run-state.private.json",
    "events": "build/pacific-instrument-ten-scene/events.jsonl",
    "receipts_directory": "build/pacific-instrument-ten-scene/receipts",
    "snapshots_directory": "build/pacific-instrument-ten-scene/review",
    "atomic_writes": true,
    "existing_artifact_policy": "verify-hash-or-fail"
  },
  "acceptance": {
    "required": true,
    "infer_from_silence": false,
    "receipt": "build/pacific-instrument-ten-scene/receipts/final-acceptance.json"
  }
}
```

The placeholder SHA-256 strings, unavailable account observation, deny-by-default
transfer state, and `null` credit ceiling make this an interface example, not an
executable run. The planner replaces hashes and account observations with visible
evidence. A concise provider-transfer approval adds only `scene-01` and its
receipt. After the canary, an operator must set a numeric ceiling and expand
`authorized_scene_ids` before the remaining requests can be submitted.

### Provider request mapping

The adapter emits an allowlisted HTTP body from each resolved scene. As of the
review date, the independent platform documents this interface:

| JSON field | Rule |
|---|---|
| `mode` | `text-to-video`, `image-to-video`, or `media-to-video`; derive from approved reference roles |
| `quality_tier` | `mini`, `standard`, or `pro` |
| `channel` | `standard`, `real`, or `wild` |
| `prompt` | Required, 3–10,000 characters; use the exact reviewed resolved prompt |
| `aspect_ratio` | `1:1`, `21:9`, `4:3`, `3:4`, `16:9`, `9:16`, or `adaptive` |
| `duration` | String value from 4 through 15 seconds; izzi v1 fixes `"10"` |
| `resolution` | `720p`, `1080p`, `1080p-plus`, or `4k`; the last two require API quality tier `pro` |
| `image_url` | Public HTTPS start frame, required for image-to-video, image at most 30 MB |
| `end_image_url` | Optional public HTTPS end frame, image at most 30 MB |
| `media_urls` | Up to 12 public HTTPS image/video URLs for media-to-video |
| `generate_audio` | Boolean |
| `fixed_lens` | Boolean |
| `seed` | Integer from -1 through 4,294,967,295; izzi uses a nonnegative resolved seed |

These fields and limits are adapter facts, not core izzi types.
[Current API parameter reference](https://www.seedance2ai.io/developers)

Mode resolution is deterministic:

```text
approved media references present -> media-to-video + media_urls
else approved title poster present -> image-to-video + image_url
else                              -> text-to-video
```

Do not place raw `asset_id` values or in-application reference labels in the
provider prompt. Resolve asset roles into public URLs only in private runtime
state. The exact outbound request, excluding authentication, receives a canonical
JSON representation and SHA-256. Use a stable key such as
`izzi-v1-<request-sha256>` for `Idempotency-Key`, subject to a one-request
capability probe because the platform does not publish a header length limit.
JSON Canonicalization Scheme is defined by RFC 8785.
[RFC 8785](https://www.rfc-editor.org/rfc/rfc8785.html)

## Submit, poll, download, and resume workflow

The platform currently documents `POST /api/v1/video/seedance2`, Bearer
authentication, an `Idempotency-Key` header, and
`GET /api/v1/tasks/{id}`. Task states are `processing`, `completed`, and
`failed`; completed output includes a video URL, last-frame URL, returned seed,
credit fields, and timestamps.
[Developer documentation](https://www.seedance2ai.io/developers)

The izzi adapter should use this receipt-backed sequence:

1. **Validate.** Check the human document against the existing schema and all
   semantic invariants. Perform no writes and no network calls on validation
   failure.
2. **Resolve.** Produce canonical resolved JSON, one request hash per scene, and
   a review report. No model call is hidden in this step.
3. **Render and snapshot.** Render exact title SVG, PNG poster, transition
   contact sheet, accessibility draft, and hashes for every scene.
4. **Human preflight.** Review the ten-installment grid, prompt diff, reference-rights
   summary, provider-transfer notice, and estimated/unknown spend.
5. **Publish.** A caller-owned publisher makes only approved references
   temporarily reachable by public HTTPS. The provider documentation does not
   describe a general upload endpoint, so izzi must not invent one.
6. **Dry-run.** Emit redacted headers and exact request bodies, with the key and
   URL queries removed. `submit` remains false.
7. **Authorize canary.** Persist a bounded provider-transfer and spend receipt;
   require an explicit `--submit` flag even when a run file permits submission.
8. **Submit once.** Write the request hash and idempotency key to local state
   before the POST. On an uncertain network result, retry the identical body
   with the same key; never create a second paid attempt merely because the
   client lost the response.
9. **Checkpoint.** Persist task ID, HTTP receipt, credit fields, and provider
   status using a temporary file plus atomic rename. Append a compact event to
   JSON Lines.
10. **Poll.** Resume by task ID, use bounded backoff, honor `Retry-After`, and
    checkpoint every status change rather than every harmless poll.
11. **Download promptly.** Write to `*.partial`, verify decodability, dimensions,
    duration, and SHA-256, then atomically rename. Do not overwrite an existing
    different hash.
12. **Review canary.** Save a poster and representative contact sheet, actual
    credit use, provisional accessibility description, and diagnostics. Stop
    until the remaining scene IDs and numeric credit ceiling are approved.
13. **Resume scenes 02–10.** Queue jobs with bounded concurrency. Each scene is
    independent; completed scenes are never resubmitted when another scene
    fails.
14. **Normalize and composite.** Convert accepted source clips to the izzi MKV
    profile, then composite deterministic title and transition material.
15. **Review actual output.** Reconcile descriptions and captions against the
    generated media, review flash/motion, and save a final installment grid.
16. **Accept explicitly.** A named reviewer action writes the final acceptance
    receipt. Silence, inactivity, successful automation, or account billing is
    not acceptance.
17. **Assemble.** Only ten explicitly accepted scene clips may enter the exact
    100-second cut, unless a separately authorized partial-assembly policy is
    used.

### Durable state machine

```text
planned
  -> assets-rendered
  -> preflight-reviewed
  -> assets-published
  -> authorized
  -> submitted
  -> processing
  -> downloaded
  -> conformed
  -> post-composited
  -> accessibility-reviewed
  -> accepted
  -> complete
```

`failed` and `retryable` are side states that retain the last successful state,
the exact attempt ID, and visible diagnostics. A transition never moves backward.
A corrected request creates a new content-addressed attempt and requires a new
idempotency key; an identical retry keeps the old key. Paid-attempt limits are
checked before the POST.

A private checkpoint contains task IDs and any still-live output URLs. An
append-only event stream and immutable receipts contain hashes, timestamps,
redacted commands, tool versions, state transitions, and outcomes. On restart,
the runner acquires a project lock, verifies the resolved-plan hash, loads the
last valid checkpoint, polls known tasks, completes pending downloads, and
continues only authorized states. It refuses to resume if the plan hash, adapter
profile, or existing artifact hash differs.

### Human-reviewable snapshots

Every stage with a visual consequence preserves a review artifact rather than
only a machine status:

```text
review/scene-01/01-title-poster.png
review/scene-01/02-transition-contact-sheet.png
review/scene-01/03-provider-source-contact-sheet.png
review/scene-01/04-conformed-contact-sheet.png
review/scene-01/05-final-contact-sheet.png
review/scene-01/06-accessibility-summary.html
review/project/ten-scene-preflight-grid.png
review/project/ten-scene-final-grid.png
```

Each snapshot receipt records its source artifact hashes and rendering command.
New attempts receive new directories; nothing silently replaces an earlier
review target. A final human gate records the accepted final-grid hash, individual
clip hashes, reviewer role or pseudonymous identifier, timestamp, and notes.

## Error and retry policy

| Observable result | Action |
|---|---|
| Network timeout during POST | Retry the exact body with the same idempotency key; do not count it as a new creative attempt |
| `400 invalid_request` | Stop; correct and revalidate locally; new body means new attempt and key |
| `401 unauthorized` | Stop and notify the operator; never print the key |
| `402 insufficient_credits` | Stop; no automatic purchase or plan upgrade |
| `404 not_found` | Verify account/key ownership and recorded task ID; do not submit a replacement automatically |
| `409 idempotency_conflict` | Compare body hash and persisted state; poll the known task or stop on a mismatched body |
| `429 rate_limited` | Honor `Retry-After`; retain queue order |
| `500 internal_error` | Retry only within the configured attempt policy and preserve the failure receipt |
| `503 service_busy` | Back off and resume; do not fan out concurrent replacements |
| Provider task `failed` | Persist error and credit/refund fields; require policy or human authorization before a new paid attempt |

The API currently publishes a general limit of 30 requests per 60 seconds per
account and a tighter limit of three Seedance reference-media validation requests
per 60 seconds per account and source IP. The v1 recommendation of one in-flight
job is intentionally below both limits and easier to resume and audit.
[API errors and limits](https://www.seedance2ai.io/developers)

## MKV output contract

Provider downloads are inputs, not final deliverables. Preserve the untouched
download by hash, then normalize each accepted scene to:

```text
container: Matroska (.mkv)
video: H.264, yuv420p
canvas: 1080 × 1920 (9:16 vertical master)
rate: 30/1 frames per second
length: 300 frames, exactly 10 seconds
fit: contain and pad; never crop without a named policy
audio: absent by default; preserve or normalize only when explicitly selected
name: {installment_id}-final.mkv
```

The conformer records `ffprobe` JSON before and after conversion, exact command
arguments, tool versions, and hashes. It fails on a different duration rather
than silently trimming, padding, interpolating, changing aspect ratio, or
replacing audio. Any such repair is a named, reviewable policy.

The izzi title poster may be sent as an image-to-video creative reference, but
the provider is not trusted to reproduce exact text. Composite the authoritative
izzi title and Hamonshu/roulette/fade transition in post. The existing landscape
offline studies demonstrate the deterministic Matroska mechanism; the vertical
profile must receive its own visual baseline:

- `examples/seedance2/generated/hamonshu-10s.mkv` (local-only)
- `examples/seedance2/generated/roulette-10s.mkv` (local-only)
- [`Seedance example workflow`](../../../examples/seedance2/README.md)

After all ten clips pass their own gates, concatenate them as an exactly
100-second editorial MKV with chapters. Preserve the individual clips, since
the master is derived evidence rather than a replacement.

Also construct a test-only review merge. Precede every installment with a
two-second, circa-1913 silent-film title card: black background, white text,
the vertical's name, and `Installment NN of 10`. The title cards sit outside
the fixed ten-second installment durations, so the default review merge is
exactly 120 seconds. Record a different title-card duration explicitly rather
than silently changing the total. Preserve each title-card SVG/PNG, installment
poster, per-installment MKV, combined review MKV, frame counts, `ffprobe` JSON,
commands, and hashes.

## Accessibility and WCAG 2.2

WCAG conformance applies to the complete web presentation and its alternatives,
not to a JSON flag or model name. Automated checks can detect some problems, but
the actual generated clip still requires human review.

For each provider scene/installment, the 100-second editorial assembly, and the
title-card review merge:

- retain a meaningful accessible name, provide a time-based alternative for
  prerecorded video-only content, and revise the provisional long description
  after inspecting the actual output;
- provide captions for prerecorded meaningful audio and either audio description
  or a media alternative where visual information is not already conveyed;
- publish a transcript when dialogue, narration, or meaningful sound is present;
- preserve a static poster and reduced-motion path;
- test exact title text at WCAG contrast thresholds and do not use color alone to
  convey meaning;
- analyze full-frame luminance and saturated-red changes, then review for no more
  than three flashes per second or the applicable flash threshold;
- on a web page, do not autoplay audio, and provide pause/stop/hide control when
  automatically starting movement lasts more than five seconds alongside other
  content;
- expose native, keyboard-operable media controls and visible focus; never place
  essential information only inside an inaccessible canvas or video bitmap.

WCAG 2.2 requires prerecorded captions for synchronized audio, an audio
description or media alternative at Level A, prerecorded audio description at
Level AA, and control for qualifying automatically moving content. It also sets
contrast and flash requirements.
[WCAG 2.2](https://www.w3.org/TR/WCAG22/)
[Understanding Three Flashes or Below Threshold](https://www.w3.org/WAI/WCAG22/Understanding/three-flashes-or-below-threshold)
[Understanding Pause, Stop, Hide](https://www.w3.org/WAI/WCAG22/Understanding/pause-stop-hide)

The final accessibility receipt states what was tested and what remains a human
judgment. It must not make a blanket “WCAG 2.2 compliant” claim from contrast,
caption, or flash checks alone.

## Consent, rights, privacy, and retention

Submitting a scene transfers prompts, generation parameters, and any referenced
assets to SixBryan LLC's platform. This is separate from local Codex tracing or
any training-conversion permission. The provider's privacy policy says it
collects account information, usage details, device data, prompts, uploaded
media, and generation parameters. It says creative content is not used to train
its models, while its terms say compliance logs can include timestamps,
prompts/keywords, result fingerprints, and account/payment information.
[Privacy policy](https://www.seedance2ai.io/privacy-policy)
[Terms](https://www.seedance2ai.io/terms-of-use)

Those are statements by the independent platform. Its public pages do not fully
identify every upstream processor, all model-provider retention rules, or a
project-specific deletion schedule. Record those matters as unavailable rather
than extending the platform's no-training statement to unnamed third parties.

Use one concise provider-transfer notice before the canary. It identifies the
provider, listed scenes/assets, purpose, budget ceiling or unknown-cost stop,
retention plan, and whether material will be public temporarily. Retain explicit
approval as a local receipt. This is “minimal notify”: one bounded notice for an
unchanged authorized batch, not ten repetitive prompts. Any new person, asset,
purpose, public-publishing scope, provider, or budget requires a new notice.

Before publication or submission:

- verify ownership or permission for every uploaded asset and generated-content
  use;
- obtain and retain appropriate authorization for an identifiable person's
  likeness or voice; ByteDance's official launch material specifically says real
  human portrait references require identity verification or prior legal
  authorization;
- exclude credentials, private correspondence, unrelated repository content,
  confidential data, and unnecessary personal identifiers;
- do not put a collaborator's email address or account identifier in the creative
  JSON or provider prompt;
- prefer synthetic, licensed, or project-owned references;
- use opaque public asset paths, remove public access after collection, and
  document any provider retention that cannot be verified;
- keep private checkpoints owner-readable and publish only redacted receipts.

The platform terms place responsibility for upload rights and review of generated
content on the user. Commercial use should proceed only when the exact purchased
offering explicitly includes the needed commercial license and the underlying
inputs and intended use are independently cleared. This proposal is technical
workflow guidance, not legal advice.

## Proposed implementation increments

These estimates cover the Seedance delivery track, not the library-wide futures
plan. They assume one experienced C++/tooling developer, existing external tools,
prompt collaborator availability, consent with one minimal bounded notice, and
no provider outage.

| Increment | Deliverable | Estimated focused time |
|---|---|---:|
| S0 | Freeze proposal, schema references, account boundary, and redacted fixtures; no provider call | 0.5–1 day |
| S1 | Authoring validator, deterministic resolver, canonical hashing, dry-run output, and unit tests | 3–5 days |
| S2 | Exact title/poster rendering, contact-sheet snapshots, caller-owned publishing interface, and preflight | 3–5 days |
| S3 | Versioned API client, secret provider, idempotent canary, polling, checkpoints, resume, and mocked failure tests | 4–6 days |
| S4 | Download verification, MKV normalization, title/transition compositing, per-scene manifests, and assembly | 4–7 days |
| S5 | Accessibility sidecars and checks, final review grids, acceptance receipts, documentation, and end-to-end hardening | 4–7 days |

Nominal total: **18.5–31 focused developer-days**, plus provider generation and
human review latency. S0–S2 are fully offline. S3 should ship behind a disabled
submit switch and use mocks before a one-scene paid canary. S4 and S5 can proceed
against locally generated fixture clips while account setup and collaborator
descriptions are still in progress.

## Acceptance conditions

The integration is ready for a real ten-installment run only when all of these are
true:

- both existing example documents still validate unchanged;
- account/operator, terms review date, commercial-use status, and credit source
  are recorded without secrets;
- all ten resolved requests, titles, transitions, and accessibility drafts have
  human-reviewable snapshots and stable hashes;
- the canary proves authentication, idempotency, polling, download, actual credit
  recording, resume, and redaction;
- network timeout, 409, 429, insufficient-credit, failed-task, and interrupted
  download tests do not duplicate paid work or overwrite evidence;
- each accepted source normalizes to 1080×1920, 30 fps, 300-frame H.264/yuv420p
  Matroska with the selected audio policy;
- each final installment has an actual-media accessibility review, `ffprobe` receipt,
  hashes, and visible contact sheet;
- the final ten-installment grid, exact 100-second editorial assembly, and
  default 120-second title-card review merge receive an affirmative human
  acceptance receipt;
- no acceptance is inferred from silence, and no provider transfer extends
  beyond the approved scenes and assets.

Until those conditions are met, `submit` remains an explicit, bounded operator
action rather than a side effect of parsing, rendering, testing, or resuming.
