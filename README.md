# izzi

Izzi is a C++20 header-only library for creating a selected, practical subset
of SVG. It supports graphing, cartography, generative design, scientific
documentation, visual thinking, and frame generation for print, video, and web
workflows.

**DOCUMENTATION**

- API reference: [Doxygen](https://bdekoz.github.io/izzi.public/docs/html/)
- [Generative pattern families](https://bdekoz.github.io/izzi.public/docs/generative_patterns.md)
  — hamonshu, guilloche, moire, surface tension, radial, damped harmonograph,
  grignani, roulette
- [Visual workflow](https://bdekoz.github.io/izzi.public/docs/visual_workflow.md) and
  [audio workflow](https://bdekoz.github.io/izzi.public/docs/audio_workflow.md)
- [Examples detail](https://bdekoz.github.io/izzi.public/docs/examples_detail.md)

**PREREQUISITES**

Development and use is hosted on top-of-tree Linux. At the moment,
this means Fedora 41 or Alma 9 operating systems on multi-core
x86_64 hardware.

Some familiarity with Linux, contemporary C++20, and SVG is assumed.


**BUILDING AND TESTING**

The CMake target is `izzi::izzi`. Configure a normal out-of-tree build with:

```sh
cmake -S . -B build/local -G Ninja \
  -DCMAKE_CXX_COMPILER=c++ \
  -DBUILD_TESTING=ON
cmake --build build/local
```

Run the current GCC and Clang header/ODR baselines with:

```sh
make check-compilers
```

The present baseline intentionally exposes known failures. Read the
[`Milestone 0 baseline`](docs/development/milestone_0_baseline.md) before interpreting the
result, and see [`CONTRIBUTING.md`](CONTRIBUTING.md) for focused tests and visual
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


**HAMONSHU WAVE CURVES**

[`src/izzi-svg-curves-hamonshu.h`](https://bdekoz.github.io/izzi.public/docs/html/izzi-svg-curves-hamonshu_8h.html)
provides a
C++20, header-only catalogue and deterministic procedural SVG paths for 153
wave studies indexed to Mori Yuzan's 1903 *Hamonshu*, volume 2. The
`svg::hamonshu::motif_config` API parameterizes line density, curvature, phase,
rotation, reflection, and sampling resolution. It emits normalized motif
linework inside a caller-supplied `svg::hamonshu::pattern_box`; clipping,
styling, and SVG layer assembly remain with the caller.

[`examples/curves-hamonshu.cc`](https://bdekoz.github.io/izzi.public/examples/curves-hamonshu.cc)
validates the
public API and renders the thirteen curated source motifs at seven curvature
ratios with one fixed ink style. Build and run it from the repository root:

```sh
g++ -Isrc -std=c++20 -Wall -Wextra -Wpedantic -Werror \
  examples/curves-hamonshu.cc -o /tmp/curves-hamonshu
(cd /tmp && ./curves-hamonshu)
```

The generated `curves-hamonshu.svg` is a monochrome parameter grid containing
one titled group per selected source specimen and curvature ratio. The
Smithsonian Libraries scan is CC0; the descriptive English labels are not
translations of historical captions.

The
[Hamonshū wave-pattern catalogue and rendering notes](https://bdekoz.github.io/izzi.public/docs/generative_patterns/hamonshu.md)
document the source-page convention, motif naming, procedural interpretation,
cartographic clipping, rebuild workflow, and provenance.

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
