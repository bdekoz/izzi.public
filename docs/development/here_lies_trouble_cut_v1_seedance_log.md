# Seedance2ai.io vendor issue log — Here Lies Trouble pilot

Recorded: 2026-08-12 America/Los_Angeles
Status: `VENDOR-AUDIT-EVIDENCE; PILOT-RUN-PAUSED-AT-HIGGS`

This log documents the observed `seedance2ai.io` behavior during the
Here Lies Trouble episode-01 pilot so the claims can be audited against the
recorded task IDs, charges, and delivered geometry. No credentials, prompts,
or raw provider responses appear here.

## Claims

1. **Requested `1080p` (9:16) delivers 720×1280**, not 1080×1920 — on the
   standard key, and again on the PRO key from the upgraded account.
2. **Requested `720p` (9:16) delivers 496×864** — a below-720 preview, not
   720×1280.
3. **Per-tier pricing is unpublished**: charges are returned per job
   (`credits_used`) with no published rate card or quote endpoint, and an
   emailed quote request to support@seedance2ai.io (2026-08-12) received no
   response.
4. **No balance endpoint**: remaining balance cannot be confirmed from the
   API; only the dashboard shows it.
5. **The resolution cap persists after the STANDARD → PRO upgrade** (segment
   3 was the first true PRO-key submission and still delivered 720×1280).

## Episode-01 pilot evidence (2026-08-12, `bin/resume-pilot-production.py`)

All jobs: `mode=image-to-video`, `quality_tier=pro`, `channel=standard`,
`aspect_ratio=9:16`, `generate_audio=false`, `fixed_lens=false`, 10 s
segments (final segment 5 s), requested resolution as listed. Charges are in
credits; 100 credits ≈ $0.83–1.00 depending on plan.

| Seg | Task ID | Requested | Credits | Delivered | Notes |
| --- | --- | --- | ---: | --- | --- |
| 1 | `sd2_la1xbvy7rx2Y` | 1080p | 240 | 720×1280 | standard key; 24 cr/s |
| 2 | `sd2_lDe5PmD2WxZv` | 1080p | 240 | 720×1280 | pre-upgrade submission, polled with PRO key |
| 3 | `sd2_3wpxYR8WvELB` | 1080p | 240 | 720×1280 | first true PRO-key submission; cap persists |
| 4 | `sd2_m6BEGlpXBz7j` | 1080p | 240 | 720×1280 | |
| 5 | `sd2_oekxJj4bgxb2` | **720p** | 110 | **496×864** | lone 720p request; below-720 delivery; charge at 11 cr/s |
| 5 (redo) | `sd2_13GE83opKxaL` | 1080p | 240 | 720×1280 | regenerated to remove the mixed-resolution segment |
| 6–16 | (see below) | 1080p | 240 each | 720×1280 | original tasks reused via idempotency (no duplicate charge); see note |
| 17 | `sd2_neAxLJdnpz4B` | 1080p | 120 | pending | 5 s final segment; 24 cr/s |

Segments 6–16 task IDs (original, reused by the redo):
`sd2_MVYxNjBLrxJd`, `sd2_m6BEGlpXZz7j`, `sd2_bMazy8WwVEYG`,
`sd2_YmezeRg7l5Nw`, `sd2_MVYxNjBLYxJd`, `sd2_k405p62GlEZ1`,
`sd2_8Xwzr6joazo3`, `sd2_kq2xd4Q2nxwe`, `sd2_gL15jkm43xe6`,
`sd2_dBbEwrwbGxea`, `sd2_8e3zk49Zo5DJ`.

**Spend:** recorded per-segment credits total ≈ 3,960 (segments 1–4, 5 redo,
6–16, 17); the sunk original segment-5 charge (110) brings the actual total
to ≈ **4,070 credits** for the episode-01 scene chain. No refunds recorded
(`credits_refunded: 0` on every completion polled).

**Redo note (idempotency):** segments 6–16 re-submissions used identical
request bodies (same start-frame URL string), so the provider returned the
original task IDs rather than regenerating; only segment 5 (different request)
was truly regenerated. The master is therefore resolution-uniform 720×1280,
but the 5→6 content transition retains the original segment-6 clip.

## Historical evidence (earlier rounds)

| Context | Request | Credits | Delivered | Source |
| --- | --- | ---: | --- | --- |
| HLT installment canary | mini / 720p / 10 s text-to-video | 22 | 496×864 | `WI-SEEDANCE-CANARY-01-001` |
| Dual 1080p canaries | standard / 1080p / 10 s text-to-video | 145 each | 720×1280 | run dirs 2026-08-09 |
| Duotone noir canaries | pro / 1080p / 5 s | 120 each | 720×1280 | `TC-LOOP-SEEDANCE-PRO-1080P-RETURNED-720-001` |
| HLT motion canaries | pro / 720p / 5 s | 55 each | 496×864 | `TC-LOOP-HLT-FROGTOWN-V2-MOTION-PRO-720P-RETURNED-496X864-001` |
| Full-length scene jobs | pro / 720p / 10 s | 110 each | 496×864 | `WI-HLT-FROGTOWN-V2-FULL-LENGTH-SCENE-COMPLETION-2026-08-12` |

## Operational observations

- Pricing page and FAQ are internally inconsistent on one-time pack features
  ("includes all features" on pack cards vs "credit top-up only" for the $599
  Power Pack in the FAQ; $999+ packs unlock premium).
- No documented balance, quote, or validation endpoint; the only published
  limits are 30 requests/60 s and 3 reference-media validations/60 s.
- GitHub Pages deploy lag on our staging side caused transient live-hash
  mismatches after pushes; that is our infrastructure, not the vendor's.

## Audit trail

- Plan + status: `docs/development/here_lies_trouble_cut_v1.md`
- Receipt: `docs/training-conversion/receipts/WI-HLT-RESUME-SNAPSHOT-COMPLETION-2026-08-12.md`
- Dyads: `DYAD-2026-08-12-PILOT-PRODUCTION-{APPROVAL,END-001,SNAPSHOT-001,
  SNAPSHOT-002,END-002}-001/002.json`
- Failure loops: `TC-LOOP-SEEDANCE-PRO-1080P-RETURNED-720-001`,
  `TC-LOOP-SEEDANCE-PRO-720P-RETURNED-496X864-001`,
  `TC-LOOP-HLT-FROGTOWN-V2-MOTION-PRO-720P-RETURNED-496X864-001`
- Run checkpoint: `build/private/pilot-production/state.json`
- Staged frames: situationshipin.space commits `d833af4`, `6add356` +
  `full-length-scene-pilot-*` files under `review/provider-inputs/`

Contact: `support@seedance2ai.io` (emailed 2026-08-12; no response as of this
log).
