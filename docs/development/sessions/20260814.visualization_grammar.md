
Propose a plan for integrating the same new generational type, but for "generation_visualization". This is alongside generation_pattern and generation_color.

this should include

- grids
- tables
- line graphs
- chord graphs
- kusama graphs


For chord and kusama graphs, these rely on pre-existing material from repository mmrl-metadata:
/home/bkoz/src/mmrl-metadata


For grids, tables, line graphs see existing usage in alpha60-results-animation (alpha60.co/animation after redirect only) and local source directories in /home/bkoz/src/:


alpha60-results-2024/
alpha60-results-2025/
alpha60-results-2026/
alpha60-results-aapi-led/
alpha60-results-animation/
alpha60-results-black-led/
alpha60-results-dragons/
alpha60-results-leaks/
alpha60-results-sports/
alpha60-results-star-wars-universe/
alpha60-results-whiteness/
alpha60-results-x/


include documentation and examples, and a "generation_visualization" index for review on situationshipin.space

Append reassesment, new developed plan for movement as new section, "Response 1" in this file.

Write an example of using the propsed api to re-create the _includes in alpha60-results-animation

Include an assessment of 1-3 improvements and optimiizations, design changes, especially if it improves WCAG, accessibility, or WASM


Append synthesized asma-loops plan as new section, "Response 2" in this file.

---

# Response 1 — generation_visualization plan (reassessment + developed plan)

## Reassessment

The brief asks for a fourth generational surface alongside
`generation_pattern`, `generation_color`, and `generation_movement`:
**generation_visualization**, covering grids, tables, line graphs, chord
graphs, and kusama graphs.

Izzi already owns the rendering APIs — `izzi-svg-graphs-line.h`,
`izzi-svg-graphs-chord.h`, `izzi-svg-graphs-bar.h`, `izzi-svg-graph.h`
(synthesis), `izzi-svg-radial-kusama.h`, `izzi-svg-grid-matrix-systems.h`,
`izzi-tables.h`, `izzi-svg-graph-json.h` — so the new surface is a
documentation + test + portal layer with one **unified contract** over the
existing generators, plus data adapters for the two source repos:

- **Chord / kusama**: `/home/bkoz/src/mmrl-metadata` — per-series JSON under
  `json/` (media-object metadata), `visualization/` outputs, and the
  mmrl chrome-extension tooling.
- **Grids / tables / line graphs**: `alpha60-results-animation` and the
  sibling `alpha60-results-*` Jekyll sites — per-series JSON in `data/`
  (`*-cumulative.json`, `*-btiha-media-objects.json`, `*-ip-swarm.json`,
  `*-week.json`, `.geojson`) rendered today by legacy `a60-meta-collection*.exe`
  binaries into `_includes/*.svg` / `*.html`.
- **First-party producer (reassessment add, user review 2026-08-14)**:
  `alpha60/src/a60-cache-recache-synthesize-uniques.cc` is the alpha60
  pipeline that synthesizes the unique BTIHA media-object sets from the
  torrent cache and renders the collection **augment** SVGs via
  `a60-svg-collection-augment.h` (which draws on the izzi `svg::` surface —
  typography, styles, and graph helpers). Its outputs (`*-cumulative*.json`,
  `*-btiha-media-objects.json`, `.geojson`, and the meta/augment renders) are
  the upstream source of the alpha60-results `data/` and `_includes/`; it is
  therefore a first-party consumer and migration target for the
  generation_visualization facade.

## Unified serialization (schema `izzi.visualization/1`)

One compact JSON document per visualization:

```json
{
  "schema": "izzi.visualization/1",
  "kind": "line-graph",
  "title": "Animation downloads by week (normalized start)",
  "description": "Line graph of media objects as peer swarms over time.",
  "source": "alpha60-results-animation/data/animation-cumulative.json",
  "config": { "series": "animation", "x": "week", "y": "downloads",
              "normalize_start": true, "viewport": [1920, 1080] }
}
```

C++ surface (`src/izzi-svg-visualization.h`, new facade):

- `enum class visualization_kind { grid, table, line_graph, chord_graph, kusama_graph }`
- `struct visualization_document { kind; title; description; source; config; viewport; }`
- `emit_grid(doc)`, `emit_table(doc)`, `emit_line_graph(doc)`,
  `emit_chord_graph(doc)`, `emit_kusama_graph(doc)` — each delegates to the
  existing izzi generator with a shared style/viewport/accessibility
  contract (`<title>`, `<desc>`, `role="img"`).
- `emit_visualization_json(doc)` and a data adapter
  `scripts/build-alpha60-visualizations.py` that reads the alpha60-results
  JSON files and mmrl-metadata JSON into visualization documents
  (replacing the legacy `a60-meta-collection*.exe` path).

## Developed plan

1. **Docs** — `docs/generative_visualization.md` + `docs/generative_visualization/`
   (`index.md`, `visualization.md` family entry, `assessment_plan.md`),
   mirroring the other sections; family name **visualization**.
2. **API** — implement the facade above; keep every emitter pure string
   serialization so native C++ and WebAssembly emit byte-identical
   documents (the text-overlay precedent).
3. **Data adapters** — `mmrl-metadata/json → chord/kusama`; alpha60-results
   `data/*.json → grids/tables/line graphs`; source hashes recorded per
   document for provenance.
3b. **First-party consumer** — audit and, where practical, re-create the
   augment/meta renders from `alpha60/src/a60-cache-recache-synthesize-uniques.cc`
   (via `a60-svg-collection-augment.h`) with the new `emit_*` facade, so the
   unique-BTIHA synthesis pipeline becomes a reference consumer of
   generation_visualization.
4. **Examples/tests** — `examples/visualization-grid.cc`, `-table.cc`,
   `-line-graph.cc`, `-chord.cc`, `-kusama.cc`; CTest registration
   (`visualization.*.demo`), `visualization-check` / `visualization-review`
   Make targets; generation-baseline coverage.
5. **Portal index** — `generation-visualization-20260814` (media_kind
   index, generation_class visualization-index, review_category proofs)
   mirroring the palette index; publish gated on portal authorization.

# Response 2 — API example re-creating the `_includes` + improvements

Note: the file's directive line ("Append synthesized asma-loops plan as
"Response 2"") is template boilerplate carried over from the movement brief;
the substantive ask here is the `_includes` API example and the 1-3
improvement assessment.

## Example: re-create `animation-downloads-by-week-normalized-start.svg`

```cpp
#include "izzi-svg.h"
#include "izzi-svg-visualization.h"

int
main()
{
  using namespace svg;

  // Line graph: the _includes SVG generated today by a60-meta-collection.exe.
  visualization_document line;
  line.kind = visualization_kind::line_graph;
  line.title = "Animation downloads by week (normalized start)";
  line.description =
    "Line graph of media objects as peer swarms over time, week 1 to week 26.";
  line.source =
    "alpha60-results-animation/data/animation-cumulative.json";
  line.config = line_graph_config{
    "animation", "week", "downloads",
    /* normalize_start */ true, /* viewport */ { 1920, 1080 }
  };
  emit_line_graph(line, "animation-downloads-by-week-normalized-start");

  // Table: the _includes HTML table with sr-only caption + sortable markup.
  visualization_document table;
  table.kind = visualization_kind::table;
  table.title = "Media Objects Itemized";
  table.source =
    "alpha60-results-animation/data/animation-cumulative-btiha-media-objects.json";
  emit_table(table, "animation-media-objects-table");
  return 0;
}
```

The data adapter reads the alpha60-results JSON and feeds the same configs
used by the legacy binaries, so the re-created `_includes` match the current
visuals with full provenance.

## 1-3 improvements / optimizations (WCAG, accessibility, WASM)

1. **Accessibility contract** — every emitted visualization carries
   `<title>`, `<desc>`, and `role="img"` (the existing `_includes` SVG
   already has a `<desc>`; make it mandatory), and tables keep the
   `sr-only` caption + keyboard-sortable pattern from the current markup.
   Labels use the Atkinson Hyperlegible default with WCAG gray-ramp contrast
   checks (shared with the color family).
2. **WASM byte-parity** — the facade is pure string serialization (the
   text-overlay precedent), so the WebAssembly build emits byte-identical
   documents to native C++; this replaces the legacy `a60-meta-collection*.exe`
   binaries with one portable header-only path and removes build artifacts
   from the Jekyll sites.
3. **Optimization** — reuse the deterministic seeded generators with one
   shared style/viewport contract (less duplicate render code), bound every
   emitted payload to the portal 16 MiB budget, and skip unchanged sources
   by data sha256 (the `proof-local-outputs.py` precedent); kusama/chord
   layouts can cache per data-hash when needed.

Open items: chord/kusama data-shape validation against mmrl-metadata `json/`,
the `generation-visualization-20260814` portal index (gated), and a
visualization family entry assessment pass.

---

# Response 3 — alpha60 consumer reassessment

Added during review (2026-08-14): include use by alpha60 source files —
specifically `alpha60/src/a60-cache-recache-synthesize-uniques.cc`.

## Findings

`/home/bkoz/src/alpha60/src/a60-cache-recache-synthesize-uniques.cc`
(616 lines) is the alpha60 pipeline "recache to unique BTIHA":

- **Synthesis** — reads the torrent cache, coalesces synthetic collections,
  and synthesizes the unique per-media BTIHA identifier sets.
- **Products** — serializes JSON (`meta_recache_serialize`), GeoJSON
  (`serialize_collection_geojson`), IP analysis (`meta_ip_analysis`,
  `media_object_analysis`), and collection **augment** SVG renders
  (`meta_render` → `a60-svg-collection-augment.h`).
- **SVG surface** — `a60-svg-collection-augment.h` draws on the izzi
  `svg::` surface (izzi typography constants including `apercu_typo`,
  styles, select modes, and graph helpers), i.e., alpha60 includes izzi
  visualization headers.
- **Upstream provenance** — its outputs (`*-cumulative*.json`,
  `*-btiha-media-objects.json`, `.geojson`, meta/augment renders) feed the
  alpha60-results-* Jekyll sites' `data/` and `_includes/`.

## Implication for generation_visualization

1. The facade must cover the augment/meta render shapes this file produces
   (grids, tables, line graphs, cartography/slice layouts).
2. A data adapter should consume the unique-BTIHA JSON/GeoJSON products
   directly (same files the Jekyll sites render today).
3. The pipeline is the reference migration target: re-creating its
   augment/meta renders with the `emit_*` facade keeps the alpha60-results
   sites on one portable, wasm-friendly visualization API.

This result is folded into Response 1 (reassessment bullet + developed plan
item 3b).

---

# Response 4 — automating `_includes` regeneration on data check-in

Asked during review (2026-08-14): using the approved improvements, can the
GitHub Pages repos (e.g., `alpha60-results-animation`) re-generate
`_includes/` automatically when new `data/*.json` or `data/*.geojson` is
checked in?

## Yes — a push-path GitHub Actions workflow

The site is a Jekyll repo with `data/` inputs, `_includes/` outputs, and a
legacy `scripts/generate-graphs.sh` (local `a60-meta-collection*.exe`).
Replace the local binary step with the pure, deterministic visualization
facade (approved improvements 2 and 3) and run it in CI:

```yaml
# .github/workflows/regenerate-includes.yml (alpha60-results-animation)
name: regenerate-includes
on:
  push:
    paths:
      - "data/*.json"
      - "data/*.geojson"
      - "scripts/generate-alpha60-includes.mjs"
      - ".github/workflows/regenerate-includes.yml"
  workflow_dispatch:

jobs:
  regenerate:
    runs-on: ubuntu-latest
    permissions:
      contents: write
    steps:
      - uses: actions/checkout@v4
      - uses: actions/setup-node@v4
        with: { node-version: 22 }
      - name: Generate _includes from data
        run: node scripts/generate-alpha60-includes.mjs
      - name: Commit regenerated _includes
        run: |
          git config user.name "alpha60 bot"
          git config user.email "alpha60-bot@users.noreply.github.com"
          git add _includes
          git diff --cached --quiet || git commit -m "Regenerate _includes from data"
          git push
```

The generator runs the **WASM build of the izzi visualization facade**
(pure string serialization, so byte-parity holds on the runner; no native
compiler needed), with the same contract as `scripts/build-alpha60-visualizations.py`
(Response 1) plus the improvements:

- **Sha-256 skip (improvement 3)** — `_includes/.generated-state.json`
  records `data/<file> -> sha256 -> _includes/<output>`; unchanged sources
  are skipped, so the diff is limited to genuinely new/changed data.
- **Determinism (improvement 2)** — identical input yields identical bytes,
  so the commit stays clean and reviewable.
- **Accessibility (improvement 1)** — every regenerated SVG/HTML carries the
  `<title>`/`<desc>`/`role="img"` and sr-only-caption contract.
- **Feed the facade directly** — the generator consumes the unique-BTIHA
  products (`*-cumulative*.json`, `*-btiha-media-objects.json`, `*.geojson`)
  produced by `a60-cache-recache-synthesize-uniques.cc` (Response 3), so the
  data path stays unchanged while the render path is portable.

## Notes and options

- **First run** seeds `.generated-state.json` and regenerates everything;
  subsequent data check-ins only touch changed outputs.
- **Who commits** — the workflow uses `GITHUB_TOKEN` (contents: write);
  branch protection or a bot identity can be substituted.
- **Alternative triggers** — a weekly `schedule` cron, or `workflow_dispatch`
  after the alpha60 synthesis run pushes new data; the push-path trigger is
  the direct answer to the question.
- **Jekyll build** — the existing Pages deploy workflow runs after the
  regenerated `_includes` commit lands, publishing the updated site
  automatically.

---

# Response 5 — other GitHub Actions approaches

Two more ways to automate regeneration on GitHub Actions, beyond the
direct-commit push-path workflow in Response 4:

## Option A — PR-based regeneration (review gate)

Same push-path trigger, but the workflow never commits to `main` directly:
it creates/updates a branch + pull request, so every data-driven
`_includes` change passes a human review (consistent with the repo's
decision-record culture) before merge and deploy.

```yaml
# .github/workflows/regenerate-includes-pr.yml
on:
  push:
    paths: ["data/*.json", "data/*.geojson", "scripts/generate-alpha60-includes.mjs"]
  workflow_dispatch:
jobs:
  regenerate:
    runs-on: ubuntu-latest
    permissions: { contents: write, pull-requests: write }
    steps:
      - uses: actions/checkout@v4
      - uses: actions/setup-node@v4
        with: { node-version: 22 }
      - run: node scripts/generate-alpha60-includes.mjs
      - uses: peter-evans/create-pull-request@v6
        with:
          commit-message: "Regenerate _includes from data"
          branch: bot/regenerate-includes
          title: "Regenerate _includes from data"
          body: "Automated regeneration from new data/*.json or *.geojson. Review the diff before merge."
```

Tradeoff: adds a review step (safer, slower to deploy); ideal when data
arrives frequently enough that humans want a gate on visuals.

## Option B — build-time generation in the Pages deploy (no committed outputs)

Don't commit `_includes/` at all: the deploy workflow runs the deterministic
facade **during the Jekyll build**, so the published site always reflects the
latest `data/` and the repo never carries regenerated artifacts.

```yaml
# .github/workflows/pages.yml (build-time generation, Jekyll + Pages)
on:
  push:
    paths: ["data/*.json", "data/*.geojson", "scripts/**"]
  workflow_dispatch:
jobs:
  build:
    runs-on: ubuntu-latest
    permissions: { contents: read, pages: write, id-token: write }
    steps:
      - uses: actions/checkout@v4
      - uses: actions/setup-node@v4
        with: { node-version: 22 }
      - name: Generate _includes into the Jekyll tree
        run: |
          node scripts/generate-alpha60-includes.mjs --out _includes
      - uses: actions/configure-pages@v5
      - uses: actions/jekyll-build-pages@v1
      - uses: actions/upload-pages-artifact@v3
      - uses: actions/deploy-pages@v4
```

Tradeoff: every deploy re-runs generation (sha-256 skip keeps it fast); no
bot commits, no `_includes` diff review, and the committed site is
ephemeral — best when the generator is trusted and fully deterministic.

Both options use the same WASM facade + sha-256 skip + accessibility
contract as Response 4; the choice is between a human review gate (A) and a
stateless, deploy-time render (B).

---

# Response 6 — script-based invocation (favored)

User direction (2026-08-14): **favor script-based invocation** for the
automation. All generation logic lives in one script, and every workflow is
a thin wrapper around it.

## Single entry point

`scripts/generate-alpha60-includes.mjs` (Node, driving the WASM facade):

```text
node scripts/generate-alpha60-includes.mjs [--out _includes]
    [--commit] [--dry-run] [--wasm path/to/izzi-visualization.wasm]
```

The script owns everything: scanning `data/*.json` + `data/*.geojson`,
loading the izzi visualization WASM module, the sha-256 skip state
(`_includes/.generated-state.json`), emitting the `_includes` artifacts,
and (with `--commit`) the git commit. Workflow YAML contains **no
generation logic** — only the invocation.

## Why this shape

- **One code path** — identical behavior locally (`--dry-run` review on
  rizal) and in CI; no drift between hand-run and action-run renders.
- **No native toolchain on runners** — the facade runs as WASM under Node;
  the script resolves the module from a committed/published artifact or
  falls back to an emsdk build step, keeping the YAML portable.
- **Testable** — the script is unit-testable without GitHub; Actions just
  executes it.
- **Small diffs** — deterministic output + sha-256 skip means a data
  check-in produces only the changed `_includes` files.

All three workflows (Response 4 direct-commit, Response 5 Option A PR gate,
Response 5 Option B deploy-time render) invoke the same script; the choice
among them is only where and how the commit/deploy happens, not how the
generation runs.

---

# Response 7 — staged refined plan (approved direction)

Approved 2026-08-14: Responses 4-6 direction (automate `_includes`
regeneration on data check-in, script-based invocation). This is the
implementation staging list.

## Staging

1. **Docs** — finish the `docs/generative_visualization/` section: add the
   `visualization.md` family entry (mmrl data-shape assessment for chord/
   kusama; facade API; tests/usage/assessment), plus `index.md` and
   `assessment_plan.md`, mirroring the pattern/color/movement sections.
2. **API completion** — finish `src/izzi-svg-visualization.h`:
   - wire `make_line_graph` (axis/label pipeline) into `emit_line_graph`;
   - implement `emit_chord_graph` (derived bipartite pairs:
     media-object ↔ attribute value from mmrl `cast-lead-N` arrays) and
     `emit_kusama_graph` (radial dot-field from attribute buckets);
   - keep every emitter pure string serialization (WASM byte-parity).
3. **Data adapters** — `scripts/build-alpha60-visualizations.py`:
   alpha60-results `data/*.json` + `data/*.geojson` → grids/tables/line
   graphs, with per-source sha256 provenance; mmrl-metadata `json/` →
   chord/kusama documents (validate `data-version` against
   `json.schema/base.json.v*`).
4. **Generator script** — `scripts/generate-alpha60-includes.mjs`
   (Response 6 contract): scan data, sha-256 skip state
   (`_includes/.generated-state.json`), drive the WASM facade,
   `--out` / `--commit` / `--dry-run`; exercise it locally against
   `alpha60-results-animation` data before any Actions wiring.
5. **Automation** — add `.github/workflows/regenerate-includes.yml`
   (push-path trigger, thin wrapper around the script, direct commit) in
   `alpha60-results-animation`; PR-gate (Option A) as a follow-up toggle;
   deploy-time render (Option B) only if the direct-commit flow proves noisy.
6. **Verification** — `make check`; WASM byte-parity check for the facade;
   determinism (two runs byte-identical); accessibility contract check
   (`<title>`/`<desc>`/`role="img"`, sr-only captions); portal
   `generation-visualization-20260814` index (gated).

## Constraints

- Zero provider spend; local-first; no portal publish, repo check-in, or
  push without explicit authorization.
- `alpha60-results-animation` is a separate repository: workflow wiring
  there requires that repo's own authorization.
- Unchanged sources are never re-rendered (sha-256 skip); regenerated
  artifacts must be byte-deterministic.
