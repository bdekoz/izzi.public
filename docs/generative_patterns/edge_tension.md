# Edge tension: cubic bundles and reference studies

`izzi-svg-edge-tension.h` provides exact planar cubic strands in
`izzi::edge_tension`. A boundary loft interpolates corresponding control
points of two cubic rails. An endpoint fan samples two rails and connects
those samples with straight or tangent-controlled cubic spans. This API is
independent of the scalar-field contours in `izzi-svg-surface-tension.h` and
the projected Grignani ribbons. Neither older renderer changes.

The [dated plan](../development/sessions/20261025.tension_revisited.md)
identifies sixteen monochrome PDF-compatible Illustrator references.
The new recipes are authored interpretations of their silhouettes and voids.
Their two- and three-color treatments use native `rurikon`, `daylily`, and
`asagiiro`; these colors were not recovered from the references. The installed
library and studies do not read the archive or require Illustrator.

## Status and remaining work

Verified 2026-10-26: the E0–E6 implementation is technically complete.
Eight GCC checks passed, including geometry, periodic rendering, header
self-containment, linkage, and catalog contracts. The worker rebuilt for that
verification reproduced all 608 distinct edge-tension SVGs and eight legacy
ribbon/contour baselines byte for byte. The
[verification receipt](../development/sessions/20261025.tension_revisited/verification-20261026.json)
records tested source hashes and results.

The saved delivery covers all eighty selected study plates and the full
376-form examples-study build. Both PDFs, 1,291 PNG hashes, and their source
bundles passed the 2026-10-26 integrity audit. The
study PDF contains the eighty parameter
plates, and the baseline gallery
shows the sixteen constructions.

The user accepted the baseline contact sheet on 2026-10-26: “review contact
sheet looks good.” The
[approved sheet](../development/sessions/20261025.tension_revisited/baseline-contact-approved-20261026.png)
and [review receipt](../development/sessions/20261025.tension_revisited/human-review-20261026.json)
preserve the exact reviewed artifact and its hash.
The [saved handoff state](../development/sessions/20261025.tension_revisited/state.json)
collects the current status, evidence hashes, review locations, and remaining
actions for the next session.

| Review artifact | Contents | Review status |
| --- | --- | --- |
| [Archived contact sheet](../development/sessions/20261025.tension_revisited/baseline-contact-approved-20261026.png) | Sixteen baseline drawings; tracked copy of the reviewed image | Accepted on 2026-10-26 |
| Study PDF | Eighty parameter plates, plus cover and contents; 82 pages total | Parameter-grid review pending |
| HTML study book | The same G1/G2/D/C/P comparisons with controls and evidence | Parameter-grid review pending |
| Baseline gallery | Individual baseline drawings | Contact-sheet acceptance covers their displayed baseline overview |

The archived contact sheet is available from a checkout. The PDF, HTML book,
and gallery are generated local outputs; use the build commands below when
they are absent. The saved state links the delivery and review evidence.

Remaining follow-up:

- Review the eighty G1/G2/D/C/P parameter plates for shape variation, line
  spacing, two-/three-ink separation, and opacity/registration across the
  sampled ranges. Record acceptance or specific adjustments. The contact
  sheet is accepted; parameter-grid review remains pending.
- After the ongoing shared catalog, guilloche and source-inventory changes
  settle, rerun `make edge-tension-check`, `make edge-tension-study`, and
  `make examples-forms` with the intended compiler. Verify the new coverage,
  source fingerprints, PDF page mappings, and hashes. Four shared inputs
  differ from the saved book snapshot; both edge-tension headers and all
  replayed edge drawings match it. The full book was audited, not rebuilt,
  during the 2026-10-26 verification.

The implementation is committed and pushed as `bb7759a4`, its verification
as `e7e26070`, the contact-sheet approval as `37c7613c`, and the consolidated
handoff state as `ef2ab23a`.
No outstanding edge-tension implementation defects were identified by this
verification.
The 2026-10-26 checks used GCC; the recorded Clang checks belong to the
earlier delivery.

Whole-fan composition studies, a guilloche adapter and surface-field
deformation are optional extensions. The
[current task record](../development/sessions/20261025.tension_revisited.md#current-status-and-follow-up)
tracks this follow-up separately from the completed implementation.

## Minimal construction

```cpp
#include <izzi-svg-edge-tension.h>
#include <iostream>

int main() {
  namespace et = izzi::edge_tension;
  et::scene_spec scene;
  scene.bundles.push_back({
    "crossing",
    {{-1,-1}, {-.5,1}, {.5,-1}, {1,1}},
    {{-1,1}, {-.5,-1}, {.5,1}, {1,-1}},
    et::loft_tuning{48, 1.0, et::correspondence::forward}
  });
  scene.instances.push_back({"first", "crossing", {}, 0});
  scene.inks = {
    {"blue", "#1B294B", .8, .45, et::blend_mode::source_over, {}},
    {"orange", "#FF8936", .8, .45, et::blend_mode::source_over, {}}
  };
  scene.composition.assignment = et::ink_assignment::interleaved;
  auto resolved = et::resolve(scene);
  std::string svg = et::render_svg(resolved);
  std::cout << svg;
}
```

`bundle_spec::tuning` holds either `loft_tuning` or `fan_tuning`. For strand
index `i`, `u=i/(N-1)` and `s=pow(u, spacing_power)`. Loft controls are
`mix(A, B, s)`. Reverse correspondence reverses all four controls of B.
For fans the rail samples are `A(s)` and `B(pow(s,mapping_power))`, with the
latter parameter replaced by `1-v` for reverse correspondence. Fan tangent
angles are radians; each handle length is its ratio times the endpoint chord.
`straight=true` gives an exact straight cubic diagnostic.

A collapsed boundary is allowed. `resolve` rejects a wholly empty bundle or
scene and reports individual collapsed strands. It validates finite controls,
counts, paint, IDs, transforms and allocation products before allocating the
resolved strand vector. Default budgets are 256 instances, 128 strands per
bundle, 65,536 cubics and 16 MiB serialized SVG. Both a conservative byte
estimate and the actual serialized size are checked. Budgets can be lowered
for callers with smaller limits.

Each resolved strand retains its cubic controls, source bundle, component,
strand index, ink ID and instance transform. IDs are independent of diagnostic
sampling. SVG uses `M … C …` for every strand. No tessellation or white void
mask is used. `diagnostic.potentially_clipped_strands` counts control hulls
outside the finite-field clip; it is conservative, not a measured intersection
count. Colors and labels are XML escaped and numbers use a fixed locale.

## Placement, repetition and inks

An `affine` follows SVG matrix order; `A*B` applies B first. Helpers provide
translation, rotation, reflection through negative scale, anisotropic scale,
and rotation about an anchor. Reflecting an instance leaves rail
correspondence unchanged. `radial_instances` separates orbit, angular phase
and local rotation. `lattice_instances` translates a cell using
`(column*pitch_x + odd_row*stagger*pitch_x, row*pitch_y)` before applying an
optional odd-row reflection. Negative row indices obey the same parity rule.

`supercell` returns the joint spatial/ink period. Stagger or alternating
reflection requires two rows; an ink row cycle enlarges this with the least
common multiple. A computed period does not prove that motif edges join.
The resolver produces a finite padded window; the scene clip selects
the displayed field. The recipe author supplies matching boundary anchors
where joining is intended.

Paint belongs to individual strands. Assignments use the original index u,
so spacing bias does not move the ink boundary:

| Assignment | Ink index for K inks |
| --- | --- |
| `bands` | `min(K-1, floor(K*u))` |
| `interleaved` | `i % K` |
| `opposed_bands` | `min(K-1, floor(K*2*min(u,1-u)))` |
| `component` | instance ink index modulo K |

`composition_spec` also selects a cyclic ink phase and drawing order by
strand or ink. Each ink owns width, opacity, global registration and blend
mode. Registration follows instance placement and precedes the camera, so
all copies of an ink move together. Source-over is the study baseline;
`blend_mode::multiply` offers an optional overprint with identical geometry.
Neither mode implies physical weaving.

## Authored recipes and controls

`izzi-review-edge-tension.h` owns the sixteen recipe definitions, their typed
fan/loft controls, fixed cameras and explicit native study descriptors. The
geometric header contains no reference IDs. Angles in descriptors are degrees
and are converted explicitly to radians. Four recipe-specific controls plus
six common controls make ten controls per form.

| Recipe | Construction and calibration |
| --- | --- |
| `o-1-021` | Four mirrored lofts. Waist offsets the inner upper anchor; the inner terminal crosses the center. Span widens the second boundary; twist rotates it. |
| `o-1-022` | Two four-fan assemblies. Orbit moves each origin; opening turns span tangents; phase turns the companion assembly; handles curl the tips. |
| `o-2-002` | Four hooked fans. Orbit keeps an open ring; twist rotates the second rail; opening turns the tangents; mapping power changes correspondence. |
| `o-2-003` | Four pinched fans with longer terminal rails. Waist is the small assembly orbit. Opening, rail twist and handles control the sharp/curl contrast. |
| `o-2-005` | Four saddle lofts around a square void. Waist moves two inner B controls; orbit separates the sides; twist rotates B about its center; handles flare the corners. |
| `o-2-006` | Four closed cubic teardrop lofts. Their loops produce four separate holes. Orbit, second-loop rotation, vertical span and handle scale change the loops. These are more separated than the source's joined ribbons. |
| `o-2-008` | One loft between a broad lower cup and an upper curling boundary. Waist moves the upper boundary's first handle; span widens the cup; twist rotates B; handle scale changes both lobes. |
| `o-2-009` | Unequal endpoint rails form an open U. Opening turns the span tangents; span widens the right rail; skew translates it; handles control the bowl depth. Camera scale is 65 to retain the complete grid. |
| `o-2-015` | One sideways crossing loft. Span widens the right terminal, skew translates B, twist rotates it, and waist offsets its first handle. |
| `o-2-019` | A leaf loft and a trailing loft. Tail length sets the second span's reach; phase turns both adjacent handles about each shared join. All interpolated strands have exactly equal first derivatives at the join. |
| `o-3-007` | Two reflected broad saddles. Waist moves inner handles, span widens the upper terminal, twist turns B and handle ratio controls the bowed sides. Horizontal terminals describe the baseline. |
| `o-3-008` | Two sheared saddles. Skew shears B, opening turns the authored handles, twist rotates B and shorter handles retain angular corners. |
| `p-006` | Four straight endpoint fans per rectangular cell. Rail bows vary with opening and waist. Extreme spans meet neighboring cells at pitch-dependent ports with matching tangents. |
| `p-007` | Alternating-handed open ribbons in staggered rows. Twist turns B and span widens both rails. Bounded motifs retain intentional gaps; the two-row period includes reflection and stagger. |
| `p-009` | Four reflected fans with tall pitch-dependent ports. Aspect reshapes interior rail controls while preserving the ports; opening bows the rails and waist moves their inner endpoints. This adaptation preserves joins instead of scaling ports away from the selected pitch. |
| `p-010` | Staggered bounded bands between opposed rounded cubics. Span sets horizontal reach; handle ratio sets roundness; row pitch and stagger create overlap strips. Endpoints are intentional motif terminals. |

The dated plan retains the original hypotheses and all numeric sample tables.
Some authored boundaries are pre-rotated so the middle twist sample gives the
baseline orientation. In loft recipes, `handle_ratio` scales authored handle
distances relative to the baseline shape; P_010 uses the nominal 1.5-unit
band height. Fan handles
use the exact chord ratios defined by `fan_tuning`. These recipe scales do
not claim a measured ratio in the source artwork and do not move endpoints.
All grid cells use the same camera per recipe, with
at least a 12-unit object margin throughout the advertised study samples.
Pattern fields have a fixed 288-unit square clip and padded repetition.

## Study and build commands

```sh
make edge-tension-check CXX=/usr/bin/g++ BUILD_DIR=build/edge-tension-gcc
make edge-tension-plates CXX=/usr/bin/g++ BUILD_DIR=build/edge-tension-gcc
make edge-tension-study CXX=/usr/bin/g++
make examples-forms CXX=/usr/bin/g++
```

The focused study target writes `build/edge-tension-study/index.html` and
`outputs/edge-tension-study.pdf`. Override `EDGE_TENSION_OUTPUT` and
`EDGE_TENSION_PDF` as needed. It records selected coverage. The complete
`examples-forms` target includes this family automatically and retains its
independent full-book coverage and compact PDF workflow.

Each form owns five plates:

| Plate | Cartesian comparison | Cells |
| --- | --- | --- |
| G1, G2 | Two authored geometry comparisons | 9 each |
| D | 24/48/72 strands × spacing powers .6/1/1.6 | 9 |
| C | Two/three inks × bands/interleaved/opposed bands | 6 |
| P | Opacity .4/.8/1 × registration 0/.02/.04 | 9 |

This is 80 study plates and 672 sweep requests, plus sixteen baseline
requests. Several requests intentionally share a cached SVG; 688 is not a
count of unique artifacts. Seed 20261025, camera and unrelated controls stay
fixed. Registration amount a translates ink k by
`(k-(K-1)/2)*a*(1,.5)` in motif units.

The study builder checks geometry/paint and raster signatures at every held
setting. It records adjacent duplicate samples separately and rejects them
for this family. Every C/P cell also removes each ink in turn on the same
paper and camera, checking its visible contribution, including the third
ink. Coverage stores the ablation hashes. Determinism receipts compare two
isolated native runs; export receipts contain the actual PDF page mappings.
The baseline contact sheet is accepted; human review of the parameter grids
remains pending.
