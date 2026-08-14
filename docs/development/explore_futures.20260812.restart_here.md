# Resume development here

Recorded: 2026-08-12 America/Los_Angeles (shutdown prep; relocated to
`docs/development/` as `explore_futures.20260812.restart_here.md` on
2026-08-12 by session direction). Session status as of 2026-08-12 evening:
see [`here_lies_trouble_cut_v1.md`](here_lies_trouble_cut_v1.md) for the
current cut-v1 plan (3-episode pilot + full-corpus estimates).

This file is the single starting point for the next development session. It
records where the repositories stand, how to resume the environment, the open
gates, and the next development proposals in priority order.

## Repository state

- Izzi: `main` at `c30c868e` (pushed). Working tree clean at shutdown.
- situationshipin.space: `main` at `35c7748` (pushed) — includes the
  location-scouting scope and the `Response.clone` bugfix.
- All pushes require the gcr SSH agent socket (the default agent socket goes
  stale): prefix push commands with
  `SSH_AUTH_SOCK=/run/user/1000/gcr/ssh`.

## Environment resume notes

- Higgs TTS 2 (full v2) runs on eureka (LAN host `eureka`, user `bkoz`, SSH
  via the gcr agent socket). Runtime: `~/higgs-audio-v2/.venv`; weights in
  `~/higgs-audio-v2/models/`; run material in `~/higgs-audio-v2/run/`.
- ROCm nightly torch (gfx1151) verified; device `Radeon 8060S Graphics`.
- Local audio runtime (MeanVC2): `build/private/voice-audition/runtime/venv/`.
- Speech corpus: `resources/here-lies-trouble/seed-audio-corpus/` (both
  recordings + transcripts). Ambience: `resources/here-lies-trouble/seed-audio-nature/`.

## Approved baselines (do not regress)

- Audio: Higgs rev-2 mix KEEP + **baseline audio production approval**
  (GitHub issue #8). Delivery: −16 LUFS / −1.5 dBTP.
- Visual: Frogtown v2 keyframe accepted; motion canary 01 KEEP (issue #5);
  full-length scene KEEP-PARTS (issue #6) with the cat pink-spike negative
  case now recorded as a negative constraint (round-2 inputs reassessed).
- Route: all six routes scouted KEEP (issues #10–#15); Hauser & Wirth
  coordinates confirmed; `sunset-scientology-pass` renamed to
  `hiho-to-runyon-canyon`. Routes in `data/locations.json`.
- Title card: madredeus v5 approved (SHA-256 `6a112554…`) and wired into the
  episode renderer.
- Milestone 0 ledger: `docs/development/milestone_0_baseline.md`
  (C++ header baseline; `make check-prefixes` passes, `CANONICAL-READY`).

## Open gates (require explicit authorization before spending or rendering)

1. `provider_submission_authorized` — Seedance image-to-video jobs for new
   location keyframes and episode segments (quote per episode required;
   provider emailed, no response as of 2026-08-12).
2. `full_episode_higgs_reconstruction_authorized` — Higgs on full 165 s
   episodes (rev-2 baseline covers the 30 s scene only).
3. `location_scouting_approval` — APPROVED; camera tracks per route are the
   remaining scouting follow-up.
4. `title_card_approval` — APPROVED and wired.
5. `constraint_update_authorized` — cat pink-spike negative constraint
   recorded; round-2 inputs reassessed.
6. `training_conversion_transfer_authorized` — remains false.

## Next development proposals

### A. Pilot: episodes 01–03 (recommended next)

Render Stage 0 audio defaults for episodes 02–03 (jobs exist, outputs empty);
then the pilot chain in
[`here_lies_trouble_cut_v1.md`](here_lies_trouble_cut_v1.md): canaries with
the pink-spike negative constraint, 165 s scene chain, full-episode Higgs on
eureka, masters + review pages. Local Stage 0 work is zero-spend.

### B. Camera tracks per scouted route

Scouting is KEEP'd; compute bearing/distance/pace per segment from the routes
now in `data/locations.json` and fold the camera tracks into the motion-canary
specs. See `docs/visual_workflow/camera_tracking_routes.md`.

### C. (Done) Title card wiring

Approved sample `title-card-v5-madredeus.png` (SHA-256 `6a112554…`) wired into
`scripts/render-here-lies-trouble-local-episode.py` via
`scripts/render-hlt-title-card-madredeus.py`.

### D. (Done) Cat pink-spike constraint update

Audited all six returned round-01 clips (`scripts/audit-hlt-frogtown-v2-pink-spike.py`);
candidate pink-spike found in motion-canary-01 (≈ 3.75–5.0 s); negative
constraint added to all canary specs; round-2 prompt templates reassessed.

### E. Full-episode Higgs reconstruction

Extend the approved rev-2 audio recipe (Higgs windows + ambience + −16 LUFS)
to full 165 s episodes using the expanded voice-reference bank; verify with
local Whisper before publication. Plan: section E of the cut-v1 plan.
Requires gate 2.

### F. Provider video proofs per episode (pilot first)

Location keyframes → three motion canaries → full-length scene segments →
episode MKV with the title card. Requires gate 1; obtain exact quotes before
submission (provider emailed, no response).

### G. Generative patterns second pass

Add dedicated tests for hamonshu, harmonograph, and grignani; decide whether
the radial family should expose named entry points; close the open items in
`docs/generative_patterns/*.md`.

## Documentation map

- Planning: `docs/development/` (explore_futures logs + milestone baseline)
- Audio workflow: `docs/audio_workflow.md` + `docs/audio_workflow/`
- Visual workflow: `docs/visual_workflow.md` + `docs/visual_workflow/`
- Pattern families: `docs/generative_patterns.md` + `docs/generative_patterns/`
- Evidence: `docs/training-conversion/` (receipts, dyads, failure loops)
- Full vertical plan:
  `docs/development/explore_futures_here_lies_trouble_full_vertical.20260812.md`;
  current cut-v1 plan:
  `docs/development/here_lies_trouble_cut_v1.md`

Status: `SHUTDOWN-PREP; RESUME-POINTER; NOT-SHARED`
