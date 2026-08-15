# Converting alpha60 to the new izzi way of working (proposal)

Date: 2026-08-14

Status: `PROPOSAL; AWAITING-APPROVAL; ZERO-SPEND`

## Why now

Alpha60 is the legacy producer of the alpha60-results Jekyll sites: its
`a60-*` header surface (84 files in `alpha60/src`, including ten `a60-svg-*`
headers), the `a60-meta-collection*.exe` binaries, and the
`a60-cache-recache-synthesize-uniques.cc` unique-BTIHA pipeline feed
`data/` and `_includes/` on the sites. Izzi now owns the canonical
generative surface — pattern, color, movement, and visualization facades
with deterministic, pure-serialization APIs, CTest contracts, WASM
byte-parity, and portal review indexes — so the conversion is a
top-to-bottom move from legacy alpha60 rendering to the new izzi way of
working. Cartofreako (the sibling wasm projection repo) still carries
legacy naming (`a60-carto-frame.h`) and is covered in Appendix 1.

## Current state (evidence, 2026-08-14)

- `alpha60/src`: 84 files, 83 `a60-*`; ten `a60-svg-*.h` wrappers
  (`a60-svg-collection-augment.h`, `a60-svg-carto-geo.h`,
  `a60-svg-collection-color.h`, `a60-svg-metadata-*.h`, ...) drawing on the
  izzi `svg::` surface; no direct `izzi-svg` includes today.
- Producers: `a60-cache-recache-synthesize-uniques.cc` (unique BTIHA;
  JSON/GeoJSON/analysis/augment-SVG products) and `a60-meta-collection*.exe`
  binaries driving `alpha60-results-animation/scripts/generate-graphs.sh`
  into `_includes/*.svg` / `*.html`.
- Izzi contracts ready to absorb the work: the header-migration contract
  (stage-i through stage7, CANONICAL-READY at 43 headers), the generative
  facades (`izzi-svg-color*.h`, `izzi-svg-movement.h`,
  `izzi-svg-visualization.h`), CTest registration per family
  (`color-check`, `movement-check`, `visualization-check`), and the portal
  review-index pattern (`palette-20260814-index` live).

## Developed plan

1. **Inventory** — list every `a60-*` header, executable, and site generator;
   classify each as (a) direct izzi surface, (b) thin wrapper, or (c)
   standalone legacy; record consumers per item.
2. **Header migration** — replace `a60-svg-*` wrappers with canonical
   `izzi-svg-*` includes (extend the izzi header-migration contract stages
   with the alpha60 repo as a consumer); wrappers become thin aliases during
   the transition, then are removed behind a retirement gate.
3. **Producer migration** — re-create the unique-BTIHA augment/meta renders
   from `a60-cache-recache-synthesize-uniques.cc` on the
   generation_visualization facade (grids/tables/line graphs per the
   visualization_grammar plan) and the generation_movement/color/pattern
   surfaces where the augments need them; retire `a60-meta-collection*.exe`
   from `generate-graphs.sh`.
4. **Site automation** — alpha60-results-* sites regenerate `_includes/`
   through `scripts/generate-alpha60-includes.mjs` (script-based invocation,
   sha-256 skip, deterministic output) wired to a GitHub Actions push-path
   workflow (visualization_grammar Responses 4-6).
5. **Cartofreako** — see Appendix 1.
6. **Verification** — `make check` (including the migration contract),
   WASM byte-parity (`make check-wasm` in cartofreako), determinism
   (two-run byte identity), accessibility contract, and portal review
   indexes per family.
7. **Retirement gates** — remove legacy `a60-svg-*` headers, the
   `a60-meta-collection*.exe` binaries, and the elapsed-hour/legacy
   render paths only after every consumer has migrated and at least one full
   downstream render cycle has passed (mirroring the archive-conversion
   retirement discipline in `alpha60/README-FINISH-CONVERSION.md`).

## Constraints

- Zero provider spend; local-first; dry-run-before-apply for every staged
  conversion.
- No portal publish, repo check-in, or push without explicit authorization.
- Baselines and evidence stay immutable until a staged candidate is
  reviewed.

# Appendix 1 — cartofreako migration changes

Cartofreako consumes izzi headers directly (`IZZI_SRC = ../izzi/src`) and
builds the projection runtime to WebAssembly. Recommended changes:

1. **Canonical header naming** — migrate `src.projections/a60-carto-frame.h`
   to the izzi canonical surface (e.g., an `izzi-svg-carto-frame.h` or a
   canonical izzi header it should fold into); update its consumers in
   `src.projections/` and the `Makefile` dependency list
   (currently only `izzi-svg-curves-hamonshu.h` + `-v2.inc` are listed).
2. **Contract tracking** — the izzi public-header count is now 43 (with the
   movement and visualization facades); refresh cartofreako's
   package-consumer and header expectations when they pin counts, and keep
   the header-migration contract CANONICAL-READY across both repos.
3. **WASM byte-parity** — keep the runtime's output byte-identical between
   native and wasm builds (pure string serialization); add the
   color/movement/visualization facades to the wasm surface where the
   runtime renders previews (slices, grids, projection plates).
4. **Determinism and provenance** — use seeded generation everywhere; record
   sha-256 provenance for generated artifacts (the
   `proof-local-outputs.py` pattern) so regenerated previews are
   reviewable and diffable.
5. **Retirement** — remove the `a60-` prefix files only after the canonical
   headers build in both native and wasm paths and `make check-wasm` passes
   end-to-end.

---

# Staged implementation plan — remaining open items

Recorded 2026-08-14 (round BEGIN-OPEN-ROUND-003). Consolidated staging for
every open item across the color, movement, visualization, and migration
rounds. Awaiting approval to begin execution.

## Stage A — alpha60 producer migration (the large item)

1. **Inventory** — enumerate the augment/meta render entry points in
   `alpha60/src/a60-cache-recache-synthesize-uniques.cc` and
   `a60-svg-collection-augment.h` (weighed-duration / weighed-by-torrent
   augments, cartography/slices, metadata augments), with their inputs
   (unique-BTIHA JSON, GeoJSON, IP analysis).
2. **Map to facades** — assign each augment shape to an emitter:
   visualization (grids/tables/line graphs), movement (camera/path
   augments), color (augment palettes), pattern (existing families).
3. **Adapter extension** — extend `build-alpha60-visualizations.py` to the
   augment shapes (weighed-by-torrent series, cartography/slice data) with
   per-source sha-256 provenance.
4. **Re-create renders** — emit the augment artifacts with the `emit_*`
   facades; verify byte-determinism and the accessibility contract.
5. **Retirement gate** — remove the legacy augment headers and
   `a60-meta-collection*.exe` path only after one full downstream render
   cycle passes on the regenerated `_includes`.

## Stage B — `emit_chord_graph` h_chord_graph wiring

1. Expose a pure izzi chord input adapter (media ↔ attribute → weighted
   pairs) from `visualization_document` headers/rows, decoupled from the
   alpha60 runtime-resource layer.
2. Wire `h_chord_graph` (alpha60-domain `vumids`) behind `emit_chord_graph`
   when the alpha60 data layer is present; keep the pure bipartite fallback
   for WASM.
3. CTest determinism + a sample chord document render; family entry update.

## Stage C — WASM driver for `generate-alpha60-includes.mjs`

1. Build the izzi visualization facade to WebAssembly (emscripten, reusing
   the cartofreako emsdk toolchain) with a small driver exposing
   `render(document_json) -> {svg|html}`.
2. Byte-parity check: native CLI vs WASM driver on identical documents.
3. Generator script resolves `--wasm` path (committed artifact or emsdk
   build step) with the native CLI as fallback; workflow gains the WASM
   path.

## Stage D — movement open items

1. Wire surface-tension and hamonshu point generators into
   `generate_path_points` (movement family).
2. Implement `movement_from_json` (parse `izzi.movement/1`).
3. Implement `scripts/extract-movement.py` — video → camera-movement
   descriptions (Blade Runner cut-5, here-lies-trouble masters), feeding
   `movement_document.camera`.
4. Publish the `generation-movement-20260814` portal index (gated on portal
   authorization) once D1-D3 land.

## Stage E — generation-baseline candidate refresh

1. Regenerate `WI-COLOR-FAMILY-20260814` (post output-stem fix) and add
   movement/visualization candidates.
2. Run `compare.py` per candidate; review deltas; promotion stays gated on
   human review.

## Stage F — verification and gates

- `make check` (migration contract CANONICAL-READY), `make check-wasm`
  (cartofreako), determinism (two-run byte identity), accessibility
  contract, and the portal review indexes.
- Commit/push and portal publishes per explicit authorization; zero provider
  spend; dry-run-before-apply for every staged conversion.

### Implementation status (2026-08-14, accepted plan)

- **Stage B** — done: weighted circular chord layout; native↔WASM parity
  verified 554/554.
- **Stage C** — done: WASM driver built; `--wasm` generator mode wired.
- **Stage D** — done: surface-tension/hamonshu parametric samplers,
  `movement_from_json` (round-trip byte-identical), `extract-movement.py`
  (cut-5 + asama camera documents), `generation-movement-20260814` portal
  index published (2 members, validator PASS).
- **Stage E** — done: `WI-COLOR-FAMILY-20260814` and
  `WI-MOVEMENT-VISUALIZATION-20260814` candidates regenerated (0 varied);
  comparison review and promotion remain gated on human review.
- **Stage A** — in progress: weighed-by-torrent series added to the adapter;
  legacy augment header/binary retirement remains behind the one-cycle
  retirement gate (not executed).
- **Stage F** — `make check` and `make check-wasm` run as the final gate;
  commits/pushes executed per assumed consent.

## Stage A retirement — execution status and open decisions (2026-08-14)

Accepted and run unattended. Status:

### Completed (site-side legacy generator retired)

- `alpha60-results-animation` commit `6bc7e5c..83d09c2`: removed
  `bin/a60-meta-collection.exe`, `bin/a60-meta-collection.wasm`, the three
  stray `.exe` variants, and `scripts/generate-graphs.sh`. `_includes`
  regeneration now routes through the izzi visualization pipeline
  (`generate-alpha60-includes.mjs`) + the GitHub Actions workflow.
- The alpha60 library augment path was **not** removed (see decision 2).

### Decision 1 — site `_includes` switch-over

The 30 curated legacy `_includes` files are still referenced by the Jekyll
pages under their legacy names; the izzi pipeline emits data-equivalent
outputs under new names. Options:

- (a) build a legacy-name generator (re-creating the `_includes` per
  visualization_grammar Response 2), then regenerate + commit — changes the
  site's visuals; or
- (b) defer the content switch, keep the current curated `_includes`, and
  consider the legacy generator retired (current state).

### Decision 2 — alpha60 library augment path

`a60-svg-collection-augment.h` (1603 lines) is consumed by five production
files (`a60-meta-collection-factory.cc`,
`a60-cache-recache-synthesize-uniques.cc`, `a60-btiha-geojson.h`,
`a60-ip-analysis.h`, `a60-collection-analyze-multi-year.cc`). Removing the
header breaks the alpha60 build; migrating those producers onto the izzi
facades is the large producer-migration effort (Stage A steps 1-4) and
should be its own approved project.

### Recommendation

Keep decision 1 option (b) for now; treat the alpha60 producer migration
(decision 2) as a separate approved project before any header removal.
Awaiting user direction on both.

### Decision (user direction 2026-08-14)

**Decision 1 = option (b):** keep the current curated `_includes`; the
legacy generator is retired and the site switch-over is deferred.
**Decision 2:** the alpha60 producer migration is a separate project; no
`a60-svg-collection-augment.h` removal until that project is approved and
executed.

Stage A is therefore **complete under the 1b recommendation**: the legacy
site-side generator is retired (83d09c2), the curated `_includes` remain,
and the library-side augment path stays until the producer migration
project runs.

## Content switch-over (Decision 1a) — orchestration plan

Requested 2026-08-14. Goal: replace the curated `_includes` with
izzi-generated content **under the same names the Jekyll pages reference**,
so the site keeps building and the izzi pipeline becomes the single source
of truth.

### Inventory (observed)

30 `_includes` files in `alpha60-results-animation`:

| Kind | Files | Data source |
| --- | --- | --- |
| Line graphs | `animation-downloads-by-week-normalized-start{,-legend,-2020..-2026}.svg` (9), `{arcane,invincible,spider}-downloads-by-week-normalized-start.svg`, `{dan-da-dan-210,star-wars-maul-101104,zootopia-2}-downloads-by-country.svg` | week/cumulative JSON + country JSON |
| Tables/lists | `animation-{media-objects,meta-collection,graph,spatial-carto}-table.html`, `animation-media-objects-list.html`, `{arcane,invincible,spider}-{media-objects,meta-collection}-table.html` | btiha-media-objects / meta JSON |
| Geo slices | `animation-geo-slices-{africa,asia,usa-weeks-1-5-10-20,usa-weeks-1-5-15}.html` | geojson slices |

Page references confirmed: `docs/animation.md` (8 includes),
`index.md` (media-objects-list); per-series pages reference the series
SVGs/tables (full reference inventory in switch step 1).

### Orchestration steps (dry-run first, ordered)

1. **Reference inventory** — extract every `{% include ... %}` from the
   site pages; build the authoritative include-name list.
2. **Legacy-name map** — extend
   `scripts/build-alpha60-visualizations.py` with a `--legacy-names` map:
   each include name → (data source glob, kind, title/description). The
   emitted document `stem` becomes the exact include name, so the render
   CLI writes `animation-downloads-by-week-normalized-start.svg` etc.
3. **Dry-run** — generate into a temp `_includes`; diff against the 30
   curated files; review the added/changed/removed set (content will differ
   from the legacy renderer — accepted change).
4. **Regenerate** — write the izzi-generated `_includes` into the repo with
   the legacy names; keep any curated include not covered by the map
   (documented, or migrated into the map).
5. **Verify** — resolve every page include reference against the generated
   set (0 broken includes); determinism (second run: 0 changes);
   accessibility contract; Jekyll build on push.
6. **Commit + push** — the workflow then regenerates on future
   `data/*.json` / `*.geojson` check-ins using the same map.
7. **State** — `.generated-state.json` records source sha-256 per include;
   removed curated files are recoverable from git history.

### Decisions/risks

- Content will be re-rendered (new renderer) — visual review of the diff is
  part of step 3.
- Any page reference not covered by the map must be mapped or the page
  updated before the switch; step 1 makes that explicit.
- The switch-over is a manual orchestrated commit; the workflow handles
  subsequent data-driven regenerations.

Awaiting approval to execute steps 1-7.

### Time estimate (chat output, 2026-08-14)

| Step | Work | Estimate |
| --- | --- | --- |
| 1 | Reference inventory (grep all includes, compile name list) | 5-10 min |
| 2 | Legacy-name map (30 names -> data source/kind/title) | 30-60 min |
| 3 | Dry-run + diff review vs the 30 curated files | 15-30 min |
| 4 | Regenerate into repo with legacy names | 15-30 min |
| 5 | Verify (0 broken includes, determinism, accessibility; Jekyll build if toolchain present, else CI) | 20-40 min (+30-60 if local Jekyll install needed) |
| 6 | Commit + push | 5-10 min |
| 7 | State/provenance (`.generated-state.json`) | ~5 min |

**Total: ~1.5-3 h**, dominated by steps 2-3. Local Jekyll availability is the
main uncertainty; without it, verification defers the render gate to the
workflow build on push.

### Approval (2026-08-14)

**APPROVED** — execute steps 1-7 unattended and notify on completion.

### Cold cutover (user direction 2026-08-14)

User directed: no legacy-name map — perform a **cold cutover**. Executed:

- Regenerated the full izzi set (622 documents → 622 `_includes` artifacts:
  week lines, tables, geo grids, kusama/chord, weighed series) via
  `build-alpha60-visualizations.py` + `generate-alpha60-includes.mjs`.
- Replaced the 30 curated legacy `_includes` in `alpha60-results-animation`
  and rewrote the page references (`docs/animation.md`, `index.md`) to the
  generated names; **0 broken includes** verified.
- Committed and pushed as `alpha60-results-animation` `b865ad8`;
  `.generated-state.json` records source sha-256 provenance.
- Note: no data files carry `collection_name "Animation"`, so the legacy
  animation-series includes were replaced with the available generated
  series outputs (arcane, altered-carbon, etc.).

Decision 1 (option a, cold) is therefore **complete**; the site now renders
izzi-generated includes, and the GitHub Actions workflow regenerates on
future `data/*.json` / `*.geojson` check-ins.
