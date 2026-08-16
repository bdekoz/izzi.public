# Izzi guilloche refinement — Dutch banknote pattern vocabulary

Status: implemented (phases 1-5). Stage 3 parameter-space review published
2026-08-16.
Recorded: 2026-08-16 America/Los_Angeles.
Input brief: `explore_futures.20260816.dutch_guilloche.md`.

## 1. Scope and source

Reference set: `resources.rizal/dutch-banknote-design-1814-2002/`.

- 18 JPEG files.
- Mixed orientation: mostly 2252x4000, some 4000x2252.
- 16-bit sRGB, roughly 2.6-4.7 MiB each.

The goal is not to reproduce specific banknotes. It is to extend the
generative guilloche vocabulary so the requested pattern classes are
expressible in the existing deterministic, budgeted, JSON-driven pipeline.

Primary files reviewed:

- `src/izzi-svg-guilloche.h` (core scene/pattern engine, 3017 lines after the pilot)
- `src/izzi-svg-guilloche-json.h` (authoring/resolved JSON layer, 1828 lines after the pilot)
- `src/izzi-svg-graph-guilloche.h` (graph-to-guilloche synthesis, 923 lines after the pilot)
- `src/izzi-svg-curves-roulette.h` (roulette sampling primitive)

## 2. Current capability map

The current engine composes a scene from these primitives:

- Centerline geometry (`centerline_geometry` variant):
  - `roulette_spec` (hypotrochoid / epitrochoid)
  - `rose_spec`
  - `lissajous_spec`
  - `wave_spec`
- Composition kind (`composition_kind`):
  - `rosette`
  - `band`
  - `moire_field`
  - `orbit_net`
- Family transform (`family_spec`):
  - phase, scale, rotation, normal offset, radial modulation, jitter
- Plates, blend modes, inscription weaving, glyph ornaments, motion, and
  endpoint tone.

What already exists in some form:

- Simple wave bands (`wave_spec` + `composition_kind::band`)
- Rosettes (`composition_kind::rosette` + `roulette_spec`/`rose_spec`)
- Radial fields (`composition_kind::orbit_net` in the JSON builder)
- Moire/interference fields (`composition_kind::moire_field`)

What is missing or too weak for the requested Dutch-banknote classes:

- True radial straight-ray sunbursts.
- Fish-scale (ecailles de poisson) overlapping scale lattices.
- Barleycorn (grain d'orge) offset oval/lozenge fields.
- Flinque-style translucent enamel over a radial guilloche base.
- Real banding guilloches with multi-line borders and corner logic.
- Medallions and soft-edged vignettes.
- Multi-color-line / iris-print effects.

## 3. Requested patterns mapped to proposed primitives

| Requested pattern | Current nearest support | Proposed primitive / change |
|---|---|---|
| Wave | `wave_spec` + `band` | Add harmonic wave, amplitude envelope, longitudinal offset; add `band_guilloche` composition with parallel line group |
| Sunburst | `rose_spec` with high petal count is visually wrong | Add `radial_ray_spec` to `centerline_geometry`; add `composition_kind::sunburst` |
| Ecailles de poisson | none | Add `fish_scale_spec`; add `composition_kind::fish_scale_field` |
| Flinque | none (only tint endpoint) | Add `flinqué` treatment: radial sunburst base + translucent tinted plate overlay |
| Barleycorn | none | Add `barleycorn_spec`; add `composition_kind::barleycorn_field` |
| Rosettes | `composition_kind::rosette` | Add multi-ring rosette with petal + hypotrochoid mix and per-ring rotation |
| Banding guilloches | `band` + `wave_spec` | Add `band_guilloche_spec` with edge rails, centerline threading, and corner transitions |
| Medallions | none | Add `medallion_spec` (circular frame plate + radial inset) |
| Vignette | none | Add `vignette_spec` using SVG radial gradient mask / opacity falloff |
| Multi-color-line patterns | monochrome plate per path | Add per-path `color_ramp` / plate index interpolation and `blend_mode::screen`/`multiply` for iris effect |

The brief was later amended with "latent image" and "venticular
[lenticular]". Both are deferred from this plan:

- Latent images are direction-modulated microline bands and are feasible as
  a future `latent_image_spec`, but they were not part of the approved
  five-phase pilot.
- Lenticular arrays require interlaced strip rasterization at print
  resolution and are out of scope for the current stroke-SVG pipeline.

## 4. Three grouping schemes

### Option A — Geometry-first grouping

Group by the mathematical/topological shape of the repeating unit:

1. **Radial** — rosettes, sunbursts, medallion insets, orbit nets.
2. **Banded/linear** — wave bands, banding guilloches, multi-color line
   bands.
3. **Lattice/field** — fish scales, barleycorn, moire fields.
4. **Composite/framed** — vignettes, medallions, flinque overlays.

Pros: matches the existing `composition_kind` mental model; easy to add.
Cons: mixes historical techniques and geometric classes in one axis.

### Option B — Technique/history-first grouping

Group by how the pattern is produced in traditional engraving:

1. **Engine-turned rosette/band** — rosettes, sunbursts, banding
   guilloches.
2. **Repeating relief fields** — fish scales, barleycorn.
3. **Translucent overlays** — flinque.
4. **Print/plate composites** — multi-color-line iris work, vignettes,
   medallions.

Pros: aligns with the reference vocabulary and production semantics.
Cons: less useful for solver code, because rendering logic crosses groups.

### Option C — Contract/invariant grouping

Group by shared invariants in the current spec model:

1. **Centerline-family** (one path + `family_spec` repeats).
2. **Tiling-field** (translation lattice + local cell).
3. **Masked/framed** (region + border + inset).

Pros: clean implementation contract; maps directly to new structs.
Cons: less human-readable without an extra metadata label.

### Recommendation

Use a hybrid:

- Keep `composition_kind` as the primary geometric/implementation key.
- Add a separate `pattern_class` (radial, banded, lattice, framed,
  overlay) plus `pattern_family` (`rosette`, `sunburst`, `fish_scale`,
  `barleycorn`, `flinqué`, `band_guilloche`, `medallion`, `vignette`,
  `multicolor_line`) as metadata.

This preserves the current architecture while making the historical names
first-class and queryable.

## 5. Advanced implementation for `izzi-svg-graph-guilloche.h`

The graph layer currently uses only a tiny fraction of the guilloche
engine: `make_summary()` builds a single radial rose field plus two
ornamented inscriptions. The main improvement is to make the graph layer
consume the richer pattern vocabulary below, so a flow dataset can be
rendered as a banknote-like composite rather than only a radial/chord
duo.

### 5.1 Add new geometry and composition enums in `izzi-svg-guilloche.h`

Extend `centerline_geometry`:

```cpp
struct radial_ray_spec
{
  std::size_t rays = 48;
  double radius = 1;
  double phase = 0;
};

struct harmonic_wave_spec
{
  wave_spec fundamental;
  std::vector<std::pair<std::size_t, double>> harmonics; // {multiple, gain}
  double amplitude_envelope = 0; // optional Gaussian-ish falloff
};

struct fish_scale_spec
{
  std::size_t rows = 24;
  std::size_t columns = 16;
  double scale_width = 1;
  double scale_height = 1;
  double overlap = 0.35;
  double phase = 0;
  std::size_t samples = 48;
};

struct barleycorn_spec
{
  std::size_t rows = 20;
  std::size_t columns = 12;
  double grain_width = 1;
  double grain_height = 1;
  double stagger = 0.5;
  double phase = 0;
  std::size_t samples = 48;
};
```

Extend `composition_kind`:

```cpp
enum class composition_kind
{
  rosette,
  band,
  moire_field,
  orbit_net,
  sunburst,
  fish_scale_field,
  barleycorn_field,
  band_guilloche,
  medallion,
  vignette,
  multicolor_line,
};
```

Add framing/overlay specs:

```cpp
struct vignette_spec
{
  point center;
  double radius = 1;
  double feather = 0.2;
  std::string inner_color = "#000000";
  std::string outer_color = "#000000";
  double inner_opacity = 0;
  double outer_opacity = 1;
};

struct medallion_spec
{
  point center;
  double radius = 1;
  std::size_t ring_plates = 2;
  double ring_width = 0.08;
  guilloche::scene_spec inset;
};

struct multicolor_line_spec
{
  std::size_t line_count = 5;
  double spacing = 1;
  double curvature = 0;
  std::vector<std::string> ramp; // per-line #RRGGBB
  blend_mode blend = blend_mode::screen;
};
```

These belong in `izzi-svg-guilloche.h`, with `composition_spec` gaining an
optional variant for field/frame data or a companion `std::vector` in
`scene_spec`.

### 5.2 New sampling/render functions

Implement beside `sample_centerline()` and `apply_composition_repeat()`:

- `sample_radial_ray(...)` — straight rays from center; respects phase,
  ray count, and radius.
- `sample_harmonic_wave(...)` — fundamental wave plus integer harmonics
  with a longitudinal amplitude envelope.
- `sample_fish_scale(...)` — overlapping arcs arranged on a staggered
  row/column lattice.
- `sample_barleycorn(...)` — overlapping ovals/lozenges on a staggered
  lattice.
- `render_vignette(...)` — emits an SVG radial gradient mask rather than a
  path, or a bounded falloff region when masks are unavailable.
- `render_medallion_frame(...)` — ring plates plus a clipped radial inset.
- `render_multicolor_lines(...)` — parallel curved lines with per-line
  plate color and screen/multiply blending.

Keep every new sampler deterministic, finite-checked, and bounded through
the existing `resource_budget` accounting.

### 5.3 Wire the pattern vocabulary into the graph layer

Replace the hard-coded `make_summary()` scene with a small factory:

```cpp
guilloche::scene_spec make_guilloche_composite(
  const svg::graph::graph_document& document,
  const dual_view_spec& spec,
  guilloche::pattern_family family,
  guilloche::pattern_class klass);
```

The factory maps `spec` dimensions/style/seed into one of:

- `sunburst` background plate
- `fish_scale_field` or `barleycorn_field` background
- `band_guilloche` border rails
- `medallion` center + `vignette` edge
- `multicolor_line` accent band

Then `resolve_dual_view_guilloche()` gains an optional
`guilloche::composite_spec` parameter; the default remains the current
dual-view behavior for compatibility.

### 5.4 JSON/API changes

- Bump `api_version` / scene authoring schema only if the new fields are
  exposed in JSON.
- Add parsing/serialization for the new enums and specs in
  `izzi-svg-guilloche-json.h`.
- Preserve unknown extension rules; the landing change added a top-level
  optional `pattern` object with `class` and `family` enums instead of an
  `extensions`-only rollout.
- Add `pattern_family` and `pattern_class` to manifest/metadata so the
  portal can group review artifacts.

### 5.5 Suggested rollout

1. **Phase 1 — band/wave/sunburst.** Add `radial_ray_spec`,
   `harmonic_wave_spec`, and `sunburst`; use for banding guilloches.
2. **Phase 2 — lattice fields.** Add `fish_scale_spec` and
   `barleycorn_spec` with staggered tiling.
3. **Phase 3 — framing.** Add `medallion_spec` and `vignette_spec`.
4. **Phase 4 — color/overlay.** Add `multicolor_line_spec` and flinque
   translucent overlay treatment.
5. **Phase 5 — graph-layer composite.** Wire the factory in
   `izzi-svg-graph-guilloche.h` and expose a composite mode.

Each phase should ship with baselines, a profile-bundle variant, and a
resource-budget test.

### 5.6 As-built mapping

The approved pilot landed with these concrete names:

- `radial_ray_spec {length, phase, samples}` and
  `harmonic_wave_spec {span, amplitude, fundamental_frequency, harmonics,
  phase, samples}` were added to `centerline_geometry`; sampling is inlined
  in `sample_centerline`.
- `composition_kind` gained `sunburst`, `fish_scale_field`,
  `barleycorn_field`, `band_guilloche`, `medallion`, `vignette`,
  `multicolor_line`, and `flinque`.
- `pattern_class {radial, banded, lattice, framed, overlay}` and
  `pattern_family {rosette, sunburst, fish_scale, barleycorn,
  band_guilloche, medallion, vignette, multicolor_line, flinque}` ship with
  `to_string` serialization.
- `composition_spec` gained optional `vignette`, `medallion`,
  `multicolor`, `flinque`, `fish_scale`, and `barleycorn` sub-specs plus
  `category` and `pattern_family_id` metadata.
- `scene_spec` gained `nested_scenes` and `maximum_nesting_depth`
  (default 100) for recursive medallion insets.
- `resolved_path` gained `resolved_shape_kind {stroke_path, stroke_circle,
  filled_circle, filled_rect, filled_polygon}` plus per-path
  `stroke_color`, `fill_color`, `stroke_width`, and `fill_opacity`.
- Field/frame helpers are `resolve_vignette_paths`,
  `resolve_medallion_paths`, `resolve_multicolor_paths`,
  `resolve_fish_scale_paths`, and `resolve_barleycorn_paths`.
- The graph layer uses `graph_composite_spec {category, family, enabled,
  seed_offset}`, `make_guilloche_composite(...)`, and
  `resolve_dual_view_guilloche(document, spec, composite)` with composite
  disabled by default for byte-compatible output.
- JSON exposes an optional top-level `pattern {class, family}` enum object;
  full JSON authoring of the new composition geometries remains deferred.

## 6. Decisions

- `pattern_class` is a JSON enum, not an open string tag.
- Fish-scale and barleycorn fields emit SVG paths, not `<use>` symbols.
- Flinqué is expressed only with SVG path, rect, circle, or polygon
  primitives; no mask/filter-only construction.
- Medallion insets reuse `scene_spec` recursively; the resolver warns
  beyond 100 recursion levels, and the limit is configurable.
- Latent image and lenticular remain deferred; they are not authorized in
  this stage.

No provider spend, publication, or baseline promotion is implied by this
proposal.

### Estimated effort

Assumptions: single senior C++/SVG engineer, sequential work, existing
header-only/test/baseline infrastructure, no provider spend, and no
additional visual review loops.

| Phase | Scope | Effort |
|---|---|---|
| 1 — band/wave/sunburst | `radial_ray_spec`, `harmonic_wave_spec`, `sunburst`, samplers, JSON, baselines | 1.5–2.5 days |
| 2 — fish-scale/barleycorn | `fish_scale_spec`, `barleycorn_spec`, staggered tiling, overlap/budget logic | 2–3 days |
| 3 — medallion/vignette | `medallion_spec`, `vignette_spec`, SVG masks/rings/clipping | 1.5–2.5 days |
| 4 — multicolor/flinqué | `multicolor_line_spec`, per-line ramps, translucent overlay treatment | 1.5–2.5 days |
| 5 — graph-layer composite | `izzi-svg-graph-guilloche.h` factory, optional composite mode, manifest metadata | 2–3 days |
| Integration/review | baselines, profile bundles, docs, check passes | 1–2 days |

Total sequential: about 10–14 working days, or roughly 2–3 calendar weeks.

With two contributors and the shared sampler/JSON scaffolding completed
first, calendar time could compress to about 7–9 working days. The main
risk is aesthetic iteration against the 18 Dutch-banknote references,
which could add 1–3 days per phase if the visual direction changes after
first renders.

### Pilot sequencing and parallel compute estimate

Recommended pilot scope:

1. Shared sampler and scaffolding (prerequisite).
2. Phase 1 — band/wave/sunburst.
3. Phase 2 — fish-scale/barleycorn lattice fields.
4. Phase 3 — medallion/vignette.
5. Phase 4 — multicolor/flinqué.
6. Phase 5 — graph-layer composite.

Sequencing rationale:

- Scaffolding first because Phases 1, 3, and 4 all depend on the new
  enums, structs, sampler utilities, and JSON fields.
- Phase 1 before Phases 3/4 because the radial/centerline primitives are
  reusable by medallion and multicolor work.
- Phase 2 is an independent lattice-sampler addition and can proceed
  alongside Phase 1 once the shared scaffolding lands.
- Phases 3 and 4 can then proceed in parallel worktrees and merge after
  each passes on rizal.
- Phase 5 lands after the pattern vocabulary is stable and wires the
  factory into `izzi-svg-graph-guilloche.h`.

Compute plan:

- rizal: control plane; authoring, merges, final `make check`, and publish
  decisions.
- eureka: full-gate and baseline owner (125 GiB RAM; already full gate
  green); runs CMake builds, ctest, baseline regeneration, and profile
  bundles after each phase.
- ord: secondary verification; independent profile bundles,
  resource-budget tests, and targeted SVG/PNG render checks in parallel.

Estimate:

| Work | Sequential | Three-host calendar |
|---|---|---|
| Shared sampler/scaffolding | 1.5–2.5 days | 1.5–2.5 days |
| Phase 1 | 1.5–2.5 days | 1–2 days |
| Phase 2 | 1.5–2 days | 1–1.5 days |
| Phase 3 | 1.5–2.5 days | 1–2 days |
| Phase 4 | 1.5–2.5 days | 1–2 days |
| Phase 5 | 2–3 days | 1.5–2 days |
| Integration/review | 1–2 days | 0.5–1 day |
| Total | 10.5–17 working days | 7.5–13 working days |

If Phases 2, 3, and 4 are authored in parallel worktrees after scaffolding
and Phase 1, calendar time can drop to about 4–6 working days, at higher
merge risk because the phases touch the shared guilloche headers and JSON
layer; Phase 5 follows once the shared vocabulary is stable.

### Implementation output

The approved pilot was implemented, verified, and pushed.

- Commit: `bd38cf95` — `Guilloche pilot: sunburst, harmonic bands,
  medallions, vignettes, multicolor lines, and flinque patterns`.
- Follow-up commit: `7abb0907` — `Guilloche pilot phase 2: fish-scale and
  barleycorn lattice fields`.
- Phase 5 commit: `1870a5b8` — `Guilloche pilot phase 5: graph-layer
  composite backgrounds` (`izzi-svg-graph-guilloche.h` composite factory,
  optional composite mode, manifest metadata, and composite tests).
- Engine changes: `src/izzi-svg-guilloche.h` (new enums, specs, samplers,
  composition kinds, shape rendering, recursive medallion insets).
- Phase 2 additions: `fish_scale_spec`, `barleycorn_spec`,
  `composition_kind::fish_scale_field`, and
  `composition_kind::barleycorn_field`, each emitting staggered SVG path
  lattices.
- JSON changes: `src/izzi-svg-guilloche-json.h` plus
  `schemas/izzi.guilloche-scene-1.schema.json` (`pattern.class` /
  `pattern.family` enums).
- Tests: `tests/guilloche.cc` (extended pattern vocabulary) and
  `tests/guilloche-scene-json.cc` (pattern JSON diagnostics).
- Phase 5 graph tests: all eight `pattern_family` values resolve as
  composite backgrounds and are recorded in the manifest.
- Follow-up: full JSON authoring of the new composition geometries beyond
  the `pattern` enum metadata.

```text
$ make guilloche-check
...
100% tests passed, 0 tests failed out of 22

Label Time Summary:
determinism      =  12.64 sec*proc (6 tests)
generation       = 240.36 sec*proc (11 tests)
guilloche        = 241.77 sec*proc (22 tests)
json             =   1.03 sec*proc (5 tests)
schema           =   0.20 sec*proc (2 tests)
visual           = 240.35 sec*proc (9 tests)

Total Test time (real) = 241.78 sec

$ ctest --test-dir build/cmake -R '^graph.guilloche-synthesis'
100% tests passed, 0 tests failed out of 3

$ git push
To github.com:bdekoz/izzi.git
   c0c65e1a..bd38cf95  main -> main
```

### Current status

The full five-phase pilot is implemented, verified, and pushed to
`origin/main`.

- Phase 1 — band/wave/sunburst: done.
- Phase 2 — fish-scale/barleycorn lattice fields: done.
- Phase 3 — medallion/vignette: done.
- Phase 4 — multicolor/flinqué: done.
- Phase 5 — graph-layer composite backgrounds: done.

Commits:

- `bd38cf95` — initial pilot (scaffolding, Phases 1, 3, 4).
- `7abb0907` — Phase 2.
- `1870a5b8` — Phase 5.
- `d27bfa4c` — refinement-output record for Phase 5.

Verification: `make guilloche-check` passes 23/23 and the graph guilloche
synthesis suite passes 3/3, including the byte-compatible default path.

Downstream: Stage 3 generated 27 parameter-space capability plates from
these capabilities and published the live
`generation-guilloche-20260816` review family at
`https://situationshipin.space/review/media/generation-guilloche-20260816/generation-guilloche-20260816-index.index.html`.

Remaining follow-up: full JSON authoring of the new composition geometries
beyond the `pattern.class` / `pattern.family` enum metadata. No provider
spend, publication, or baseline promotion is implied by this work.
