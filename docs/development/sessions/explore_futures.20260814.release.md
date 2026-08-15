# Izzi v3 release planning — documentation audit

Date: 2026-08-14 (release executed 2026-08-15)

Status: `RELEASE-PLANNING; DOCUMENTATION-AUDIT-COMPLETE; AWAITING-APPROVAL`

## Release context

Current version: `project(izzi VERSION 2.20260812 ...)`. Tags:
`v1.20250214`, `v2.20260203`, `v2.20260810`, `v2.20260812`,
`public-snapshot-2026-08-13`; `git describe` reports
`public-snapshot-2026-08-13-40-g678c1da7` (40 commits since the snapshot
tag).

A **new major version (v3)** is warranted by breaking changes landed after
`v2.20260812`:

- Default typography converged to Atkinson Hyperlegible (byte-affecting).
- Color family: `active_spectrum(sortbyhuep)` signature removed, seeded RNG,
  `band_model {rgb, hsv}`, `color_band_cache` replacing
  `next_in_color_band`, constexpr data modules.
- New generative surfaces: `generation_movement` (`izzi-svg-movement.h`) and
  `generation_visualization` (`izzi-svg-visualization.h`) — new public
  headers (43 total).
- Package contract now expects 43 headers / 4 include fragments.

## Documentation audit (method)

- Inventoried every markdown file (93 under `docs/`, plus root
  `README.md`, `index.md`, `CONTRIBUTING.md`).
- Ran a relative-link integrity scan across `docs/` and the root docs.
- Ran the Doxygen HTML audit (`check-public-mirror-links.py --html-only`).
- Compared documentation claims against the current tree: header counts,
  example counts, status lines, section indexes, portal review indexes.

## Findings

### Links

- **0 broken relative links** across `docs/` and root markdown.
- **Doxygen HTML:** regenerated 2026-08-14 (`70235a91`, local); 452 pages,
  14,700 html/asset links, **0 broken**.
- Public-mirror dry-run audit (2026-08-14): PASS, 0 broken links — but the
  mirror predates the new sections and must be refreshed at release.

### Currency gaps (fix list)

1. **Root indexes** — `README.md` DOCUMENTATION and `index.md` list only
   `generative_patterns`; add the `generative_colors`,
   `generative_movement`, and `generative_visualization` sections.
2. **Status lines** — the four generative section indexes carry stale
   statuses (`NOT-SHARED`, `FAMILY-ENTRY-FIRST-PASS`,
   `PREVIEWS-PENDING-REGENERATION`) that contradict the implemented and
   portal-published state; refresh to CURRENT/PUBLISHED per section.
3. **Stale numbers** — `docs/development/milestone_0_baseline.md` cites
   "106 examples / 100 visual" (the generation baseline now carries 115
   examples in candidates); `explore_futures.20260814.movement.md` cites
   "42/42 headers" (currently 43); audit any other header/example counts.
4. **Portal linkage** — the live review indexes (`palette-20260814-index`,
   `generation-visualization-20260814`, `generation-movement-20260814`) are
   not referenced from the family docs; add "review" links per section.
5. **Color gallery previews** — `docs/generative_colors/` gallery remains
   marked pending; the rendered preview set exists
   (`outputs/review/feedback/visual/color/round-01/` + the portal PNGs) and
   should be wired into the gallery.
6. **Changelog** — no `CHANGELOG.md` exists; create one for v3 covering the
   breaking changes and new surfaces.

### Verified current

- README PREREQUISITES (OCR/subtitle machinery) — current.
- Doxygen API reference — current (regenerated).
- Header-migration contract — CANONICAL-READY (43/43).
- Package contract — 43 headers / 4 fragments.

## v3 release plan (skeleton)

1. Apply the documentation fix list (items 1-6 above).
2. Sync the documentation (REQUIRED — see Release requirements below).
3. Decide baseline promotion: the regenerated candidates
   (`WI-COLOR-FAMILY-20260814`, `WI-MOVEMENT-VISUALIZATION-20260814`) are
   reviewed; promotion requires human acceptance (or defer).
4. Verify: `make check` (43/43), `make check-wasm` (cartofreako), link
   audits, portal validator.
5. Version bump: `project(izzi VERSION 3.20260815 ...)` (calendar scheme
   matching v2 tags) + `CHANGELOG.md` v3 entry.
6. Tag `v3.20260815` and push; portal indexes already live (palette,
   visualization, movement).

## Release requirements (mandatory gates)

A release is not complete until every gate below passes:

1. **Documentation sync (izzi → izzi.public) — REQUIRED.** Run
   `publish-public-mirror.sh --push --regen-doxygen` so the public mirror
   carries the released `docs/`, Doxygen `docs/html`, and root docs, then
   re-run the mirror link audit (0 broken links / stale URLs). The release
   tag is **not cut until the mirror is refreshed and audited**. This
   applies to every izzi release, not only v3.
2. `make check` PASS (migration contract CANONICAL-READY).
3. `make check-wasm` PASS (cartofreako).
4. Link audits (docs relative + Doxygen HTML) at 0 broken.
5. Portal validator PASS for any new review indexes.
6. Baseline decision recorded (promotion or deferral with receipt).
7. `CHANGELOG.md` entry + version bump committed before tagging.

## Gates / decisions

- Documentation edits and the version bump require commit/push
  authorization.
- Baseline promotion stays gated on human review.
- The alpha60 producer migration is a separate project and does not block
  the v3 release (site cold cutover is complete).

## Release execution (2026-08-15)

- Documentation fix list applied: root `README.md`/`index.md` now link the
  color/movement/visualization sections; generative section statuses
  refreshed (CURRENT + portal-index LIVE); stale numbers corrected
  (`movement.md` 43/43, `milestone_0_baseline.md` note on 115 examples);
  family docs link their live portal indexes; `CHANGELOG.md` created.
- Version bumped to `3.20260815`; `tests/package-consumer` pins
  `find_package(izzi 3 ...)`; migration stage-i baseline regenerated
  (CANONICAL-READY, 43/43); `make check` PASS; `make check-wasm` PASS.
- Doxygen docs/html regenerated (`70235a91`).
- Public mirror refresh + tag `v3.20260815` + push per the release plan.

Status: `RELEASED-2026-08-15` (tag pushed; baseline promotion and the
alpha60 producer migration remain open, non-blocking).
