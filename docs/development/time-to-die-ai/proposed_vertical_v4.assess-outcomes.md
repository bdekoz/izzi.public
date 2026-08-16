---
document_type: assess-outcomes
style: randoma11y-full
specification_version: "3.9"
document_type_modifiers:
  - coda-share
topic: "ai-time-to-die vertical production plan v4 (episodes 01-03, measured walkthrough assessment)"
evidence_boundary: "Measured izzi repository records and portal review issues from 2026-08-13/14 only; provider spend bounded to three Seedance jobs (590 credits total)."
intended_audience: "The project owner and authorized collaborators"
evidence_scope:
  - "izzi repository records: proposal_vertical_v1.md, proposed_vertical_v2.md, proposal_vertical_v3.md (walkthrough), draft-2.md, draft-8.md, explore_futures.20260813.md, render scripts"
  - "situationshipin.space portal catalog and review issues #18-#28"
  - "portal voice bank (29 Kokoro voices) and reference set (26 guilloche/moire/surface-tension images)"
  - "local-object dyad records and the draft-9 KEEP decision mirror"
output_format: pdf
observation_cutoff: "2026-08-14T12:15:00-07:00"
predecessor_or_unavailable:
  predecessor: "proposed_vertical_v3.md"
  review_artifact: "plan-vertical-ai-time-to-die-v3"
  review_surface: "situationshipin.space plan review"
  disposition: "RETAINED_UPDATED by this record"
---

# Assess Outcomes — ai-time-to-die v4 production plan

**Evidence status:** OBSERVED

**Observation cutoff:** 2026-08-14T12:15:00-07:00

## Assessment question and bounded conclusion

**Question:** after the draft-1..9 walkthrough, the published episode masters
01-03, and the portal review vocabulary, is the ai-time-to-die vertical
production-ready as a measured plan, and what does the evidence not
establish?

**Bounded conclusion:** the production plan is ready for review as a
measured, zero-spend-by-default plan. The episode template is locked at
**8 segments / 57 s / 1080x1920@24** by the draft-9 KEEP (issue #28);
episode masters 01-03 are rendered, verified, and published as full-motion
reviews awaiting G4; the canonical corpus is measured at **79.0 s**, bounding
the series to 2-3 episodes. The evidence does **not** establish baseline
promotion (G4 is open), provider reliability beyond three jobs, or a
production path beyond the current canonical corpus without new material.

## Evidence boundary and predecessor continuity

This assessment is bounded to measured izzi repository records and portal
review issues from 2026-08-13/14 only, with provider spend bounded to three
Seedance jobs (590 credits total). No credential, raw provider prompt,
training-conversion transfer, or hidden reasoning is in scope.

**Predecessor continuity:** this record supersedes
`proposed_vertical_v3.md` (portal plan review
`plan-vertical-ai-time-to-die-v3`) for current planning. The v3 record
superseded `proposed_vertical_v2.md`, which superseded
`proposal_vertical_v1.md`; each predecessor is retained in the repository and
the revision ledger.

**Sources in scope:**

| Source | Kind | Evidence status |
| --- | --- | --- |
| `proposal_vertical_v1.md` (composition + transformations) | REPOSITORY | OBSERVED |
| `proposed_vertical_v2.md` (v2 synthesis: template, pilots, orchestration) | REPOSITORY | OBSERVED |
| `proposal_vertical_v3.md` (complete-vertical walkthrough) | REPOSITORY | OBSERVED |
| `draft-2.md`, `draft-8.md` (round plans + implementation records) | REPOSITORY | OBSERVED |
| `explore_futures.20260813.md` (draft-1 session record) | REPOSITORY | OBSERVED |
| `scripts/render-time-to-die-draft*.py`, `scripts/compose-vector-overlay.py` | REPOSITORY | OBSERVED |
| situationshipin.space catalog + issues #18-#28 | PUBLIC | OBSERVED |
| Portal voice bank (29 Kokoro voices; zf_xiaoyi V29 KEEP #27) | PUBLIC | OBSERVED |
| Portal reference set (26 guilloche/moire/surface-tension images) | PUBLIC | OBSERVED |
| `examples.rizal/local-objects/dyads/` + draft-9 KEEP mirror | ARCHIVAL | OBSERVED |
| `resources.rizal/time-to-die-ai/` corpus (28 files, 841,586,395 bytes) | REPOSITORY | OBSERVED |

## Outcomes and contribution

**Demonstrated outcomes (all OBSERVED):**

- **Episode template locked:** 8 segments / 57 s / 1080x1920@24 through a
  nine-round draft loop; draft-9 KEEP (issue #28) with a ~0.1 min audio-only
  remux because the video was byte-identical to draft-8.
- **Complete vertical arc:** proposal → drafts 1-9 → Phase 6 v2 synthesis →
  episode masters 01-03 published full-motion, awaiting G4.
- **izzi vector text overlay:** red-on-white 20 pt two-region bullet text
  from OCR'd BOC `CANONICAL-MARKER` blocks (top BOC-9956, 14 lines, page 38;
  bottom BOC-9942, 9 lines, page 33) via `lane_offset_y`; glyph y-flip
  fixed; verified red text spans y 3..1849 (11.6k px top / 36.3k px bottom)
  and OCR reads both blocks.
- **Mask pipeline:** MediaPipe FaceLandmarker landmarks through the exact
  vertical transform with wild guilloche/moire/surface-tension textures at
  50 % opacity changing every 1 s; eye1-eye2-mouth triangle assignments
  (blue guilloche wild @ eye1, green moire wild @ eye2, moire @ mouth) with
  transparent white-line compositing; PASS at draft-7 (issue #23 path).
- **Voice selection:** 29 Kokoro female stock voices materialized and
  reviewed; **zf_xiaoyi (V29) KEEP (issue #27)**; MeanVC2 converts ~30 s of
  canonical dialogue in ~1-2 min on CPU.
- **Audio layout (issue #26):** canonical segments carry voice-remapped
  dialogue, HLT T5 carries its original audio, shinkansen bed elsewhere;
  verified by RMS 5022 at t=10 s vs 2001 bed-only at t=27 s.
- **Portal capabilities:** full-motion video reviews, audio media kind
  (29-voice bank), image-reference sets (26 images), and the plan-vertical
  review class now hosting this record.

**Contribution framing:** the contribution is a measured production pipeline
and plan for a bounded remix vertical, not an invention claim over the
underlying izzi generators or the Seedance provider. Claims are separated
from inference in the findings ledger.

## Integrated practice and research fit

- **Network topology:** rizal is the control plane (repository, receipts,
  review publication, orchestration dispatch, gate records); ord runs
  izzi/ffmpeg segment renders (T1 masks, T2/T3 overlays, T4 interjections,
  concat/encode); eureka runs MeanVC2 voice conversion and Higgs TTS when
  authorized; the provider is external and gated.
- **Cross-project reuse:** the accepted here-lies-trouble pilot stands in for
  T5 (zero spend by default); guilloche/moire/surface-tension generators and
  the house-style review vocabulary are shared across verticals.
- **Review classes:** the walkthrough exercised proofs, audio,
  image-reference, and now plan-vertical review — making the production plan
  the first reviewable artifact of every future vertical.
- **Document-type fit:** this record is an `assess-outcomes` +
  `coda-share` report in the devastation-pacific-house-style system, with
  `randoma11y-full` styling.

## Software × data × AI methods

**Corrected defects and successful audits (OBSERVED):**

- Glyph y-flip between izzi font coordinates (y-up) and SVG (y-down) was
  only caught by OCR-verifying the rendered overlay; orientation
  verification is now a standing gate.
- The audio state machine was corrected at draft-8: canonical source plays
  its own audio and subtitles during T1a/T1b, with the shinkansen bed
  everywhere else.
- Tiling was abandoned after the provider resolution cap was confirmed
  (requested 1080p → 720x1280; requested 720p → 496x864); contain-fit
  composition is the production rule.

**Methods and measured values:**

| Method / tool | Measured value |
| --- | --- |
| Whole-episode local render (draft-4..8) | 3-5 min |
| Draft-9 audio-only remux | ~0.1 min |
| MeanVC2 canonical remap | ~1-2 min per 30 s window, CPU |
| Provider spend (round total) | 590 credits (110 + 240 + 240) |
| Canonical corpus | 79.0 s → 2-3 episode bound |
| Two-region text verification | y 3..1849; OCR reads BOC-9956 / BOC-9942 |
| Audio verification | RMS 5022 (canonical t=10 s) vs 2001 (bed-only t=27 s) |
| Portal payload | 96 MiB cap; ~86 MiB used |

**Generated artifacts:** canonical MKVs (h264+flac), full-motion review
proxies (360x640 MP4), the 29-voice audio bank, and the 26-image reference
set. Unverified effects are listed in the findings ledger rather than
claimed.

## Limitations, counterevidence, and unresolved questions

- **Provider resolution cap persists:** every requested 1080p job delivered
  720x1280, including the PRO-key submission; requested 720p delivered
  496x864. Counterevidence: none of the three jobs exceeded the cap.
- **PGS subtitle burn** remains `NOT-BURNED` (bitmap track, no local burn
  path); the track is referenced, not dropped.
- **WASM byte-parity** for `izzi-svg-text-overlay.h` is not yet verified
  (status UNAVAILABLE).
- **Portal payload** grows with full-motion reviews; the 96 MiB cap is
  current but requires per-series planning rather than reactive raises.
- **Custom accents** (Asian-American, Southern) require human reference
  recordings; the stock Kokoro bank is sufficient for production selection
  but not for those accents.
- **Higgs TTS** remains gated; MeanVC2 is the production voice path.
- **Episode 03** uses a documented callback plus a provider location, not
  new canonical material — the corpus bound is unchanged.

## Futures and next verification

All futures are INFERRED or MODELED unless stated:

| Future | Status | Smallest credible verification |
| --- | --- | --- |
| G4 episode KEEP for masters 01-03 | INFERRED | Portal KEEP on the episode-master reviews + decision mirror to the local-object store |
| PGS subtitle burn pipeline | INFERRED | Bounded `mkvextract` + ffmpeg `pgssub` test on one canonical window |
| WASM byte-parity for the overlay header | MODELED | Byte-parity check against the emitted overlay header |
| Per-series portal payload plan | INFERRED | Budget model for the next vertical before its first full-motion publish |
| Higgs TTS where authorized | MODELED | One gated Higgs window on eureka with Whisper verification |
| Provider resolution probe on a new key/tier | MODELED | One bounded 5 s 1080p job, geometry + credits recorded |

## Source, claim, run, and revision ledgers

The full ledgers live in the companion manifest
(`proposed_vertical_v4.assess-outcomes.manifest.json`); this section is the
reader-facing summary.

**Runs (OBSERVED):**

| Run | Procedure | Result |
| --- | --- | --- |
| RUN-D1 | `render-time-to-die-draft1.py` + `publish-video-proof.mjs` | 57.0 s draft-1 attempted render published; REVISE #18 |
| RUN-D2 | Draft-2 rework per five findings | Round-2 render; REVISE loop |
| RUN-D8 | Two-region text + audio state machine | y 3..1849 verified; RMS 5022/2001; PGS NOT-BURNED |
| RUN-D9 | Audio-only remux of byte-identical draft-8 | KEEP #28; template locked |
| RUN-MASTERS | Episodes 01-03 renders + portal publish | Full-motion episode-master reviews live, awaiting G4 |

**Revisions (dispositions):**

| Revision | Predecessor | Disposition | Reason |
| --- | --- | --- | --- |
| REV-V2 | `proposal_vertical_v1.md` | SUPERSEDED | v2 synthesis with measured estimates |
| REV-V3 | `proposed_vertical_v2.md` | RETAINED_UPDATED | v3 walkthrough + plan-vertical class |
| REV-V4 | `proposed_vertical_v3.md` | RETAINED_UPDATED | v4 expands to cut-v1 detail as assess-outcomes |
| REV-V4A | `proposed_vertical_v4.assess-outcomes.md` | NEW | This record + PDF |

**Findings:** see manifest findings ledger (F-001 template locked, F-002
corpus bound, F-003 resolution cap, F-004 voice selection, F-005 PGS
NOT-BURNED, F-006 WASM parity UNAVAILABLE, F-007 masters awaiting G4, F-008
payload growth), each with source_ids, counterevidence_or_limit, and
next_verification.

<!-- CODA-SHARE APPENDIX DIVIDER: forced-new-page; bookmark-target -->

## Coda

**Share**

This final appendix summarizes the bounded report and preserves its sharing
boundary. It does not authorize external transfer.

1. [Share summary](#share-summary)
2. [Portable Prompt](#portable-prompt)
3. [Sharing boundary](#sharing-boundary)

<!-- CODA-SHARE CONTENT: forced-new-page -->

### Share summary

The ai-time-to-die walkthrough produced a measured, zero-spend-by-default
production plan: an 8-segment / 57 s episode template locked by the draft-9
KEEP (#28), episode masters 01-03 published full-motion and awaiting G4, a
79.0 s canonical corpus bounding the series to 2-3 episodes, zf_xiaoyi (V29)
voice selection, two-region OCR bullet text, wild 50 % opacity masks, and the
portal plan-vertical review class. Limits: the provider resolution cap
persists, PGS subtitles are NOT-BURNED, and WASM byte-parity is unverified.

### Portable Prompt

**INFERRED / AGENT SYNTHESIS — not transcript**

```text
document_type: assess-outcomes
style: randoma11y-full
document_type_modifiers:
  - coda-share
evidence_scope:
  - izzi repository records: proposal_vertical_v1.md, proposed_vertical_v2.md, proposal_vertical_v3.md (walkthrough), draft-2.md, draft-8.md, explore_futures.20260813.md, render scripts
  - situationshipin.space portal catalog and review issues #18-#28
  - portal voice bank (29 Kokoro voices) and reference set (26 guilloche/moire/surface-tension images)
  - local-object dyad records and the draft-9 KEEP decision mirror
output_format: pdf

Produce ai-time-to-die episodes with the accepted draft-9 pipeline: 8
segments / 57 s / 1080x1920@24, canonical dialogue remapped to zf_xiaoyi via
MeanVC2, izzi red-on-white 20 pt two-region bullet text, wild 50 % opacity
masks, T5 as the HLT stand-in unless provider spend is authorized, shinkansen
bed elsewhere, full-motion portal review, validator passing under the 96 MiB
budget, estimated against the 79 s canonical corpus bound and the measured
3-5 min per-episode render time. Keep every claim inside the declared
evidence boundary, preserve evidence states, and do not promote baselines
without an explicit G4 KEEP.
```

### Sharing boundary

| Field | Record |
|---|---|
| Authority status | AUTHORIZED-BY-PROJECT-OWNER |
| Privacy review status | BOUNDED-REPO-FACTS-ONLY |
| Intended recipient | project owner and authorized collaborators |
| Permitted purpose | review and continuation of ai-time-to-die production |
| Included material | Final report PDF only, if separately authorized |
| Excluded material | Source corpus, secrets, sensitive data, private host objects, hidden reasoning, and unauthorized third-party material |
