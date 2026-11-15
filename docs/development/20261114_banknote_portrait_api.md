# Banknote portrait API and parameter exploration

Current entry points: [Portrait engraving](../portrait.md) and the
[portrait/print edition map](../visual_workflow/tool_portrait_print.md).

The new portrait model is based on the eleven photographs in
`resources.rizal/transnational-portraiture-engraving`, including their broken
facial rows, long hair cuts, crossing meshes and selective feature outlines.
The anniversary photographs supply likeness; the banknotes supply mark and
composition observations. The reference atlas
records orientations, source hashes and observations.

Start with the high-contrast, low-curve proof
or interactive comparison.
The full explorer PDF
includes all 124 cases, independent of active web filters, with section
bookmarks and a complete numeric case index across 23 pages.
The complete flow/contour proof
and width sweep
use actual-size vectors. Enlarged source and result PNGs, recipes, measurements
and geometry hashes accompany each case.

## Contract

`tensor-flow/1` preparation and `banknote/1` synthesis retain the owned
`prepare -> resolve -> render_svg` interface. The independent banknote engine
emits filled vector cuts, circles and a two-direction shadow mesh. Optional
feature contours follow high-contrast sections of the source darkness field.
No face detection, anatomical reconstruction or style-transfer model is used.

```cpp
namespace p = izzi::portrait;
p::prepare_spec prep;
prep.algorithm = "tensor-flow/1";
prep.crop = {0, 0, image_width, image_height};
prep.analysis_long_edge = 600;
prep.tone.contrast = 1.65;
prep.tensor.radius = 18;
prep.tensor.sigma = 7;

auto recipe = p::banknote_engraving_v1();
recipe.width_mm = 32;
recipe.seed = 2026111424;
recipe.banknote.dot_percentage = 33;
recipe.banknote.line_percentage = 33;
recipe.banknote.crosshatch_percentage = 33;
recipe.banknote.maximum_width_mm = 0.14;
recipe.banknote.contour_emphasis = 0.5;
recipe.banknote.flow_follow = 0;
recipe.banknote.maximum_turn_rad = 0;

p::resource_budget budget;
budget.maximum_work_units = 2'000'000'000;
budget.maximum_working_pixels = 8'000'000;
auto fields = p::prepare(source, prep, budget);
auto marks = p::resolve(fields, recipe, budget);
auto svg = p::render_svg(marks, {}, {}, budget);
```

The source adapter supplies oriented linear luminance, an explicit membership
mask and optional protection/direction planes. Existing factual photograph
crops and masks are reused; old tone and composition recipes are not used.
The adapter resolves 1.5th and 98.5th percentile grayscale levels inside the
mask, records the numbers, then applies the selected contrast and gamma.
Source files remain unchanged.

## Elemental controls

| Public field | Meaning and constraints |
| --- | --- |
| `banknote.dot_percentage` | Relative tone weight for coherent dot rows; finite 0-100 |
| `banknote.line_percentage` | Relative tone weight for tapered primary cuts; finite 0-100 |
| `banknote.crosshatch_percentage` | Relative tone weight for two crossing shadow directions; finite 0-100 |
| `banknote.maximum_width_mm` | Maximum **line** width, including contour and hatch cuts; independent of dot diameter |
| `banknote.minimum_width_mm` | Narrowest cut and rounded end; must meet the print minimum |
| `banknote.maximum_dot_diameter_mm` | Independent maximum dot size; at least the stipple minimum |
| `banknote.row_spacing_mm` | Physical separation of rows; must accommodate maximum cut/dot size plus gap |
| `banknote.maximum_length_mm` | Maximum cut centerline length; rounded caps extend by the end radius |
| `banknote.taper_fraction` | Fraction of each cut used for each end taper, in (0, 0.5] |
| `banknote.highlight_darkness` | Transition from short light-area cuts to longer dark-area cuts |
| `banknote.shadow_darkness` | Onset of cross-hatching; in (highlight threshold, 1) |
| `banknote.crossing_angle_rad` | Included angle between the two hatch orientations, centered on primary flow |
| `banknote.feature_suppression` | Suppression of hatching by the authored feature-protection plane, 0-1 |
| `banknote.contour_emphasis` | Strength of source feature contours, 0-1; zero disables the layer |
| `banknote.flow_follow` | Blend of source-derived orientation and the preparation fallback direction, 0-1 |
| `banknote.maximum_turn_rad` | Maximum change in direction per integration step, 0-pi/2 |
| `prepare.fallback_angle_rad` | Direction of straight flow when `flow_follow=0` |
| `prepare.tensor.radius`, `.sigma` | Mask-normalized tensor smoothing, in analysis pixels |
| `prepare.tensor.tone_smoothing` | Blend of raw and smoothed darkness, 0-1 |
| `prepare.tone.contrast`, `.gamma`, `.brightness` | Explicit source tone controls, independent of mark mix |

DLH values are normalized by their positive sum. **33/33/33 is valid and
means equal thirds**; 100/0/0, 0/100/0 and 0/0/100 select the pure modes.
All-zero and nonfinite mixtures are rejected. These are tone weights, not
percentages of mark counts or a promise of exact final ink-area fractions.

Hatching is eligible only in shadows. Its two directions each receive half
the eligible allocation. Ineligible or feature-protected hatch tone remains
open paper. A pure hatch portrait consequently leaves highlights largely
blank. Minimum widths, support, spacing, crossings and coverage constraints
can prevent realization of the requested tone; receipts retain `TONE_UNDERRUN`.
Deep shadows can saturate rather than following a perfectly linear tone ramp.

Contour emphasis is separate from DLH. It traces high-contrast portions of
the 0.5 darkness isocontour, reducing the admitted contrast threshold from
0.24 toward 0.08 as emphasis increases. Cut width grows from the minimum to
65% of the available width range. These contours consume shared coverage
first; DLH operates on the remaining local tone. The pure-mode labels describe
DLH, so a pure-dot case with nonzero contour emphasis also contains the clearly
identified `feature-contours` layer.

Flow blends unoriented double-angle vectors. With follow=0 and turn=0,
primary cuts are straight and parallel to the fallback direction. With
follow=1, they follow the tensor field, subject to the turn limit. Integration
steps are half a row pitch. Source feature contours preserve source curves
even when the surrounding mark flow is straight.

## Exploration design

The main source is P12-2 at 32 mm wide. Eight transfer checks use the glasses
portrait P08-1 and the photographed covering P18-2 at their recorded physical
widths. All sources, tone levels, seeds and resolved controls are in
parameter-space.json.

- **44 width cases:** eleven DLH mixtures, including equal thirds and the
  three pure modes, at 0.065, 0.100, 0.140 and 0.180 mm maximum line width.
- **8 transfer cases:** three pure modes and 20/55/25 on each additional source.
- **72 expanded cases:** equal thirds and the three pure modes, crossed with
  contour emphasis 0/0.5/1, straight/restrained/full flow, and contrast 1.04/1.65.
- **24 highlighted cases:** the high-contrast subset with straight or restrained
  flow, collected in a dedicated two-page proof.

The expanded grid fixes the maximum line width at 0.140 mm. Straight flow uses
follow=0 and turn=0; restrained uses follow=0.25 and turn=0.045 radians; full
uses follow=1 and turn=0.22. Contour, flow and contrast are varied independently.
Width comparisons retain fixed dot size, flow, tone and physical size.

Measured layer fractions normalize the independently rasterized area of each
layer at 600 dpi. The full artwork is rasterized separately for union area,
so overlapping layers are not double-counted in total coverage. Layer fractions
and DLH weights answer different questions and are labeled separately.

The first width study shows that pure dots remain identical as the line cap
changes. Pure lines and line-led mixtures respond visibly to tighter caps.
Some wider caps are inactive because requested local cuts are already thinner;
unchanged geometry is a useful result. Pure hatching reads as selective dark
features rather than a complete tonal portrait. The expanded grid lets the
reviewer judge whether contour accents restore those features without making
the surrounding strokes excessively curved.

## Implementation and reproduction

The public structures and validation are in [izzi-portrait.h](../html/izzi-portrait_8h.html).
The independent synthesis is in izzi-portrait-banknote.inc,
registered in the installation manifest. The native adapter accepts banknote
`KEY=VALUE` overrides and writes round-trip precision receipts. Tone overrides
are `tone_contrast`, `tone_gamma`, and `tone_brightness`; the flow direction
override is `flow_angle_rad`. Other keys match the banknote field names.

The whole-photograph holiday adapter also uses `analysis_long_edge` (an integer
from 16 through 4096) and `coverage_sample_mm` to fit larger physical plates
within the existing resource budget. These driver overrides map directly to
preparation and print settings; the default values remain 600 pixels and
0.025 mm. The holiday study
uses 800 pixels and 0.05 mm, all-one rectangular support, and no face crop.
Its native vector geometry is generated at the final placed size.

```sh
python3 scripts/explore-anniversary-portrait.py
python3 scripts/render-guilloche-anniversary-24-stage3.py --deliver
python3 scripts/check-guilloche-anniversary-24-stage3.py --replay
```

Geometry remains deterministic within the pinned binary/numeric environment,
paint-independent and generated at its final physical width. The source-pixel
mask admits complete marks, including contours; no raster mask is hidden in
the SVG. Exact capsule envelopes enforce within-pass clearance. Different mark
passes may cross, and a shared sampled union ceiling limits their combined ink.
Long rejected cuts undergo bounded bisection to retain admissible portions.
Preparation, graph tracing, spatial indices, geometry and SVG output obey
resource limits; no partial success is returned after exhaustion.

Contour emphasis and flow are now implemented. Solid shadow masses remain a
separate possible extension, not an implied capability. Whole-sheet tension,
moire phase, negative-space placement and palette roles are composition controls
in the stage 3 recipes. Physical survival of the 0.055 mm cuts, 0.03 mm gaps,
RGB neon colors and duplex alignment remains a printer-proof task.

## Review findings

The 124 cases produce 103 distinct geometry hashes. Pure dots are unchanged
across the four line-width caps, while pure lines respond to the cap. Straight
and restrained flow expose a different visual rhythm from full tensor flow;
the source contours remain independently adjustable.

In the high-contrast, equal-DLH restrained-flow comparison, contour emphasis
0, 0.5 and 1 admits 0, 254 and 304 contour marks. Independently rasterized
union areas are 139.70, 147.43 and 153.98 mm². These are observations for P12-2
at 32 mm, not universal response curves. The added edges clarify glasses and
mouth boundaries. Pure hatching remains deliberately sparse in highlights,
especially on the brightly covered transfer source.

The twenty-pair review includes five contextual applications of equal DLH,
contrast 1.65, contour 0.5, follow 0.25, turn 0.045 and a 0.140 mm line cap.
Their composition settings vary; use the controlled parameter proofs to judge
individual portrait controls. Completion and verification evidence is recorded
in the [session document](sessions/20261114.guilloche_anniversary_24_stage_3.md)
and artifact validation.
