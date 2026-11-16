# Audio workflow

Workflow navigation: [Using](using.md) · [General](workflows.md) ·
[Visual](visual_workflow.md).

Audio workflows cover source preparation, speech and voices, and transcription.
Choose a route below; [Using](using.md#audio) describes each route's full scope.
The [general workflow](workflows.md#workflow) supplies shared planning and
review stages. The [development index](development/index.md) links production
status and evidence.

## Choose a production route

### Speech and voices

| Route | Detailed documentation |
| --- | --- |
| Voice auditions and redubbing | [Human voice selection](audio_workflow/human_voice_selection.md), [MeanVC2 transformations and Kokoro references](audio_workflow/tool_meanvc2_kokoro.md) |
| Higgs speech generation | [Recorded high-compute local TTS path](audio_workflow/tool_higgs.md) |

### Transcription

Audio/video transcription recognizes speech from the selected media stream.
Review the recognized text and speaker structure before using enrichment to
produce a screenplay.

| Route | Detailed documentation |
| --- | --- |
| Audio/video transcription | [Local Whisper command, timestamped text/subtitles and verification](audio_workflow/tool_transcribe_audio.md), [runnable examples](../examples/audio-transcription/README.md) |
| Transcription enrichment | [Reviewed transcript to Final Draft, Fountain and PDF](transcription-enrichment_workflow.md), [pipeline and FDX contract](development/transcription-enrichment/workflow.md), [example fixture](../examples/transcription-enrichment/README.md) |

For book interiors, EPUB, ebook covers or mobile PDFs, use
[Visual → Print and publication](visual_workflow.md#print-and-publication).

## Document map

Shared preparation, rendering and annotation references support the routes
above.

| Guide | Scope |
| --- | --- |
| [Directory index](audio_workflow/index.md) | The same Speech and voices and Transcription navigation |
| [Render defaults](audio_workflow/defaults.md) | Targets, seeds and contracts |
| [Audio preparation and assembly](audio_workflow/etl.md) | Source decode, VAD windows, transcripts, hashing and manifests |
| [Voice reference bank](audio_workflow/analyze_expand_voice_reference_bank.md) | Reference-bank analysis and expansion |
| [Annotation schema](audio_workflow/annotation_schema.md) | RESPONSE annotations, VAD alignment and MeanVC2 timing notes |
| [Audio experiments](audio_workflow/audio_workflow_experiments.md) | Historical first-pass material and experiments |

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
- Evidence: receipts and dyads under `examples.rizal/local-objects/legacy/izzi-docs-training-conversion-2026-08-12/`; failure
  loops under `examples.rizal/local-objects/legacy/izzi-docs-training-conversion-2026-08-12/failure-loops/`; trace under
  `examples.rizal/local-objects/legacy/izzi-docs-training-conversion-2026-08-12/training-conversion-2026-08-08-izzi-futures.trace.jsonl`.

The legacy archive paths above are historical locations and were absent during
the November 14 documentation review. Current production receipts are linked
from the [development index](development/index.md).

Historical inventory status (August 12): `FIRST-PASS-SORT; NO-REMOVAL; NOT-SHARED`.

Navigation updated November 16, 2026. Historical paths and observations above
retain their recorded dates.
