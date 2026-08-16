# alpha60-results consumer audit and Milestone 0 proposal

Audit date: 2026-08-08  
Status: read-only public-consumer audit; proposed work is not yet authorized downstream  
Scope: the public alpha60-results hub, every public slice linked from that hub, and representative graph, table, and map pages

## Executive finding

alpha60-results is already a meaningful consumer environment for Izzi. The public sites combine JSON and GeoJSON data, generated inline SVG charts, sortable HTML tables, Leaflet maps, JavaScript interaction, and at least one WebAssembly-backed map projection. That makes the family unusually valuable as a smoke consumer for Izzi's data-visualization and WCAG 2.2 goals.

The evidence does **not** establish which Izzi revision produced each deployed artifact. It establishes direct use of `izzi-*` consumer resources and a documented native-or-WebAssembly graph-generation workflow. The exact dependency edge from each public deployment to this checkout is **UNAVAILABLE** until a build manifest, lock file, or reproducible consumer job records it.

Milestone 0 should therefore add a bounded consumer contract and candidate visual baseline, while leaving every alpha60-results repository and GitHub Pages deployment untouched. Automated checks may nominate a baseline; only an affirmative human visual review may accept it.

## Evidence and authority boundary

This audit used only public pages and public default-branch source on 2026-08-08. It inspected rendered HTML structure, representative source assets, and four visual samples: the hub, a dense AAPI chart, an Olympics page, and the Animation page at a mobile viewport. Repository tips below identify the inspected source state; they do not prove which commit a given GitHub Pages deployment serves.

The audit did not:

- modify, clone into, commit to, or deploy any alpha60-results repository;
- inspect private repositories, account settings, unpublished data, GitHub Actions logs, or deployment credentials;
- run a complete WCAG conformance evaluation, screen-reader study, or multi-browser assistive-technology matrix;
- infer accessibility conformance from a filename containing `wcag-22`;
- infer that a generated plan represents user acceptance of every proposed implementation detail.

Labels used below are:

- **DIRECT**: visible in a cited public page or public repository source;
- **INFERENCE**: a bounded conclusion from direct evidence, identified as such;
- **UNAVAILABLE**: evidence was not public, was not inspected, or could not be resolved.

This document supplements the existing Izzi futures plan. It does not rewrite the historical plan or authorize downstream work.

## Public topology

The [public hub](https://alpha60-devops.github.io/alpha60-results/) describes peer-swarm sampling associated with media objects distributed through BitTorrent and links the slice sites below. Its [data documentation](https://alpha60-devops.github.io/alpha60-results/docs/data-json.2026.html) describes cumulative and weekly JSON plus cumulative GeoJSON, including download/upload counts, media metadata, geographic aggregation, and network-characteristic fields. The hub's [generation documentation](https://alpha60-devops.github.io/alpha60-results/docs/generation.html) describes slices, notebook analysis, native or WebAssembly graph generation, generated SVG includes, and GitHub Pages publication.

| Public surface | Public source inspected | Direct observations | Evidence state |
|---|---|---|---|
| [alpha60-results hub](https://alpha60-devops.github.io/alpha60-results/) | [`c767c240`](https://github.com/alpha60-devops/alpha60-results/commit/c767c240e94acc2207612f45b2b8c81576018332) | Hub, formats, project/generation documentation, and slice navigation | DIRECT |
| [AAPI-Led](https://alpha60-devops.github.io/alpha60-results-aapi-led/) | [`44d0e87e`](https://github.com/alpha60-devops/alpha60-results-aapi-led/commit/44d0e87ead98e78c049c2bf195d6420f0e18c660) | Dense weekly graph, maps, sortable tables, and commentary on the [AAPI analysis page](https://alpha60-devops.github.io/alpha60-results-aapi-led/docs/aapi.html) | DIRECT |
| [Animation](https://alpha60-devops.github.io/alpha60-results-animation/) | [`58a0af71`](https://github.com/alpha60-devops/alpha60-results-animation/commit/58a0af715a81caa07671413b83b2726c47c48ce3) | Graph, maps, tables, and responsive content on the [Animation analysis page](https://alpha60-devops.github.io/alpha60-results-animation/docs/animation.html) | DIRECT |
| [Sports](https://alpha60-devops.github.io/alpha60-results-sports/) | [`06d79227`](https://github.com/alpha60-devops/alpha60-results-sports/commit/06d792274cfe92113aeb97bda1c6d515364130a2) | [Olympics 2026](https://alpha60-devops.github.io/alpha60-results-sports/docs/olympics.html) contains graphs, maps, tables, and commentary; [World Cup 2026](https://alpha60-devops.github.io/alpha60-results-sports/docs/world-cup.html) is currently sparse | DIRECT |
| [Star Wars Universe](https://alpha60-devops.github.io/alpha60-results-star-wars-universe/) | [`7ad252d0`](https://github.com/alpha60-devops/alpha60-results-star-wars-universe/commit/7ad252d058ba5a85f5756a188ccdea04f26b00ad) | Multiple charts, maps, tables, and written findings on the [Andor page](https://alpha60-devops.github.io/alpha60-results-star-wars-universe/docs/andor.html) | DIRECT |
| [Black-Led](https://alpha60-devops.github.io/alpha60-results-black-led/) | [`13b2b669`](https://github.com/alpha60-devops/alpha60-results-black-led/commit/13b2b66978ce9147ab3211fab112e286c8e720e8) | Graph, maps, sortable tables, and commentary on the [Black-Led analysis page](https://alpha60-devops.github.io/alpha60-results-black-led/docs/black.html) | DIRECT |
| [Whiteness](https://alpha60-devops.github.io/alpha60-results-whiteness/) | [`48323bb7`](https://github.com/alpha60-devops/alpha60-results-whiteness/commit/48323bb7995e804fa6897b7f1b3f24cfe0b95048) | Graph, maps, sortable tables, and commentary on the [Whiteness analysis page](https://alpha60-devops.github.io/alpha60-results-whiteness/docs/white.html) | DIRECT |
| [Hacks and Leaks](https://alpha60-devops.github.io/alpha60-results-leaks/) | [`89cf86f0`](https://github.com/alpha60-devops/alpha60-results-leaks/commit/89cf86f03c4f447a1630235c1a018e924e19289b) | Multiple graphs, maps, tables, and commentary on the [Leaks page](https://alpha60-devops.github.io/alpha60-results-leaks/docs/leaks.html) | DIRECT |
| [Dragons](https://alpha60-devops.github.io/alpha60-results-dragons/) | No matching public repository found | The hub links it, but the public Pages URL returned GitHub Pages 404 during this audit | DIRECT for link/404; repository UNAVAILABLE |

The hub also exposes 2025 and 2026 groupings. They are navigation groupings in the inspected hub, not additional independently linked slice repositories.

## Data-analysis and visualization role

### Directly observed pipeline

The hub describes sampling, aggregation into time intervals, analysis/geolocation, and publication of anonymized JSON with IP addresses removed. Its published formats support several complementary visual encodings:

- time-series and collection comparisons as inline SVG;
- geographic and network-property exploration as GeoJSON-backed maps;
- exact values and sortable comparisons as semantic HTML tables;
- narrative interpretation adjacent to the visualizations;
- native or WebAssembly generation of graph and table fragments for static-site inclusion.

This is more than an image gallery. It is a public data-analysis surface where generation correctness, readable labeling, interaction semantics, provenance, and accessible alternatives all matter.

### Izzi and SVG evidence

The sports source provides especially clear consumer evidence. Its [Olympics page source](https://github.com/alpha60-devops/alpha60-results-sports/blob/06d792274cfe92113aeb97bda1c6d515364130a2/docs/olympics.md) loads `izzi-map-leaflet-geojson-v7.4.js`, `izzi-table-wcag-22.css`, `izzi-table-sort-wcag-22.js`, `izzi-table-sort-wcag-22.css`, and `izzi-graph-hover-txt-polyline-red.js`, then embeds generated SVG files. Other slices use the same resource family with map versions 7.3, 7.4, and 7.8.

The Animation site's [version 7.8 map resource](https://github.com/alpha60-devops/alpha60-results-animation/blob/58a0af715a81caa07671413b83b2726c47c48ce3/resources.static/izzi-map-leaflet-geojson-v7.8.js) directly loads `cartofreako-cahill-keyes.mjs` and a WebAssembly module, and provides labeled native controls. This is direct evidence of a cartofreako-backed WebAssembly consumer path as well as of version fragmentation across sites.

**INFERENCE:** these sites are strong regression consumers for Izzi output semantics and presentation.  
**UNAVAILABLE:** the exact Izzi commit, compiler, fonts, browser, data snapshot, and command used to produce each deployed SVG.

## Accessibility and WCAG 2.2 signals

WCAG status must remain **UNVERIFIED** until the pages are evaluated against the complete applicable success criteria and with representative assistive technologies. The current source nevertheless contains useful positive signals and concrete audit targets. [WCAG 2.2](https://www.w3.org/TR/WCAG22/) is the normative reference; W3C's [complex-images guidance](https://www.w3.org/WAI/tutorials/images/complex/) recommends both concise identification and an equivalent detailed description for charts and other complex graphics.

### Positive signals observed

- Representative analysis pages declare a document language and include a `main` landmark.
- Most substantive generated charts use `role="img"` and contain direct-child `title` and `desc` elements.
- Many tables use native table markup, captions, header cells, and native buttons. The sortable-table script sets `aria-sort`, broadly following the shape of W3C's [sortable table example](https://www.w3.org/WAI/ARIA/apg/patterns/table/examples/sortable-table/).
- Table styles include screen-reader-only text and visible focus rules.
- Several map links have labels that announce a new window, and the newer Animation map implementation uses native labeled buttons and `aria-pressed`.
- Narrative findings and tables often provide useful information adjacent to a chart instead of requiring sighted hover interaction alone.

These are useful implementation signals, not proof of conformance. W3C explicitly cautions that its sortable-table example requires browser and assistive-technology testing before production use.

### Observed gaps and risks

The following are direct observations unless marked otherwise:

1. The hub begins its visible hierarchy at level two and its logo image has no alternative text in the rendered markup sampled. Several slice logos also lack `alt`; the Andor page supplies it.
2. Generated chart SVGs commonly contain a literal stray `s` text node immediately inside the root. Their descriptions also contain the spelling error `y-xis`. The generating component is **UNAVAILABLE**, so this audit cannot assign the defect to Izzi with certainty.
3. Chart root titles are frequently filenames or identifiers, while descriptions are generic. They do not consistently summarize key trends or identify a complete structured long description. W3C's [image guidance](https://www.w3.org/WAI/tutorials/images/) calls for a complete text equivalent when a graph conveys detailed information.
4. The inspected graph-hover script handles mouse enter, movement, and leave, but not focus or keyboard events. The SVG data marks sampled were not focusable. Static titles, descriptions, tables, and prose mitigate some information loss, but interaction equivalence is **UNVERIFIED**.
5. The dense AAPI chart has colliding labels at desktop size. The Animation chart becomes very small and its labels are difficult to read at a 390-by-844 viewport. These are visual audit observations, not standalone WCAG determinations.
6. Raw inline chart markup contains a very large number of text nodes and nested point titles. **INFERENCE:** without deliberate grouping or hiding, assistive-technology output may become excessively verbose. An accessibility-tree and screen-reader check is required.
7. The inspected table CSS removes the default outline from `summary`, hides `.sr-only` at one mobile breakpoint, and changes table elements to block display. These declarations are regression risks; their actual accessibility effects are **UNVERIFIED**.
8. Map implementations differ materially between versions 7.3, 7.4, and 7.8. The newer version adds stronger semantics and responsive controls, so older pages need independent keyboard, zoom, focus, and alternative-content checks.
9. Some sports map links have copied or mismatched accessible labels, including references to unrelated Andor content. Several Olympics week links have mismatched week numbers in their visible label, dialog title, or handler.
10. The World Cup page's rendered document title says “Olympics 2026,” although its heading says “World Cup.” Its graph, map, and table sections are currently empty.
11. The AAPI site's source link targets an `alpha60-results-aapi` path rather than the linked `alpha60-results-aapi-led` repository. The Leaks site's source link targets the Sports data repository. The intended destinations are **UNAVAILABLE**.
12. `scope="row"` appears on some `td` elements in the Olympics table, and some displayed commentary totals do not match the adjacent table snapshot. These belong in semantic and data-to-narrative consistency tests.
13. The Dragons link is currently broken, as recorded in the topology table.

Responsive reflow, distinguishability, non-text contrast, and hover/focus behavior belong in the test matrix because they are covered by WCAG 2.2's [Distinguishable guideline](https://www.w3.org/WAI/WCAG22/Understanding/distinguishable.html). A filename or source comment claiming WCAG 2.2 compliance must never substitute for those tests.

## Milestone 0 consumer baseline

### Purpose

Create a small, deterministic Izzi-side smoke consumer that detects changes in generated structure and appearance before those changes reach the public alpha60-results sites. It must not become a mirror of the downstream repositories or their full data corpus.

### Candidate fixture set

The minimum useful set spans behavior, not every public page:

| Fixture | Why it belongs in the baseline |
|---|---|
| Sparse Olympics-style time series | Basic axes, labels, legend, pointer marks, and readable desktop composition |
| Dense AAPI-style multi-series chart | Label collision, repeated ticks, long names, hover targeting, and scaling stress |
| Multi-chart Andor/Leaks-style page | Multiple inline SVG roots, unique IDs, script scoping, and document-order behavior |
| Animation-style mobile chart | Reflow, readable type, touch target, and narrow-viewport behavior |
| Sortable result table | Native semantics, caption, keyboard sorting, `aria-sort`, focus visibility, and mobile layout |
| GeoJSON map shell | Loading/error state, keyboard controls, detached window title, accessible labels, and no-WebAssembly fallback |
| No-JavaScript rendering | Useful chart/table description and values without hover, sorting, or maps |
| Print rendering | Contact-sheet and scientific-documentation legibility without clipped labels or interaction-only information |

Prefer tiny synthetic or carefully minimized public fixtures that preserve the data shapes and stress conditions. Do not copy the whole downstream data corpus into Izzi. Record source URL, public source revision, license review, fixture-generation command, and SHA-256 for any retained public excerpt. The hub identifies the results as CC BY-NC-SA; redistribution and derived-fixture handling should receive an explicit license check before retention.

### Machine checks

Each generated artifact should pass:

- XML/HTML parsing and deterministic canonical serialization;
- no unintended root text nodes such as the observed stray `s`;
- unique IDs across multiple inline SVGs;
- a meaningful accessible name and description for informative graphics;
- a link from complex graphics to equivalent prose or structured data;
- valid heading order, landmark presence, document language, image alternatives, table captions, and header associations;
- keyboard-reachable interactive controls with visible focus and a non-pointer equivalent;
- correct `aria-sort` transitions and sort results;
- exact consistency between data totals, labels, week numbers, link targets, and narrative summaries;
- local and public-link integrity, with an allowlisted temporary exception rather than silently blessing a 404;
- desktop, narrow viewport, 200% zoom, forced-colors/high-contrast, reduced-motion, no-JavaScript, and print rendering;
- contrast and automated accessibility scans, followed by targeted manual checks where automation cannot decide.

Generated SVG text alternatives should follow W3C's [SVG guidance](https://www.w3.org/WAI/tutorials/images/tips/), including explicit title association where appropriate. Automated conformance tools are evidence, not an accessibility acceptance oracle.

### Visual artifacts

For each fixture, retain a candidate package containing:

- original SVG and canonicalized SVG;
- desktop, mobile, zoomed, forced-color, and print PNGs where applicable;
- a cropped absolute-difference image and perceptual-difference score;
- DOM/semantic assertion results and a concise accessibility report;
- browser, font, locale, timezone, renderer, Izzi revision, fixture hash, and command metadata;
- a knolled contact sheet showing every candidate next to its accepted baseline and diff.

Text, IDs, and accessibility semantics are tested separately from pixels. A permissive pixel threshold must never hide missing labels, an empty graph, or an accessible-name regression.

## Bounded future integration

If authorized after Milestone 0 planning, add an Izzi-side consumer directory such as `tests/consumers/alpha60-results/` with these boundaries:

1. A manifest pins the public repository, revision, page/fixture role, expected artifact class, and license status.
2. The default test is offline and deterministic. An explicit `refresh` operation may fetch public inputs, verify hashes, and write a candidate cache rather than update accepted baselines.
3. A generated synthetic fixture remains the primary test input; minimized public samples are supplemental provenance cases.
4. A renderer produces SVG, DOM assertions, screenshots, print output, and a contact sheet using a pinned browser/font toolchain.
5. Known downstream defects are tracked as expected findings with owner and expiry; they are not encoded as permanent desired output.
6. The harness has no deployment key, push path, or write access to downstream repositories.
7. Promotion to an alpha60-results pull request or GitHub Pages change requires separate authority and separate downstream testing.

The consumer manifest should distinguish four independently moving versions: Izzi source, alpha60-results source, data fixture, and rendering toolchain. A green result is meaningful only when all four are recorded.

## Unattended and resumable snapshot workflow

An unattended run should use an explicit state machine:

`INVENTORY -> FETCH_OR_REUSE -> VALIDATE -> GENERATE -> RENDER -> COMPARE -> REPORT -> AWAITING_HUMAN -> ACCEPTED`

Required operating properties:

- Write a checkpoint after every state and resume from the first incomplete state only when input hashes and the plan hash still match.
- Use a content-addressed run directory and append-only receipts. Never replace the last successful run while a new run is incomplete.
- Separate immutable accepted baselines, candidate baselines, and transient render output.
- Record failures with the exact command, exit status, bounded log excerpt, affected fixture, and hashes; do not fabricate a correction or acceptance.
- Retry only classified transient fetch/browser failures with a fixed limit and backoff. Deterministic validation failures stop immediately.
- Refuse silent baseline updates, force-pushes, downstream deployments, or acceptance based on elapsed time.
- Produce a final summary suitable for a monitoring session: completed/failed/skipped counts, known exceptions, changed artifacts, and the path to the review grid.

### Human acceptance gate

The final gate is affirmative and visual. A reviewer should inspect the knolled grid and diff overlays at full size, then record:

- reviewer identity or local actor label;
- reviewed run and manifest hashes;
- accepted and rejected fixture IDs;
- accessibility/manual-check disposition;
- timestamp and concise rationale.

Silence, an unattended timeout, or a passing perceptual threshold is not acceptance. Rejection returns the affected fixture to `VALIDATE` or `GENERATE`; it does not overwrite the accepted baseline.

## Work items and estimate implications

| Work item | Deliverable | Focused engineering estimate |
|---|---|---:|
| `A60-M0-01` | Consumer inventory, revision/license manifest, and synthetic fixture specification | 0.5–1 day |
| `A60-M0-02` | Deterministic fixture exporter and offline cache contract | 1–2 days |
| `A60-M0-03` | SVG/HTML semantic assertions, link checks, and data-to-label consistency checks | 1.5–2.5 days |
| `A60-M0-04` | Pinned browser rendering, interaction checks, accessibility scan, print render, and resumable receipts | 2–4 days |
| `A60-M0-05` | Candidate baselines, knolled review grid, reviewer receipt, and operating documentation | 1–2 days plus reviewer availability |

Total future effort is approximately **6–11.5 focused engineering days**, plus the final human review. If this runs in parallel with Izzi's general example-baseline work and shares its renderer/contact-sheet infrastructure, the likely elapsed impact is **3–6 working days**, not the sum of both tracks. If it is serialized behind that work, assume roughly **1.5–2.5 additional weeks** for one engineer.

This track changes Milestone 0 in two useful ways:

- It adds a real downstream consumer contract before public API or serializer cleanup begins.
- It makes semantic/accessibility drift and visual drift first-class acceptance evidence instead of discovering them after a Pages deployment.

The estimates exclude fixing or deploying any alpha60-results site, resolving the Dragons repository, editing public data, or performing a formal WCAG conformance audit. Those require separate scope and authority.

## Milestone 0 acceptance conditions

The alpha60-results consumer portion of Milestone 0 is complete only when:

1. every selected fixture maps to a direct public use case and a recorded Izzi work item;
2. offline generation is deterministic from pinned inputs;
3. structural, semantic, interaction, visual, and print receipts are retained;
4. known public defects are separated from intended baseline behavior;
5. the run can stop and resume without replacing accepted evidence;
6. no downstream write or deployment capability exists in the harness;
7. an affirmative human review accepts the knolled baseline grid.

Until then, alpha60-results remains an audited prospective smoke consumer, not a verified compatibility guarantee.
