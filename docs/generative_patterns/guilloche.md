# Family: guilloche

Headers: `src/izzi-svg-guilloche.h` (3017 lines),
`src/izzi-svg-guilloche-json.h` (1828 lines),
`src/izzi-svg-graph-guilloche.h` (923 lines).

## API surface

- Core namespace `izzi::guilloche`: `point`, `path`, `api_version`;
  enums `render_profile`, `closure_mode`, `roulette_kind`,
  `composition_kind`, `pattern_class`, `pattern_family`, `blend_mode`,
  `semantic_role`, `glyph_ornament_kind`, `diagnostic_severity`;
  `diagnostic`; `resolved_shape_kind`.
- Centerline geometry: `roulette_spec`, `rose_spec`, `lissajous_spec`,
  `wave_spec`, `radial_ray_spec`, `harmonic_wave_spec`.
- Field/frame/overlay specs: `fish_scale_spec`, `barleycorn_spec`,
  `vignette_spec`, `medallion_spec`, `multicolor_line_spec`,
  `flinque_overlay_spec`, `latent_image_spec`.
- Composition machinery: `centerline_spec`, `family_spec`,
  `composition_spec`, `scene_spec` (with `nested_scenes` and
  `maximum_nesting_depth`), plates, inscriptions, motion, endpoint tone.
- JSON authoring: scene authoring/resolved/profile-bundle schemas plus an
  optional top-level `pattern {class, family}` enum object.
- Graph synthesis: `izzi::graph::dual_view_style/spec/result`,
  `graph_composite_spec`, and `resolve_dual_view_guilloche(...)` with
  optional composite backgrounds for every pattern family.

## Config

Scene JSON authoring and resolved schemas; profiles and density controls.

## Tests and examples

- Tests: `tests/guilloche.cc`, `tests/guilloche-scene-json.cc`,
  `tests/guilloche-profile-companions.cc`, `tests/guilloche-odr/*`,
  `tests/graph-guilloche-synthesis.cc`.
- Examples: `examples/guilloche-plates.cc`,
  `examples/guilloche-glyph-v3.cc`,
  `examples/guilloche-transition-frames.cc`,
  `examples/guilloche-capability-plates.cc` (nine accepted categories plus
  three additive P1 and two additive P2 reference-guided categories, with
  four additive P3 tuning categories, all using
  four parameter series and three variations per series), and
  `examples/guilloche-latent-plates.cc` (12 additive P5 plates).

## Usage

- Guilloche v4 title cards and proofs
  (`docs/visual_workflow/tool_guilloche.md`).
- Portal review: [Guilloche Capability 20260816](https://situationshipin.space/review/media/generation-guilloche-20260816/generation-guilloche-20260816-index.index.html).

## Assessment

Status: `ASSESSED-FIRST-PASS` (post-pilot).
Open items: full JSON authoring of the new composition geometries beyond
the `pattern` enum metadata; lenticular patterns remain deferred.

## P1 tuning

P1 adds opt-in C++ presets for `rosette`, `sunburst`, and
`band_guilloche`. Existing composition defaults and accepted capability
plate filenames remain unchanged. Reference photographs were used only for
line geometry: band spacing, rosette hierarchy, radial spacing, and texture
scale. No photographed text or other typographic content is represented.

| Family | P1 defaults | Rationale |
| --- | --- | --- |
| `rosette` | 12 petals, 11 lines, phase step `0.018`, scale step `0.018`, radial modulation `0.018` at frequency `3`, and 384 samples | Produces a compact core with closely nested, slightly modulated rings rather than a sparse single rose. |
| `sunburst` | 120 repeats, 2 ray tiers, inner-radius fraction `0.18`, tier scale step `0.035`, and 2 ray samples | Leaves an annular center opening and uses close, interleaved ray spacing while keeping the path and point cost low. |
| `band_guilloche` | fundamental frequency `12`, harmonics `(2, 0.28)` and `(3, 0.12)`, 11 lines, phase step `0.16`, normal-offset ratio `0.20`, 5 rows, automatic row spacing of `4.5` amplitudes, and 384 samples | Forms repeated, tightly spaced loop bands at canvas scale. The centerline scale is fixed at `1`, so the requested span is interpreted directly instead of being multiplied a second time. |

The additive P1 capability series use these ranges:

- rosette symmetry `8/12/16`, density `7/11/15`, scale
  `0.30/0.38/0.46` of the canvas extent, and phase `0/0.035/0.07`;
- sunburst symmetry `72/120/168`, density `1/2/3` tiers, scale
  `0.34/0.42/0.50`, and phase `0/0.025/0.05`;
- band symmetry `8/12/16` cycles, density `7/11/15` lines, span
  `0.78/0.88/0.96` of canvas width, and phase `0/0.14/0.28`.

The new series call the unchanged capability `base_scene()`. Therefore the
experimental render profile and its path, point, and SVG-byte budgets remain
the same. No time-, environment-, or pointer-derived variation is added;
existing optional variation continues to use scene and family seeds.

## P2 tuning

P2 adds opt-in C++ presets for `fish_scale` and `barleycorn`. The original
spec defaults and all accepted and P1 capability series remain unchanged.
The new `fish_scale_spec::seam_depth`, `barleycorn_spec::overlap`, and
`barleycorn_spec::relief` members are trailing aggregate fields with defaults
of zero, so existing aggregate initializers retain their former output.

Reference photographs were used only for repeated-cell geometry. In
particular, `AGC_20260815_192449327.jpg` informed the close rows and visible
return seams, `AGC_20260815_192724103.jpg` informed the pointed relief cells,
and `AGC_20260815_192223548.jpg` plus `AGC_20260815_192247676.jpg` informed
the dense staggered lattice proportions. Photographed text, numbers, serials,
signatures, and other typographic elements are not represented.

| Family | P2 defaults | Rationale |
| --- | --- | --- |
| `fish_scale` | 36 rows by 28 columns; width `1`, height `0.72`; overlap `0.44`; stagger `0.5`; seam depth `0.12`; phase `0`; 48 samples | Broad, shallow scales overlap by nearly half a cell height. Alternating half-cell rows suppress vertical channels, while the bowed return seam gives each closed scale a shallow relief edge. |
| `barleycorn` | 26 rows by 40 columns; width `0.64`, height `1`; overlap `0.18`; stagger `0.5`; relief `0.65`; phase `0`; 48 samples | Narrow vertical grains form a dense staggered field. Moderate row overlap closes horizontal gaps, and the relief exponent sharpens the four extrema from an ellipse toward a pointed engraved cell. |

The additive P2 capability series use these review ranges:

- fish-scale stagger `0.40/0.50/0.60`, density
  `28x22/36x28/44x34`, cell size `0.032/0.040/0.048` of the canvas
  extent, and phase `0/0.025/0.050` radians;
- barleycorn stagger `0.42/0.50/0.58`, density
  `20x32/26x40/32x48`, cell size `0.030/0.035/0.040` of the canvas
  extent, and phase `-0.08/0/0.08` radians.

The fixed P2 review values are fish-scale aspect `1:0.72`, overlap `0.44`,
and seam depth `0.12`, plus barleycorn aspect `0.64:1`, overlap `0.18`, and
relief `0.65`. Both series use 48 samples per cell. These values keep the
maximum P2 review fields below the existing `base_scene()` density budgets;
the render profile and budget declarations are not raised.

P2 introduces no stochastic cell variation. Phase, row stagger, overlap,
aspect, and relief are deterministic parameters, and any variation elsewhere
in a scene remains governed by the existing scene and family seeds.

## P3 tuning

P3 adds opt-in C++ presets for `medallion`, `vignette`,
`multicolor_line`, and `flinque`. Existing spec defaults and all accepted,
P1, and P2 capability series remain unchanged. New spec fields are trailing
aggregate members with compatibility values: medallion inset `0.9` and plate
stroke width, filled-circle vignette falloff `1` with zero inset, and discrete
one-cycle multicolor lines without a path-level blend override.

Reference photographs were used only for frame and transition geometry.
`AGC_20260815_192353385.jpg`, `AGC_20260815_192415710.jpg`, and
`AGC_20260815_192807866.jpg` informed the narrow concentric rings and inset
proportions. `AGC_20260815_192435019.jpg`, `AGC_20260815_192533174.jpg`, and
`AGC_20260815_192618169.jpg` informed the close curved-line ramps.
`AGC_20260815_192724103.jpg` informed the restrained overlay radius relative
to its underlying radial field. No photographed text, numbers, serials,
signatures, or other typography is represented.

| Family | P3 defaults | Rationale |
| --- | --- | --- |
| `medallion` | 6 rings, radial spacing `0.028`, stroke-width ratio `0.0045`, and inset-radius fraction `0.76` | Builds a narrow multi-ring frame with a clearly separated inset instead of allowing the nested scene to approach the outer ring. |
| `vignette` | feather `0.18`, falloff exponent `1.6`, inset `0.035`, opacities `0.04` to `0.82`, 24 annular steps | Uses non-overwriting annular strokes, a compact feather band, and a late-strengthening falloff at the frame edge. |
| `multicolor_line` | 17 lines, spacing `0.006` of span, curvature `0.12` of span, half-cycle curvature, four-stop smoothstep ramp, screen blend, 160 samples | Forms a close bowed iris transition; interpolation distributes the full color ramp across the line family rather than repeating discrete stops. |
| `flinque` | 96 rays, 2 tiers, tier scale step `0.035`, tint opacity `0.18`, overlay-radius fraction `0.86`, 2 samples per ray | Keeps the translucent tint inside the radial perimeter so the engraved ray edge remains visible. |

The additive P3 capability series use these review ranges:

- medallion rays `48/72/96`, ring count `4/6/8`, radius
  `0.30/0.36/0.42` of the canvas extent, and inset phase
  `0/0.04/0.08` radians;
- vignette rays `72/96/120`, annular steps `16/24/32`, radius
  `0.42/0.48/0.54`, and feather `0.12/0.18/0.24`;
- multicolor line count `13/17/21`, spacing
  `0.004/0.006/0.008` of span, span `0.72/0.82/0.92` of canvas width,
  and curvature `0.08/0.12/0.16` of span;
- flinque rays `72/96/120`, tiers `1/2/3`, overlay radius
  `0.78/0.86/0.94`, and tint opacity `0.12/0.18/0.24`.

All P3 builders call the unchanged capability `base_scene()`. The render
profile and resource budgets are not raised, and the general profile-variant
plate generator needs no P3-specific density adjustment. P3 adds no random
variation; all geometry is fixed by explicit parameters and existing seeded
scene behavior remains unchanged.

## P4 authoring

P4 adds an optional `layout.geometries` array. It is additive: the existing
`radial-field` composition is still generated, so documents that omit the
array retain their previous output. Every entry requires `id` and `kind` and
may set `origin` (default: `layout.center`) and `plate_index` (default: `0`).
Radial `radius`/`outer_radius` values default to `layout.radius`; band and
multicolor `span` default to twice that radius; field `cell_size` defaults to
five percent of the radius. The optional top-level `pattern {class, family}`
metadata remains valid and continues to classify the compatibility radial
field.

The geometry-specific keys and C++ tuning defaults are:

| `kind` | Optional keys and defaults |
| --- | --- |
| `sunburst` | `outer_radius`; `rays:120`, `ray_tiers:2`, `inner_radius_fraction:0.18`, `phase:0`, `scale_step:0.035`, `samples:2` |
| `band_guilloche` | `span`, `amplitude` (default: `0.08 * layout.radius`), `fundamental_frequency:12`, `harmonics:[{multiple:2,gain:0.28},{multiple:3,gain:0.12}]`, `phase:0`, `line_count:11`, `phase_step:0.16`, `normal_offset_ratio:0.20`, `rows:5`, `row_spacing:0` (automatic `4.5 * amplitude`), `samples:384` |
| `fish_scale_field` | `cell_size`; `rows:36`, `columns:28`, `scale_width:1`, `scale_height:0.72`, `overlap:0.44`, `stagger:0.5`, `seam_depth:0.12`, `phase:0`, `samples:48` |
| `barleycorn_field` | `cell_size`; `rows:26`, `columns:40`, `grain_width:0.64`, `grain_height:1`, `overlap:0.18`, `stagger:0.5`, `relief:0.65`, `phase:0`, `samples:48` |
| `medallion` | `radius`; `ring_plates:6`, `ring_spacing:0.028`, `ring_stroke_width_ratio:0.0045`, `inset_radius_fraction:0.76`, `inset_scene_index:0` |
| `vignette` | `radius`; `feather:0.18`, `falloff:1.6`, `inset:0.035`, `inner_color:#070912`, `outer_color:#070912`, `inner_opacity:0.04`, `outer_opacity:0.82`, `steps:24`, `annular_steps:true` |
| `multicolor_line` | `span`; `line_count:17`, `spacing_fraction:0.006`, `curvature_fraction:0.12`, `curvature_cycles:0.5`, four-stop P3 `ramp`, `interpolation:smoothstep`, `blend:screen`, `apply_blend:true`, `samples:160` |
| `flinque` | `radius`; `tint:#2DE2E6`, `opacity:0.18`, `overlay_radius:0.86`, `rays:96`, `ray_tiers:2`, `phase:0`, `tier_scale_step:0.035`, `samples:2` |

Minimal complete document with an authored sunburst:

```json
{
  "schema_version": "izzi.guilloche-scene/1",
  "scene_id": "p4-sunburst",
  "canvas": {"width": 1080, "height": 1080, "background": "#02030A"},
  "content": {"kind": "word", "word": "P4", "accessible_label": "P4 sunburst"},
  "treatment": {"kind": "floral-cycloid", "seed": 7, "floral_group_range": [1, 3], "petal_range": [5, 11], "cycloid_amplitude": 12, "density": "experimental"},
  "layout": {"kind": "radial", "symmetry_order": 6, "center": [540, 540], "radius": 360, "geometries": [{"id": "sun", "kind": "sunburst"}]},
  "review": {"profile": "experimental", "human_acceptance": "PENDING"}
}
```

Geometry resolution remains deterministic. P4 introduces no time-, pointer-,
or environment-derived values and does not alter render-profile or resource
budget declarations; authored geometry is charged to the existing path,
point, and SVG-byte budgets during normal resolution.

## P5 latent-image

P5 adds the additive `composition_kind::latent_image` family and classifies
it as `pattern_class::banded`. The geometry is a square parallel microline
band centered on `centerline.origin`; `centerline.scale` is its span. Each
row is divided into cells, and row/column parity alternates the local stroke
direction between the primary and secondary angles.

`latent_image_spec` uses these defaults and bounded ranges:

| Parameter | Default | Accepted range | Rationale |
| --- | ---: | ---: | --- |
| `line_spacing` | `6` | `[0.25, 1024]` | Controls the fine-line density while preventing zero spacing and unbounded row counts. |
| `cell_size` | `72` | `[1, 8192]` | Sets the direction-modulation period along each row. |
| `primary_angle` | `-0.2` radians | `[-π/3, π/3]` | Supplies the first shallow engraved direction. |
| `secondary_angle` | `0.2` radians | `[-π/3, π/3]` | Supplies the alternating shallow direction. |
| `phase` | `0` radians | `[-π/3, π/3]` | Rotates both directions together; each angle plus phase must remain in `[-π/3, π/3]`. |
| `samples` | `8` | `[2, 4096]` | Samples every deterministic cell stroke without changing its straight-line geometry. |

`make_p5_latent_image(origin, span, spec)` also bounds span to
`(0, 16384]`. The JSON `layout.geometries` form uses kind `latent_image`,
defaults span to twice `layout.radius`, and exposes the same six spec
parameters plus the common `id`, `origin`, and `plate_index` properties.

`resolve_latent_image_paths` emits one open stroke path for every row/cell
pair. Even parity selects the primary angle and odd parity selects the
secondary angle. Path identifiers contain stable row and column indices,
and normal scene resource estimation charges every path and sampled point
before resolution.

`examples/guilloche-latent-plates.cc` reuses one bounded base scene and emits
four series with three variations each: angle magnitude
`0.12/0.20/0.28`, line spacing `8/6/4`, span
`0.68/0.80/0.92` of canvas extent, and phase
`-0.08/0/0.08` radians. Cell size remains `72` and samples remain `8`.

The supplied photographs informed only the use of fine parallel engraved
bands and local direction changes. No photographed text, numbers, serials,
signatures, or other typographic elements are represented. P5 introduces no
time-, pointer-, or environment-derived variation; geometry is fixed by
explicit parameters and existing scene seeds retain their prior behavior.

## P6 rollup

The Dutch design exploration retains the nine accepted capability families
and adds five documented, opt-in stages:

- P1 tunes `rosette`, `sunburst`, and `band_guilloche`.
- P2 tunes `fish_scale` and `barleycorn`.
- P3 tunes `medallion`, `vignette`, `multicolor_line`, and `flinque`.
- P4 adds JSON authoring through the optional `layout.geometries` array and
  its geometry-specific parameters.
- P5 adds the deterministic `latent_image` composition and its 12-plate
  parameter-space example.

Each stage is additive: new presets, fields, authored geometries, and
examples are opt-in. Existing defaults, accepted capability series,
aggregate compatibility values, and the compatibility radial field remain in
place when the corresponding additions are omitted.

Resolution remains deterministic across the round. Geometry is controlled by
explicit parameters and the existing scene and family seeds, with no time-,
pointer-, or environment-derived variation. Existing resource estimation and
render-profile declarations remain authoritative: generated paths, sampled
points, and SVG bytes continue to be charged to the established budgets.

The reference policy remains paths-only. Photographs informed line and path
geometry, spacing, hierarchy, proportions, texture, frames, transitions, and
direction changes; no photographed text, numbers, serials, signatures, or
other typographic content is represented.

The sole remaining open item for this round is lenticular arrays, which
remain deferred.

## Stage 4 — Dutch lighthouse radiate

Stage 4 adds the opt-in `dutch_radiate_tuning` aggregate and
`make_dutch_radiate(origin, radius, brown_plate_index, blue_plate_index,
tuning)` builder. Existing composition defaults and accepted filenames are
unchanged.

`dutch_radiate_tuning` defaults are:

- `rays:48` and `inner_radius_fraction:0.08`;
- `brown_rays:0` and `blue_rays:0` (zero inherits `rays`, so single-count
  call sites keep their exact output);
- `brown_extent_fraction:1` (outer-layer radius; independent from
  `blue_extent_fraction`);
- `blue_extent_fraction:0.94`;
- `blue_loop_depth:0.16`, `blue_loop_frequency:24`, and
  `blue_loop_phase:0`;
- `blue_samples:64`, `brown_samples:2`, `seed:0`, and angular `phase:0`.

The builder returns exactly two compositions with a shared center and angular
phase. The first composition is the longer warm-brown radial-ray plate. The
second composition is the shorter muted blue-green plate, with sampled radial
modulation producing the interlaced looping treatment. Keeping the layers on
separate plates permits independent color, stroke width, registration,
rotation, opacity, and blend review.

`examples/guilloche-dutch-review.cc` emits a single 4800 by 2880 grid.
Columns use stroke weights `0.5`, `1`, `2`, `4`, and `8`. Row 1 repeats the
black/white reference parameters. Row 2 explores independent per-layer ray
counts (`brown_rays` vs `blue_rays`) and radii (`brown_extent_fraction` vs
`blue_extent_fraction`), inner radius, loop depth, loop frequency, angular
phase, and five named accessible color pairs. Row 3 reviews registration
error, asymmetric layer weights, a wave overlay, triangle form, and a hexagon
multi-polygon multi-pattern treatment with per-color opacity from `0.2` to
`1.0`.

The geometry proportions come from the path-only sol-5.6 analysis: center
fractions `0.637/0.522`, inner-radius fraction `0.08`, outer-radius fraction
`0.38`, 48 rays over 360 degrees, blue extent `0.94`, and the observed
two-layer blue-green and warm-brown separation. The review plate recenters
that geometry in each cell while preserving its radial proportions.

Provenance remains path-only. The reference analysis used photographed
geometry and color relationships only. No photographed text, numerals,
serials, signatures, labels, or other typographic content may be transcribed
or reconstructed by this stage.

## Stage 5 — Dutch multi-motif

Stage 5 adds five opt-in tuning aggregates and builders without changing
existing defaults or accepted filenames:

- `dutch_stack_tuning` with
  `make_dutch_stack_vignette(origin, width, height, red_plate, tuning)`;
- `dutch_quad_tuning` with
  `make_dutch_quad_medallion(origin, radius, black_plate, blue_plate,
  white_plate, orange_plate, tuning)`;
- `dutch_hexagon_tuning` with
  `make_dutch_hexagon_field(origin, width, height, gray_plate, brown_plate,
  tuning)`;
- `dutch_mandala_tuning` with
  `make_dutch_mandala(origin, radius, gray_plate, dark_red_plate,
  white_plate, tuning)`;
- `dutch_triptych_tuning` with
  `make_dutch_yellow_triptych(origin, width, height, gray_plate,
  brown_plate, tuning)`.

The stack builder supplies both GX1 and GX2, with per-group review extents
preserving the broad GX1 field and narrower GX2 central panel. The remaining
builders supply GX3, GX4 group 1, GX4 group 2, and GX5 respectively. Every
builder returns compositions assigned to caller-selected plates, so color,
weight, registration, rotation, opacity, and blend remain scene concerns.

`examples/guilloche-dutch-stage5-review.cc` emits six 2x 3-by-5 grids —
`gx1`, `gx2`, `gx3`, `gx4g1`, `gx4g2`, and `gx5` — matching the accepted
stage-4 v3 plate treatment. Each grid is 4800 by 2880, each cell is 960
square, and the column stroke weights are `0.5`, `1`, `2`, `4`, and `8`.
Row 1 repeats each builder's default tuning with black and white reference
plates; GX1 and GX2 are single-layer and therefore use black only. Row 2 uses
deterministic per-cell seeds, records the resolved tuning values in scene
titles, and cycles additional layers through five named accessible-palette
pairs. Row 3 applies registration glitch, asymmetric layer weights, extreme
tuning, relative layer rotation, and multiply-blend treatments. Per-cell
plates are not generated.

The six sol-5.6 analysis notes are paths-only geometry contracts:

- GX1 is a muted-red stacked vignette with a central elongated oval,
  interlaced waists, two rosette terminals, and a broad wave lattice.
- GX2 is a narrower continuous red panel with two circular terminals,
  transitional woven sections, and a dominant vertical oval.
- GX3 is a four-layer concentric medallion with 32 radial rays, scalloped
  rings, four vertical satellites, and paired lateral ripple polygons.
- GX4 group 1 is a gray-over-brown field of four staggered rows and six
  columns of six-sided medallions.
- GX4 group 2 is a sixfold gray, dark-red, and white mandala with radial bars,
  recessed sectors, concentric separators, and a perimeter vignette.
- GX5 is a gray-and-brown bounded triptych combining an upper halftone field,
  a graded circle, and an overlapping 28-petal sunflower.

These notes record paths, layer relationships, proportions, spacing, and
color roles only. No photographed text, numbers, numerals, serials,
signatures, labels, or other typography is represented or reconstructed.

Stage 5 output remains an experimental, gated portal family. The SVG grids,
and optional PNG review renders are local review artifacts; portal
publication, baseline promotion, accessibility claims, and visual acceptance
remain pending explicit human review. Deterministic grid hashes are checked
separately from that human gate.
