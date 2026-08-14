# Here Lies Trouble full vertical — episode arc proofs (video + audio)

Recorded: 2026-08-12 America/Los_Angeles
Status: `PLAN-DRAFT; NO-PROVIDER-SPEND; NOT-SHARED`

Session update 2026-08-12 evening: superseded for current planning by
[`here_lies_trouble_cut_v1.md`](here_lies_trouble_cut_v1.md) (pilot 01–03 +
full-corpus estimates). This document remains the arc table and evidence
anchor. Location scouting is KEEP'd for all six routes (issues #10–#15,
`sunset-scientology-pass` renamed `hiho-to-runyon-canyon`); the madredeus
title card is approved and wired; the cat pink-spike negative constraint is
recorded with round-2 inputs reassessed.

This plan completes video and audio proofs for the full Here Lies Trouble
episode arc that walks the planned route, using both source recordings
(`here-lies-trouble-1.20260415.mp3` and `here-lies-trouble-2.20260415.mp3`),
and plots the locations for the complete vertical series.

## Arc definition

Ten episodes, one per location beat. Frogtown carries five beats (the
three-point corridor from the location-scouting scope), and the five palette
locations carry one episode each:

| Ep | Location | Beat / segment | Source recording | Source slice (ms) | Slice status |
| --- | --- | --- | --- | --- | --- |
| 01 | Frogtown | beat-01 bike-path entrance | recording 1 | 31 000–196 000 | existing (approved audio windows) |
| 02 | Frogtown | beat-02 river overlook | recording 1 | 196 000–342 000 | existing defaults (146 s, boundary-trimmed) |
| 03 | Frogtown | beat-03 underpass / bridge shadow | recording 1 | 342 000–500 000 | existing defaults (158 s, boundary-trimmed) |
| 04 | Frogtown | beat-04 riparian planting | recording 1 | 500 000–665 000 | new — window + defaults needed |
| 05 | Frogtown | beat-05 final shared river view | recording 1 | 665 000–830 000 | new — window + defaults needed |
| 06 | DTLA — Los Angeles Central Library | entry + reading room exterior | recording 2 | 0–165 000 | new — window + defaults needed |
| 07 | MOCA Geffen Contemporary | plaza + gallery threshold | recording 2 | 165 000–330 000 | new |
| 08 | Hauser & Wirth | Arts District rail-adjacent block | recording 2 | 330 000–495 000 | new |
| 09 | Hammer Museum | Wilshire front + sculpture garden | recording 2 | 495 000–660 000 | new |
| 10 | Hi-Ho to Runyon Canyon (renamed from Sunset Boulevard pass) | canyon-mouth exterior pass | recording 2 | 660 000–825 000 | new |

Corpus headroom: recording 1 (1806 s) supports ~10 × 165 s slices; recording 2
(1423 s) supports ~8. The ten-episode arc uses roughly half the corpus, leaving
room for revisions and a second arc.

## Location and route plot

The planned route is plotted in the location-scouting scope
(`location.html`). Frogtown is confirmed; the palette locations carry
unverified candidate coordinates to be confirmed during scouting:

| Location | Candidate coordinates (UNVERIFIED — confirm in scouting) |
| --- | --- |
| Frogtown start | 34.1055476, -118.2436172 (confirmed) |
| Frogtown next | 34.1077176, -118.2467679 (confirmed) |
| Frogtown final | 34.1079369, -118.2526539 (confirmed) |
| DTLA Central Library | ≈ 34.0505, -118.2541 (candidate) |
| MOCA Geffen | ≈ 34.0502, -118.2388 (candidate) |
| Hauser & Wirth | ≈ 34.0347, -118.2323 (candidate) |
| Hammer Museum | ≈ 34.0597, -118.4440 (candidate) |
| Sunset pass (4810 Sunset Blvd) | ≈ 34.0990, -118.2960 (candidate) |

Suggested walking order: Frogtown (E01–E05, 920 m corridor; start→next 377 m,
next→final 542 m, multi-episode at walking pace) → DTLA Central Library →
MOCA Geffen → Hauser & Wirth → Hammer Museum → Hi-Ho to Runyon Canyon. The route is
stylized across episodes, so the sequence is editorial, not strictly
geodesic. Frogtown candidate points stay editable in the browser until the
scouting review is KEEP.

## Per-episode proof chain (video)

Each episode follows the approved Frogtown v2 pattern:

1. Location keyframe — one accepted source frame per location (style-frames
   with the Neon Addict aesthetic; Frogtown frame already accepted:
   `sample-01-frogtown-source.png`).
2. Three silent motion canaries (5 s, 720p image-to-video): GROUP-WALK,
   CONVERSATION-AND-LISTENER-REACTIONS, LOCATION-EXPLORATION, each with the
   location's anchor wording and the Blade Runner-derived motion grammar
   (local-only seeds; no film names in prompts).
3. Full-length scene in segments (provider image-to-video; 165 s content at
   30 fps, 496×864 delivered) — assembled into the episode MKV.
4. Title card per episode: guilloche style, light gray background, madredeus
   type, episode numeral + `here/nlies/trouble`
   (`scripts/render-hlt-title-card-madredeus.py`).
5. Filmstrip + proxy review pages on situationshipin.space (established
   pattern: `review/...-filmstrip/` and `review/...-proxy/`).

## Per-episode proof chain (audio)

1. Engineering windows: 11 per slice via the local VAD windowing pipeline
   (nominal 15 s, low-energy boundaries).
2. MeanVC2 random-default transformation pass (all voice codes) as the
   engineering baseline.
3. Higgs TTS 2 zero-shot dialogue regeneration with the expanded
   voice-reference bank (per-window voice codes V01/V02/V03), mixed with the
   location ambience bed and delivery normalization (−16 LUFS / −1.5 dBTP,
   approved baseline recipe).
4. Local Whisper intelligibility verification before publication; review
   proxy published with the video.

## Gates and authorizations (all currently NOT authorized)

1. `provider_submission_authorized` — Seedance image-to-video jobs for the
   new location keyframes and episode segments. Estimate per episode:
   3 canaries ≈ 180 credits (5 s 720p at prior 60-credit rate) plus the
   165 s scene segments (≈ 11 × 10 s segments; exact quote required before
   submission, hard-stop at the quote gate). 10 episodes ≈ 1 800 credits
   canaries + scene segments — confirm quotes per episode.
2. `full_episode_higgs_reconstruction_authorized` — the rev-2 baseline covers
   the 30 s scene; applying Higgs to full 165 s episodes is a new scope.
3. `location_scouting_approval` — KEEP on the plotted route per location
   (Frogtown route already drafted in `data/locations.json`).
4. `title_card_approval` — KEEP on the madredeus guilloche card before it is
   wired into episode renders.
5. `constraint_update_authorized` — cat pink-spike negative case recorded as
   a negative constraint (round-2 inputs reassessed).

## Suggested stages

- Stage 0 — Data: window recording 2 (episodes 06–10) and recording 1 slices
  for episodes 04–05; run MeanVC2 default passes; publish engineering
  manifests. (Local, zero spend.)
- Stage 1 — Scouting: confirm candidate coordinates in the location map and
  KEEP the route; generate location keyframes for review.
- Stage 2 — Visual: per-location motion canaries → full-length scene segments
  (provider gate per episode) → episode MKV with title card.
- Stage 3 — Audio: per-episode Higgs dialogue regeneration + ambience mix +
  Whisper verification (eureka, zero provider spend).
- Stage 4 — Review: publish filmstrip + proxy pages per episode; collect
  GitHub reviews; record receipts and dyads per episode; promote the
  accepted baseline per episode.

## Evidence anchors

- Approved audio baseline: `DYAD-2026-08-12-HLT-HIGGS-PROXY-R2-REVIEW-RECEIVED-001.json`
- Frogtown spec and route beats: `examples/seedance2/here-lies-trouble-episode-01-frogtown.v2.json`
- Audio defaults pipeline: `build/private/voice-audition/full-episode-random-defaults.spec.json`
- Reference bank: `docs/audio_workflow/analyze_expand_voice_reference_bank.md`
- Higgs tool: `docs/audio_workflow/tool_higgs.md`
- Location scope: `data/locations.json` on situationshipin.space
