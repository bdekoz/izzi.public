# Generation family refresh

Recorded: 2026-08-15 America/Los_Angeles
Status: `IMPLEMENTED; LOCAL; ZERO-SPEND`

Every izzi generation family has a `make <family>-review` target that
regenerates its review artifacts from the current tree (top of tree), plus a
portal publish script on situationshipin.space that copies those artifacts
into the review catalog with the izzi commit recorded.

## Targets

| Target | Family | Produces | Portal publish |
|---|---|---|---|
| `make movement-review` | movement | `outputs/review/feedback/visual/movement/round-01/` — `movement-asama-loops.svg/.png`, `movement-camera-blade-runner.svg/.png`, `manifest.json` | `scripts/publish-movement-index.mjs --izzi-commit <sha> --artifacts-dir <dir>` |
| `make pattern-review` | patterns (moiré, surface tension, guilloche, interference motion) | moire.v1 / surface-tension.v1 plates + motion tests; guilloche round-05 static proofs | `scripts/publish-make-check-generation.mjs --commit <sha> --reference-set <dir>` (after `make generation-snapshot`) |
| `make color-review` | color | `outputs/review/feedback/visual/color/round-01/` palette SVGs | `scripts/publish-palette-index.mjs --izzi-commit <sha> --review-dir <dir>` |
| `make visualization-review` | visualization | `outputs/review/feedback/visual/visualization/round-01/` — `visualization-{line,grid,kusama,chord}.svg/.png`, `manifest.json` | `scripts/publish-visualization-index.mjs --izzi-commit <sha> --artifacts-dir <dir>` |
| `make synthesis-review` | synthesis (3D + 4D) | `3d-synthesis/round-01/` (27 proofs) + `4d-synthesis/round-01/` | 3D grid catalog entry (no dedicated script; see the 2026-08-15 refresh helper) |
| `make generation-families-review` | all of the above | every family output | run each family's publish script in sequence |

Verification mirrors: `pattern-review-verify`, `synthesis-review-verify`
(`synthesis-3d-verify`, `synthesis-4d-verify`), plus the per-family checks
(`movement-check`, `visualization-check`, `color-check`,
`guilloche-current-verify`, `moire-v1-verify`, `surface-tension-v1-verify`,
`interference-motion-verify`).

## What each renderer does

- **movement** — `scripts/render-movement-review.py` runs the
  `izzi_movement_asama-loops` demo (asama camera track) and the
  `izzi_movement_camera_preview` CLI (committed camera-path preview for any
  `izzi.movement/1` document; example `movement-camera-preview.cc`), then
  renders 1080px PNGs with Inkscape.
- **visualization** — `make visualization-review` first regenerates the
  izzi.visualization/1 documents via `build-alpha60-visualizations.py`, then
  `scripts/render-visualization-review.py` renders the four canonical
  members (line/grid/kusama/chord) through the `izzi_visualization_render`
  CLI and Inkscape.
- **synthesis** — `synthesis-review` runs `synthesis-3d-review` (27-portrait
  matrix) and `synthesis-4d-review` (30 episodes, cache-hit aware).
- **pattern** — umbrella over `moire-v1-review`, `surface-tension-v1-review`,
  `guilloche-current-review`, and `interference-motion-review`.

## Refresh flow (both repos)

1. `make generation-families-review` (or a single `make <family>-review`) in
   izzi at the desired commit; record `git rev-parse HEAD`.
2. In situationshipin.space, run the family's publish script with that
   commit (see the table), then:
   ```sh
   node scripts/update-build-manifest.mjs
   node scripts/check-review-site.mjs
   ```
3. Commit and push the portal; the GitHub Pages deploy updates the live
   review catalog.

## Dependencies

- Inkscape is required for PNG rendering in movement/visualization review
  (and the palette publisher). It is already listed as a fallback in
  `docs/dependencies.md` (cairosvg entry).
- Movement camera documents come from `scripts/extract-movement.py` against
  the local clip corpus (Blade Runner cut-5 stays local; the derived
  `izzi.movement/1` documents live under
  `outputs/review/feedback/visual/movement/camera/`).
- Visualization documents come from the alpha60-results and mmrl-metadata
  data directories (defaults in `build-alpha60-visualizations.py`).

## Verification (2026-08-15)

- `make movement-review` — 2 members rendered and manifest verified.
- `make visualization-review` — 862 documents regenerated; 4 members
  rendered and manifest verified.
- `make synthesis-review` — 27 proofs (3D) + 4D cache hit.
- `make pattern-review-verify` — moire (10 plates), surface tension
  (10 plates), guilloche v4, and interference motion (2 families, 12
  videos, 86 files) all PASS.
