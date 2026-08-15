# izzi

Izzi is a C++20 header-only library for directly creating a selected, practical subset
of SVG. 

It supports graphing, cartography, generative design, generative art, scientific
documentation, visual thinking, and artifact generation for large-format inkjet prints, multi-channel animation, 
vertical video, and web workflows.

**DOCUMENTATION**

- **API reference:** [Doxygen](https://bdekoz.github.io/izzi.public/docs/html/)
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
- **Workflows**
  - [Visual](https://bdekoz.github.io/izzi.public/docs/visual_workflow/)
  - [Audio](https://bdekoz.github.io/izzi.public/docs/audio_workflow/)
- [Examples](https://situationshipin.space/review/izzi-generation-20260814/)

**PREREQUISITES**

Development and use is hosted on top-of-tree Linux. At the moment,
this means Fedora 41 or Alma 9 operating systems on multi-core
x86_64 hardware.

Some familiarity with Linux, contemporary C++20, and SVG is assumed.

**Text / subtitle / OCR machinery** (used by the time-to-die vertical and the
subtitle capability):

- `tesseract` 5.x — OCR of frames and PDFs into bullet-text and subtitle
  content (already the default OCR engine in the time-to-die draft scripts).
- `ffmpeg` / `ffprobe` with `libass` — probe clip geometry/duration, extract
  frames, and burn SRT/WebVTT subtitles.
- `python3` (stdlib) and `node` — OCR orchestration and subtitle
  serialization helpers.
- Atkinson Hyperlegible fonts installed locally
  (`/home/bkoz/.fonts/atkinson_hyperlegible.2026/`) for legible subtitle and
  label rendering.


**BUILDING AND TESTING**

The library target is `izzi::izzi`. The canonical entry points are the
Makefile wrappers:

```sh
make configure          # cmake -S . -B build/cmake -DBUILD_TESTING=ON
make all                # build the library and tests
make check              # full suite: migration contract CANONICAL-READY,
                        # package contract, generation, and family gates
```

Or configure an out-of-tree build directly:

```sh
cmake -S . -B build/local -G Ninja -DCMAKE_CXX_COMPILER=c++ -DBUILD_TESTING=ON
cmake --build build/local
```

Run the full suite under both GCC and Clang:

```sh
make check-compilers
```

`make check` is expected to pass cleanly on the current tree. The Milestone 0
baseline (`docs/development/milestone_0_baseline.md`) is a historical record
of the migration-era known failures, retained for provenance only. See
[`CONTRIBUTING.md`](CONTRIBUTING.md) for focused tests, family gates
(`color-check`, `movement-check`, `visualization-check`), and visual
candidate generation.


**INSTALLING AND CONSUMING**

Install the canonical header-only package to a local prefix and consume it
through the exported target:

```sh
cmake -S . -B build/install \
  -DBUILD_TESTING=OFF \
  -DCMAKE_INSTALL_PREFIX="$PWD/build/prefix"
cmake --build build/install --target install
```

A downstream CMake project can then use:

```cmake
find_package(izzi 2 CONFIG REQUIRED)
target_link_libraries(my-program PRIVATE izzi::izzi)
```

The default installation contains canonical `izzi-*` headers and the versioned
include fragments. New code should include `izzi-svg.h`. Run
`make check-install` to verify build-tree discovery, relocated canonical
installation, exact inventories, and external consumers.

The graph authoring and resolved-document schemas are installed below
`share/izzi/schemas`. They are versioned independently from the CMake package.


**TYPED GRAPH BOUNDARY**

[`src/izzi-svg-graph.h`](https://bdekoz.github.io/izzi.public/docs/html/izzi-svg-graph_8h.html)
separates graph data, layout
specification, deterministic layout, and rendering. It currently provides
typed radial-orbit and flattened linear-chord data/specifications and produces
a renderer-neutral scene with stable IDs, geometry, marginal totals, reading
order, accessibility intent, and structured diagnostics. The retained compact
MMRL orbit language is available as
`svg::graph::radial_orbit_spec::mmrl_defaults()` and the named `kusama`
preset.

[`src/izzi-svg-graph-json.h`](https://bdekoz.github.io/izzi.public/docs/html/izzi-svg-graph-json_8h.html)
strictly parses
`izzi.graph/1` authoring JSON from a caller-supplied string. It performs no file
I/O, rejects unknown members and incompatible data/layout kinds, and emits an
`izzi.graph-resolved/1` document in deterministic member order. Every default,
resolved scene coordinate, semantic reading order, and caller-supplied input
and schema hash is materialized. See `tests/data/graph/` for compact radial and
linear-chord inputs. The public schemas are
`schemas/izzi.graph-1.schema.json` and
`schemas/izzi.graph-resolved-1.schema.json`.

This boundary does not replace the existing radial/Kusama and chord renderers
yet. Their compatibility adapters and visual parity work are a separate
migration stage, so a valid scene is not a claim that historical output has
already been reproduced or that a graph conforms to WCAG 2.2.


**USING**

Source Documentation [here](https://bdekoz.github.io/izzi.public/).


See the files in the [examples](https://bdekoz.github.io/izzi.public/examples/) subdirectory.


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
