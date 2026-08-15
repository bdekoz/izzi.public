# Alpha60 complete conversion plan (R2)

Date: 2026-08-15

Status: `PLANNED; SEPARATE-PROJECT; AWAITING-APPROVAL; ZERO-SPEND`

The alpha60 producer migration (R2), moved out of
`explore_futures.20260814.release_post.md` into this dedicated plan. It
completes Stage A of
[`explore_futures.20260814.migration.md`](explore_futures.20260814.migration.md):
migrate the five alpha60 consumers of the legacy `a60-svg-collection-augment.h`
onto the izzi facades, then retire the legacy render path.

## Goal

Every alpha60-produced visualization is rendered by the izzi facades
(visualization/movement/color/pattern) through the script-based pipeline;
the legacy augment header and render path are removed together behind the
one-cycle retirement gate.

## Current state (evidence)

- Site side: **complete** — `alpha60-results-animation` `_includes` are
  izzi-generated (cold cutover, `b865ad8`), regeneration runs through the
  vendored pipeline (`ci/izzi/`), and the `regenerate-includes` workflow is
  verified (R4).
- Library side: `a60-svg-collection-augment.h` (1603 lines) is still
  consumed by five production files:
  `a60-meta-collection-factory.cc`, `a60-cache-recache-synthesize-uniques.cc`,
  `a60-btiha-geojson.h`, `a60-ip-analysis.h`,
  `a60-collection-analyze-multi-year.cc`.
- Data adapters exist (`build-alpha60-visualizations.py`); the weighed-by-
  torrent series is covered; cartography/slice shapes are next.

## Phases

### Phase 1 — Inventory

- Enumerate each consumer's render entry points and output shapes
  (weighed-duration / weighed-by-torrent augments, cartography and slice
  layouts, metadata augments) and their inputs (unique-BTIHA JSON, GeoJSON,
  IP analysis).
- Record output-file inventory per consumer (stems, kinds, byte ranges).

### Phase 2 — Shape map

- Assign every augment shape to an izzi emitter: visualization
  (grids/tables/line graphs), movement (camera/path augments), color
  (augment palettes), pattern (existing families).
- Note gaps: shapes with no current emitter get a facade extension or an
  explicit documented substitute.

### Phase 3 — Adapter extension

- Extend `build-alpha60-visualizations.py` for the augment shapes
  (cartography/slices next after weighed-by-torrent), with per-source
  sha-256 provenance and legacy-name stems where the site requires them.
- Regenerate the site `_includes` from the extended adapter; diff review.

### Phase 4 — Producer rewiring

- Route each consumer's render step through the pipeline (documents →
  `izzi_visualization_render` / WASM driver) or replace the inline render
  call with facade emitters; keep the data products (JSON/GeoJSON)
  unchanged.
- Verify per consumer: byte-determinism, accessibility contract, no
  behavior change to the data products.

### Phase 5 — Retirement gate

- After one full downstream render cycle on regenerated outputs passes
  review: remove `a60-svg-collection-augment.h` and the legacy render path
  together (no compatibility aliases), rebuild alpha60, and re-run
  `make check-wasm`.

## Verification

- Per phase: alpha60 production build, determinism (two-run byte identity),
  accessibility contract, `make check-wasm` (cartofreako).
- Full `make check` in izzi after any facade changes; migration contract
  CANONICAL-READY.

## Estimate

~2-3 working days (15-25 h): inventory 1-2 h, shape map 2-4 h, adapter
extension 2-3 h, producer rewiring 8-16 h, retirement gate + rebuild +
wasm checks 2-4 h.

## Gates

- Separate-project approval required before Phase 4 rewiring.
- The retirement gate (Phase 5) requires one full downstream render cycle
  and human review.
- Zero provider spend; local-first; dry-run-before-apply.

## Execution (2026-08-14, "read and process")

### Phase 1 — Inventory (recorded)

Render entry points per consumer (evidence from `alpha60/src`):

| Consumer | Entry points | Shapes |
| --- | --- | --- |
| `a60-meta-collection-factory.cc` | `setup_render` | meta-collection renders (tables/line graphs) |
| `a60-cache-recache-synthesize-uniques.cc` | `meta_render` → `augment_*` | weighed-by-torrent, weighed-duration, weighed-cumulative-state, stateless, cartography/slices |
| `a60-btiha-geojson.h` | geojson serialization + augment header | cartography overlays over GeoJSON features |
| `a60-ip-analysis.h` | `augment_swarm_ips` | weighted state + cartography renders for IP analysis |
| `a60-collection-analyze-multi-year.cc` | render-state routines | multi-year analysis visuals |

Augment shapes (from `a60-svg-collection-augment.h`):
`augment_weighed_by_torrent`, `_by_partition`, `_swarm_vdata`,
`_vcarto_layers`, `_vcarto_composite`, `_variations`, `_once`, `_slice`,
`_cumulative_slice`, `_swarm_duration`, `_weighed_duration_since`,
`_by_sample`, `_stateless`, `_weighed_cumulative_state`,
`_weighed_duration`. GeoJSON slice features carry `country_code`, `city`,
`geoname_id`, and downloader/uploader breakdowns.

### Phase 1 — call-site inventory (implemented, 2026-08-14)

Concrete call sites and output stems from `alpha60/src`:

- `a60-meta-collection-factory.cc:236` — `setup_render(cll, peer, true,
  false)`.
- `a60-cache-recache-synthesize-uniques.cc` — `augment_unique_collection`
  (line 241) → `augment_unique_collection_ck` (line 156), which calls
  `augment_stateless` (179), `augment_weighed_cumulative_state` with the
  cartography variants ckh4/ckh3/ckh3_engc/ckh2 (187-233).
- `a60-ip-analysis.h:34` — `augment_swarm_ips` → `augment_weighed_cumulative_state`
  (line 50).
- `a60-collection-analyze-multi-year.cc:127` — direct `render_state`
  manipulation.
- `a60-btiha-geojson.h` — includes the augment header for cartography
  overlays over GeoJSON features.

Output stems (log + paired SVG): `<match>-augment-weighed-duration-since-<d>`,
`<match>-augment-weighed-by-torrent`, `-weighed-by-partition`,
`-weighed-variations-<match>`, `-weighed-by-sample`, plus the
weighed-cumulative-state and cartography variants.

### Phase 2 — Shape map (recorded)

- Visualization: line-graph (weighed-by-torrent / duration / sample series),
  table (metadata, media objects, geo slices), grid (vdata / variations /
  partitions).
- Movement: no camera/path shapes in these augments — n/a.
- Color: augment palettes come from the existing izzi color family (render
  style choice only).
- Pattern: cartography overlays map to the existing pattern families.

### Phase 3 — Adapter extension (implemented)

`build-alpha60-visualizations.py` geo-slice handling extended: GeoJSON
features now emit a **geo-slices table** (`country_code`, `city`,
downloader size, uploader size) and, where a numeric downloader size exists,
a **country-ranked line**. Regeneration + sha-256 provenance re-run.

### Phase 4 — decision needed

Producer rewiring (the five C++ consumers onto the pipeline/facades) is the
separate-project gate: **not started** — requires explicit approval.

## Phase 4 decision gate — pilot plan (proposal, 2026-08-14)

Goal: produce a bounded pilot assessment that gives the human everything
needed to approve (or revise) the Phase 4 producer rewiring, without any
alpha60 C++ changes.

### Pilot selection

**`augment_weighed_by_torrent`** from `a60-cache-recache-synthesize-uniques.cc`
on one collection (e.g., the `animation`-class data present in
`alpha60-results-animation/data`). Rationale: the adapter already emits the
weighed-by-torrent series (`<match>-weighed-by-torrent-*.svg`), so the pilot
can compare the legacy output against the izzi pipeline directly.

### Pilot steps

1. **Legacy capture** — run the producer's weighed-by-torrent augment for the
   selected collection (or use an existing output) → reference SVG + log;
   record stem, bytes, sha-256.
2. **izzi re-create** — build the visualization document from the same data
   via `build-alpha60-visualizations.py` and render through
   `izzi_visualization_render` (native and WASM); record the same metadata.
3. **Assessment** — compare: geometry overlap (visual diff), labels and
   accessibility contract (title/desc/role), determinism (two-run byte
   identity), provenance (source sha-256), and styling gaps (palette, stroke,
   cartography overlays, metadata annotations).
4. **Evidence package** — pilot results + Phase 2 shape map + adapter
   coverage + the remaining four consumers' impact, presented for the Phase 4
   approval decision.

### Decision gate

Human approves Phase 4 rewiring (pilot pattern generalizes), or revises the
approach (e.g., facade gap fixes, different pilot). No alpha60 C++ changes
happen before the gate.

### Time to the decision gate

- Pilot legacy capture + izzi render: 1-2 h.
- Comparison + assessment: 1-2 h.
- Evidence package + write-up: ~1 h.

**Total: ~0.5-1 working day** to the Phase 4 decision gate.

## Pilot assessment — decision-gate evidence (2026-08-14)

Pilot: `augment_weighed_by_torrent` for `altered-carbon-resleeved`
(83 media objects from
`data/altered-carbon-resleeved-cumulative-btiha-media-objects.json`,
sha-256 `76c7dd3e…`).

### Results

- **Data fidelity: PASS** — the izzi weighed-by-torrent document carries 83
  points whose `size_total` values are byte-identical to the source JSON
  (first three: 4.830381763, 0.729931714, 2.787039635), i.e., the pipeline
  faithfully represents the shared data.
- **Determinism: PASS** — two isolated renders of the izzi document are
  byte-identical.
- **Accessibility: PASS** — the rendered SVG carries `<title>`, `<desc>`,
  and `role="img"`.
- **Provenance: PASS** — source sha-256 recorded per document; the legacy
  producer's analysis log (`altered-carbon-resleeved-unique-btiha-cumulative.log`,
  sha-256 `83d20fae…`) was captured from `a60-meta-collection-factory`.

### Finding (legacy visual reproduction)

Reproducing the legacy producer's augment **SVG** locally was blocked by the
recache pipeline's cache-layout dependency
(`a60-cache-recache-synthesize-uniques` requires the elapsed-hour/current
timestamp sample trees; the torrent cache alone does not satisfy it), so the
pilot compared the izzi render against the **shared data + analysis log**
rather than a byte-level visual diff. This is a producer-environment
dependency for Phase 4/5, not an izzi-side gap.

### Decision-gate summary

The izzi pipeline is data-faithful, deterministic, accessible, and
provenance-bound for the pilot shape; the remaining Phase 4 risk is
producer-side (cache layout, styling parity for cartography/metadata
overlays). Recommendation: approve Phase 4 rewiring with the pilot pattern,
phased per consumer, with the legacy-SVG visual diff deferred until the
producer cache-layout dependency is resolved.


Before starting, assess which one of the consumers would be the easiest test conversion pilot. Preference is a60-meta-collection-factory.cc

Suggest implementation plan for pilot by appending to this file in "Response 1" section below.
