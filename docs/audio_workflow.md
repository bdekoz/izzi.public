# Audio workflow

Parent workflow: [Workflows](workflows.md).

First-pass index for the Here Lies Trouble audio development workflow. This is
a pointer index only: nothing was moved, removed, or rewritten. Each entry
lists where the working material currently lives so future passes can split or
merge without losing evidence.

## Document map

| File | Bucket |
| --- | --- |
| `docs/audio_workflow/index.md` | Directory entry page mirroring this index |
| `docs/audio_workflow/defaults.md` | Render defaults, targets, seeds, and contracts |
| `docs/audio_workflow/etl.md` | Source decode, VAD windows, transcripts, hashing, manifests |
| `docs/audio_workflow/analyze_expand_voice_reference_bank.md` | Reference-bank analysis and expansion |
| `docs/audio_workflow/tool_higgs.md` | Higgs TTS 2 (high compute; eureka GPU host) |
| `docs/audio_workflow/tool_meanvc2_kokoro.md` | Low-compute local pass: MeanVC2 transformation + Kokoro target voices |
| `docs/audio_workflow/annotation_schema.md` | RESPONSE annotations, VAD alignment, MeanVC2 timing notes |
| `docs/audio_workflow/audio_workflow_experiments.md` | First-pass unclassified material and experiments |

## Current working material (2026-08-12)

- Living docs: `docs/audio_workflow/human_voice_selection.md`,
  `docs/development/seedance/audio.md`,
  `docs/development/seedance/test-plan.md`,
  `docs/development/seedance/audio-higgs.md`,
  `docs/development/seedance/plan.md`.
- Scripts: `scripts/izzi-audio-excerpt-select.py`,
  `scripts/izzi-meanvc-audition-batch.py`, `scripts/izzi-meanvc-full-episode.py`,
  `scripts/izzi-human-voice-select.py`, `scripts/render-audio-audition-waveforms.py`,
  `scripts/render-hlt-frogtown-v2-audio-canary.py`,
  `scripts/build-hlt-voice-reference-bank.py`,
  `scripts/generate-hlt-frogtown-v2-higgs-audio.py`,
  `scripts/build-hlt-frogtown-v2-higgs-30s-review.py`,
  `scripts/check-local-audio-auditions.py`,
  `scripts/check-local-audio-full-episodes.py`,
  `scripts/check-response-annotations.py`.
- Evidence: receipts and dyads under `docs/training-conversion/`; failure
  loops under `docs/training-conversion/failure-loops/`; trace under
  `docs/training-conversion/training-conversion-2026-08-08-izzi-futures.trace.jsonl`.

Status: `FIRST-PASS-SORT; NO-REMOVAL; NOT-SHARED`
