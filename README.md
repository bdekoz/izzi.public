# izzi

Izzi is a C++20 header-only library for directly creating visual elements using a shrewdly-selected subset of the full Scalable Vector Graphics [(SVG) standard](https://www.w3.org/TR/SVG2/). 

It supports graphing, cartography, scientific documentation, generative design, generative art, visual thinking, physical artifact generation for large-format inkjet printers, multi-channel animation, vertical video, and web workflows.

**DOCUMENTATION**

- **Intro**
  - [Prerequisites](docs/prerequisites.md)
  - [Building and testing](docs/building.md)
  - [Installing and consuming](docs/installing.md)
  - [Typed graph boundary](docs/graph.md)
  - [Using](docs/using.md)
- **Generative families**
  - [Pattern](https://bdekoz.github.io/izzi.public/docs/generative_patterns.html)
    — hamonshu, guilloche, moire, surface tension, radial, damped
    harmonograph, grignani, roulette
  - [Color](https://bdekoz.github.io/izzi.public/docs/generative_colors.html)
    — palettes, bands, RGB/HSV quantization, perceptual tints
  - [Movement](https://bdekoz.github.io/izzi.public/docs/generative_movement.html)
    — path movement and screen media effects
  - [Screen media glitch](docs/generative_movement/screen-glitch.md)
    — composition, [film decay](docs/generative_movement/film-decay.md),
    and [video glitch](docs/generative_movement/video-glitch.md)
  - [Visualization](https://bdekoz.github.io/izzi.public/docs/generative_visualization.html)
    — grids, tables, line/chord/kusama graphs
- **API reference:** [Doxygen](docs/html/index.html),
  [Screen Glitch](docs/html/screen-glitch/index.html)
- **Workflows**
  - [Workflows](docs/workflows.md)
  - [Visual](https://bdekoz.github.io/izzi.public/docs/visual_workflow/)
  - [DOCX word title videos](docs/visual_workflow/tool_docx_word_titles.md)
    — configurable word holds, paragraph clips, layered review reels and Instagram exports, with [runnable examples](examples/docx-word-titles/README.md)
  - [Audio](https://bdekoz.github.io/izzi.public/docs/audio_workflow/)
  - [Audio/video transcription](docs/audio_workflow/tool_transcribe_audio.md)
    — local Whisper recognition, timestamped transcripts and subtitles, with [runnable examples](examples/audio-transcription/README.md)
- [Examples](https://situationshipin.space/review/izzi-generation-20260814/)

**BUILD AND EXAMPLES**

```sh
make all
make clean
make examples-forms
make examples-composition
```

`make all` configures `build/cmake` and compiles the registered library tests
and examples. `make clean` removes those compiled artifacts while retaining
CMake configuration. `make examples-forms` validates native form controls and
temporal behavior; `make examples-composition` validates authored wild scenes
and crossovers. Each independently exports an HTML/SVG/PNG book and compact
PDF, with source hashes and coverage receipts. Evidence lives in
`build/examples-study` and `build/examples-composition`; dated PDFs live in
`outputs`. See [Building and testing](docs/building.md) for dependencies and
[Examples workflows](docs/development/examples-workflows.md) for commands,
selectors and output overrides.

The [Meta guilloche 2 production record](docs/development/sessions/20260929.meta_guilloche_2.md)
links the September 29 Review 4 stills, 44-page PDF and four technically
verified compact reels. See the [guilloche workflow](docs/visual_workflow/tool_guilloche.md)
for native rendering, complete-background exits and delivery checks.

**PUBLISHING THE PUBLIC SITE**

The public docs site ([https://bdekoz.github.io/izzi.public/](https://bdekoz.github.io/izzi.public/))
is a curated snapshot of this repository: `docs/`, `examples/`, `schemas/`, and the root
landing files, with `src/` and other private trees excluded. Refresh it with:

```sh
scripts/publish-public-mirror.sh --push
```

Add `--regen-doxygen` to regenerate `docs/html` (with `SOURCE_BROWSER = NO`,
source-listing pages stripped) and commit that to this repository first. Use
`--dry-run` to build and audit the snapshot without changing any repository.
The refresh also regenerates the focused Screen Glitch reference. In the
snapshot, source-header links use available API pages and links to excluded
local artifacts become plain labels; genuinely missing public targets still
fail the audit. Dry runs use committed `HEAD`, so commit reviewed documentation
changes before refreshing the public site.
The three current workflow SVGs and their hardware inventory are copied from
`resources.static/workflows/` into the snapshot's `docs/workflow-diagrams/`;
their documentation links are rewritten to those public copies.
