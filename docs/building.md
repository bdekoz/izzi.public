# Building and testing

Back: [izzi](../README.md)

The library target is `izzi::izzi`. The canonical entry points are the
Makefile wrappers:

```sh
make configure          # cmake -S . -B build/cmake -DBUILD_TESTING=ON
make all                # build the library and tests
make clean              # remove compiled artifacts, keeping CMake configuration
make examples-forms     # validate native forms and controls; export a study book
make examples-composition # validate authored scenes; export a composition book
make check              # full suite: migration contract CANONICAL-READY,
                        # package contract, generation, and family gates
```

## `make all`

`make all` is the default target, so plain `make` has the same effect. It first
configures `BUILD_DIR` with CMake and then builds every target in CMake's default
build graph. The default values are:

```text
BUILD_DIR=build/cmake
BUILD_TYPE=Debug
CXX=c++
```

Override them on the command line when a separate build tree or compiler is
needed, for example:

```sh
make all BUILD_DIR=build/clang BUILD_TYPE=Release CXX=clang++
```

This target compiles the registered tests and examples but does not execute the
test suite. Use `make check` when test execution is required.

## `make clean`

`make clean` uses `BUILD_DIR` (default: `build/cmake`) and does nothing if
that directory has not been configured. For example, `make clean
BUILD_DIR=build/clang` cleans the Clang build. It delegates to CMake's `clean`
target, removing compiler and linker outputs while preserving CMake's cache and
other configuration files, so the tree can be rebuilt without reconfiguration.

## Example forms and compositions

`make examples-forms` builds native baselines, parameter comparisons and
temporal studies. `make examples-composition` builds authored wild scenes and
two- and three-family crossovers. Each command runs its own strict checks and
exports an independent HTML/SVG/PNG book plus a dated compact PDF.
Neither command invokes the other.

Evidence lives in `build/examples-study` and `build/examples-composition`.
Compact PDFs are named `outputs/izzi-examples-YYYY-MM-DD-study.compact.pdf` and
`outputs/izzi-examples-YYYY-MM-DD-composition.compact.pdf`. Each has its own
coverage, source fingerprints, plate manifest and export receipts.

```sh
make examples-forms-pilot
make examples-composition-pilot
make examples-forms EXAMPLES_STUDY_JOBS=2
make examples-composition EXAMPLES_COMPOSITION_JOBS=2
make examples-forms-vector
make examples-composition-vector
make examples-forms-reels
make examples-composition-reels
```

Override the `EXAMPLES_STUDY_` or `EXAMPLES_COMPOSITION_` variables for output
directories, PDF paths, process counts, scratch space and reel destinations.
The `-compact` targets rebuild just the compact PDF from completed evidence;
`-vector` also exports the full vector book. Pilots and selected renderer
invocations validate their requested scope without claiming full coverage.

Native worker compilation requires a C++20 compiler and RapidJSON. Rendering
uses Python with Pillow, Pycairo, PyGObject/librsvg and fontconfig, plus installed
Atkinson Hyperlegible Next Regular and Bold OTF fonts. PDF regression checks
use Poppler tools. Reels additionally require FFmpeg and FFprobe.

See [Example forms and compositions](development/examples-workflows.md)
for selectors, artifact contracts, caching, coverage requirements and direct
export commands. Historical example-generation evidence remains documented
in the [September 21 session](development/sessions/20260921.make_examples_review_1.md).

## Direct CMake and compiler checks

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
baseline (`docs/development/sessions/20260808.milestone_0_baseline.md`) is a historical record
of the migration-era known failures, retained for provenance only. See
[`CONTRIBUTING.md`](../CONTRIBUTING.md) for focused tests, family gates
(`color-check`, `movement-check`, `visualization-check`), and visual
candidate generation.
