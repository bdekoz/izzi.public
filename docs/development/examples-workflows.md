# Example forms and compositions

The examples workflows build two independent books from the native worker
catalog. `make examples-forms` validates individual forms and controls.
`make examples-composition` validates authored wild scenes and crossovers.
Each produces its own HTML index, SVG/PNG plates, coverage, source bundle and
compact PDF. Neither target runs or requires the other.

The native-form command family is now `examples-forms` (formerly
`examples-study`). The renderer's `study` kind, `EXAMPLES_STUDY_` overrides,
artifact paths and reel delivery names retain their existing names.

```sh
make examples-forms
make examples-composition
make examples-forms-pilot
make examples-composition-pilot
make examples-forms-compact
make examples-composition-compact
make examples-forms-vector
make examples-composition-vector
make examples-forms-reels
make examples-composition-reels
```

The pilot study selects representative forms across families. The composition
pilot selects one authored example for each role: wild, two-family crossover
and three-family crossover. Pilots validate the selected work and record
`scope: pilot`; they do not claim complete catalog or family coverage.

## Outputs and overrides

| Workflow | Book and evidence | Compact PDF | Reels |
| --- | --- | --- | --- |
| Study | `build/examples-study` | `outputs/izzi-examples-YYYY-MM-DD-study.compact.pdf` | `outputs/examples-study/reels` |
| Composition | `build/examples-composition` | `outputs/izzi-examples-YYYY-MM-DD-composition.compact.pdf` | `outputs/examples-composition/reels` |

Both output directories contain `index.html`, `catalog.json`, `coverage.json`,
`coverage-progress.json`, `plates.json`, `exports.json`, `inputs.json`, and
`source/manifest.json`. Every catalog, coverage record and plate manifest
identifies its `kind` as `study` or `composition`. An output directory belongs
to one kind; the renderer rejects attempts to reuse it for the other.

Use `EXAMPLES_STUDY_OUTPUT`, `EXAMPLES_STUDY_COMPACT_PDF`, `EXAMPLES_STUDY_PDF`,
`EXAMPLES_STUDY_JOBS`, `EXAMPLES_STUDY_TMP`, `EXAMPLES_STUDY_REELS_OUTPUT` and
`EXAMPLES_STUDY_REELS_JOBS` to override study settings. Composition settings
use the corresponding `EXAMPLES_COMPOSITION_` prefix. Each workflow uses four
independent rendering processes by default; `JOBS` accepts 1 through 8.

```sh
make examples-forms EXAMPLES_STUDY_JOBS=2
make examples-composition EXAMPLES_COMPOSITION_OUTPUT=build/composition-candidate
```

The main targets export canonical SVGs and 100 dpi PNGs, then build a compact
PDF. Compact PDFs retain vector annotations, source links and outlines while
embedding lossless artwork crops from the verified PNGs. A `-vector` target
also exports the complete vector PDF specified by its `PDF` variable.
A `-compact` target rebuilds only the compact PDF from completed evidence.
Each PDF has a `.plates.json` receipt with hashes and page mappings.

## Individual forms and scenes

Use the renderer directly to validate a selected form or authored scene:

```sh
python3 scripts/render-examples.py study \
  --form tv-broadcast/ignition-bursts \
  --output build/ignition-bursts-study --strict --png

python3 scripts/render-examples.py composition \
  --composition contour-wild-hierarchy \
  --output build/contour-composition --strict --png
```

The scene ID must occur in the authored composition definitions; inspect
`coverage.json` from a composition book for the resolved IDs. Both selectors
are repeatable. `--form` belongs to the study workflow; `--composition` belongs
to the composition workflow. Selected builds record `scope: selected` and can
pass their requested checks without claiming complete coverage. Full compact
exports require `scope: all`. Use `--pdf path.pdf` on the renderer to export a
selected or pilot book directly.

## What each workflow checks

Studies render native baselines and complete parameter comparisons. Static
comparisons must show geometry and pixel changes at every held setting;
temporal comparisons may include intentional holds. Seed, unrelated controls
and camera remain fixed. Adjacent duplicate samples are recorded separately.
The edge-tension family rejects neighboring duplicates and verifies every
C/P study ink by removing it and comparing the raster on the same paper. A large comparison is validated before pagination.
Duplicate baselines remain inspection candidates rather than automatic
removal decisions. Study coverage does not require authored compositions.

Compositions render authored wild scenes and crossovers independently of the
study grid. Component panels and ablations must demonstrate each component's
painted contribution. Motion must move completely outside the art viewport
before the ending hold. A full composition build requires two wild scenes,
all four variation dimensions, and two- and three-family crossover
participation for every registered family. Composition coverage does not
require the native study grids to have been rendered.

Both workflows check native source ownership against
`examples.data/review/source-inventory.json`, reject discovery errors, and
record technical completeness separately from pending human review.

## Native authority, snapshots and caching

Workers under `scripts/*-native.cc` opt in with `IZZI_REVIEW_WORKER`. Their
`--catalog` output declares active controls, types, bounds, defaults, study
axes and capabilities. Authored scenes are defined in
`examples.data/review/compositions.json` and resolved against that catalog.
The Python layer does not maintain a second control registry.

Worker caches include compiler identity, flags and recursive include
fingerprints. Render identities include native inputs, worker/source hashes,
seed, progress, camera, palette and raster environment. Cache receipts verify
SVG bytes and two isolated native invocations. Parallel workers publish
complete cache files atomically.

Each build captures source and renderer fingerprints before rendering, bundles
those sources, and checks them again before completing exports. If an input
changes during a build, that build fails with a rerun instruction. Editing a
source file does not update an already running worker or its loaded catalog.

After exports complete, `cleanup.json` records removed assets owned by the
previous manifest in that workflow's directory. Render caches and unrelated
files remain available. Separate output directories let either workflow be
regenerated without deleting the other's plates.

## Reading and exporting the books

The HTML index filters by family, form, role and parameter. Every plate records
controls, seed, frozen camera, source links and measured properties. Printed
contents and PDF outlines follow the same order. The cover identifies the
workflow, build date and source versions. SVG clips are explicitly stepped
proofs; continuous movies use native frame evaluation.

Reel targets first complete their own book, then encode landscape and portrait
movies with full decode and frame-count checks. Study reels contain native
media diagnostics and registered temporal forms. Composition reels contain
authored wild and crossover chapters. The complete deliveries are named
`examples-study-complete` and `examples-composition-complete`; study deliveries
also include `screen-glitch-complete` and per-media reels.

Native media diagnostics pair the clean input and processed picture at the
same native time and camera. Concealment may use an earlier processed picture;
frame receipts record that history. CRT shutdown is checked for a black final
processed frame. Authored compositions are checked for a white ending.

Direct exporters accept one completed book:

```sh
python3 scripts/export-examples-compact.py \
  --examples build/examples-study --output outputs/study.compact.pdf
python3 scripts/render-examples-reels.py \
  --examples build/examples-composition --output outputs/composition-reels
```

Compact PDFs and complete reels require successful full coverage for their
own workflow. Reel `--chapter` selects diagnostic chapters from a selected
book. Exporters reject mixed workflow manifests. Generated books and movies
are local artifacts; they do not promote visual baselines or imply human
acceptance.

## Historical delivery

The focused GCC and Clang checks pass, including isolated new public headers
and the core umbrella's two-file linkage check. The review catalogs also pull
in older drawing headers with non-inline definitions. A broader two-file probe
has the same 80 strong external definitions under both compilers with or
without the new adapters; no new strong definitions were added. Those inherited
linkage failures remain a separate legacy cleanup task. The native review
workers each use one translation unit.

The 2026-09-30 edition covers 360 forms in 31 families, with 1,294 controls and
97 wild/crossover compositions. All 184 movie chapters and 12 complete reels
are rendered and technically verified in both orientations. SVG/PNG pages,
the HTML review and the verified
compact PDF are available.
The full vector PDF also passes complete structural checks and representative
visual inspection.
See the [dated delivery record](sessions/20260930.make_examples_review_2.md)
for artifact links, counts, verification evidence and implementation limits. Human
visual approval is pending.

## Dutch stage 7 focused review

`make guilloche-dutch-stage7-review` produces the independent
local gallery at
`outputs/ad-hoc/guilloche.dutch-stage7/index.html`: one corrected
reconstruction, a source-fitted vector master, ten authored compositions,
a full 364-character script OTF including numerals, and aligned
evidence. `make guilloche-dutch-stage7-check` verifies native geometry, schema
contracts, isolated generation and the font. The ordinary example-baseline
runner supplies an isolated destination in both normal and complete modes.

The native `contour-loop` descriptor supplies parameter studies; the focused
example owns the full frame, source lettering and composition variations.
Carrier frequency and scallop depth use separate comparisons: zero depth
removes the scallops, so frequency is studied at the nonzero baseline depth.
See the [stage-7 plan and execution record](sessions/20261026.dutch_guilloche_stage_7.md).
The [saved handoff](sessions/20261026.dutch_guilloche_stage_7_resume.md) records
the current outputs, verification, uncommitted work and resume commands.
These local artifacts do not publish a portal, promote a baseline or establish
human visual acceptance.
