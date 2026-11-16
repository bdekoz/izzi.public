# Visual workflow

Workflow navigation: [Using](using.md) · [General](workflows.md) ·
[Audio](audio_workflow.md).

Visual workflows cover native graphics, motion and video, print and publication.
Choose a route below; [Using](using.md#visual) describes each route's full scope.
The [general workflow](workflows.md#workflow) supplies shared planning and
review stages. The [development index](development/index.md) links production
status and evidence.

## Choose a production route

### Graphics

| Route | Detailed documentation |
| --- | --- |
| SVG authoring | [Examples](../examples), [patterns](generative_patterns.md), [colors](generative_colors.md), [movement](generative_movement.md), [visualization](generative_visualization.md) |
| Native form studies | [Examples workflows](development/examples-workflows.md): parameter comparisons, controls and study-book exports |
| Authored compositions | [Examples workflows](development/examples-workflows.md#what-each-workflow-checks): wild scenes, crossovers and composition-book exports |
| Typed graph boundary | [Graph data, layout and resolved scenes](graph.md), [Kusama and chord rendering](generative_visualization/kusama-and-chord.md) |
| Portrait engraving | [Native photograph-to-vector API](portrait.md), [banknote controls](development/20261114_banknote_portrait_api.md) |

### Motion and video

| Route | Detailed documentation |
| --- | --- |
| MOSS appearance and local animation | [Pinned appearance history, saved-asset CPU replay and full-episode review](visual_workflow/tool_moss_local.md) |
| Seedance image-to-video | [Provider adapter and delivery contract](visual_workflow/tool_seedance.md) |
| Guilloche reels and title cards | [Native field animation and compact film delivery](visual_workflow/tool_guilloche.md) |
| Screen media glitch | [Ordered native composition](generative_movement/screen-glitch.md), [film decay](generative_movement/film-decay.md), [video glitch](generative_movement/video-glitch.md) |
| Supercut | [Review-cut contracts and checks](supercut.md), [usage examples](#supercut-usage) |
| DOCX word title videos | [Timed words, paragraph clips, layered reels and Instagram exports](visual_workflow/tool_docx_word_titles.md), [examples](../examples/docx-word-titles/README.md) |

### Print and publication

| Route | Detailed documentation |
| --- | --- |
| Portrait and print editions | [DLH engraving, holiday/anniversary editions, print exports and checks](visual_workflow/tool_portrait_print.md) |
| Print books, mobile PDFs and EPUB | [Publication workflow](visual_workflow/tool_publication.md), [manifest and CLI reference](publication.md), [standalone example](../examples/publication/README.md) |

Screenplay formatting from reviewed transcripts belongs to
[Audio → Transcription](audio_workflow.md#transcription), with its
[transcription enrichment guide](transcription-enrichment_workflow.md).

## Supercut usage

`scripts/izzi-supercut.py` derives deterministic seed/review cuts from a
source file and records their provenance. Times are accepted as seconds,
`MM:SS`, or `HH:MM:SS`.

Single cut:

```bash
python3 scripts/izzi-supercut.py \
  --input resources.rizal/blade-runner/source.mkv \
  --begin 1:02:03.5 --length 30 --cut-number 4 \
  --tiers orig,1080p,proxy \
  --output-root outputs/ad-hoc/supercut
```

Batch (see `schemas/izzi.supercut-1.schema.json` for the `cuts` contract):

```bash
python3 scripts/izzi-supercut.py --batch cuts.json \
  --output-root outputs/ad-hoc/supercut --resume
```

Options: `--accurate` seeks after the input for frame-accurate re-encoded
tiers; `--fps` sets an output frame-rate target; `--dry-run` prints the
ffmpeg commands without running them; `--resume` skips outputs whose
recorded SHA-256 already matches.

Outputs land under the output root in `ssk.orig` (stream copy),
`ssk.1080p` (1920×1080 x264 qp18 + FLAC), and `ssk.proxy` (640×360 x264
crf23 + AAC), with the historical `-cut-N.mkv`, `-1080p.1.mkv`, and
`-360p.2.mkv` suffixes. Each run writes `manifest.json` (input hash, filter
graphs, codecs, geometry, per-output hashes) and `SHA256SUMS`.

The same contract is available in C++ through
`src/izzi-media-supercut.h` (`izzi::media::supercut`). Verify the toolchain
with `make supercut-check`, which runs the header contract test and a lavfi
smoke derivation.

## Document map

Shared visual method, preparation and review references support the routes
above.

| Guide | Scope |
| --- | --- |
| [Directory index](visual_workflow/index.md) | The same Graphics, Motion and video, and Print and publication navigation |
| [Visual method](visual_workflow/method.md) | Visual passes and project methods |
| [Visual experiments method](visual_workflow/visual_experiments_method.md) | Form-first experiments and comparison design |
| [Style packs](visual_workflow/style_packs.md) | Style frames and aesthetic proofs |
| [Motion grammar](visual_workflow/motion_grammar.md) | Motion seeds, canary grammar and camera tracks |
| [Camera tracking routes](visual_workflow/camera_tracking_routes.md) | Route waypoints and derived camera tracks |
| [Visual preparation and assembly](visual_workflow/etl.md) | Keyframes, filmstrips, proxies, hashing and episode assembly |
| [Proofs and review surfaces](visual_workflow/proofs.md) | Filmstrip and proxy review pages |
| [Visual experiments](visual_workflow/visual_workflow_experiments.md) | Historical first-pass material |
| [Organization proposal](visual_workflow/reorganization_proposal.md) | Historical directory proposal |

Navigation updated November 16, 2026. Dated experiments remain in the session
records; the task guides above describe the currently delivered paths.
