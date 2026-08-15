# Alpha60 complete conversion plan (R2)

Date: 2026-08-15

Status: `PLANNED; SEPARATE-PROJECT; AWAITING-APPROVAL; ZERO-SPEND`

The alpha60 producer migration (R2), moved out of
`explore_futures.20260815.release_post.md` into this dedicated plan. It
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
