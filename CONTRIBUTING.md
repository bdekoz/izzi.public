# Contributing to Izzi

Izzi is a C++20 header-only SVG library. Keep changes small, deterministic, and
reviewable across native, print, video, and WebAssembly consumers.

## Configure and build

Requirements for the core checks are CMake 3.20 or newer, a C++20 compiler, and
RapidJSON headers for JSON-dependent public headers.

Run the focused typed-graph boundary checks with:

```sh
ctest --test-dir build/local -R '^graph\.' --output-on-failure
```

These cover deterministic pure layout, strict JSON diagnostics, normalized
resolved fixtures, Draft 2020-12 schemas, and a two-translation-unit ODR probe.
Changes to the resolved fixtures or corpus-defined MMRL defaults require a new
candidate and human review; do not update those baselines as a side effect of
an unrelated refactor.

```sh
cmake -S . -B build/local -G Ninja \
  -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_CXX_COMPILER=c++ \
  -DBUILD_TESTING=ON
cmake --build build/local
```

The library target is `izzi::izzi`. It is a CMake `INTERFACE` target; consumers
compile the headers directly.

## Run the baseline checks

```sh
make check-gcc
make check-clang
make check-compilers
make check-prefixes
make check-install
```

The current isolated-header/ODR baseline intentionally fails the same 23 of 35
tests under both observed compilers. Do not hide, skip, or normalize those
failures. See
[`docs/development/milestone_0_baseline.md`](docs/development/milestone_0_baseline.md) for the exact
inventory. A change is a regression if it adds a failing header/test or makes
the compiler outcomes diverge without an explicit, reviewed reason.

Run a focused test by name:

```sh
ctest --test-dir build/gcc -R 'header-self-contained.izzi-svg.h' \
  --output-on-failure
```

New public headers must compile first and alone. Namespace-scope header
definitions must be ODR-safe, and overlapping includes must link from at least
two translation units.

The root CMake manifest is the exact public install surface. Additions must
update that manifest and pass its configure-time filesystem comparison. The
default install contains canonical `izzi-*` headers only; the deprecated
alpha60-era forwarding headers and their install mode were removed, so no
legacy forwarding files may be reintroduced under `src/` or the install
surface.

## Example and visual checks

Read `tests/generation-baseline/README.md`
before rendering. Generate a candidate without replacing the current baseline:

```sh
python3 tests/generation-baseline/generate.py
```

Inspect the candidate manifest, individual SVG/PNG snapshots, contact sheet,
and print PDF. Baseline promotion is explicit and preserves prior evidence;
neither a successful renderer nor silence from a reviewer means acceptance.

Do not run examples from the repository root: many historical programs write
fixed filenames into their working directory. The harness isolates them.

## Change discipline

- Include exactly what each public header uses.
- Prefer typed values and explicit context over ambient state.
- Escape untrusted XML text and attributes; keep deliberate raw markup APIs
  visibly separate.
- Record seeds, frame rates, dimensions, tool versions, and hashes for generated
  artifacts.
- Keep network calls, FFmpeg, browser rendering, credentials, and paid provider
  actions outside the header-only core and behind explicit adapters.
- Preserve compatibility for alpha60 and cartofreako through named fixtures and
  migration notes.
- Add focused tests for a correction before broad cleanup.
- Run `git diff --check` and both available compilers before handoff.

Generated visual evidence can be large. Do not overwrite an accepted baseline,
commit transient build directories, or infer visual/WCAG acceptance from an
automated score.

## Licensing state

Existing source headers state the GNU General Public License, version 3 or later.
The repository currently lacks an authoritative root license file. Contributors
must not infer a different license, relicense third-party work, or add material
whose redistribution rights are unclear. A root license artifact remains a
maintainer/rights-holder decision recorded in the Milestone 0 ledger.
