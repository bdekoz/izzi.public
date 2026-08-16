# Support email draft — seedance2ai.io audit package

Recorded: 2026-08-15 America/Los_Angeles
Status: `DRAFT; NOT-SENT`

Copy the block below into the reply to `support@seedance2ai.io` (or a new
message with the same subject line as the 2026-08-12 quote request). Attach:

1. `explore_futures.20260815.seedance2ai_job_log.csv` — every recorded job
   (task ID, timestamps, requested vs. delivered geometry, credits, refunds).
2. `explore_futures.20260815.vendor_audit_seedance2ai.md` — full audit with
   the same data as tables, the derived rate card, and the evidence trail.

Do **not** attach prompts, credentials, CDN/video URLs, or the repository.

---

**Subject:** Re: izzi rate/quote request + resolution-cap evidence package

Hello,

Following up on our quote request of 2026-08-12: attached is the organized
evidence package for every paid video-generation request we have recorded on
seedance2ai.io (2026-08-08 through 2026-08-14).

Summary:

- 36 accepted jobs plus 1 sunk regeneration attempt (a 720p segment we
  regenerated at 1080p to keep the master uniform).
- 6,087 credits charged total, 0 refunds recorded (`credits_refunded: 0` on
  every completion poll), 0 failed jobs.
- Every request with `resolution: "1080p"` and `aspect_ratio: "9:16"` was
  billed at the 1080p tier but delivered 720x1280 — across `standard` and
  `pro` quality tiers, across both account keys (including the first true
  PRO-key submission, task `sd2_3wpxYR8WvELB`), across text-to-video and
  image-to-video, and across 4/5/10 second durations.
- Every request with `resolution: "720p"` and `aspect_ratio: "9:16"` was
  delivered 496x864, below the requested 720x1280.
- Two `1080p` landscape (16:9) requests were delivered 1280x720.
- Observed charges imply the following effective rates: 2.2 credits/second
  (mini), 14.5 credits/second (standard 1080p), 24 credits/second (pro
  1080p), 11 credits/second (pro 720p). We could not find a published rate
  card to reconcile these against, and the API exposes no balance or quote
  endpoint.

The attached CSV lists every job with task ID, timestamps (UTC), requested
geometry, delivered geometry (ffprobe-verified where the original was
retained), credits charged, and refunds. The attached audit adds the full
tables, the episode-01 pilot chain details, and the idempotency observation
(re-submitting identical request bodies returned the original task IDs —
we saw no duplicate charges).

Questions we would appreciate answers to:

1. What is the maximum native output resolution per quality tier, and why
   does `resolution: "1080p"` return 720x1280 (and `"720p"` return 496x864)
   across tiers and keys?
2. Is 496x864 the intended "720p" output, or a defect? Should clients reject
   it?
3. Can a per-tier rate card (credits per second x resolution x model) and a
   balance/quote endpoint be published? Right now `credits_used` is only
   visible after submission.
4. Are below-spec-resolution deliveries eligible for `credits_refunded`? We
   recorded 0 refunds on all 36 completed jobs.
5. Can you reconcile the 6,087 credits against your ledger, including the
   sunk 110-credit 720p segment (`sd2_oekxJj4bgxb2`) and the idempotent
   6-16 re-submissions?
6. Is returning the original task ID for identical-body re-submissions the
   documented idempotency behavior, and can you confirm those jobs were
   charged once?
7. Can you confirm the account plan and credit balance at each date range in
   the audit timeline (2026-08-08/09, 08-11, 08-12, 08-14)?

We are happy to provide ffprobe receipts or any specific run artifacts on
request. No credentials, prompts, or private asset URLs are included in
this package.

Thank you,
izzi production
