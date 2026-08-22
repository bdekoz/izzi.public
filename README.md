# izzi

Izzi is a C++20 header-only library for directly creating a selected, practical subset
of SVG. 

It supports graphing, cartography, generative design, generative art, scientific
documentation, visual thinking, and artifact generation for large-format inkjet prints, multi-channel animation, 
vertical video, and web workflows.

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
    — one-path and multi-path movement along generated SVG paths
  - [Visualization](https://bdekoz.github.io/izzi.public/docs/generative_visualization.html)
    — grids, tables, line/chord/kusama graphs
- **API reference:** [Doxygen](https://bdekoz.github.io/izzi.public/docs/html/)
- **Workflows**
  - [Workflows](docs/workflows.md)
  - [Visual](https://bdekoz.github.io/izzi.public/docs/visual_workflow/)
  - [Audio](https://bdekoz.github.io/izzi.public/docs/audio_workflow/)
- [Examples](https://situationshipin.space/review/izzi-generation-20260814/)

**PUBLISHING THE PUBLIC SITE**

The public docs site ([https://bdekoz.github.io/izzi.public/](https://bdekoz.github.io/izzi.public/))
is a curated snapshot of this repository: `docs/`, `examples/`, and the root
landing files, with `src/` and other private trees excluded. Refresh it with:

```sh
scripts/publish-public-mirror.sh --push
```

Add `--regen-doxygen` to regenerate `docs/html` (with `SOURCE_BROWSER = NO`,
source-listing pages stripped) and commit that to this repository first. Use
`--dry-run` to build and audit the snapshot without changing any repository.
