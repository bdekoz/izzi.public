---
document_type: special-topics
style: randoma11y-accent
specification_version: "3.9"
topic: "ai-time-to-die vertical project v2 synthesis (Phase 6, post draft-9 KEEP)"
evidence_boundary: "Measured izzi repository records and portal review issues from 2026-08-13/14 only; provider spend bounded to two Seedance jobs."
intended_audience: "The project owner and authorized collaborators"
output_format: pdf
lang: en-US
---

::: {.page data-folio="PAGE 1 / 4"}

# Special Topics — ai-time-to-die v2 synthesis

**Evidence status:** OBSERVED

**Observation cutoff:** 2026-08-14T04:00:00-07:00

## Topic and current finding

Draft 9 of the 57-second ai-time-to-die vertical episode was reviewed with a
KEEP on the portal (issue #28). The v2 synthesis converts the measured
draft-1..9 work into staging, gates, estimates, pilot options, and
orchestration for producing full episodes. The canonical source corpus is
79.0 seconds, which bounds the series to 2-3 episodes before new material is
added.

## Evidence and analysis

Measured episode template: 8 segments / 57 s at 1080x1920@24. Whole-episode
local render wall time is 3-5 minutes (draft-4..8); draft-9 was a ~0.1-minute
audio-only remux because the video was byte-identical to draft-8. Provider
spend measured 350 credits total (110 + 240). Voice remap: MeanVC2 with the
selected Kokoro female voice zf_xiaoyi (V29, KEEP issue #27) converts the
canonical dialogue locally in 1-2 minutes. T5 reuses the here-lies-trouble
pilot with original audio; provider-gated episodes cost 110-240 credits each
when authorized.

## Method, limits, and sources

Method: measured renders over nine drafts, portal review vocabulary
(KEEP / KEEP-PARTS / REVISE / REJECT), and the local-object dyad records.
Limits: the provider resolution cap persists (1080p requested, 720p or
496x864 returned); PGS subtitle burn remains NOT-BURNED; WASM byte-parity is
not yet verified. Sources: izzi repository, situationshipin.space review
issues #18-#28, and this conversation thread.

::::

<!-- CODA APPENDIX DIVIDER: forced-new-page; recto-preferred; bookmark-target -->
<!-- CODA-SHARE APPENDIX DIVIDER: forced-new-page; bookmark-target -->

::: {.page data-folio="PAGE 2 / 4"}

## Coda

**Simplified Prompt**

Preserves the shortest self-contained assistant response that can be reused
to repeat or adapt this special-topic report.

1. [Simplified Prompt](#simplified-prompt)

**Share**

2. [Share summary](#share-summary)
3. [Aggregated portable prompt](#aggregated-portable-prompt)
4. [Sharing boundary](#sharing-boundary)
5. [Use AI to change and add your own reassessment info](#ai-guided-reassessment)

::::

<!-- CODA CONTENT: forced-new-page -->
<!-- CODA-SHARE CONTENT: forced-new-page -->

::: {.page data-folio="PAGE 3 / 4"}

### Simplified Prompt

**Response clip**

| Field | Value |
|---|---|
| Clip ID | ST-CODA-AITTD-V2-001 |
| Source role | assistant |
| Source reference | izzi Codex thread 2026-08-14 (Phase 6 direction) |
| Date or date status | 2026-08-14 |
| Boundary start | 2026-08-14T03:00:00-07:00 |
| Boundary end | 2026-08-14T04:15:00-07:00 |
| Clip status | EXACT |
| Omissions or redactions | NONE |
| Evidence status | OBSERVED |

```text
document_type: special-topics
style: randoma11y-accent
topic: ai-time-to-die vertical project v2 synthesis (Phase 6, post draft-9 KEEP)
evidence_boundary: Measured izzi repository records and portal review issues from 2026-08-13/14 only; provider spend bounded to two Seedance jobs.
intended_audience: The project owner and authorized collaborators
output_format: pdf

Phase 6 is unlocked by the draft-9 KEEP (issue #28): synthesize
proposed_vertical_v2.md with staging, gates, measured estimates, pilot 1 and
pilot 1-3 options, and rizal/ord/eureka orchestration, then generate the
devastation-pacific-house-style special-topics record (randoma11y-accent,
coda-share) as proposed_vertical_v2.pdf.
```

::::

::: {.page data-folio="PAGE 4 / 4"}

### Share summary

The draft-9 KEEP closes the ai-time-to-die draft loop: the 57 s episode
template, zf_xiaoyi canonical voice remap, two-region bullet text, wild
masks, and full-motion review format are all accepted. v2 production is a
measured, zero-spend-by-default pipeline over a 79 s canonical corpus.

### Aggregated portable prompt

**Evidence status:** INFERRED — aggregated portable prompt, not transcript.

```text
document_type: special-topics
style: randoma11y-accent
document_type_modifiers:
- coda-share
evidence_scope:
- measured izzi repository records
- portal review issues #18-#28
output_format: pdf

Produce ai-time-to-die episodes with the accepted draft-9 pipeline: 8
segments / 57 s / 1080x1920@24, canonical dialogue remapped to zf_xiaoyi via
MeanVC2, izzi red-on-white 20 pt two-region bullet text, wild 50% opacity
masks, T5 as the HLT stand-in unless provider spend is authorized, shinkansen
bed elsewhere, full-motion portal review, validator passing under the 80 MiB
budget, estimated against the 79 s canonical corpus bound and the measured
3-5 min per-episode render time.
```

### Sharing boundary

This record may be shared with the project owner and authorized
collaborators. Provider prompts and spend receipts remain private and gated.

### Use AI to change and add your own reassessment info

Take the aggregated portable prompt from this appendix and combine it with
the bundled Devastation Pacific skill file to run your own bounded review.

::::
