# izzi

Izzi is a C++20 header-only library for directly creating visual elements using a shrewdly-selected subset of the full Scalable Vector Graphics [(SVG) standard](https://www.w3.org/TR/SVG2/). 

It supports graphing, cartography, scientific documentation, generative design, generative art, visual thinking, physical artifact generation for large-format inkjet printers, multi-channel animation, vertical video, and web workflows.

**DOCUMENTATION**

- **Intro**
  - [Prerequisites](docs/prerequisites.md)
  - [Building and testing](docs/building.md)
  - [Installing and consuming](docs/installing.md)
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
  - [General](docs/workflows.md)
  - [Visual](docs/visual_workflow.md)
    - [Graphics](docs/visual_workflow.md#graphics)
    - [Motion and video](docs/visual_workflow.md#motion-and-video)
    - [Print and publication](docs/visual_workflow.md#print-and-publication)
  - [Audio](docs/audio_workflow.md)
    - [Speech and voices](docs/audio_workflow.md#speech-and-voices)
    - [Transcription](docs/audio_workflow.md#transcription)
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

The [portrait and print workflow](docs/visual_workflow/tool_portrait_print.md)
maps the completed anniversary and holiday editions. Anniversary stage 4
refreshes twenty front/back pairs; holiday v3 and v4 each deliver fifty
whole-photo designs. Holiday v4 retains the output name `fall-crazy-pants`.
Native DLH controls, guilloche, 600 dpi artwork, PDF exports and validation
records are linked there. Physical print proofing and final selection remain.

The [development index](docs/development/index.md) tracks recent work and
distinguishes technical delivery from human acceptance. See the
[November documentation review](docs/development/sessions/20261114.documentation_review.md)
for current gaps and the staged improvement plan.

**PUBLISHING THE PUBLIC SITE**

The public docs site ([https://bdekoz.github.io/izzi.public/](https://bdekoz.github.io/izzi.public/))
is a curated snapshot of this repository: `docs/`, `examples/`, `schemas/`,
`config/publication/`, and the root documentation including `CHANGELOG.md`, with
`src/` and other private trees excluded. Refresh it with:

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
