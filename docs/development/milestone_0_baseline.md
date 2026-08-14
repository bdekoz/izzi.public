# Milestone 0 baseline

Observation date: 2026-08-08  
Baseline source commit: `284e1777183eedd9f29265728b9267c76f1e6488`  
Status: reproducible baseline; known failures are not waived

This document is the compact issue ledger for Izzi's C++20/header-only
baseline. It records what the current harness proves and what must become green
before Milestone 1 can claim a safe core. The broader rationale remains in
[`explore_futures.20260808.md`](explore_futures.20260808.md).

## Toolchain and reproduction

The observed matrix uses:

- GCC 15.3.1;
- Clang 21.1.8;
- CMake 3.31.11;
- Ninja 1.13.1;
- C++20 with compiler extensions disabled.

Run both compiler baselines:

```sh
make check-compilers
```

The command remains expected to return nonzero. Each compiler now discovers 38
tests: the original 34 header/ODR tests now report 11 pass and 23 known header-
isolation failures, while the two XML-safety and two blob-RNG tests pass. The
complete current result is therefore 15 pass and 23 fail per compiler. A
default `cmake --build` remains useful because diagnostic targets are excluded
from `ALL`; it must not be mistaken for a passing test suite.

To inspect one compiler independently:

```sh
make check-gcc
make check-clang
ctest --test-dir build/gcc -N
ctest --test-dir build/gcc --rerun-failed --output-on-failure
```

The CTest definitions set `CCACHE_DISABLE=1`, so direct and Make-wrapped runs
measure library behavior consistently in restricted environments.

## Public-header inventory

Every top-level `src/*.h` file is treated as a public v1 header. `PASS` means it
compiled first and alone under both observed compilers; it does not imply that
every runtime behavior or multi-translation-unit use is correct.

| Header | GCC | Clang |
|---|---:|---:|
| `a60-svg-base-types.h` | FAIL | FAIL |
| `a60-svg-codecvt.h` | FAIL | FAIL |
| `a60-svg-color-band.h` | FAIL | FAIL |
| `a60-svg-color-palette.h` | FAIL | FAIL |
| `a60-svg-color.h` | FAIL | FAIL |
| `a60-svg-composite-and-layer-basics.h` | FAIL | FAIL |
| `a60-svg-constants.h` | FAIL | FAIL |
| `a60-svg-curves-damped-harmonograph.h` | PASS | PASS |
| `a60-svg-curves-grignani.h` | FAIL | FAIL |
| `a60-svg-curves-hamonshu.h` | PASS | PASS |
| `a60-svg-curves-roulette.h` | PASS | PASS |
| `a60-svg-elements-components.h` | FAIL | FAIL |
| `a60-svg-elements.h` | FAIL | FAIL |
| `a60-svg-graphs-bar.h` | FAIL | FAIL |
| `a60-svg-graphs-chord.h` | FAIL | FAIL |
| `a60-svg-graphs-line.h` | PASS | PASS |
| `a60-svg-grid-matrix-systems.h` | FAIL | FAIL |
| `a60-svg-linear.h` | PASS | PASS |
| `a60-svg-markers.h` | FAIL | FAIL |
| `a60-svg-radial-arc.h` | FAIL | FAIL |
| `a60-svg-radial-base.h` | PASS | PASS |
| `a60-svg-radial-direction-arc-glyph.h` | FAIL | FAIL |
| `a60-svg-radial-fill-hexagon.h` | FAIL | FAIL |
| `a60-svg-radial-kusama.h` | FAIL | FAIL |
| `a60-svg-radial.h` | PASS | PASS |
| `a60-svg-render-basics.h` | FAIL | FAIL |
| `a60-svg-render-state.h` | FAIL | FAIL |
| `a60-svg-sequences.h` | PASS | PASS |
| `a60-svg.h` | PASS | PASS |
| `izzi-json-basics.h` | PASS | PASS |
| `izzi-points-cluster.h` | FAIL | FAIL |
| `izzi-points.h` | FAIL | FAIL |
| `izzi-tables.h` | FAIL | FAIL |

Totals: 33 headers, 10 pass under both compilers, 23 fail under both compilers.

## Known-issue ledger

| ID | Observed issue | Reproduction/evidence | Disposition |
|---|---|---|---|
| `M0-HDR-001` | 23 public headers depend on transitive include order, contain a cycle, or expose a downstream dependency | `make check-compilers`; table above | ACTIVE; Milestone 1 include-graph work |
| `M0-ODR-001` | Including the umbrella header in two translation units previously produced duplicate definitions | `odr.two-translation-unit`; `WI-HEADER-ONLY-ODR` receipt | CORRECTED/VERIFIED for the 14-header `<a60-svg.h>` executable closure under GCC and Clang in Debug and Release; isolated-header work remains active |
| `M0-XML-001` | Root serialization emitted a literal `s`; hostile text/attribute escaping lacked a focused contract | focused dual-compiler tests, `xmllint`, and post-XML visual candidate | CORRECTED/VERIFIED; human visual acceptance remains unavailable |
| `M0-RUNTIME-001` | `line-graph-4` aborts in a RapidJSON Pointer validity assertion on both isolated runs | generation manifest and retained runtime stderr | ACTIVE; diagnose before accepting a changed graph baseline |
| `M0-DETERMINISM-001` | `color-band-expand-to-larger` emits three different SVG outputs across two runs | generation manifest and amber-bordered grid tiles | OBSERVED; seed/intent decision required |
| `M0-MARKER-RNG-001` | `make_path_blob` used hidden wall-clock/global C RNG state, so `markers-1` varied across captures | `TC-LOOP-MARKERS-CROSS-CAPTURE-RNG-001` and two post-RNG candidates | CORRECTED/VERIFIED; marker SVG and pixels are exact across independent captures; human acceptance remains unavailable |
| `M0-RESOURCE-001` | Four previews report an external-image load failure | dark-red grid borders and render receipts | OBSERVED; resource policy unresolved |
| `M0-VISUAL-001` | The first whole-grid renderer silently substituted 106 identical broken-image glyphs despite exit status zero | invalidated history plus `TC-LOOP-VISUAL-GRID-RENDERER-001` | CORRECTED; Pillow assembly and pixel-diversity gate verified |
| `M0-CONSUMER-001` | Cartofreako and alpha60-results smoke consumers are documented but not implemented as pinned offline jobs | futures plan and [`explore_futures_alpha60_results.md`](explore_futures_alpha60_results.md) | NOT-STARTED |
| `M0-LICENSE-001` | Source headers state GPL version 3 or later, but no authoritative root license file exists | header notices and repository inventory | BLOCKED on rights-holder confirmation; no relicensing is inferred |

## Example and visual baseline

The generation harness compiles 106 top-level examples and runs each twice in
an isolated temporary directory. The promoted pre-correction `current` baseline
contains:

- 106 successful compilations;
- 100 visual examples and 106 well-formed SVG outputs;
- five successful nonvisual examples;
- one reproducible runtime failure;
- one generator whose outputs vary across two runs;
- 106 preview images and a 106-tile contact sheet;
- a one-page 36 by 49.9 inch PDF test print.

See `tests/generation-baseline/README.md`
and the current manifest. Promotion preserves the previous candidate, but a
promoted machine baseline is not human visual acceptance. The current PNG,
SVG, and PDF must receive an affirmative reviewer receipt before they become an
accepted visual reference.

### Corrected visual candidates

Later work preserves separate, unpromoted candidates rather than rewriting the
promoted machine anchor:

| Candidate | Content ID | Verification state |
|---|---|---|
| Post-XML | `sha256:13cd75bc4845e7d6585d5f50037b4cf77e492b5a3701913b1e9a9e32fcd0f104` | 106 SVGs; root-text correction localized; human acceptance unavailable |
| First post-RNG | `sha256:2244020f22a3b2e9eeac8d67483e89c8c877e0dc4e5c5ad30ccb5a4f20f9378b` | deterministic marker output introduced; complete grid/PDF preserved |
| Independent post-RNG repeat | `sha256:b9ccdd91dc26234bd2e804c9c429280f4bc3fb488440034107343dc1d94263a1` | `markers-1` exact as SVG bytes, preview bytes, and decoded pixels |
| Post-ODR | `sha256:dd56d7d194f65b2ee8a49c786445921da896f9af2a6653c4ccffe6cf0f3ef2a8` | 103/106 SVGs and previews exact to the independent post-RNG anchor; only three known color-band outputs vary; complete grid/PDF inspected |

The two post-RNG captures and the post-ODR candidate differ only in the three
already known `color-band-expand-to-larger` outputs. Their complete 106-tile
grids and PDF test prints remain available for affirmative human review; none
of these candidates was promoted.

## Milestone exit ledger

| Deliverable | State |
|---|---|
| CMake/CTest skeleton and Make entry points | VERIFIED |
| 33 isolated-header tests | VERIFIED as a failing baseline |
| Two-translation-unit ODR test | VERIFIED: the same single CTest links and executes cross-TU identity/state probes under GCC and Clang in Debug and Release |
| GCC and Clang core matrix | VERIFIED: 15 pass / 23 known isolated-header failures per compiler |
| Dual-compiler strict example matrix | VERIFIED: GCC 106/106 and Clang 106/106; see the final strict-candidate receipt |
| XML/golden example baseline | VERIFIED post-XML, post-RNG, and post-ODR candidates; human acceptance UNAVAILABLE |
| Known-issue list | VERIFIED in this document |
| alpha60-results smoke-consumer definition | VERIFIED as a proposal; implementation NOT-STARTED |
| Cartofreako smoke-consumer definition | PROPOSED; implementation NOT-STARTED |
| Build and contribution instructions | VERIFIED locally; see `CONTRIBUTING.md` |
| Authoritative root license | BLOCKED on rights-holder confirmation |

Milestone 0 therefore remains **ACTIVE**. It is not complete merely because all
failures are reproducible.
