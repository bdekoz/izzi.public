# Here Lies Trouble — cut v1 plan (pilot 3 episodes + full-corpus estimate)

Recorded: 2026-08-12 America/Los_Angeles
Status: `PLAN-CUT-V1; PILOT-READY-FOR-AUTHORIZATION; NOT-SHARED`

This is the working plan for the Here Lies Trouble vertical corpus
(implemented project id `here-lies-trouble`; the plan documents that spelled
the project `here-lies-love`, resolved as a visible spelling slip). It starts
with a three-episode pilot, then estimates time and Seedance credit spend for
the full corpus. It supersedes
[`explore_futures_here_lies_trouble_full_vertical.20260812.md`](full-vertical.md)
for current planning; the resume pointer is
[`explore_futures.20260812.restart_here.md`](../sessions/explore_futures.20260812.restart_here.md).

## Corpus definition

Source cuts are the two recordings in the audio source corpus
(`resources/here-lies-trouble/seed-audio-corpus/*.mp3`):

| Recording | Duration | Episodes (165 s each) |
| --- | ---: | --- |
| `here-lies-trouble-1.20260415.mp3` | 1805.8 s | 01–05 (Frogtown beats 01–05) |
| `here-lies-trouble-2.20260415.mp3` | 1422.6 s | 06–10 (five palette locations) |

Ten episodes × 165 s content + 3 s title card = 168 s masters. The corpus uses
roughly half of each recording, leaving revision headroom.

## Pilot: 1–3 episodes (adaptive scope, updated 2026-08-12)

The pilot is now adaptive: **start with episode 01** as the smallest provable
unit, extend to episodes 02–03 only if the balance and review allow. All pilot
episodes share the approved Frogtown keyframe
(`sample-01-frogtown-source.png`), the confirmed three-point corridor, and the
approved rev-2 audio recipe, so the pilot exercises the full chain once before
any further spend. Two pilot plans are on the table:

**Shared pilot foundations** (both plans):

- Stage 0 audio defaults for episodes 02–03 (episode 01 already rendered) from
  the existing spec jobs
  (`build/private/voice-audition/full-episode-random-defaults.spec.json`;
  output dirs exist but are empty). Local, zero spend.
- Full-episode Higgs on eureka (gate
  `full_episode_higgs_reconstruction_authorized` still required), −16 LUFS /
  −1.5 dBTP, Whisper verification.
- Episode masters with the approved madredeus v5 title card, MKV assembly,
  filmstrip + proxy review pages on situationshipin.space.

### Plan 1 — episode 01 only

The smallest provable unit: the full-length scene proof for episode 01 plus
its audio and review. Episode-01 motion canaries are already KEEP'd, so no
new canary spend is needed.

**Approved 2026-08-12** (dyad `DYAD-2026-08-12-PILOT-PRODUCTION-APPROVAL-001`):
requested resolution **1080p**, quality tier pro, new pipeline improvements,
worker topology eureka + ord (rizal stays control plane), unattended run via
`bin/resume-pilot-production.py` with checkpoint/resume after outages. The
pipeline stops at the credit gate after segment A until a total ceiling is
confirmed (observed 1080p rate ≈ 24 cr/s projects the full chain at ≈ 3,960
credits; the 720p-class rate of 11 cr/s projects ≈ 1,815).

1. **Episode-01 scene proof** — 165 s as a last-frame continuity chain of 10 s
   provider segments (17 jobs: 16 × 10 s + 1 × 5 s). If the balance does not
   cover it, a within-balance ≈ 95 s proof (10 jobs) runs first as evidence.
2. **Higgs episode-01 audio** (gate required) — full 165 s dialogue
   regeneration, ambience bed, delivery normalization, Whisper verification.
3. **Master + review** — episode-01 MKV with the madredeus title card,
   filmstrip + proxy pages, human review.

| Plan 1 item | Jobs | Credits |
| --- | ---: | ---: |
| Episode-01 scene proof (full 165 s) | 17 | ≈ 1,815 |
| Episode-01 within-balance proof (≈ 95 s) | 10 | ≈ 1,045 |
| Higgs + master + review | 0 (local) | 0 |
| **Plan 1 total** | **17** | **≈ 1,815** |

Plan 1 time: ≈ 2–3 working days (scene chain ~1 day, Higgs ~0.5–1 day,
masters/review ~0.5 day, plus review cycles).

### Plan 1-2-3 — episodes 01–03

Extends Plan 1 through the three Frogtown beats so the pilot covers
multi-episode continuity (shared keyframe, corridor, audio recipe).

1. **Episode-01 scene proof** — as in Plan 1 (17 jobs; ≈ 95 s within-balance
   variant acceptable as the first gate).
2. **Episodes 02–03 canaries** — 3 × 5 s each (GROUP-WALK, CONVERSATION,
   EXPLORATION) with the pink-spike negative constraint (round 2).
3. **Episodes 02–03 scene chains** — 165 s each (17 jobs per episode).
4. **Higgs audio for all three** (gate required) + masters + reviews.

| Plan 1-2-3 item | Jobs | Credits |
| --- | ---: | ---: |
| Episode-01 scene proof (canaries already KEEP'd) | 17 | ≈ 1,815 |
| Episodes 02–03 each (3 canaries + 165 s scene) | 20 | ≈ 1,980 |
| **Plan 1-2-3 total** | **57** | **≈ 5,775** |

Plan 1-2-3 time: ≈ 1 week of focused work (≈ 3 days provider chains incl.
staging and reviews; ≈ 1–2 days Higgs; ≈ 1 day masters/review).

### Pilot decision ladder (2026-08-12 constraints)

1. **Confirm balance** in the dashboard — recorded spend is 1,427 credits;
   ≈ 1,073 remain of this month's STANDARD release if no rollover (rollover
   and any prior balance are unconfirmed; there is no balance endpoint).
2. **Zero-spend local work first** — Stage 0 audio defaults for episodes
   02–03, then freeze scene candidates; this measures the audio leg and is
   reusable regardless of the visual outcome.
3. **Episode-01 scene proof** — with remaining balance alone: ≈ 95 s scene
   chain (10 jobs, ≈ 1,045 credits) or the full 165 s (17 jobs, ≈ 1,815
   credits) if rollover/balance covers it.
4. **Human review** of the episode-01 proof.
5. **Purchase/upgrade only on approval** — either the $199.90 Professional
   Pack (10,000 credits) or the prorated PRO upgrade; both remain unapproved.
6. **1080p test with the PRO key** — `SEEDANCE2AI_KEY_NAME=seedance2_pro.key`
   on one bounded 5 s pro `1080p` job (placeholder key currently; real value
   required before any test), record geometry + `credits_used`, then choose
   the production resolution.
7. **Extend to episodes 02–03** only if approved and budgeted (≈ 1,980
   credits and 20 jobs each).

### Pilot summary (credits and time)

Observed on `seedance2ai.io` (model `seedance2`, quality `pro`, channel
`standard`, requested `720p`): 5 s image-to-video = 55 credits; 10 s
image-to-video = 110 credits (11 credits per output second). Provider
documentation example: 5 s standard text-to-video = 30 credits. Exact
per-tier pricing is not published; the account operator emailed the provider
and has received no response, so these are the recorded observations, not a
quote.

| Pilot plan | Jobs | Credits | Time |
| --- | ---: | ---: | --- |
| Plan 1 — episode 01 (full 165 s proof) | 17 | ≈ 1,815 | ≈ 2–3 working days |
| Plan 1 — within-balance proof (≈ 95 s) | 10 | ≈ 1,045 | ≈ 1–1.5 working days |
| Plan 1-2-3 — episodes 01–03 | 57 | ≈ 5,775 | ≈ 1 week of focused work |

Timing evidence: a 10 s provider job completes in about 3 minutes of
processing (created→updated), scene segments run sequentially within an
episode (last-frame handoff), and the 30 s Higgs scene took minutes of GPU
time on eureka plus local fitting/mixing. Human review cycles (1–2 rounds per
episode) add the dominant calendar latency.

## Status update: Plan 1 segment A + 1080p (2026-08-12 unattended run)

Segment A of the episode-01 scene chain ran via `bin/resume-pilot-production.py`
from rizal:

- Task `sd2_la1xbvy7rx2Y` (10 s, requested `1080p`, quality `pro`) completed.
- Charge: **240 credits** (24 credits per output second at requested 1080p),
  0 refunded.
- Delivered: H.264, **720×1280**, 24 fps, 10.04 s — the account/key
  resolution cap persists (requested 1080p returns 720×1280; requested 720p
  returns 496×864). The PRO-key probe remains the next resolution test.
- Projected full 165 s chain at 24 cr/s: ≈ **3,960 credits**. The credit gate
  holds after segment A until a ceiling is confirmed (recorded remaining
  balance estimate ≈ 1,073; rollover unconfirmed).
- Last frame extracted, staged, and pushed to situationshipin.space (site
  commit `d833af4`); live URL hash verified `f11e1942…` (one transient
  mismatch was GitHub Pages deploy lag).
- Resume: `python3 bin/resume-pilot-production.py` (idempotent; skips DONE
  segments). Continue past the gate with `--max-total-credits <N>`, or
  `--no-credit-gate` only with an explicit human-confirmed ceiling.

## Full-corpus estimate (10 episodes)

### Seedance credit spend vs STANDARD monthly usage

| Corpus item | Jobs | Credits |
| --- | ---: | ---: |
| Canaries (3 × 5 s) × 10 episodes | 30 | 1,650 |
| Scene segments (165 s × 11 cr/s) × 10 episodes | 170 (10 s chain) | 18,150 |
| **Full corpus** | **200 videos** | **≈ 19,800** |

If the provider accepts 15 s segments, the same content is 140 videos at the
same credit total (165 s × 11 cr/s per episode).

STANDARD annual plan allowance: **2,500 credits/month and up to 125
videos/month**. Comparison:

| Measure | Pilot (3 eps) | Full corpus (10 eps) | STANDARD monthly |
| --- | ---: | ---: | --- |
| Credits | ≈ 5,940 (2.4×) | ≈ 19,800 (7.9×) | 2,500 |
| Videos | 60 (0.5×) | 140–200 (1.1–1.6×) | 125 |

The full corpus is **not within one month of STANDARD usage**: credits are the
binding constraint (≈ 8 STANDARD months), and even the video count exceeds the
125/month figure at 10 s segmentation. It fits the video count of PRO/MAX but
not their monthly credits (PRO 6,000/month ≈ 3.3 months; MAX 13,000/month ≈
1.5 months). At STANDARD pricing (100 credits ≈ $1.00), the full corpus is
≈ $198 of credits; the pilot is ≈ $59.

Resolution acceptance: the user confirmed 720p-class output is fine for this
corpus. Provider returns are 496×864 below-720 creative previews at the
requested `720p`; they remain labeled as such and are never upscaled or called
production 720p.

### Full-corpus time estimate

| Stage | Full corpus (10 episodes) |
| --- | --- |
| Stage 0 audio defaults (episodes 02–10) | ~2 working days |
| Provider motion + scenes (~200 jobs) | ~3–4 weeks incl. staging, queue waits, and per-episode reviews |
| Higgs full-episode audio (10 × 165 s) on eureka | ~1.5–2 weeks |
| Masters, filmstrip, review pages | ~1 week |
| Human review cycles | 1–2 rounds per episode (dominant calendar cost) |
| **Full-corpus completion** | **≈ 4–6 weeks of focused work** |

Pure generation wall-clock is much smaller (≈ 10–15 h provider processing,
≈ 10–20 h local audio + assembly); review gates and per-episode sequencing
dominate the calendar.

## Stage 0 data prep (local, zero spend)

1. Extend `full-episode-random-defaults.spec.json` with the remaining cuts:
   recording 1 slices for episodes 04–05 and recording 2 slices for episodes
   06–10 (arc table in the full-vertical plan), each with deterministic
   assignment seeds.
2. Render MeanVC2 default passes (3 neutral targets, CPU, cached windows).
3. Publish engineering manifests + voice-assignment timelines; run
   `check-local-audio-full-episodes.py` (PASS before promotion).

Pilot note: episode 01 is rendered; episodes 02–03 jobs already exist in the
spec but their output dirs are empty — they are the first Stage 0 work item.
Prerequisite for episodes 06–10: recording 2 has no scene-cut candidate file
yet (`here-lies-trouble-2.20260415.candidates.json` is absent), so its
transcript must first run through the scene-cut pipeline before those jobs
can be frozen in the spec.

## E — Full-episode Higgs reconstruction (plan update)

Extend the approved rev-2 recipe (Higgs windows + ambience + −16 LUFS / −1.5
dBTP) from the 30 s scene to full 165 s episodes:

1. Per episode, align the transcript to the engineering windows (11 per
   slice) using the existing alignment pattern.
2. Generate each window on eureka with Higgs TTS 2 (GPU, ROCm) using the
   expanded voice-reference bank (`build-hlt-voice-reference-bank.py`,
   per-window voice codes V01/V02/V03), temperature 0.8 / top-p 0.9.
3. Trim + pitch-preserving time-fit each window to its slot (rubberband),
   normalize windows (−18 LUFS / −1.5 dBTP), bed the location ambience
   (−32 LUFS, duck 5 dB), and deliver −16 LUFS I / −1.5 dBTP / LRA ≤ 12.
4. Verify intelligibility with local Whisper before publishing any proxy.

Gate: `full_episode_higgs_reconstruction_authorized` remains **false**; this
plan is ready for explicit authorization.

## Location scouting — complete

GitHub review issues `bdekoz/situationshipin.space#10–#15` were received:
KEEP for frogtown, dtla-central-library, moca-geffen, hammer-museum, and the
Sunset pass; Hauser & Wirth carries scouted coordinates and the user
confirmed scouting complete. Issue #15 comment renames the Sunset pass
location from `sunset-scientology-pass` to `hiho-to-runyon-canyon`. Routes
will be folded into `data/locations.json` (situationshipin.space) with
coordinates from the issues.

| Ep | Location | Route points |
| --- | --- | --- |
| 01–05 | Frogtown | start → next → final (34.10555, -118.24362) → (34.10772, -118.24677) → (34.10794, -118.25265); 920 m |
| 06 | DTLA Central Library | 4 points near 34.0504, -118.2554 |
| 07 | MOCA Geffen | 4 points near 34.0502, -118.2388 |
| 08 | Hauser & Wirth | 4 points near 34.0462, -118.2343 |
| 09 | Hammer Museum | 3 points near 34.0593, -118.4429 |
| 10 | Hi-Ho to Runyon Canyon (renamed) | 7 points near 34.1019, -118.3399 |

## Suggested locations and times (recording windows)

Mid-August LA: sunrise ≈ 06:10, sunset ≈ 19:45; golden hour ≈ 06:30–07:30 and
18:45–19:30. Suggestions assume weekday visits and street/public-path access;
it is fine if some locations do not work this time.

**Approved 2026-08-12 by the user** (bounded to the suggested windows below;
fallback policy unchanged).

| Location | Suggested time | Why |
| --- | --- | --- |
| Frogtown corridor (920 m) | Tue–Thu 06:30–08:30 | Soft morning light on the river path, few cyclists; one pass covers beats 01–05 |
| DTLA Central Library | Weekday 10:00–11:30 | Morning light on the Goodhue Building; grounds open, lunch crowds avoided |
| MOCA Geffen | Weekday 11:00–12:00 or 15:00–16:00 (open day) | Plaza + warehouse threshold quietest outside gallery rushes |
| Hauser & Wirth | Weekday 10:00–11:00 | Arts District streets quiet before gallery hours |
| Hammer Museum | Weekday 11:00–12:00 or Friday evening | Wilshire front in morning light; Friday evening open for the sculpture garden |
| Hi-Ho to Runyon Canyon | Weekday 07:00–09:00 | Best canyon-mouth light, least crowd; busy after 09:00 on weekends |

Fallsafe: any route that is closed, private, or crowded this round can be
replaced with an adjacent public path or captured as a street-level keyframe
still; the route registry stays editable until the keyframes are reviewed.

## Pilot spend option (recorded for decision, not authorized)

Recorded Seedance spend to date: **1,427 credits** across 16 completed tasks
(0 refunds). If this all drew from the STANDARD monthly release of 2,500,
the remaining balance this month is **≈ 1,073 credits** (rollover not
confirmed; the platform does not publish a balance endpoint and the emailed
quote is unanswered).

Episode-01 canaries already exist and are KEEP'd, so a 1-episode pilot needs
only the 165 s scene chain: ≈ **1,815 credits** (17 jobs). That exceeds the
≈ 1,073 remaining this month by ≈ 742 credits, so a no-purchase 1-episode
pilot is only feasible if unused credits roll over from a previous month or
additional balance exists. The one-time Professional Pack ($199.90, 10,000
credits, up to 500 videos, one-year validity) would cover the episode-01
pilot plus ≈ 4 more episodes (10,000 − 1,815 ≈ 8,185 → ≈ 4 × 1,980 + headroom);
the full 10-episode corpus needs ≈ 19,800 credits, i.e. a second pack or
~4 more STANDARD monthly releases. Purchase remains a separate decision gated
on human approval of the pilot.

**Purchase test objective (user direction 2026-08-12):** the Professional Pack
purchase is also the test of whether the new spend comes with an API key that
allows **1080p video**. Evidence so far: requested `1080p` pro jobs returned
720×1280 and requested `720p` pro jobs returned 496×864, so output appears
capped below the requested resolution at the account/key level. The platform
docs do not promise a higher resolution tier for the packs, so the test is
empirical: immediately after purchase, submit **one bounded 1080p test job**
(single 5 s image-to-video, pro, requested `1080p`, fixed ceiling), record the
actual returned geometry and `credits_used`, and only then decide the
production resolution for episode segments. The test job is part of the
purchase gate, not a separate authorization.

## STANDARD → PRO switch evaluation (recorded 2026-08-12)

**Feasibility: yes.** The platform FAQ states an upgrade is switching to a
higher tier, done via the **Upgrade** button on the higher-tier pricing card;
the system calculates a prorated price automatically, and the new limits and
billing cycle take effect immediately. The upgrade bill is the new plan price
minus the prorated "remaining value" of the current cycle, calculated from the
remaining unused credits — "you only pay the difference." Unused credits are
preserved (no expiry on subscriptions; one-time packs valid 1 year),
subscriptions can be paused or canceled with access until the end of the
billing period, and multiple subscriptions can be held on one account (credits
stack). Source: seedance2ai.io/pricing FAQ (fetched 2026-08-12).

| | STANDARD (current) | PRO (switch) |
| --- | ---: | ---: |
| Annual price (new-member 50% off) | $299 ($24.90/mo equiv) | $599 ($49.90/mo equiv) |
| Monthly credits | 2,500 | 6,000 |
| Monthly videos (up to) | 125 | 300 |
| Per-credit value | 100 cr ≈ $1.00 | 100 cr ≈ $0.83 |
| Model access | Seedance 2.0 | adds Seedance 2.5; priority queue; HD watermark-free output claim |

**Cost-planning effect:**

- Credits: full corpus ≈ 19,800 → **3.3 months of PRO releases** vs 7.9 months
  of STANDARD; the 200-video count fits inside one PRO month (≤ 300).
- Upgrade proration: with ≈ 1,073 unused credits this month (recorded spend
  1,427 of 2,500), the switch bill is $599 minus the prorated value of that
  remaining balance — exact figure is account-side.
- One-shot corpus comparison: STANDARD + two Professional Packs =
  $299 + $399.80 = **$698.80** for ~50,000 credits; PRO alone = **$599**
  (72,000 credits/year, corpus uses 27.5%) but credits arrive at 6,000/month,
  so an in-window (4–6 week) corpus needs one pack on top: **$798.90**.
- 1080p test: PRO is the credible route to test the resolution cap (premium
  tier; "HD watermark-free output"). Platform docs are contradictory on packs:
  the pricing cards list packs as "includes all features," while the FAQ says
  the $599 Power Pack is credit top-up only (premium stays at entry access)
  and $999+ packs unlock premium. The empirical single-job test therefore
  decides: one bounded 5 s pro `1080p` job after upgrade/purchase, record
  geometry + `credits_used`, then pick the production resolution.

Recommendation: if 1080p is a requirement, prefer the prorated **PRO upgrade**
over the $199.90 pack as the test vehicle (upgrade is prorated and
cancellable; a pack is a sunk one-time purchase). If the PRO test still
returns 720×1280 / 496×864, the cap is platform-wide and the already-recorded
720p-class acceptance stands. Either way, the episode-01 pilot runs first and
the purchase/upgrade stays gated on its human approval.

## Estimate accuracy and audio-first sequencing (recorded 2026-08-12)

Question: would the estimates be more accurate if all audio work were
completed first and the scenes were known?

**Yes for the audio and scene legs, no for the provider legs.** The two
uncertain legs that audio-first work can measure directly:

1. **Audio leg** — per-episode MeanVC2 render time, Higgs generation time on
   eureka for a full 165 s episode, transcript alignment, and Whisper
   verification are currently extrapolated from one 30 s scene and one
   defaults session. Completing the audio work first replaces those ranges
   with measured per-episode times.
2. **Scene leg** — freezing the scene-cut candidates and engineering windows
   per episode fixes the segment counts, the transcript alignment input, and
   the dialogue content for Higgs. Scene-candidate generation is local and
   zero-spend.

Audio-first does **not** tighten the provider legs: the credit rate is
already observed (11 cr/s pro image-to-video), and the resolution-cap and
quote questions can only be answered by a real provider job. It also does not
remove the human-review calendar, which remains the widest uncertainty.

**Sequencing recommendation:** run the local, zero-spend audio work first —
Stage 0 default renders for all remaining episodes (02–10) and Higgs on the
pilot episodes — because that work is reusable regardless of visual outcomes
and it converts the schedule's audio ranges into measured values. Defer
full-corpus Higgs on episodes 04–10 until the visual pilot is approved, so
GPU time is not spent on scenes that might be revised. After Stage 0 + pilot
Higgs, re-publish the per-episode time estimate with measured numbers before
any provider spend.

## Gates and authority

| Gate | State |
| --- | --- |
| `title_card_approval` | **TRUE** — madredeus v5 sample approved (SHA-256 `6a112554…`) and wired into the episode renderer |
| `location_scouting_approval` | **TRUE** — all six routes scouted; rename applied |
| `constraint_update_authorized` | **TRUE** — pink-spike negative constraint recorded and round-2 inputs reassessed |
| `full_episode_higgs_reconstruction_authorized` | **FALSE** — plan ready (section E) |
| `provider_submission_authorized` | **FALSE** — pilot submission requires explicit authorization and exact quote |
| `training_conversion_transfer_authorized` | **FALSE** |

## Evidence anchors

- Resume pointer: `docs/development/sessions/explore_futures.20260812.restart_here.md`
- Full-vertical arc: `docs/development/here-lies-trouble/full-vertical.md`
- Audio workflow: `docs/audio_workflow.md` + `docs/audio_workflow/`
- Higgs: `docs/audio_workflow/tool_higgs.md`, `docs/development/seedance/audio-higgs.md`
- Seedance evidence: `docs/development/archive/seedance.md`,
  `docs/training-conversion/receipts/WI-HLT-FROGTOWN-V2-FULL-LENGTH-SCENE-COMPLETION-2026-08-12.md`
- Pink-spike loop:
  `docs/training-conversion/failure-loops/TC-LOOP-HLT-FROGTOWN-V2-CAT-PINK-SPIKE-001.json`
- Begin dyad: `docs/training-conversion/dyads/DYAD-2026-08-12-SNAPSHOT-BEGIN-001.json`

## Status report — pilot run (2026-08-12, unattended, PRO key)

Snapshot at 2026-08-12 ~18:28 PDT via `bin/resume-pilot-production.py` from
rizal (checkpoint `build/private/pilot-production/state.json`):

| Segment | Status | Task | Credits | Delivered geometry |
| --- | --- | --- | ---: | --- |
| 1 | DONE | `sd2_la1xbvy7rx2Y` | 240 | 720×1280 (standard key) |
| 2 | DONE | `sd2_lDe5PmD2WxZv` | 240 | 720×1280 (pre-upgrade submission, polled with pro key) |
| 3 | DONE | `sd2_3wpxYR8WvELB` | 240 | 720×1280 (**first PRO-key submission**) |
| 4 | PROCESSING | `sd2_…` | 240 charged | pending |

- **Resolution evidence:** every requested `1080p` (9:16) job has delivered
  **720×1280**, including segment 3 submitted with the populated PRO key from
  the upgraded account. The API-path resolution cap persists; vertical 1080p
  means **1080×1920** (not 1920×1080). The accepted 720p-class output stands
  unless a future probe shows otherwise.
- **Credits:** 240 per 10 s segment (24 cr/s); 720 spent across segments 1–3;
  projected full 165 s chain ≈ 3,960 at the observed rate. Ceiling in use:
  5,000; gate cleared for the unattended run.
- **Key:** `seedance2_pro.key` populated (53 bytes, mode 600), same account
  (STANDARD → PRO upgrade); used for segments 3–17.
- **ETA:** provider chain ≈ 1–1.5 h wall-clock from the resume point; pilot
  completion remains gated on worker SSH (eureka/ord unreachable from this
  environment) and the Higgs/master/review stages, ≈ 1 week.
- **Resume:** `SEEDANCE2AI_KEY_NAME=seedance2_pro.key python3
  bin/resume-pilot-production.py --max-total-credits 5000 --no-credit-gate`.
  Snapshot dyad: `DYAD-2026-08-12-PILOT-PRODUCTION-SNAPSHOT-001`.

**Decision 2026-08-12 (user):** continue the pilot as a **720p pilot**.
Remaining segments request `resolution: 720p`; the pipeline stops and reports
if the provider delivers below-720 geometry (e.g., 496×864) so no clip is
silently called 720p. Segment 5 is the first 720p PRO-key submission and
records delivered geometry + credits as the empirical answer on the 720p
request path.

**Decision update 2026-08-12 (user):** no resolution transition — keep going
as the original plan. Config reverted to requested `1080p` for segments 6–17
so the pilot does not mix resolutions. Segment 5 had already been submitted
at requested 720p (110 credits charged; 11 cr/s confirmed on the PRO key) and
is completed/verified in place before the chain continues; if its delivered
geometry is below 720×1280 it is resubmitted at 1080p to keep the master
uniform.

**Vendor reliability + alternatives (2026-08-12):** the provider was assessed
as unreliable on the API path: requested 1080p delivers 720×1280, requested
720p delivers 496×864, per-tier pricing is unpublished, there is no balance
endpoint, and the emailed quote request is unanswered. The approved segment
5–17 redo is **paused** pending a vendor decision; no further Seedance spend
is occurring.

### Alternative video-render providers (pricing basis: 720p-class,
image-to-video, ~10 s continuity chains, no-audio; list/reseller prices,
mid-2026 — verify before purchase)

Corpus basis: 10 episodes × 165 s scene + 150 s canaries = **1,800 s** of
generated video.

| Provider | Approx $/sec | Corpus est. | Notes |
| --- | ---: | ---: | --- |
| **Kling (3.0 / V3)** | $0.08–0.18 | $144–324 | 10 s clips; strong character/motion control; mature API |
| **MiniMax Hailuo 2.3** | $0.05–0.09 | $90–160 | 6–10 s clips; vertical support; cheapest |
| **Luma Ray 2 Flash** | $0.06–0.11 | $108–198 | 10 s clips; simple fal/Luma API |
| Runway Gen-4 (reference) | $0.05–0.12 | $90–216 | 10 s clips + extend chains |
| Veo 3.1 (reference) | $0.15–0.40 | $270–720 | 8 s clips; highest quality, priciest |
| Sora 2 (reference) | $0.10–0.35 | $180–630 | 720p fast tier cheapest |

Current Seedance path for the same corpus: **43,200 credits** at the observed
24 cr/s (1080p-request → 720×1280 delivery) ≈ **$359 at PRO rate** ($432 at
STANDARD rate), plus the prorated PRO upgrade already paid; the 720p-request
path is cheaper (~19,800 credits ≈ $164) but delivers 496×864 — below-720.

Recommendation: if switching, run a bounded pilot with **Kling** or **MiniMax
Hailuo** first (cheapest comparable quality, 10 s vertical image-to-video),
using the same last-frame continuity and the same delivered-geometry gate;
the pipeline's submit/poll/download adapter would need a provider-specific
replacement. Do not commit the full corpus to any vendor before a 3-segment
continuity proof at the claimed resolution.
