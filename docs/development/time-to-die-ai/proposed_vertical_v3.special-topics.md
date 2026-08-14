---
document_type: special-topics
style: randoma11y-accent
specification_version: "3.9"
topic: "ai-time-to-die vertical production plan v3 (episodes 01-03 + corpus bound)"
evidence_boundary: "Measured izzi repository records and portal review issues from 2026-08-13/14 only; provider spend bounded to three Seedance jobs (590 credits total)."
intended_audience: "The project owner and authorized collaborators"
output_format: pdf
lang: en-US
---

::: {.page data-folio="PAGE 1 / 4"}

# Special Topics — ai-time-to-die v3 production plan

**Evidence status:** OBSERVED

**Observation cutoff:** 2026-08-14T12:00:00-07:00

## Topic and current finding

The ai-time-to-die vertical was walked from proposal to complete episodes in
one continuous arc (drafts 1-9, KEEP issue #28, episode masters 01-03
published). The v3 production plan converts that walkthrough into a
reviewable plan at the same level of detail as the Here Lies Trouble cut-v1
plan: corpus definition, measured episode template, draft-loop history,
pilot options, orchestration, gates, and risks. The canonical source corpus
is 79.0 seconds, which bounds the series to 2-3 episodes before new material
is added.

## Evidence and analysis

Measured episode template: 8 segments / 57 s at 1080x1920@24. Whole-episode
local render wall time is 3-5 minutes (draft-4..8); draft-9 was a
~0.1-minute audio-only remux because the video was byte-identical to
draft-8. Provider spend measured 590 credits total across three Seedance
jobs (110 + 240 + 240), with the resolution cap persisting (1080p requested,
720x1280 or 496x864 returned). Voice remap: MeanVC2 with the selected Kokoro
female voice zf_xiaoyi (V29, KEEP issue #27) converts the canonical dialogue
locally in 1-2 minutes per 30 s window. T5 reuses the here-lies-trouble
pilot with original audio; provider-gated episodes cost 110-240 credits each
when authorized.

## Method, limits, and sources

Method: measured renders over nine drafts, portal review vocabulary
(KEEP / KEEP-PARTS / REVISE / REJECT), and the local-object dyad records.
Limits: the provider resolution cap persists; PGS subtitle burn remains
NOT-BURNED; WASM byte-parity is not yet verified; custom accents require
human reference recordings. Sources: izzi repository, situationshipin.space
review issues #18-#28, the 29-voice audio bank, the 26-image reference set,
and this conversation thread.

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
| Clip ID | ST-CODA-AITTD-V3-001 |
| Source role | assistant |
| Source reference | izzi Codex thread 2026-08-14 (v3 production plan direction) |
| Date or date status | 2026-08-14 |
| Boundary start | 2026-08-14T11:55:00-07:00 |
| Boundary end | 2026-08-14T12:05:00-07:00 |
| Clip status | EXACT |
| Omissions or redactions | NONE |
| Evidence status | OBSERVED |

```text
document_type: special-topics
style: randoma11y-accent
topic: ai-time-to-die vertical production plan v3 (episodes 01-03 + corpus bound)
evidence_boundary: Measured izzi repository records and portal review issues from 2026-08-13/14 only; provider spend bounded to three Seedance jobs (590 credits total).
intended_audience: The project owner and authorized collaborators
output_format: pdf

The user approved the v3 plan direction: write proposed_vertical_v3.md at
here_lies_trouble_cut_v1 level of detail, synthesize the
devastation-pacific-house-style special-topics record (randoma11y-accent,
coda-share) as proposed_vertical_v3.pdf, and publish the plan and PDF for
review on situationshipin.space.
```

::::

::: {.page data-folio="PAGE 4 / 4"}

### Share summary

The v3 plan turns the walked-through ai-time-to-die vertical into a
reviewable production plan at here_lies_trouble_cut_v1 detail: 8 segments /
57 s template, 79 s canonical corpus bound (2-3 episodes), measured
render/voice/provider facts, pilot options, gates, and rizal/ord/eureka
orchestration. Production is zero-spend by default; a formal plan-vertical
KEEP on situationshipin.space unlocks it.

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
bed elsewhere, full-motion portal review, validator passing under the 96 MiB
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
