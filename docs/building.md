# Building and testing

Back: [izzi](../README.md)

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
baseline (`docs/development/sessions/20260808.milestone_0_baseline.md`) is a historical record
of the migration-era known failures, retained for provenance only. See
[`CONTRIBUTING.md`](../CONTRIBUTING.md) for focused tests, family gates
(`color-check`, `movement-check`, `visualization-check`), and visual
candidate generation.
