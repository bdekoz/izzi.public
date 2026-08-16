# Izzi futures — ai-time-to-die generation experiment (draft plan, IN PROGRESS)

Status: `PUBLISHED-LIVE-2026-08-13; AWAITING-HUMAN-REVIEW (G4)`

## Session-collaboration note (correct behavior)

Recorded per user instruction on 2026-08-13:

- When the user says "pause and wait for further instructions until `resume
  work` is written", stop all task work immediately and yield. Do not start,
  continue, or pre-empt pending tasks, and do not ask repeated clarifying
  questions while paused.
- A short acknowledgment that we are paused is the correct response.
- If an incoming message is visibly truncated (for example, ending in "that
  is composed of:"), treat it as incomplete, note that the payload was cut
  off, and wait for the full text rather than guessing the remainder.
- Resume only on the explicit phrase "resume work", then continue from the
  saved context without restarting finished work.

Snapshot-dyad lifecycle (per session direction 2026-08-13): the dyad snapshot
opened with "snapshot dyad begin" before the first pause, remained open
through the pause/resume cycle, and is closed here with "snapshot dyad end".
The session record is this file plus the draft-1 outputs below; no separate
dyad object was created.

## Decision summary

Run a bounded generation experiment that creates the first draft of the new
vertical project **ai-time-to-die**, publishes it to the
`situationshipin.space` proofs surface (`proofs.html`), and routes the
draft-1 attempted render through a human approval gate before any v2
planning, provider spend, or baseline promotion.

The experiment is deliberately local-first and zero-spend until a human
approves otherwise. The source proposal
[`docs/development/time-to-die-ai/proposal_vertical_v1.md`](proposal_vertical_v1.md)
is authoritative for the project composition (the chat copy of the
composition was truncated on 2026-08-13). This plan is the `plan new project`
review record that proposal asks for, staged behind the existing `proofs for
inspection` portal class.

## Naming and identity

| Role | Exact value |
|---|---|
| Project display name | `ai-time-to-die` |
| Portal family slug | `ai-time-to-die` |
| Resource root (unchanged) | `resources.rizal/time-to-die-ai/` |
| Proposal source | `docs/development/time-to-die-ai/proposal_vertical_v1.md` |
| First artifact id | `ai-time-to-die-draft-1-attempted-render` |
| Review category | `proofs` (existing class; new `plan new project` class deferred) |
| Desired output format | 1080p vertical, 1080×1920 |
| Title lead-in | 2 seconds per episode |

The chat name `ai-time-to-die` and the proposal/resource name `time-to-die-ai`
both appear in scope; the canonical display name is `ai-time-to-die`, and the
existing resource tree keeps its name.

## Verified starting state (2026-08-13)

- Live `https://situationshipin.space/proofs.html` returns HTTP 200 and is
  byte-identical to the local checkout at `/home/bkoz/src/situationshipin.space`
  (verified by `diff`).
- Portal catalog: 260 items, `publication_state: PUBLIC-PROTOTYPE`,
  `human_review_state: UNREVIEWED`, proxy cap 16 MiB, no MKVs in the Pages
  tree, checker + build-receipt scripts pass.
- Publication pipeline exists and is approval-gated:
  `node scripts/publish-video-proof.mjs --approve PROJECT-APPROVED …`
  (catalog entry, review page + manifest, proxy copy, build receipt refresh,
  validator). `--render-proxy` derives a 360×640 MP4 from an MKV with ffmpeg.
- Existing proof pattern to copy:
  `hlt-episode-01-cut-v1` (`build/private/pilot-production/master/hlt-episode-01-pilot.mkv`
  → 360×640 MP4, SHA-256 bound, `UNREVIEWED`/`NOT-PROMOTED`).
- Corpus present: `resources.rizal/time-to-die-ai/` ≈ 803 MiB (Blade Runner 4k
  cuts 4/5, codex screenshots/screengrab, danmu-text-dyads, audit-usage PDFs,
  shinkansen WAVs, just-dreams).
- Local tooling verified: ffmpeg, ffprobe, node, python3; MeanVC2 venv at
  `build/private/voice-audition/runtime/venv/`; Higgs runs on eureka (gated);
  pushes to the portal require `SSH_AUTH_SOCK=/run/user/1000/gcr/ssh`.
- No danmaku/danmu support exists in izzi yet — T3 is new izzi work.

## Project composition (from proposal_vertical_v1.md)

| # | Transformation | Inputs | First-pass output | Executor | Cost class |
|---|---|---|---|---|---|
| T1 | Horizontal→vertical camera-track conversion, female-voice remap ("southern american accent" / "filipino lawyer"), white guilloche face masks changing every 1 s | `seed-video-corpus/Blade.Runner.Final-Cut.4k-cut-4-1080p.1.mkv` (canonical), `…cut-5-1080p.1.mkv` (camera-tracking style), voice reference bank | Vertical 1080×1920 clip with re-voiced dialogue and mask overlay | izzi render + MeanVC2/Higgs audio | Local $0; voice gate G6 |
| T2 | Codex death/rebirth interstitials: gpt-5.6-sol-max cut off → gpt-5.6-luna-medium → deepseek-v4-flash "ai object permanence" track breaking into canonical source | `seed-images/codex/` screenshots + screengrab | Interstitial segments composited into timeline | izzi/ffmpeg | Local $0 |
| T3 | Danmaku text effect: random slow-to-medium left-to-right scroll, 10 s | `seed-images/danmu-text-dyads/0-…`, `1-…`, `2-…` | New izzi text effect + rendered overlay | izzi (new feature) | Local $0 |
| T4 | Audit-report glitch interjections: 1–5 pages, 1 s each, random intervals, chronological (first report at start, last at end); 1 s guilloche/surface-tension transition, fade to white on entry / black on exit; shinkansen audio | `seed-images/audit-usage-reports/*.pdf`, `seed-audio-ambient/20190914-shinkansen-{1,2}.wav` | Interjections composited into timeline | pdftoppm + izzi + ffmpeg | Local $0 |
| T5 | here-lies-love composite test: 3 s top-to-bottom glitch entry from black, 3 s bottom-to-top exit to white; new episodes with maximum location variety; ≤10 s segments; 1080p vertical by tiling two 720p frames + letterbox bars | Existing KEEP'd here-lies-trouble material for draft 1; provider episodes only if gated | Composite test segment | izzi + ffmpeg; provider gated | Local $0; provider gated G5 |

Estimate notes: full episode-count and expense estimates are deliberately
deferred to `proposed_vertical_v2.md` and must be computed from measured
durations (MKV `ffprobe`, interjection counts, guilloche render wall-times)
after draft 1, per the audio-first measurement discipline already recorded in
`here-lies-trouble/cut-v1.md`.

## Staging

### Phase 0 — Snapshot dyad (done)

Session record and correct-behavior note captured above; `resume work`
received on 2026-08-13; plan written to this file.

### Phase 1 — Scope freeze (human gate G1)

Approve: naming table above, draft-1 render bound, and the authoritative
composition. Defaults: draft 1 is one bounded vertical segment, 30–60 s,
1080×1920, containing at least one attempted pass of T1–T4 and a T5 stand-in
from existing KEEP'd here-lies-trouble material. No provider submission.

### Phase 2 — Corpus inventory (local, zero-spend)

- Hash and size every source file (`sha256sum`), probe MKV/WAV/MP4 durations
  and geometry (`ffprobe`), record in
  `outputs/review/feedback/visual/ai-time-to-die/draft-1/manifest.json`.
- Confirm the danmu-text-dyads are plain-text inputs and the PDFs render to
  frames; do not publish any source PDF content beyond the bounded proof.

**DONE 2026-08-13:** 28 files, 841,586,395 bytes, hashed and probed into
`outputs/review/feedback/visual/ai-time-to-die/draft-1/manifest.json`
(videos, audio, PDF page counts, screenshot geometry, text dyads).

### Phase 3 — Draft-1 attempted render (local, zero-spend)

- T1: write camera-track vertical conversion using cut-5 as the tracking
  style, preserving maximum center-frame area; mask faces with per-second
  guilloche-v4 wild variations; re-voice dialogue via MeanVC2 (default) with
  the closest available female voice, documenting accent mismatch until G6
  authorizes a new reference bank or Higgs on eureka.
- T2: cut the codex interstitial story (sol-max death → luna-medium → flash
  rebirth) from screenshots + screengrab; composite as interruptions.
- T3: implement the danmaku effect in izzi (new text-overlay generator with
  WCAG-considerate contrast default) and render 10 s overlays from the three
  dyad texts.
- T4: extract report pages to frames, place 1–5 pages per interjection at
  random intervals in chronological order, apply guilloche/surface-tension
  transitions (white in, black out), mix shinkansen ambience underneath.
- T5: 3 s glitch slide in from black, 3 s glitch slide out to white, tile two
  720p frames + letterbox bars to 1080×1920 per the documented workaround.
- Render canonical `ai-time-to-die-draft-1-attempted-render.mkv` and one
  bounded review proxy in `outputs/review/feedback/visual/ai-time-to-die/draft-1/`.

**DONE 2026-08-13:** 57.0 s, 1080×1920@24, h264 + flac canonical
(102,739,991 B) and 360×640 MP4 review proxy (3,896,007 B, under the 16 MiB
cap). Renderer: `scripts/render-time-to-die-draft1.py`; notes and hashes in
`render-notes.json` and `SHA256SUMS` beside the media. Timeline: title 2 s →
first audit report 3 s → T1 20 s → middle report 3 s → T2 8 s → T1 10 s →
last audit report 3 s → T5 8 s.

Documented draft-1 deviations (also in `render-notes.json`):

- T1 voice remap NOT-ATTEMPTED: MeanVC2 weights and venv are present, but the
  pipeline requires new candidate/selection manifests for the Blade Runner
  audio; deferred to gate G6. Original dialogue retained.
- T5 uses the existing KEEP'd here-lies-trouble pilot as stand-in; provider
  episodes not authorized (G5).
- T3 danmaku rendered with ffmpeg drawtext (10 s left-to-right scroll); the
  izzi-native generator is a follow-up.

### Phase 4 — proofs.html generation (approval-gated)

1. Dry-run the publication plan:
   `node scripts/publish-video-proof.mjs --dry-run --approve PROJECT-APPROVED
   --source <mkv> --render-proxy --artifact-id ai-time-to-die-draft-1-attempted-render
   --family ai-time-to-die --generation-class attempted-render
   --feedback-round draft-1 --review-scope DRAFT-1-ATTEMPTED-RENDER …`
   (from `/home/bkoz/src/situationshipin.space`).
2. Human gate G3: explicit `PROJECT-APPROVED` for publication. No other
   token or implicit approval counts.
3. Run the real publish; it refreshes `data/build-manifest.json` and runs the
   validator. `proofs.html` itself is static — "generation" means the catalog
   (`data/review-items.json`), the exact review page + manifest, and the
   refreshed receipt that `proofs.html` renders.
4. Commit and push the portal via the gcr SSH agent socket.
5. Verify the live handoff URL returns the intended page and the manifest
   resolves to the expected source and derivative hashes.

**Dry-run publication plan (presented, nothing written):**

```json
{
  "artifact_id": "ai-time-to-die-draft-1-attempted-render",
  "title": "ai-time-to-die — Draft 1 — attempted render",
  "family": "ai-time-to-die",
  "generation_class": "attempted-render",
  "review_scope": "DRAFT-1-ATTEMPTED-RENDER",
  "source": "outputs/review/feedback/visual/ai-time-to-die/draft-1/ai-time-to-die-draft-1-attempted-render.mkv",
  "published_path": "review/media/ai-time-to-die/ai-time-to-die-draft-1-attempted-render.review.mp4",
  "dimensions": "360x640",
  "duration_seconds": 57,
  "review_url": "https://situationshipin.space/review/ai-time-to-die-draft-1-attempted-render/",
  "action": "ADD"
}
```

Actual publication still requires the human `PROJECT-APPROVED` token (G3),
then commit + push of the portal via the gcr SSH agent socket.

**PUBLISHED 2026-08-13 (human approval received):** portal commit `14c70a7`
(`cb1f3fe..14c70a7`), catalog now 261 items, validator passed, live
verification returned HTTP 200 for:

- `https://situationshipin.space/review/ai-time-to-die-draft-1-attempted-render/`
- its `manifest.json` (proxy SHA-256 `d60bae38…`; live proxy bytes match)
- `review/media/ai-time-to-die/ai-time-to-die-draft-1-attempted-render.review.mp4`

Phase 5 (human review of draft 1, gate G4) is now open: decision vocabulary
`KEEP` / `KEEP-PARTS` / `REVISE` / `REJECT`, recorded via GitHub issue on
`bdekoz/situationshipin.space` and mirrored to the dyad/local-object store.
No baseline promotion without an explicit human `KEEP`.

### Phase 5 — Human approval of draft 1 (human gate G4)

- Hand off one exact URL in chat:
  `https://situationshipin.space/review/ai-time-to-die-draft-1-attempted-render/`
  plus the `proofs.html` catalog link.
- Review decisions: `KEEP`, `KEEP-PARTS`, `REVISE`, `REJECT` (portal
  vocabulary). Record via GitHub issue on `bdekoz/situationshipin.space` and
  mirror the decision into the dyad/local-object store
  (`examples.rizal/local-objects/legacy/izzi-docs-training-conversion-2026-08-12/`
  per the training-conversion README).
- No baseline promotion without explicit human approval. A `KEEP` unlocks
  Phase 6; a `REVISE` starts round 2 of this same loop.

### Phase 6 — v2 synthesis (post-approval)

After a `KEEP`: synthesize `docs/development/time-to-die-ai/proposed_vertical_v2.md`
with staging, gates, measured estimates (minutes, segment/episode count, run
time, expense, pilot 1 and pilot 1–3 options, orchestration over rizal/ord/
eureka), then generate the devastation-pacific-house-style record
(document-type `special-topics`, `coda-share`, `randoma11y-accent`) as
`proposed_vertical_v2.pdf`.

**DONE 2026-08-14 (draft-9 KEEP, issue #28):**

- `proposed_vertical_v2.md` synthesized with measured values (8 segments /
  57 s episode; 3–5 min local render; 350 provider credits spent; 79.0 s
  canonical corpus bounds the series to 2–3 episodes; zf_xiaoyi voice KEEP).
- `proposed_vertical_v2.special-topics.md` + manifest + 4-page
  `proposed_vertical_v2.pdf` (randoma11y-accent, coda-share) generated via
  the devastation-pacific-house-style pipeline; special-topics and
  coda-share checkers PASS in source and PDF modes.
- G4 KEEP mirrored to the local-object store
  (`examples.rizal/local-objects/legacy/izzi-docs-training-conversion-2026-08-12/decisions/`).

## Human gates with defaults

| Gate | Decision required | Default if not overridden |
|---|---|---|
| G1 | Approve scope/naming/draft-1 bound | Accept this plan's defaults; proceed |
| G2 | Draft-1 render bound (duration, segments) | One 30–60 s 1080×1920 segment |
| G3 | Publication approval | Refuse publication (requires `PROJECT-APPROVED`) |
| G4 | Draft-1 human review | `REVISE` → round 2; never auto-promote |
| G5 | Provider spend (T5 new episodes) | Skip in draft 1; no Seedance submission |
| G6 | Voice remap authority (new references / Higgs) | MeanVC2 closest available voice; accent mismatch documented |
| G7 | New `plan new project` review class in portal | Defer schema change; this plan stays a docs record |
| G8 | v2 doc + house-style PDF generation | Auto-run only after G4 `KEEP` |

## Definition of done

- `check-review-site.mjs` passes after the publish; live `proofs.html` again
  equals local.
- Catalog shows `ai-time-to-die-draft-1-attempted-render` with bound SHA-256,
  bytes, dimensions, duration, and source provenance; canonical MKV absent
  from the Pages tree.
- The exact review URL returns the page and its manifest resolves to the
  expected hashes.
- Human approval decision is recorded (issue + dyad mirror) with the handoff
  URL; `baseline_state` remains `NOT-PROMOTED` until an explicit `KEEP`.

## Risks and open items

- Provider resolution cap: Seedance has returned 720×1280 (requested 1080p)
  and 496×864 (requested 720p); T5's tiling workaround assumes this cap and
  draft 1 avoids provider work entirely.
- 16 MiB proxy cap: keep the draft-1 proxy bounded; use `--render-proxy`
  360×640, and if the render exceeds cap, publish a ten-frame filmstrip
  instead of a video proxy.
- T3 is net-new izzi functionality; scope it as a small text-overlay
  generator with a WCAG-considerate default and verify in `make check`.
- Voice direction ("southern american accent" / "filipino lawyer") needs
  authorized reference voices; default documents the gap rather than
  silently accepting a poor match.
- Push requires the gcr SSH agent socket; a stale default socket is the known
  failure mode.
- The truncated chat composition is superseded by `proposal_vertical_v1.md`;
  if the user's intended composition differs, G1 is the correction point.

## Immediate next step

Human approval of this plan (gate G1 + G3 readiness). On approval, execute
Phases 2–3 locally, then present the dry-run publication plan for the
`PROJECT-APPROVED` token before anything is published.
