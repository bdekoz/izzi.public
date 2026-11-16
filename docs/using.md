# Using

Back: [izzi](../README.md)

Choose a workflow by the input you have and the result you want to produce.
General covers shared planning, production and review. Visual groups graphics,
motion, print and publication; Audio groups speech, voices and transcription. Their
detailed guides provide dependencies, commands, input contracts, output paths
and verification steps.

## General

| Workflow | Scope | Detailed documentation |
| --- | --- | --- |
| General production | Plan, produce and revise a project through stage-1, stage-2, pilot and release. Covers audio/visual review gates, compute roles and recorded production evidence. | [General workflow](workflows.md), [production loop](workflows.md#workflow), [stage gates](workflows.md#stage-gates) |

## Visual

The [Visual workflow](visual_workflow.md) covers native graphics, animation,
video, print and publication. Its shared [method](visual_workflow/method.md),
[motion grammar](visual_workflow/motion_grammar.md) and
[review proofs](visual_workflow/proofs.md) support the routes below.

### Graphics

| Workflow | Scope | Detailed documentation |
| --- | --- | --- |
| SVG authoring | Compose native shapes, text, patterns, colors, movement and data visualizations in C++20. Use the examples as starting points and the family guides for controls and construction methods. | [Examples](../examples), [patterns](generative_patterns.md), [colors](generative_colors.md), [movement](generative_movement.md), [visualization](generative_visualization.md) |
| Native form studies | Render individual forms and parameter comparisons, check controls and temporal behavior, and export an HTML/SVG/PNG study book, compact PDF or reels. | [Examples workflows](development/examples-workflows.md), [forms commands](building.md#example-forms-and-compositions) |
| Authored compositions | Combine native families into wild scenes and two- or three-family crossovers. Check each component's contribution, motion exits and composition coverage; export a separate book and reels. | [Examples workflows](development/examples-workflows.md), [selected forms and scenes](development/examples-workflows.md#individual-forms-and-scenes) |
| Typed graph boundary | Turn typed radial-orbit or linear-chord data and layout specifications into deterministic, renderer-neutral scenes. Validate authoring JSON and emit resolved geometry, stable IDs, reading order and diagnostics. | [Typed graph boundary](graph.md), [Kusama and chord rendering guide](generative_visualization/kusama-and-chord.md) |
| Portrait engraving | Prepare decoded photographs as tone, support and direction fields; resolve dots, lines, crosshatching, contours and flow at a physical width; emit vector marks as SVG. | [Portrait engraving](portrait.md), [banknote controls](development/20261114_banknote_portrait_api.md) |

### Motion and video

| Workflow | Scope | Detailed documentation |
| --- | --- | --- |
| MOSS appearance and local animation | Follow the recorded Here Lies Trouble route from curated appearance crops and MOSS stills to a film built from saved artwork, poses, masks and speech timing. Covers training provenance, deterministic CPU replay, restart inputs and full-episode review. | [MOSS appearance and local animation](visual_workflow/tool_moss_local.md) |
| Seedance image-to-video | Produce provider-based motion canaries or scene segments from prepared images. Documents the project's adapter entry points, delivery settings and submission gates. | [Seedance provider adapter](visual_workflow/tool_seedance.md) |
| Guilloche reels and title cards | Animate native guilloche, moiré and optical forms; prepare still comparisons and review PDFs; render compact portrait and landscape reels with recorded timing and complete motion exits. | [Guilloche production guide](visual_workflow/tool_guilloche.md), [native guilloche constructions](generative_patterns.md#native-guilloche-patterns) |
| Screen media glitch | Compose ordered film, VCR, TV-broadcast, satellite, receiver and display effects over stationary or moving artwork. Control seeds, clocks and source history, and retain recipes and render receipts. | [Screen media glitch](generative_movement/screen-glitch.md), [film decay](generative_movement/film-decay.md), [video glitch](generative_movement/video-glitch.md), [runnable example](../examples/screen-glitch/main.cc) |
| Supercut | Derive individual or batch review cuts from existing media, with original, 1080p and proxy tiers. Record source hashes, timing, encoding settings and output checksums for repeatable cuts. | [Supercut contracts and checks](supercut.md), [usage examples](visual_workflow.md#supercut-usage) |
| DOCX word title videos | Turn document words into timed titles, with optional paragraph clips and review reels. Layer the titles over background video and derive vertical Instagram exports. | [DOCX word title workflow](visual_workflow/tool_docx_word_titles.md), [runnable examples](../examples/docx-word-titles/README.md) |

### Print and publication

| Workflow | Scope | Detailed documentation |
| --- | --- | --- |
| Portrait and print editions | Combine photograph engraving with masks or whole-photo layouts, guilloche, color and typography. Reproduce holiday-card or anniversary editions as SVG, 600 dpi PNG, galleries and print PDFs, then select designs and inspect physical proofs. | [Portrait and print workflow](visual_workflow/tool_portrait_print.md) |
| Print books, mobile PDFs and EPUB | Package original artwork, text, fonts and source hashes through a publication manifest. Produce portrait, landscape-facing or full-bleed print interiors, reflowable EPUB, ebook covers and optional mobile PDFs; validate and review the exports. | [Publication workflow](visual_workflow/tool_publication.md), [manifest and CLI reference](publication.md), [standalone example](../examples/publication/README.md) |

For screenplay formatting from reviewed transcripts, follow
[Audio → Transcription](#transcription).

## Audio

The [Audio workflow](audio_workflow.md) covers source preparation, speech
production and recognition. Shared guides describe
[preparation and assembly](audio_workflow/etl.md),
[render defaults](audio_workflow/defaults.md) and
[annotation alignment](audio_workflow/annotation_schema.md).

### Speech and voices

| Workflow | Scope | Detailed documentation |
| --- | --- | --- |
| Voice auditions and redubbing | Prepare local synthetic voice references and MeanVC2 transformations, verify complete review sets, record each participant's voice choice and assemble redubbed speech. | [Human voice selection](audio_workflow/human_voice_selection.md), [MeanVC2 and Kokoro](audio_workflow/tool_meanvc2_kokoro.md) |
| Higgs speech generation | Generate speech from text and reference audio through the recorded high-compute local TTS path, then check recognition and audition the results. | [Higgs TTS tool guide](audio_workflow/tool_higgs.md) |

### Transcription

Transcribe speech, review the recognized text and speaker structure, then
use enrichment when screenplay outputs are needed.

| Workflow | Scope | Detailed documentation |
| --- | --- | --- |
| Audio/video transcription | Recognize a local media file with an explicit Whisper model. Select streams, channels or excerpts; preserve raw hypotheses; deliver timestamped TXT, Markdown, JSON, SRT and VTT with hash-bound verification and separate retries. | [Transcription workflow](audio_workflow/tool_transcribe_audio.md), [runnable examples](../examples/audio-transcription/README.md) |
| Transcription enrichment | Format a reviewed, speaker-structured transcript into Final Draft, Fountain and PDF screenplay outputs while preserving dialogue. The Here Lies Trouble route also assesses production fit against a supplied needs document. | [Transcription enrichment](transcription-enrichment_workflow.md), [pipeline and FDX contract](development/transcription-enrichment/workflow.md), [example fixture](../examples/transcription-enrichment/README.md) |

## Setup and reference

Follow [Building and testing](building.md) for repository commands or
[Installing and consuming](installing.md) to use the header-only library in
another CMake project. Workflow-specific guides list the additional Python,
media, model, font and source-asset requirements for their route.

Use the [Doxygen reference](html/index.html) and
[Screen Glitch API reference](html/screen-glitch/index.html) for declarations.
The [development index](development/index.md) links production records and
their technical and human review status.
