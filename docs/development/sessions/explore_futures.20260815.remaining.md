# Remaining items — detailed implementation plan

Date: 2026-08-15

Status: `PLANNED; AWAITING-APPROVAL; ZERO-SPEND`

Companion to `explore_futures.20260814.release.md` (v3.20260815 released).
This plan details the implementation of every remaining open item, in
priority order.

## R1 — Generation-baseline promotion

Goal: bring the promoted generation baseline current with the released
tree, or document the deferred decision.

1. Run `compare.py` for each regenerated candidate
   (`WI-COLOR-FAMILY-20260814`, `WI-MOVEMENT-VISUALIZATION-20260814`)
   against the promoted `current` baseline; capture the inventory deltas
   (added/renamed/removed examples, byte changes).
2. Review the deltas: typography bytes (Atkinson Hyperlegible), color
   determinism (0 varied), movement/visualization additions, stale-baseline
   removals (e.g., `color-band-next`, `color-rgb-to-hsv-1` moved to
   `tests/`).
3. On human acceptance: promote the candidate to `current` (history
   preserved under `tests/baselines/generation/history/`), regenerate the
   grid/PDF evidence, and update the counts in
   `docs/development/milestone_0_baseline.md` and the generation README.
4. If not accepted: keep the candidates unpromoted and record the review
   receipt; no silent promotion.

Verification: `tests/generation-baseline/compare.py` PASS against the
promoted manifest; link audit; `make check`.

## R2 — Alpha60 producer migration (large item)

Goal: migrate the five consumers of `a60-svg-collection-augment.h`
(`a60-meta-collection-factory.cc`, `a60-cache-recache-synthesize-uniques.cc`,
`a60-btiha-geojson.h`, `a60-ip-analysis.h`,
`a60-collection-analyze-multi-year.cc`) onto the izzi facades, then retire
the legacy augment header.

1. **Inventory** — enumerate each consumer's render entry points and output
   shapes (weighed-duration / weighed-by-torrent augments, cartography and
   slice layouts, metadata augments) and their inputs (unique-BTIHA JSON,
   GeoJSON, IP analysis).
2. **Shape map** — assign every augment shape to an izzi emitter:
   visualization (grids/tables/line graphs), movement (camera/path
   augments), color (augment palettes), pattern (existing families).
3. **Adapter extension** — extend `build-alpha60-visualizations.py` for the
   augment shapes (weighed-by-torrent series done; cartography/slices next),
   with per-source sha-256 provenance.
4. **Producer rewiring** — route each consumer's render step through the
   pipeline (documents → `izzi_visualization_render` / WASM driver) or
   replace the inline render call with facade emitters; keep the data
   products (JSON/GeoJSON) unchanged.
5. **Retirement gate** — after one full downstream render cycle on
   regenerated outputs passes review, remove
   `a60-svg-collection-augment.h` and the legacy render path together (no
   compatibility aliases), then rebuild alpha60 and re-run the wasm checks.

Verification per stage: alpha60 production build, determinism (two-run byte
identity), accessibility contract, `make check-wasm`.

## R3 — Movement/visualization polish

Goal: replace the parametric fallbacks with deep family wiring.

1. **Surface-tension sampling** — wire `source_spec` / `contour_layer_spec`
   point generation into `movement::generate_path_points` for the named
   kinds (`paired-drops`, `vertical-chain`).
2. **Hamonshu sampling** — wire `motif_config` point generation for
   `nested-current-scrolls` and the catalogue motifs.
3. **Chord** — `emit_chord_graph` is already a weighted circular layout;
   keep the alpha60-domain `h_chord_graph` out of the pure izzi surface.
4. **Docs/tests** — update the family entries, add sampling CTests, and
   refresh the movement/visualization baseline candidates.

Verification: `make movement-check`, `make visualization-check`,
determinism, byte-parity (native vs WASM).

## R4 — Site switch-over verification

Goal: confirm the cold-cutover site builds and regenerates in CI.

1. Watch the GitHub Actions build/deploy for `alpha60-results-animation`
   after `b865ad8`; fix any Jekyll include/asset failures.
2. Trigger a data check-in (or `workflow_dispatch`) to verify the
   regenerate-includes workflow produces zero unexpected diffs
   (`.generated-state.json` skip path).
3. Record the CI receipt in the migration doc.

Verification: Jekyll build PASS, workflow regeneration PASS, 0 broken
includes.

## R5 — Documentation and release follow-ups

1. Post-promotion numbers (R1) in `milestone_0_baseline.md` and the
   generation README.
2. Public mirror refresh after any doc/baseline changes
   (`publish-public-mirror.sh --push --regen-doxygen`).
3. Portal index updates if new review artifacts land (pattern/generation
   indexes stay current).

## Gates

- Baseline promotion and any portal publish require explicit human review /
  authorization.
- R2 header removal requires the one-cycle downstream gate.
- Zero provider spend; local-first; dry-run-before-apply throughout.

## Time estimates (R1-R5, 2026-08-14)

| Item | Work | Estimate |
| --- | --- | --- |
| R1 — baseline promotion | compare.py per candidate (~1-2 min each), delta review (typography/color/movement/visualization changes), promotion with history, doc count updates | 1.5-3 h (dominated by delta review; human acceptance required) |
| R2 — alpha60 producer migration | inventory 5 consumers (1-2 h); shape map to emitters (2-4 h); adapter extension (2-3 h); producer rewiring in 5 C++ files (8-16 h); retirement gate + alpha60 rebuild + wasm checks (2-4 h) | ~2-3 working days (15-25 h) |
| R3 — movement/visualization polish | surface-tension + hamonshu point-generator wiring (4-6 h), CTest + doc updates + candidate refresh (1-2 h) | 5-8 h |
| R4 — site switch-over verification | CI Jekyll build watch + fix (0.5-1 h), workflow regeneration verification + receipt (0.5-1 h) | 1-2 h (CI wait included) |
| R5 — docs/release follow-ups | post-promotion numbers, mirror refresh (~10 min), portal index updates | 1-2 h |

Ordering: R4 first (fast, unblocks CI confidence), then R1 (promotion gate),
then R3 (polish), then R2 (large, its own project), with R5 following each
landed change. Total focused effort: roughly **4-6 working days**, dominated
by R2.

## Execution status (2026-08-14/15)

### R4 — complete

- GitHub Actions "Deploy Jekyll" for the cold cutover (`b865ad8`) **success**
  — the site builds with izzi-generated includes.
- Regeneration idempotent against the committed `_includes`: 622 files, 0
  byte-differing.
- **CI fix**: the `regenerate-includes` workflow's cross-repo izzi checkout
  failed (private repo, 404). Fixed by vendoring the pipeline
  (`alpha60-results-animation/ci/izzi/`: adapter, generator, WASM facade)
  and dropping the checkout; also made the generator state deterministic
  (removed `rendered_at` churn) and added rebase-before-push for concurrent
  runs. Re-verified: **both** push-triggered and `workflow_dispatch` runs
  complete success (`73076fc`).

### R5 — complete

- Post-promotion numbers: n/a (R1 promotion deferred, gated on human
  review).
- Public mirror refresh performed after the doc changes
  (`publish-public-mirror.sh --push`).
- Portal: no new review artifacts this round; existing indexes stay current.
