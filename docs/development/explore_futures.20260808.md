# Izzi futures: source audit and evolution plan

**Audit date:** 2026-08-08

**Status:** exploratory report and proposed roadmap; accompanying motion studies are prototypes, not a committed v2 API

**Primary repository:** izzi

**Downstream consumers examined:** alpha60, cartofreako, and devastation-pacific-house-style

## Executive assessment

Izzi is already a useful visual system, not merely a speculative SVG wrapper. It has a broad drawing vocabulary, palettes and typography, generative forms, static graph work, SVG sequence generation, Doxygen output, WebAssembly experiments, and two important production consumers:

- alpha60 uses izzi as a visualizer and extends it with a large domain-specific visualization layer;
- cartofreako uses izzi in native and WebAssembly builds and exercises the new Hamonshu and roulette geometry with real tests.

The newest Hamonshu and roulette code is the clearest indication of the project's future: bounded inputs, deterministic geometry, finite-value checks, exact closure calculations, self-contained configuration objects, and meaningful examples. Those headers are substantially cleaner than the oldest parts of the library.

The central finding is nevertheless a release blocker:

> Izzi is distributed and described as header-only, but the current headers do not satisfy the C++ one-definition rule when two ordinary translation units are linked.

A two-translation-unit link probe produced hundreds of duplicate definitions. Only 10 of 33 public headers passed an isolated-header compilation probe. The current system works mainly because examples are single-translation-unit programs and downstream builds include favorable header combinations. Expanding the public API before repairing this foundation would compound the migration cost.

The other high-priority defects are serializer correctness and safety. Current output includes a stray literal `s` immediately inside the root `<svg>` element. User-controlled text and metadata are inserted without XML escaping. `svg_element` performs file output from its destructor, and that output may throw. These are correctness, accessibility, composability, and agent-safety problems at the same time.

The recommended direction is therefore:

1. make the existing header-only promise true and continuously test it;
2. separate a deterministic scene/render model from serialization, files, conversion tools, and network delivery;
3. make strict, versioned JSON Schema contracts the stable interface for humans, agents, native tools, and WebAssembly;
4. make accessible data visualization a first-class output contract, not a palette name;
5. implement Hamonshu and roulette as deterministic transition compositors parallel to legacy blink and wink behavior;
6. prove the new architecture in alpha60, cartofreako GitHub Pages, and devastation-pacific-house-style before broadening the graph catalogue.

This sequencing preserves the project's creative character while turning it into a dependable library.

## Scope and evidence

The audit used these repository snapshots:

| Repository | Audited commit | Role |
|---|---|---|
| izzi | `f3ad53f451bbab301bc6b9b04e4e4671dc293bbe`, 2026-08-03 | library under review |
| alpha60 | `a50add804c0eb34643e9d39c01867a6ab4e54eb9`, 2026-08-06 | visualizer consumer and compatibility boundary |
| cartofreako | `55917523ec3f62310e00cb55110403a539bf4c1f`, 2026-08-08 | native/WASM consumer, test source, and GitHub Pages pilot |
| devastation-pacific-house-style | `73d9a0d76427cf61115f917266d674e7b99229b7`, 2026-08-08 | normative visual/report style and second accessibility pilot |

The Devastation Pacific working tree was actively evolving during this audit and was left untouched. Its current specification front matter and some supporting records were at different version stages. An izzi adapter must consume a pinned, accepted snapshot rather than copy an uncommitted working state.

Audit work included:

- tracked-file, dependency, and public-header inventory;
- strict GCC and Clang C++20 compilation;
- isolated compilation of every public header;
- a two-translation-unit ODR link probe;
- representative execution of triangle, title/description, Hamonshu, and roulette examples;
- XML well-formedness validation of the emitted SVG;
- Doxygen XML inspection;
- review of documentation, examples, scripts, JSON parsing, graph code, sequence code, CSS, and JavaScript;
- read-only inspection of downstream usage and tests;
- review of current official WCAG, JSON Schema, SVG, Seedance, and seedance2ai.io material.

No paid Seedance API request, upload, site deployment, or modification of a downstream repository was performed.

## Current-state scorecard

| Goal | Current state | Assessment |
|---|---|---|
| Clean contemporary C++20 | New curve headers are good C++20; core and older generative code retain unsafe lifetime patterns, reserved identifiers, global state, include-order dependencies, and legacy idioms | Mixed |
| Header-only | Single-TU examples work; multi-TU linking fails with duplicate definitions | Not currently satisfied |
| Selected SVG implementation | Broad and creatively useful element/render vocabulary; supported SVG subset and serializer invariants are undocumented | Capable but informal |
| PNG, PDF, MKV sequencing | SVG frames are produced and external tools are used downstream; conversion is not a coherent, reproducible pipeline | Partial |
| WebAssembly | A small Emscripten script exists and cartofreako proves the approach; izzi has no own browser smoke test or compiler matrix | Partial |
| Standard JSON configuration | RapidJSON support exists, but validation and error handling are permissive and unsafe; no published schema/version contract | Early |
| Generative art | Radial forms, patterns, color systems, Hamonshu, roulette, harmonographs, and many examples are a real strength | Strong |
| Scientific documentation and notation | SVG/PDF-friendly primitives exist, but generic chart/data semantics, units, uncertainty, provenance, and accessible alternatives are incomplete | Promising |
| Visual thinking | The example catalogue demonstrates a distinctive exploratory practice | Strong |
| Accessible data visualization, including WCAG 2.2 | Some title/description markup, WCAG-named CSS, and sortable-table work exist; the full semantic, interaction, testing, and conformance contract does not | Foundational work required |
| Useful to AI agents | Source can be generated against, but hidden I/O, implicit randomness, permissive JSON, unstable diagnostics, and lack of capability/schema discovery make reliable automation difficult | Weak interface, strong potential |

## What is already valuable

### A broad visual vocabulary

The 33 public headers cover SVG documents and elements, colors, typography, render helpers, radial layouts, sequences, graphs, JSON, patterns, points, and generative curves. The 106 tracked C++ examples are more than demonstration boilerplate: they are a catalogue of forms and visual experiments.

The project should retain this breadth. The evolution plan is not a proposal to collapse izzi into a minimal DOM wrapper. Its value is precisely the combination of:

- small SVG primitives;
- higher-level visual grammar;
- generative form;
- deterministic scientific rendering;
- sequences and transitions;
- reusable styles.

### Hamonshu and roulette are good forward models

[`izzi-svg-curves-hamonshu.h`](../html/izzi-svg-curves-hamonshu_8h.html) supplies a 153-entry, source-indexed catalogue, stable source coordinates, explicit motif configuration, bounded sampling, validation, and deterministic path construction. [`izzi-svg-curves-roulette.h`](../html/izzi-svg-curves-roulette_8h.html) separates curve kinds and configuration, bounds sample counts, rejects non-finite inputs, and computes exact closure periods using integer ratios.

The representative programs generated:

- 91 curated Hamonshu form samples;
- 99 roulette parameter samples;
- XML-well-formed SVG in both cases.

The corresponding cartofreako tests go beyond visual inspection and check determinism, closure, uniqueness, opacity, IDs, and distributions. Those tests should be moved or mirrored at the owning izzi layer.

The spelling in current source and historical reference is **Hamonshu**. New schema and API names should use `hamonshu`. If an agent supplies `homanshu`, validation should return a “did you mean `hamonshu`?” diagnostic rather than silently create a permanent alias.

### Real consumers keep the plan honest

Alpha60 includes izzi visualization headers from 32 source files and carries roughly 9,200 additional lines of `a60-svg-*` visualization code. It consumes documents, elements, styles, palettes, typography, graphs, radial layouts, points, clustering, sequences, and HTML fragments. Its `a60-sequence.cc` already composites image backgrounds with a wink sequence and emits numbered SVG frames. This is the direct behavioral precedent for the requested curve transitions.

Cartofreako has the stronger build and test harness: a substantive Makefile, native tests, WebAssembly targets, generated assets, and deterministic Hamonshu/roulette checks. It demonstrates that izzi can support scientific cartography, but also reveals the current reversed ownership: cartofreako tests behavior that should first be guaranteed by izzi.

These consumers should become compatibility fixtures, not merely anecdotal users.

## Source and C++ style audit

### Header-only and ODR correctness

The most serious technical issue is widespread non-inline definition in headers. A relocatable link of two separately compiled examples failed with duplicate definitions beginning in core scale, DPI, unit, color, element, rendering, and sequence functions.

For a real header-only library, every namespace-scope definition in a public header must be one of:

- `inline`;
- `constexpr` or an inline variable where appropriate;
- an internal-linkage implementation detail that cannot create per-TU semantic divergence;
- a template;
- a class-body definition, which is implicitly inline.

Adding `inline` mechanically is necessary but not sufficient. Mutable function-local statics, RNG objects, ID counters, caches, and configuration references must also have deliberate cross-TU semantics.

**Required gate:** compile and link a two-TU consumer in CI for every supported compiler and build mode.

### Header self-containment and dependency direction

Only 10 of 33 public headers compiled when included alone in an otherwise empty C++20 translation unit. Most rely on transitive include order. The radial-fill family contains an include cycle that can produce redefinitions. The chord graph includes `a60-metadata.h`, so a nominal izzi header depends directly on alpha60 domain data.

The intended dependency direction should be:

```text
izzi  <-  cartofreako  <-  alpha60
  ^
  +---- devastation-pacific render adapter / report generator
```

Izzi should know nothing about alpha60 metadata, browser-a11y datasets, cartographic collection types, or one report taxonomy. Consumers translate their domain data into izzi value objects.

Every public header should:

- include exactly what it uses;
- compile first and alone;
- avoid umbrella-header recursion;
- avoid `using namespace` at namespace scope;
- expose no downstream-domain types.

### Uneven generations of code

The newest curve code uses explicit configs, validation, `std::string_view`, inline constants, and clear namespaces. Older code includes:

- globally declared `point_2t` and harmonograph functions;
- raw SVG string emission mixed with geometry generation;
- a prose instruction accidentally left in a radial header;
- broad namespace imports and short aliases such as `uint`, `ulong`, and `ushort`;
- 467 uses of `_M_*` identifiers, a naming space reserved to the implementation in C++;
- duplicated helpers and rendering protocols spread across large headers.

The cleanup should converge old code toward the new curve style while avoiding a flag-day rewrite.

### Object lifetime, I/O, and exceptions

The current root document type combines document construction, stream state, serialization, filename selection, and file output:

- construction starts the root element;
- destruction finishes it and writes a file;
- the destructor can encounter throwing I/O;
- explicit `write()` catches an exception and rethrows it by value;
- `svg_element` retains a `const typography&` that may outlive its source;
- `element_base` is polymorphic but lacks a virtual destructor.

This is unsafe for exceptions, difficult to compose, surprising to agents, and poorly matched to WebAssembly. It also prevents callers from treating SVG as a pure value.

The replacement contract should be explicit:

```cpp
izzi::svg::document document{canvas, metadata};
document.add(...);

auto serialized = izzi::svg::serialize(document, options);
if (!serialized)
  return report(serialized.error());

auto written = izzi::io::write_file(path, *serialized);
```

No destructor should perform observable I/O. The core library should not need a filesystem at all.

C++20 does not provide `std::expected`, so izzi can use a small documented `result<T, diagnostic>` type, an exception layer, or both. It should not silently require C++23 to improve errors.

### Serializer correctness and injection safety

The representative triangle output begins:

```xml
<svg ... role="img">
s
<title>
triangle
</title>
```

The document is XML-well-formed, but the literal `s` is unintended text content and becomes part of every derived raster/PDF input.

More importantly, text, titles, descriptions, IDs, and other values can be inserted without consistent XML escaping. A title supplied as `Research & <results>` can break output; malicious input can inject markup or script into web-served SVG.

The serializer needs:

- one XML text escaping function;
- one XML attribute escaping function;
- validation or encoding for IDs and URLs;
- an explicit policy for trusted raw fragments;
- deterministic attribute ordering and floating-point formatting;
- control-character rejection;
- tests for Unicode, bidi text, quotes, ampersands, angle brackets, and hostile input;
- a secure-static mode that rejects script, event attributes, external references, and unsafe `foreignObject` content.

Raw SVG should be an unmistakable expert escape hatch, never the default string type.

### Global context and reproducibility

Current rendering uses mutable global/reference-returned DPI and radial ranges, multiple `random_device`-seeded generators, ambient output names, and logging to `clog`. Sequence code has implicit float-to-frame conversions and inconsistent endpoint behavior.

A `render_context` should explicitly carry:

- viewport and physical units;
- optional raster DPI;
- locale and language;
- font resolver and resource policy;
- deterministic seed or caller-provided RNG;
- ID prefix/policy;
- numeric precision;
- secure/static/interactivity policy;
- diagnostics sink.

The same config, asset hashes, izzi version, and seed should produce byte-identical output on the supported toolchain, or the manifest should state precisely which normalization level is guaranteed.

### Small correctness issues point to missing tests

Examples include:

- unqualified `abs(double)` in radial arc code, accepted by the audited GCC path but rejected by strict Clang;
- `22/7` used as integer division when initializing a `double` approximation of pi;
- a likely stale/unfinished vertical-sync calculation;
- an empty linear header;
- an incomplete bar-graph header with undefined state;
- error branches that log and continue with sentinel values.

These are individually repairable. Collectively, they show that compilation and semantic tests do not yet guard the public surface.

## Build, portability, and packaging audit

All tracked C++ examples passed a strict GCC 15 C++20 syntax sweep with `-Wall -Wextra -Wpedantic -Werror`. That is a useful baseline.

The strict Clang 21 sweep failed in radial examples because `abs(double)` resolved incorrectly. The repository's scripts use `gnu++20`, `-march=native`, duplicated variables, shell backticks, relative working-directory assumptions, and executable output in the source tree. The Emscripten script is a seven-line compile helper rather than a verified WebAssembly target. `em++` was unavailable in the audit environment, so izzi's own WASM build was not verified.

The repository has no root CMake package, Meson package, pkg-config metadata, install target, CI compiler matrix, formatting configuration, or formal test target. It also has no root license file even though source headers state GPLv3-or-later terms.

Recommended packaging:

```text
include/izzi/...                 public headers
schemas/...                      versioned JSON Schemas
web/...                          versioned CSS/JS assets
examples/...                     portable, runnable examples
tests/...                        unit, compile, link, schema, golden, browser
cmake/izzi-config.cmake.in       package export
CMakeLists.txt                   INTERFACE targets and tests
LICENSE                          authoritative license text
```

Export `izzi::svg` as a CMake `INTERFACE` target, with separate optional targets such as `izzi::json` and `izzi::web` if their dependencies differ. Keep a simple Make entry point for current workflows, but derive compiler flags and source lists from one maintained definition.

The new public include layout should use names such as `<izzi/svg/document.hpp>`. Existing `a60-svg*.h` headers should remain as a time-bounded v1 compatibility layer while alpha60 and cartofreako migrate. A new `izzi::svg` namespace is preferable to an unowned global `svg` namespace; legacy forwarding should be explicit and removable rather than a permanent global namespace alias.

## JSON audit

[`izzi-json-basics.h`](../html/izzi-json-basics_8h.html) provides useful RapidJSON helpers, but it is not yet a safe configuration boundary:

- compiler branches use different include paths;
- missing files and parse failures print to stderr and return a document that callers may continue using;
- type errors are often logged rather than returned;
- unsupported values can become a magic `0.00123` sentinel;
- booleans and strings are permissively coerced to numeric values;
- diagnostics do not contain schema versions or JSON Pointer paths;
- functions in the header contribute to the ODR failure.

There is no standard JSON parser in C++20. “Standard JSON” should therefore mean RFC-compatible JSON plus a published, versioned [JSON Schema Draft 2020-12](https://json-schema.org/draft/2020-12) contract, not a nonexistent C++ standard-library JSON API. RapidJSON can remain an implementation adapter if it is isolated from domain value objects.

The desired pipeline is:

```text
UTF-8 bytes
   -> strict JSON parse
   -> JSON Schema validation
   -> typed izzi value objects
   -> semantic validation
   -> canonical resolved config
   -> render
```

Diagnostics should be stable data:

```json
{
  "code": "izzi.config.out_of_range",
  "path": "/transition/roulette/samples_per_turn",
  "message": "must be between 8 and 4096",
  "actual": 4,
  "hint": "use 192 for the default quality profile"
}
```

Every top-level contract should contain `$schema` and `schema_version`, reject unknown properties by default, define units in field names or types, and use conditional schemas for variant-specific configuration.

## Documentation audit

The current README is concise and visually inviting, but it does not provide:

- installation or package-consumption instructions;
- an architecture or lifetime model;
- a supported SVG subset/status table;
- supported compiler and dependency versions;
- native, print, video, and WASM workflows;
- JSON schema documentation;
- accessibility guarantees and limitations;
- migration or release policy;
- a test command.

The examples detail page is only a short link list, and several expected rendered SVG files are not tracked. Seven examples contain absolute `/home/bkoz/...` data paths. A fresh user or agent cannot reproduce those examples from the checkout.

Doxygen is a useful existing asset, but it is configured to extract almost everything without making warnings fatal. An XML probe found approximately 816 public member definitions and only about 41 percent with a brief or detailed description. Generated Doxygen HTML accounts for roughly 1,716 tracked files and 21 MB, increasing repository weight while it can become stale.

Recommended source documentation:

- `docs/getting_started.md`;
- `docs/architecture.md`;
- `docs/svg_support.md`;
- `docs/configuration.md` and generated schema reference;
- `docs/sequences_and_video.md`;
- `docs/accessibility.md`;
- `docs/print_and_pdf.md`;
- `docs/wasm.md`;
- `docs/agent_guide.md`;
- `docs/migration_v2.md`.

Generate API HTML in CI and publish it as a Pages artifact or dedicated branch. Make warnings fatal for the new API and ratchet documentation coverage upward instead of accepting a permanent all-or-nothing threshold.

## Examples and tests audit

The repository has 106 tracked `.cc` examples and no formal `tests/` suite. Four examples contain runtime throw-based checks; most prove only that an image can be generated. The compile-all script does not run examples, validate their XML, compare semantics, or exercise multiple translation units.

The test strategy should have distinct layers:

1. **Header contract:** every public header compiles first and alone.
2. **ODR contract:** at least two translation units include overlapping public headers and link.
3. **Unit tests:** geometry, colors, units, XML escaping, IDs, layout, scales, easing, and errors.
4. **Property tests:** finite coordinates, bounded samples, closure, monotonic scales, valid frame indexes.
5. **Schema fixtures:** valid and invalid JSON with exact diagnostic codes and paths.
6. **Semantic SVG goldens:** normalized XML structure and meaningful attributes, not only byte snapshots.
7. **Determinism:** same resolved config and seed produce the same canonical output; selected seed changes produce expected variation.
8. **Conversion checks:** exact raster dimensions, PDF page boxes, fonts/resources, and sequence manifests.
9. **Browser/WASM checks:** load, render, keyboard operation, reduced-motion behavior, and console cleanliness.
10. **Downstream links:** pinned alpha60 and cartofreako consumers compile and run representative visual tests.

Use both GCC and Clang, current Emscripten, debug/release, sanitizers for runnable native tests, and a standards mode without `-march=native`. Keep visual galleries, but treat them as review aids in addition to assertions.

## Graph and web-accessibility audit

### Current graph code

The line graph header has useful axes, ticks, paths, markers, image tooltips, and hover behavior. It is also tied to a particular 1920×1080 weekly SpeedIndex visualization: fixed dimensions, 53-tick assumptions, compile-time Mozilla branches, broad inherited render state, exact floating-point comparisons, and direct logging.

The chord graph directly consumes alpha60 metadata and age-related concepts. It should move to alpha60 until a generic node/edge data model exists. The bar header is incomplete, and the linear header is empty.

The right refactoring is not to add more large `make_*_graph` functions. It is to introduce:

- data values with optional units, missingness, uncertainty, and provenance;
- scale objects: linear, logarithmic, time, ordinal, band;
- axes and tick policies independent of one dataset;
- marks: line, point, bar, area, rule, text, cell, link;
- layout: margins, legends, facets, small multiples;
- an accessibility description model;
- render profiles for screen, print, frame sequence, and reduced motion.

### Current CSS and JavaScript

The file `src.js/izzi-table-wcag-22.css` is a useful experiment, not a WCAG conformance guarantee. Current concerns include:

- a screen-reader-only utility becomes `display:none` in a mobile rule;
- `summary` removes its outline without a complete replacement;
- transitions use `all` and have no `prefers-reduced-motion` path;
- focus styling helps only elements that are actually focusable;
- mobile table display rewrites need browser and assistive-technology testing.

The graph-hover JavaScript is pointer/mouse-centered, attaches a document-wide mousemove listener, contains hard-coded IDs and verbose logging, and does not provide equivalent focus, keyboard, touch, or status-message behavior. Tooltip handling does not yet meet the dismissible, hoverable, and persistent expectations described for [WCAG 2.2 content on hover or focus](https://www.w3.org/WAI/WCAG22/Understanding/content-on-hover-or-focus.html).

The sortable-table work follows a useful W3C APG pattern, but the [APG sortable table example](https://www.w3.org/WAI/ARIA/apg/patterns/table/examples/sortable-table/) explicitly warns that illustrative examples need production testing, particularly across browser and assistive-technology combinations. Izzi should preserve semantic HTML tables and treat script as progressive enhancement.

## Accessibility as a first-class goal

The new project goal should be stated as:

> Izzi is excellent for creating accessible forms of data visualization, with explicit support for building and verifying experiences that target WCAG 2.2.

“Support for” is deliberate. [WCAG 2.2 conformance applies to complete web pages](https://www.w3.org/TR/WCAG22/), including responsive variations, not to a CSS file, palette, SVG fragment, or C++ library in isolation. Izzi can provide strong defaults, machine-checkable constraints, semantic artifacts, and a verification harness. It cannot declare every downstream page conformant.

Every chart should be capable of producing an **accessible chart package**:

```text
<figure>
  visible title and concise insight
  SVG visualization with linked title/description
  optional progressive interaction
  long description or structured findings
  semantic source-data table
  source, units, date, uncertainty, and download links
</figure>
```

W3C's [complex image tutorial](https://www.w3.org/WAI/tutorials/images/complex/) supports the summary-plus-full-description pattern. Low-level SVG accessibility behavior remains an area of active standards work: the 2026 [SVG Accessibility API Mappings Working Draft](https://www.w3.org/TR/svg-aam-1.0/) explicitly says not to implement from that draft yet. Izzi should track it, but base current authoring behavior on stable HTML, ARIA, WCAG, tested browser behavior, and conservative fallbacks.

### Required chart contract

| Need | Izzi contract | Verification |
|---|---|---|
| Text alternative and structure | title, short summary, long description, data table, semantic headings/caption | DOM/accessibility-tree inspection and manual screen-reader smoke test |
| No color-only meaning | direct labels plus configurable dash, marker, texture, or symbol channels | grayscale/forced-colors review and series-distinguishability test |
| Text and non-text contrast | compute every actual foreground/background and adjacent graphical pairing | automated contrast record plus rendered-artifact sampling |
| Reflow and zoom | responsive layout, table wrapper, no clipped reading content at target widths | browser tests at 200% and 400%, narrow viewport |
| Keyboard operation | focusable interactive marks or a synchronized semantic control/list | keyboard-only scenario tests |
| Focus visibility | visible non-obscured focus treatment in normal and forced colors | browser assertions and visual review |
| Tooltips | hover and focus parity; dismissible, hoverable, persistent; touch alternative | Playwright scenarios plus manual touch/AT review |
| Motion and flashing | static/reduced-motion profile, pause/stop behavior where needed, flash guard | reduced-motion browser test and transition luminance analysis |
| Target size | interaction targets that meet the WCAG 2.2 minimum or documented exception | computed-style/geometry test |
| Status updates | live status only where interaction changes sorted/filtered content | accessibility-tree and announcement test |

The main relevant success criteria include 1.1.1, 1.3.1, 1.4.1, 1.4.3, 1.4.10, 1.4.11, 1.4.13, 2.1.1, 2.2.2, 2.3.1, 2.4.7, 2.4.11, 2.5.8, 4.1.2, and 4.1.3. The [W3C use-of-color guidance](https://www.w3.org/WAI/WCAG22/Understanding/use-of-color.html) is particularly important for graph series and status encoding.

### Expanded graph styles

Build graph breadth in an order that maximizes scientific and accessibility value:

**Foundation**

- semantic table;
- line and multi-line;
- bar, grouped bar, stacked bar, and diverging bar;
- scatter and connected scatter;
- area and stacked area;
- histogram and dot plot.

**Scientific comparison**

- error bars and uncertainty bands;
- box, interval, and range plots;
- heatmaps with labeled/color-independent alternatives;
- small multiples/facets;
- slope and bump charts;
- annotations, thresholds, reference bands, and event markers.

**Relational and specialized**

- generic node/link graph;
- chord only after the generic model is independent of alpha60;
- geographic overlays through cartofreako adapters;
- radial/polar variants only when a clearer Cartesian/table alternative is available.

Every style should use the same scales, data model, semantic package, theme tokens, and tests. Novel graph forms are welcome, but must explain their reading grammar and provide an equivalent table or prose interpretation.

## Proposed v2 architecture

The architectural center should be a small deterministic render model. JSON, C++ callers, alpha60, cartofreako, and WebAssembly all resolve into that model. SVG is one serialization of it; file writing and external conversion are explicit later steps.

```text
user JSON -------+
C++ value API ---+--> parse + validate --> resolved spec --> scene/layout
domain adapters -+                                      |       |
                                                        |       +--> SVG string
                                                        |       +--> semantic HTML/table
                                                        |       +--> accessibility manifest
                                                        |
                                                        +--> frame timeline
                                                                 |
                                                                 +--> SVG frame set
                                                                 +--> sequence manifest

explicit adapters:
SVG -> PNG rasterizer
HTML -> tagged PDF-capable renderer
PNG frames -> ffmpeg/MKV
published asset -> seedance2ai.io request
```

The core stays header-only and performs no process execution or network access. Small reference executables may be built from it; those executables are not part of the header-only runtime requirement.

### Suggested component layout

```text
include/izzi/
  core/
    color.hpp
    diagnostic.hpp
    geometry.hpp
    result.hpp
    units.hpp
  svg/
    document.hpp
    element.hpp
    path.hpp
    serialize.hpp
    text.hpp
  config/
    json.hpp
    schema.hpp
  art/
    hamonshu.hpp
    roulette.hpp
    harmonograph.hpp
  sequence/
    timeline.hpp
    transition.hpp
    basic.hpp
    curve.hpp
  chart/
    data.hpp
    scale.hpp
    mark.hpp
    layout.hpp
    accessibility.hpp
    render.hpp
  theme/
    tokens.hpp
  compat/v1/
schemas/
web/
```

This is a responsibility map, not a demand for one header per listed noun. Keep headers cohesive and compilation cost visible.

### Core design rules

- Public types are owned by `izzi::*` namespaces.
- Each public header compiles first and alone.
- All header definitions are ODR-safe.
- Value types own their data unless a borrow is explicit in the type and lifetime.
- No destructor writes files, logs, or mutates global state.
- No randomness occurs without an explicit seed/RNG policy.
- No raw user string becomes XML/HTML markup implicitly.
- Every physical quantity has a unit in its type or field name.
- Render context is explicit and immutable during one render.
- Deterministic IDs, float formatting, and attribute ordering are defined.
- Unsupported SVG features fail with structured diagnostics.
- Accessibility metadata travels with the chart/figure model, not as an afterthought.
- C++ and JSON APIs resolve to the same typed objects and validation rules.

### Supported SVG profile

Publish an izzi SVG profile instead of saying only “select parts of SVG.” For each element/feature, record:

- status: supported, partial, serialization-only, or unsupported;
- processing mode: secure static, static, animated, interactive;
- browser/rasterizer/PDF-converter test coverage;
- accessibility behavior;
- security restrictions;
- first izzi schema/API version.

The profile should initially cover the subset izzi already uses reliably: root document, groups, paths, basic shapes, text/tspan, title/description, definitions, clip paths, masks, gradients if verified, images with an explicit resource policy, transforms, and presentation/style attributes. Script and `foreignObject` should be opt-in and excluded from secure-static output.

## Frame sequences and curve transitions

### Existing sequence behavior

[`izzi-svg-sequences.h`](../html/izzi-svg-sequences_8h.html) contains fade, blink, wink, vertical-sync roll, dot-grid effects, optical dots, and swipe-left. The functions return vectors of SVG fragments and are used by alpha60 to composite numbered frame files.

That is useful behavior, but the current API leaves timing, composition, naming, random seeds, and endpoint rules implicit. New curve transitions should be behaviorally parallel to blink and wink while using a cleaner timeline underneath.

### Canonical timeline

Use integer frame positions as the canonical representation:

```cpp
struct frame_rate {
  std::uint32_t numerator;
  std::uint32_t denominator;
};

struct timeline_spec {
  frame_rate rate;
  std::size_t frame_count;
};

struct frame_context {
  std::size_t index;
  std::size_t count;
  double progress; // defined in [0, 1]
};
```

For `count > 1`, define `progress = index / (count - 1)` so both endpoints are present. For a single-frame transition, define progress as 1. Durations supplied in seconds must resolve once to an integer frame count under a named rounding policy; the resolved manifest stores the integer result. Do not repeatedly multiply floating-point seconds by FPS inside each effect.

Each generated frame should contain or reference:

- frame index and presentation timestamp;
- from-layer and to-layer IDs;
- transition parameters and resolved seed;
- complete SVG or a typed fragment with explicit ownership;
- accessible static fallback policy;
- content hash after serialization.

### Hamonshu transition

Hamonshu is suited to directional and field-like transitions. The source-indexed motif remains static geometry; the transition changes its reveal/composition state.

Initial modes:

1. **`stroke-reveal`** — reveal a stable motif with `stroke-dasharray`/`stroke-dashoffset`.
2. **`mask-reveal`** — use motif linework, expanded stroke, or a filled derived field as a mask between from/to layers.
3. **`wave-wipe`** — move a clipped Hamonshu field across the canvas.
4. **`phase-field`** — animate phase while holding sampling and topology stable, primarily for abstract interstitials.

Configuration should reference the stable source coordinate (`first_page`, `last_page`, `motif`) plus the existing density, curvature, phase, rotation, reflection, and samples-per-curve fields. Schema validation should resolve the source coordinate to the catalogue entry and persist its stable `pattern_id` and descriptive provenance in the output manifest.

### Roulette transition

Roulette is suited to radial, tracing, and iris transitions.

Initial modes:

1. **`stroke-reveal`** — trace one closed epi/hypotrochoid without resampling it per frame.
2. **`iris-mask`** — scale or reveal a filled closed path around a selected origin.
3. **`radial-wipe`** — rotate and expand a stable roulette mask.
4. **`field-dissolve`** — progressively reveal a deterministic field of roulette cells.

The config reuses `roulette_kind`, integer fixed/rolling radius terms, point distance, phase, samples per turn, origin, and scale. Exact closure and the maximum-sample guard remain invariants.

### Morphing rule

Do not interpolate arbitrary path strings. A curve morph is allowed only when both endpoints have:

- the same command topology;
- the same point/segment count;
- the same closure state;
- finite corresponding coordinates.

Otherwise use a mask, transform, stroke reveal, or crossfade. This prevents frame-to-frame popping and invalid intermediate geometry.

### Motion and accessibility

Every animated transition config must declare a reduced-motion behavior:

- `none` for a direct cut;
- `cross-fade`;
- `short-fade`;
- `poster` for one representative static frame.

The web adapter must honor `prefers-reduced-motion`. Frame generation for video remains deterministic, but transition preflight should analyze large-area luminance changes and reject unsafe rapid flashing profiles. Legacy blink/wink wrappers should gain safe defaults and explicit frame timing.

### Compatibility API

After the new timeline is stable, provide narrow wrappers such as:

```cpp
hamonshu_transition_seq(...);
roulette_transition_seq(...);
```

They can return the legacy `strings` type for alpha60 while delegating all timing and curve work to the new implementation. Mark the old fragment-only sequence API as v1, document the migration, and do not duplicate algorithms between layers.

### Transition acceptance tests

- exact frame count and endpoint inclusion;
- stable serialized output for the same config and seed;
- no non-finite coordinates or sample-limit bypass;
- first and last frame visual state matches the requested from/to layers;
- stable path topology for morph-capable modes;
- unique, deterministic IDs across composed layers;
- valid XML for every frame;
- reduced-motion/static alternative exists;
- no unbounded full-frame flash pattern;
- representative SVG-to-PNG-to-MKV pipeline succeeds in CI;
- alpha60 can replace one wink call with each new compatibility wrapper.

## Title frames for seedance2ai.io

### Integration boundary

There are two distinct services involved:

- [ByteDance's official Seedance 2.0 material](https://seed.bytedance.com/en/blog/seedance-2-0-official-launch) describes the model and its multimodal text, image, audio, and video inputs.
- [seedance2ai.io's API](https://www.seedance2ai.io/developers) is an independent platform operated by SixBryan LLC and explicitly says it is not affiliated with the model developers.

Izzi should integrate with the user-named independent platform only through an optional delivery adapter. The core title renderer should remain useful with any video model, local editor, or ordinary MKV pipeline.

As of 2026-08-08, the platform documents `POST /api/v1/video/seedance2`, Bearer authentication, an `Idempotency-Key`, and polling via `GET /api/v1/tasks/{id}`. Its image-to-video request accepts a public HTTPS `image_url` start frame, an optional `end_image_url`, 4–15 second durations, named aspect ratios and resolutions, and images up to 30 MB. These are external, version-sensitive constraints and must live in a versioned adapter.

### Proposed user JSON

This example deliberately separates deterministic rendering from vendor delivery. It contains no API secret and no public URL; the publisher injects the URL only after the PNG has been produced, validated, hashed, and uploaded.

```json
{
  "$schema": "https://bdekoz.github.io/izzi.public/examples/seedance2/ten-scene-authoring.proposed.schema.json",
  "schema_version": 1,
  "project_id": "seedance-opening-001",
  "canvas": {
    "width_px": 1920,
    "height_px": 1080,
    "background": "#101820",
    "safe_area_px": {
      "top": 86,
      "right": 96,
      "bottom": 86,
      "left": 96
    }
  },
  "timeline": {
    "frame_rate": {
      "numerator": 30,
      "denominator": 1
    },
    "frame_count": 150
  },
  "title": {
    "text": "PACIFIC SIGNAL",
    "subtitle": "Sequence 01 / 8 August 2026",
    "language": "en-US",
    "accessible_name": "Pacific Signal title card",
    "description": "Warm-white centered title over a dark field, revealed by a monochrome Hamonshu wave.",
    "field_background": "#173A55",
    "layout": {
      "horizontal": "center",
      "vertical": "center",
      "max_width_px": 1536
    },
    "typography": {
      "family": "Atkinson Hyperlegible Next",
      "weight": 800,
      "size_px": 96,
      "line_height": 1.05,
      "letter_spacing_em": 0.01
    },
    "color": "#FCFBF7"
  },
  "transition": {
    "kind": "hamonshu",
    "mode": "mask-reveal",
    "start_frame": 0,
    "frame_count": 38,
    "easing": "smoothstep",
    "seed": 4242,
    "reduced_motion": "cross-fade",
    "accent_color": "#7FE1D1",
    "hamonshu": {
      "pattern": {
        "first_page": 1,
        "last_page": 1,
        "motif": 1
      },
      "density": 1.0,
      "curvature": 1.3,
      "phase_radians": 0.0,
      "rotation_radians": 0.0,
      "reflected": false,
      "samples_per_curve": 48
    }
  },
  "output": {
    "directory": "build/seedance-opening-001",
    "frame_pattern": "frame-%08d",
    "formats": ["svg", "png"],
    "still_asset_id": "title-still",
    "manifest": "manifest.json"
  },
  "delivery": {
    "adapter": "seedance2ai-v1",
    "asset_role": "start-frame",
    "asset_id": "title-still",
    "request": {
      "quality_tier": "standard",
      "channel": "standard",
      "prompt": "Begin on the supplied title field, then move into a quiet Pacific night scene with stable typography only in the opening frame.",
      "aspect_ratio": "16:9",
      "duration": "5",
      "resolution": "1080p",
      "generate_audio": false,
      "fixed_lens": true,
      "seed": 4242
    }
  }
}
```

A roulette transition substitutes:

```json
{
  "kind": "roulette",
  "mode": "iris-mask",
  "start_frame": 0,
  "frame_count": 38,
  "easing": "smoothstep",
  "seed": 4242,
  "reduced_motion": "cross-fade",
  "roulette": {
    "kind": "hypotrochoid",
    "fixed_radius": 5,
    "rolling_radius": 2,
    "point_distance": 2.5,
    "phase_radians": 0.0,
    "samples_per_turn": 192,
    "origin": "center"
  }
}
```

### Schema contract

Publish a complete Draft 2020-12 schema at `schemas/title-sequence-1.schema.json` and its stable public URL. It should:

- set `additionalProperties: false` in every closed object;
- require `schema_version`, `project_id`, `canvas`, `timeline`, `title`, and `output`;
- bound canvas dimensions, frame counts, rates, font sizes, line heights, samples, and string lengths;
- define color syntax and separately validate actual contrast after compositing;
- reject absolute/traversing output paths in the reference CLI;
- use `oneOf` so only the config matching `transition.kind` is accepted;
- enumerate `none`, `fade`, `blink`, `wink`, `hamonshu`, and `roulette` transition kinds;
- enumerate supported easing and reduced-motion values;
- validate Hamonshu source coordinates against the compiled catalogue during semantic validation;
- validate roulette geometry and sample bounds;
- isolate `delivery` behind an adapter/version discriminator;
- reject secrets, authorization headers, arbitrary shell commands, and credentials;
- allow extension only through an explicitly versioned `extensions` object, not unknown top-level keys.

JSON Schema catches shape and ranges. C++ semantic validation must additionally check:

- safe area fits the canvas;
- title layout fits or reports overflow;
- font resolution is deterministic;
- foreground/background and graphic contrast records are valid;
- frame spans fit the timeline;
- output paths remain within the selected output root;
- vendor aspect ratio and resolution agree with the rendered asset;
- the requested vendor duration and quality combination is currently supported.

### Render and delivery workflow

1. **Parse and validate.** Return structured diagnostics with JSON Pointer paths; make no files.
2. **Resolve resources.** Choose fonts and linked assets under an explicit policy. Record source paths/URIs, hashes, and licenses in the resolved manifest.
3. **Resolve timing.** Convert all user-friendly durations to exact integer frame spans and persist them.
4. **Render.** Produce the still SVG and deterministic transition SVG frames.
5. **Preflight SVG.** Parse as XML; check dimensions, safe area, title/description linkage, text overflow, unique IDs, external references, and secure-static policy.
6. **Rasterize explicitly.** Invoke a pinned CairoSVG, Inkscape, or equivalent adapter to produce opaque PNG with exact pixel dimensions. Record tool version and command arguments.
7. **Preflight PNG.** Check dimensions, color mode, opacity policy, file size, and SHA-256.
8. **Publish.** A caller-owned publisher uploads the selected asset and returns a public HTTPS URL. The current API documentation does not define a general asset uploader, so izzi must not invent one.
9. **Map the request.** The `seedance2ai-v1` adapter adds `mode: image-to-video` and the generated `image_url`, preserves the vendor field names, derives an idempotency key from canonical request JSON plus asset hash, and emits a redacted dry run.
10. **Submit and poll.** Read the API key from an environment/credential provider, never the project JSON. Respect `Retry-After` and rate limits. Persist task ID, status transitions, returned seed, credit data, video URL, last-frame URL, timestamps, and errors as a receipt.
11. **Download and verify.** Save the result through a caller-authorized step and hash it.
12. **Finish exact titles in post.** If typography must remain exact, composite izzi-rendered title/transition frames before or after the generated clip instead of asking the model to reproduce the text.

The final recommendation follows from published model limitations: ByteDance reports that text-rendering accuracy still has room for improvement. The deterministic izzi frame is therefore a useful visual reference, but should not be trusted as the sole mechanism for preserving exact wording inside generated motion.

### Generated 10-second studies

Two reproducible offline proofs accompany this report:

| Study | Clip | Exact-title poster | Verified media |
|---|---|---|---|
| Hamonshu wave/mask reveal | `examples/seedance2/generated/hamonshu-10s.mkv` (local-only) | `examples/seedance2/generated/hamonshu-10s-poster.png` (local-only) | 1280×720, 30 fps, 300 frames, 10.000 seconds, H.264/yuv420p, no audio |
| Roulette hypotrochoid iris | `examples/seedance2/generated/roulette-10s.mkv` (local-only) | `examples/seedance2/generated/roulette-10s-poster.png` (local-only) | 1280×720, 30 fps, 300 frames, 10.000 seconds, H.264/yuv420p, no audio |

The [study README](../../examples/seedance2/README.md), [C++20 generator](../../examples/seedance2/title-transition-study.cc), and user JSON configs are included. The generator uses izzi's existing Hamonshu and roulette geometry, writes safe standalone SVG frames, and relies on ffmpeg/librsvg only for explicit encoding. It performs no network access and submits no API request.

Current artifact hashes:

```text
f316b6e860cd586a7de833215bdf5666698dd6f363f0377fc6080d2bbf806a0a  hamonshu-10s.mkv
6695e5dd1dc8defceb25aee1487f981057470c2b3573e3c8f62031685331c7eb  hamonshu-10s-poster.png
8ccf27eb3d86e7368f32bc804253360773669a54691a63d35bc601cd9e1603f4  roulette-10s.mkv
e4f78669a5ddfcbe1e14e2004c76a18a2733149095f9b151111488216f4a1974  roulette-10s-poster.png
```

These are design/transition studies, not generated Seedance model output. The fully revealed poster is the proposed `image_url` source asset after caller-owned publication; the MKV demonstrates the exact deterministic transition/post-production path.

### Ten-scene, ten-second storyboard schema

The next contract should describe a whole job rather than require ten unrelated title configs. It should have two representations:

- the human-facing [authoring schema](../../examples/seedance2/ten-scene-authoring.proposed.schema.json) and [ten-scene authoring example](../../examples/seedance2/ten-scene-authoring.example.json);
- the generated [resolved execution schema](../../examples/seedance2/ten-scene-project.proposed.schema.json) and [complete resolved example](../../examples/seedance2/ten-scene-project.example.json).

In the authoring form, each of exactly ten scenes requires only `scene_id`, `title`, and `description`. Project defaults carry the shared 10-second timeline, title style, transition policy, provider profile, and output rules:

```json
{
  "$schema": "https://bdekoz.github.io/izzi.public/examples/seedance2/ten-scene-authoring.proposed.schema.json",
  "schema_version": 1,
  "project_id": "pacific-instrument-ten-scene",
  "creative_direction": {
    "summary": "A restrained scientific-documentary journey across the Pacific observation system.",
    "language": "en-US",
    "global_constraints": [
      "no watermark",
      "no generated text after the exact title frame",
      "no unsafe flashing"
    ]
  },
  "defaults": {
    "timeline": {
      "frame_rate": {
        "numerator": 30,
        "denominator": 1
      },
      "frame_count": 300,
      "duration_seconds": 10
    },
    "transition_policy": {
      "kind": "alternate-hamonshu-roulette",
      "preset_version": 1,
      "frame_count": 75,
      "reduced_motion": "cross-fade"
    },
    "prompt_policy": "derive-and-review"
  },
  "scenes": [
    {
      "scene_id": "scene-01",
      "title": "PACIFIC INSTRUMENT",
      "description": "An exact title field opens before yielding to a wide predawn ocean horizon."
    }
  ]
}
```

That is an abbreviated excerpt; the linked authoring example is valid and supplies all ten scenes plus the remaining project defaults. Optional per-scene fields allow a `prompt_override`, extra constraints, reference asset IDs, a seed override, a transition override, and an accessibility hint.

The resolver expands concise input into the execution form below. The execution plan is deliberately verbose because it is the reviewed, hashable record used for rendering, retries, receipts, and delivery. The two checked examples are independently valid illustrations, not yet a golden source/output pair; once the resolver exists, a canonical pair must become an acceptance fixture.

The resolved execution shape is:

```json
{
  "$schema": "https://bdekoz.github.io/izzi.public/examples/seedance2/ten-scene-project.proposed.schema.json",
  "schema_version": 1,
  "project_id": "pacific-instrument-ten-scene",
  "defaults": {
    "canvas": {
      "width_px": 1280,
      "height_px": 720,
      "background": "#081820",
      "safe_area_px": {
        "top": 58,
        "right": 64,
        "bottom": 58,
        "left": 64
      }
    },
    "timeline": {
      "frame_rate": {
        "numerator": 30,
        "denominator": 1
      },
      "frame_count": 300,
      "duration_seconds": 10
    },
    "delivery": {
      "adapter": "seedance2ai-v1",
      "submit": false,
      "quality_tier": "standard",
      "channel": "standard",
      "aspect_ratio": "16:9",
      "duration": "10",
      "resolution": "720p",
      "generate_audio": false,
      "fixed_lens": true
    }
  },
  "assets": [],
  "scenes": [
    {
      "scene_id": "scene-01",
      "description": "Human-readable editorial description of the intended scene.",
      "prompt": "The exact concrete director instruction sent to the provider adapter.",
      "constraints": [
        "no watermark",
        "no logo",
        "no unsafe flashing"
      ],
      "reference_asset_ids": [],
      "seed": 1001,
      "title_frame": {
        "text": "PACIFIC INSTRUMENT",
        "subtitle": "SCENE 01 / SIGNAL AT FIRST LIGHT",
        "accessible_name": "Pacific Instrument opening title",
        "description": "Warm-white title and cyan rule on a deep Pacific blue field.",
        "field_background": "#173A55",
        "color": "#FCFBF7",
        "accent_color": "#7FE1D1",
        "hold_frames": 45,
        "use_as_provider_start_frame": true
      },
      "transition_in": {
        "kind": "hamonshu",
        "mode": "mask-reveal",
        "start_frame": 45,
        "frame_count": 75,
        "reduced_motion": "cross-fade",
        "hamonshu": {
          "pattern": {
            "first_page": 1,
            "last_page": 1,
            "motif": 1
          },
          "density": 1.0,
          "curvature": 1.3,
          "phase_radians": 0.0,
          "rotation_radians": 0.0,
          "reflected": false,
          "samples_per_curve": 48
        }
      },
      "accessibility": {
        "accessible_name": "Predawn Pacific horizon",
        "long_description": "A nearly black-blue ocean meets a level horizon while a thin dawn line grows gradually.",
        "motion_note": "Slow forward drift and a gradual wave-mask reveal.",
        "flash_policy": "no-unsafe-flashing"
      }
    }
  ],
  "assembly": {
    "enabled": true,
    "mode": "cut",
    "expected_duration_seconds": 100,
    "output": "build/pacific-instrument-ten-scene/pacific-instrument-100s.mkv",
    "chapter_markers": true,
    "audio_policy": "preserve"
  }
}
```

The excerpt shows one scene; both schemas require `minItems: 10` and `maxItems: 10`, and both accompanying examples supply all ten.

#### Why the resolved plan retains both description and prompt

- `description` is the human-facing editorial intent and review boundary.
- `prompt` is the exact provider-facing direction after resolution but before credentials or asset URLs are added.
- `constraints` are appended under a defined canonical formatting rule and remain separately inspectable.
- `accessibility.long_description` is the expected nonvisual account; after generation, a human or approved review process must update it if the actual clip differs.

The author need not write all four. When `prompt_override` is absent, the v1 resolver deterministically compiles `prompt` from the shared creative direction and scene description; it must not make a hidden model call. An agent-written refinement is captured as an explicit reviewed override. The resolver derives the default seed from the canonical project/scene identity, resolves the versioned transition preset, constructs the exact title card, merges global and scene constraints, and creates a provisional accessibility description. `izzi storyboard plan` must expose all derived values for review; `submit` refuses an unresolved authoring document.

One string should not be forced to serve all four jobs in the execution record.

The v1 resolved provider prompt uses one canonical form so request hashes,
idempotency keys, and reviews all see the same text:

```text
<prompt>

Constraints:
- <constraint 1>
- <constraint 2>
```

#### Scene invariants

The v1 proposal deliberately fixes 30 fps, 300 frames, and 10 seconds per scene. Semantic validation additionally requires:

- array order `scene-01` through `scene-10` with no duplicate IDs;
- exactly ten unique scene seeds unless intentional reuse is explicitly supported in a later schema;
- every `reference_asset_id` resolves to one top-level asset whose hash matches;
- title text fits the safe area with resolved fonts;
- title/text and essential graphic contrast is checked after rendering;
- transition spans fit frames 0–299;
- Hamonshu source coordinates resolve to a catalogue entry;
- roulette geometry closes and stays under sample limits;
- provider aspect ratio/resolution agrees with the canvas;
- all output paths stay under the configured relative output root.

The user JSON contains asset IDs, never injected public URLs, authorization headers, or API keys.

#### Ten-scene workflow

The workflow has three explicit stages.

**A. Offline plan and assets**

1. Validate authoring JSON syntax, the published authoring schema, scene order/identity, asset references, and path invariants.
2. Resolve defaults, prompts, constraints, seeds, title cards, accessibility drafts, and versioned transition presets into ten complete scene configs. Validate the resolved schema, write a canonical plan with one hash per scene, and stop for human/agent review.
3. Render ten exact-title SVG/poster assets and ten transition overlays.
4. Preflight XML, pixels, safe area, typography, contrast, external references, motion/flash policy, and asset hashes.
5. Emit a redacted dry run containing ten derived provider requests, expected output names, unresolved public URLs, and any unavailable cost estimate. Reading a config never submits a paid job.

**B. Explicit provider jobs**

6. After a separate publish command, add public HTTPS URLs only for referenced assets and scenes whose `use_as_provider_start_frame` is true. Derive `mode: media-to-video` when `reference_asset_ids` is nonempty, otherwise `image-to-video` when a provider start frame is selected, otherwise `text-to-video`.
7. Require an explicit `--submit`/paid-provider authorization even if the config says `submit: true`. Read the Bearer key from a credential provider.
8. Derive each `Idempotency-Key` from project ID, scene ID, canonical request hash, and reference-asset hashes.
9. Queue, rather than blindly parallelize, the ten jobs. The current independent platform documents a general limit of 30 requests per 60 seconds and a tighter 3 per 60 seconds limit for Seedance requests that validate reference media.
10. Poll and persist each scene independently. A failure in scene 07 must not cause completed scenes 01–06 to be resubmitted.
11. Download each result and retain provider task ID, status history, returned seed, credits, URLs, timestamps, errors, and file hash.

**C. Exact finish and assembly**

12. Verify dimensions, duration, stream layout, and decodability. The default policy fails a scene that is not exactly conformable; trimming, padding, frame interpolation, or audio replacement requires an explicit named policy.
13. Normalize each accepted provider result to the canonical 1280×720, 30 fps, 300-frame working clip.
14. Composite izzi's exact title and Hamonshu/roulette/fade transition in post. A provider start image remains a creative reference, not the authority for exact typography.
15. Run accessibility/motion review against the actual clip and reconcile the provisional long description.
16. Write `scene-01-final.mkv` through `scene-10-final.mkv`, per-scene ffprobe/QA receipts, and hashes.
17. When all ten scenes are complete, assemble them with hard cuts into an exactly 100-second MKV and chapter markers. A later overlapping-transition assembly mode must compute and disclose its shorter duration.

The resumable state machine is:

```text
validated
  -> assets-rendered
  -> assets-published (only when needed)
  -> submitted
  -> processing
  -> downloaded
  -> conformed
  -> post-composited
  -> reviewed
  -> complete
```

Each transition is monotonic and receipt-backed. `failed` retains the last successful state and diagnostic, while `retryable` records `Retry-After`. The master assembly consumes only ten `complete` scene records unless an explicit partial-assembly policy is selected.

Proposed reference commands:

```text
izzi storyboard validate ten-scene.json --json
izzi storyboard plan ten-scene.json --dry-run --output build/plan.json
izzi storyboard render-assets build/plan.json
izzi storyboard publish build/plan.json --publisher <caller-owned-adapter>
izzi storyboard submit build/plan.json --submit --resume
izzi storyboard collect build/plan.json --resume
izzi storyboard finish build/plan.json
izzi storyboard assemble build/plan.json
```

## Making izzi useful to AI agents

Agents work best with explicit capabilities, bounded side effects, stable schemas, and machine-readable failure. Improving those qualities also improves ordinary tooling.

### Reference command surface

Build one small optional CLI against the header-only library:

```text
izzi capabilities --json
izzi schema list --json
izzi schema show title-sequence@1
izzi validate project.json --json
izzi resolve project.json --output resolved.json
izzi render project.json --output-root build --manifest
izzi inspect build/manifest.json --json
izzi examples list --json
```

Requirements:

- stdout is either the requested artifact or documented JSON;
- diagnostics go to stderr and are also available as structured JSON;
- exit codes have stable meanings;
- `validate` and `resolve` never write render output;
- `render` writes only beneath an explicit validated output root;
- `--dry-run` enumerates planned files and external adapter calls;
- network and external process use require an explicit adapter command;
- prompts and configs never contain credentials;
- rerunning a canonical request is idempotent unless `--force` is explicit.

### Capability manifest

Publish a generated manifest containing:

- izzi version and schema versions;
- supported SVG profile features;
- chart and transition kinds;
- config defaults, ranges, units, and deprecations;
- compiler/WASM support;
- deterministic-output level;
- external converter requirements;
- examples and small valid config fragments.

The manifest should be generated from the same registries used by C++ and schema generation so documentation cannot drift.

### Agent-oriented documentation

Add:

- `docs/agent_guide.md` with safe task recipes and common diagnostics;
- `llms.txt` linking only to authoritative, current docs and schemas;
- compact valid/invalid fixtures;
- “choose this graph when…” guidance;
- a JSON Pointer error catalogue;
- migration recipes from v1;
- a statement of side effects for every reference command.

An MCP or agent skill may later wrap the CLI/WASM API, but it should not become the first stable interface. The schemas, pure renderer, and CLI must be dependable independently of any one agent framework.

### Provenance and reproducibility

Every render manifest should record:

- canonical resolved config and its hash;
- izzi version/commit and schema version;
- compiler or WASM build identity where relevant;
- input asset hashes;
- font resolution;
- RNG algorithm and seed;
- converter versions;
- output hashes;
- warnings, accessibility checks, and unverified items;
- timestamps only as provenance, never as inputs to deterministic geometry unless requested.

This makes agent work inspectable, resumable, and scientifically useful.

## Output pipelines: SVG, PNG, PDF, MKV, and WASM

### Keep SVG as the primary render artifact

Izzi should construct standards-oriented SVG and metadata. It should not become a rasterizer, PDF engine, video encoder, upload client, and browser framework inside one header-only package.

External adapters should be:

- explicit;
- version-detected or pinned;
- dry-runnable;
- represented in manifests;
- independently testable.

### PNG and MKV

For frame sequences:

- use exact canvas pixels;
- use zero-padded names such as `frame-00000000.svg` and `frame-00000000.png`;
- store rational frame rate and exact frame count;
- reject missing/duplicate frame indexes;
- emit a machine-readable ffmpeg input manifest or a safely escaped suggested command;
- test the first, middle, and last raster plus final stream metadata.

Alpha60 can continue to orchestrate more complex campaigns, but it should consume the same izzi sequence manifest.

### PDF and large-format print

Separate vector geometry from print-production policy:

- use `viewBox` plus explicit physical page dimensions;
- model trim, bleed, safe area, and page box;
- avoid applying raster DPI to vector geometry;
- record font embedding/subsetting and fallback;
- record color profile and conversion tool;
- test page dimensions and representative rendered pages.

An SVG-to-PDF conversion does not automatically create tagged PDF or PDF/UA structure. For accessible reports, izzi should also produce semantic HTML/table/description data for a tagged-PDF-capable report generator. Final PDF accessibility and print color must be validated on the rendered artifact.

### WebAssembly

Expose the same pure operation used natively:

```text
JSON UTF-8 -> diagnostics or { SVG UTF-8, manifest JSON }
```

The WASM boundary should not require a POSIX filesystem, locale globals, exceptions crossing JavaScript, or destructor I/O. Add a browser smoke test that loads the module, validates a fixture, renders it, places the SVG in a controlled DOM container, and checks output plus console errors.

## Cartofreako GitHub Pages integration

### Current opportunity

Cartofreako's GitHub Pages site is a strong first accessibility pilot because it is:

- data- and table-rich;
- already built around generated manifests and projection/pass matrices;
- already an izzi consumer;
- already tested in native and browser/WASM contexts;
- public enough to reveal real reflow, keyboard, performance, and asset-versioning problems.

The current `_layouts/default.html` has useful baseline choices, including underlined links and a semantic `main` container, but it uses local inline styles and does not currently load an izzi WCAG/chart bundle. The phrase “WCAG22 compliant” in a style comment or link rule should not be treated as a site conformance result.

### Asset ownership

Do not make the site fetch mutable CSS or JavaScript from a sibling checkout or unversioned CDN URL. Izzi should publish versioned web assets, and cartofreako should copy/pin them into:

```text
assets/izzi/<izzi-version>/
  izzi-base.css
  izzi-table.css
  izzi-chart.css
  izzi-table-sort.js
  izzi-chart.js
```

Use namespaced classes or CSS cascade layers to avoid collisions with Jekyll and existing layout rules. The static SVG and table must remain usable if JavaScript is disabled or fails to load.

### First graph pilots

Generate chart data from checked or build-produced JSON rather than hand-maintained page prose:

1. **Projection × generation-pass coverage matrix.** Render a heatmap-like SVG with direct symbols/labels and a complete semantic table. Derive it from `generation-profile.json` and the canonical projection/pass registry.
2. **Generated artifact inventory.** Render grouped bars or dot plots for artifacts by projection, pass, and output format, with a linked table and manifest timestamp/hash.
3. **Build/release trend, when historical data exists.** Show counts or sizes over releases with a table and explicit absence of historical data where unavailable.

The first two are enough to validate the full accessible chart package without inventing a new data collection system.

### Page pattern

```html
<figure class="izzi-chart" aria-labelledby="coverage-title">
  <figcaption id="coverage-title">
    Projection and generation-pass coverage
  </figcaption>
  <p id="coverage-summary">...</p>
  <!-- generated static SVG with its own title/description linkage -->
  <details class="izzi-chart-data">
    <summary>View source data</summary>
    <!-- semantic table with caption and scoped headers -->
  </details>
  <p class="izzi-chart-source">Source: ... / generated: ...</p>
</figure>
```

The table should use a responsive overflow wrapper, not globally turn table rows/cells into `display:block`. Sorting, filtering, and focusable point inspection are optional progressive enhancements.

### Cartofreako acceptance gate

- Pages builds with a pinned izzi web-asset version and no sibling checkout.
- The two pilot charts render from canonical JSON in a clean checkout.
- No-JS output retains the figure, summary, source, and complete data table.
- Keyboard users can operate every enabled interaction.
- Tooltip behavior works for pointer, focus, escape, hover transfer, and touch.
- Automated HTML/accessibility checks pass under the agreed policy.
- Manual tests cover 200%/400% zoom, narrow reflow, forced colors, reduced motion, keyboard-only navigation, and at least one screen-reader/browser pairing.
- The project reports test scope and remaining limitations; it does not claim whole-site WCAG 2.2 conformance solely from the integration.

## Devastation Pacific house-style integration

### Relationship and naming

The local repository is named `devastation-pacific-house-style` (singular). The requested feature is to use izzi WCAG 2.2 styles and graphs with that project.

This integration is different from cartofreako:

- cartofreako is an application/site consuming a reusable visual library;
- Devastation Pacific is the normative owner of a specific editorial/report system.

Izzi must not redefine that house style. The Devastation Pacific repository owns its canonical document-type and style registries, versioning, typography, evidence language, RandomA11y rules, report structure, and compliance wording. Izzi owns generic rendering, chart semantics, contrast calculations, SVG/HTML generation, and reusable QA primitives.

### Current normative requirements relevant to izzi

The examined specification requires or targets:

- Atkinson Hyperlegible Next for editorial text and Atkinson Hyperlegible Mono for data/metadata;
- a restrained, hard-left-aligned grid and evidence-first hierarchy;
- explicit `OBSERVED`, `INFERRED`, `MODELED`, `UNAVAILABLE`, and `SUPERSEDED` distinctions;
- exact canonical `document_type` and `style` values rather than inferred defaults;
- color plus labels/patterns/symbols/line styles in charts;
- independently validated actual foreground/background pairings;
- semantic tables and a data table or prose equivalent for charts;
- WCAG 2.2 AA and tagged PDF/PDF-UA as targets, with cautious claims until verification;
- rendered-page inspection and structural PDF checks.

Izzi can make these rules easier to satisfy, but cannot certify the report or PDF.

### Pinned style-profile contract

Add a small machine-readable exchange owned and versioned by Devastation Pacific:

```json
{
  "$schema": "https://example.invalid/devastation-pacific/visual-profile-1.schema.json",
  "profile_version": 1,
  "specification_version": "accepted-release-version",
  "document_type": "special-topics",
  "style": "house-style",
  "language": "en-US",
  "tokens": {
    "paper": "#FCFBF7",
    "ink": "#14171A",
    "accent": "#173A55",
    "muted": "#4D565D",
    "rule": "#9DA8AF",
    "soft": "#EEF1F2"
  },
  "typography": {
    "editorial_family": "Atkinson Hyperlegible Next",
    "data_family": "Atkinson Hyperlegible Mono"
  },
  "color_validation": []
}
```

The URL above is intentionally non-resolving and illustrative; the owning repository must choose the real schema identity through its governance process.

For `randoma11y-accent` or `randoma11y-full`, the profile must include the frozen date/timezone source record and every literal rendered color pairing. Izzi validates and renders only the supplied frozen profile. It must not make a live RandomA11y request, infer a style, generate tints without validation, or persist aliases.

The current Devastation Pacific working snapshot was in an active version transition: specification front matter, README/agent text, metadata schema, and new uncommitted contract work did not yet all describe the same registry/version. That is a reason to pin an accepted release/tag and hash, not to normalize values in izzi.

### Chart profile

Add an izzi `devastation_pacific` chart profile that maps generic chart tokens to the pinned style profile:

- paper, ink, accent, muted, rule, and panel tokens;
- editorial vs data font roles;
- square corners and flat fields;
- hard grid alignment;
- minimum type sizes by output medium;
- direct labels and restrained legends;
- Mono for values, dates, units, status, and provenance;
- line dash/marker/pattern channels in addition to color;
- semantic evidence-status labels adjacent to marks;
- source, cutoff, assumptions, and uncertainty;
- full data table/prose alternative.

Evidence status should be data, not only visual styling:

```json
{
  "value": 42,
  "unit": "hours",
  "evidence_status": "MODELED",
  "source_id": "method-2026-08-08",
  "uncertainty": {
    "low": 31,
    "high": 58
  }
}
```

A reader must be able to recover `MODELED` and the range from text/table data when color, texture, or the graph itself is unavailable.

### Report/PDF boundary

For this project, izzi should emit:

- chart SVG;
- semantic HTML figure/table/description;
- source/provenance JSON;
- contrast and accessibility-check manifest;
- print geometry suitable for the report generator.

The Devastation Pacific report generator owns pagination, semantic HTML document structure, tagged-PDF-capable conversion, bookmarks, reading order, font embedding/subsetting, section continuity, final PDF preflight, and inspection of every rendered page.

Do not commit or separately distribute font binaries from izzi. Resolve them from an authorized production environment and record what was used.

### Devastation Pacific pilot

Use one bounded `special-topics` fixture first, not a normative audit report with extensive continuity requirements:

1. one hero metric;
2. one line or interval chart with uncertainty;
3. one ranked table/bar chart;
4. explicit evidence statuses;
5. house-style only;
6. semantic HTML and PDF render;
7. complete source-data tables and contrast record.

After that passes visual and structural QA, exercise fixed, prevalidated fixtures for `randoma11y-accent` and `randoma11y-full`. Historical PDFs should not be regenerated merely to test an adapter.

### Devastation Pacific acceptance gate

- One accepted specification/profile version and hash is pinned.
- `document_type` and `style` are exact accepted registry values.
- Unknown or mismatched values fail closed.
- Every actual color adjacency used by the chart has a retained validation record.
- Font resolution is recorded and missing required fonts are disclosed or block final delivery under the profile.
- Chart meaning survives grayscale and a text/table-only reading.
- HTML reading order and table associations are verified.
- PDF generation uses a tagged-capable path and follows the project's preflight.
- Every rendered page is inspected by the project workflow.
- Output says “targets” or “preflighted against” WCAG/PDF-UA unless a named validator and retained result justify stronger language.

## Alpha60 integration and migration

Alpha60 is not only a user of izzi; it is the main source-compatibility risk and the best sequence integration test.

Recommended coordination:

- add an `A60_IZZI_INCLUDE_DIR`/CMake consumer test to izzi CI using a pinned alpha60 commit;
- keep v1 forwarding headers during one documented deprecation window;
- migrate alpha60's generic visual primitives downward only after removing alpha60 metadata dependencies;
- keep alpha60-specific collection, age, identity, and campaign visualization in alpha60;
- have `a60-sequence.cc` consume the new sequence manifest and compatibility wrappers;
- validate representative outputs semantically, not by fragile whole-file bytes alone;
- release compatible izzi/alpha60 versions together when the namespace/include migration lands.

The long-term ownership test is simple: if an API can be explained without alpha60 domain nouns, it may belong in izzi. Otherwise it stays in alpha60.

## Cleanup and simplification program

Apply cleanup in dependency order.

### Core correctness

- remove the root `s` output defect;
- centralize XML text/attribute escaping;
- make file output explicit;
- repair destructor and polymorphic lifetime contracts;
- make every header ODR-safe and self-contained;
- break radial include cycles;
- replace reserved `_M_*` identifiers;
- remove global aliases and namespace imports from the new API;
- use `std::uint*_t`, `std::size_t`, `std::numbers::pi`, `std::span`, `std::string_view`, `std::filesystem::path`, and `std::chrono` where they clarify ownership or units;
- use `[[nodiscard]]` for results and pure calculations where ignoring the value is likely a bug.

### API cohesion

- split geometry creation from SVG string emission;
- split document values from streams/files;
- split data parsing from graph rendering;
- replace inherited catch-all render state with composed specs;
- move alpha60-specific graph code out of izzi;
- either complete bar/linear APIs under the new model or remove them from the public umbrella until ready;
- name physical units and coordinate spaces explicitly;
- consolidate repeated color, scale, ID, and path helpers.

### Diagnostics

- replace stderr side effects and magic sentinels with `result` values;
- assign stable error codes and JSON Pointer paths;
- include actual value, expected domain, and a bounded hint;
- aggregate independent config errors in one validation pass;
- make warnings explicit in the output manifest.

### Tooling

- add CMake `INTERFACE` targets and install/export tests;
- retain a simple Make compatibility entry point;
- add `clang-format` and `.editorconfig` after agreeing on style;
- run GCC, Clang, and Emscripten CI;
- add sanitizer and browser jobs;
- generate, rather than check in, bulky API HTML;
- add an authoritative license file and release/version policy.

Elegance here means fewer ambient rules and fewer ways to make invalid output, not shorter names at the expense of meaning.

## Evolution roadmap

The milestones are dependency-ordered. Calendar estimates should be assigned only after the baseline tests quantify the affected surface.

### Milestone 0 — establish and freeze the baseline

Deliver:

- a CMake/CTest skeleton and reproducible `make check` entry point;
- inventory of public v1 headers and supported compilers;
- 33 isolated-header compile tests;
- two-TU ODR link test;
- strict GCC and Clang example compilation;
- representative XML/golden tests;
- issue list for current failures;
- authoritative license and contribution/build instructions.

Exit criteria:

- every known baseline failure is reproducible in one command;
- no new public API can merge while header isolation or ODR checks regress;
- alpha60 and cartofreako pinned smoke consumers are defined.

### Milestone 1 — make the core safe and truly header-only

Deliver:

- ODR-safe definitions and inline variables;
- self-contained include graph with cycles removed;
- pure document/scene values;
- explicit serialization and file writing;
- XML/attribute escaping and secure-static policy;
- deterministic ID/number formatting;
- explicit render context and seed policy;
- first `izzi::svg` CMake target;
- v1 compatibility headers and migration notes.

Exit criteria:

- 33/33 public headers compile alone under GCC and Clang;
- the multi-TU test links;
- no destructor performs file I/O;
- hostile text fixtures remain data, not markup;
- root SVG output contains no stray text;
- alpha60 and cartofreako representative consumers still build.

### Milestone 2 — versioned JSON and deterministic sequences

Deliver:

- parser adapter isolated from typed configs;
- Draft 2020-12 schema registry and structured diagnostics;
- canonical resolved-config and render manifests;
- rational timeline/frame model;
- migrated fade/blink/wink behavior;
- Hamonshu and roulette transition modes;
- title-sequence schema and reference CLI commands;
- SVG/PNG/MKV adapter fixtures.

Exit criteria:

- all valid/invalid schema fixtures return stable results;
- sequence frame count, endpoints, IDs, and hashes are deterministic;
- reduced-motion behavior is present;
- alpha60 renders one Hamonshu and one roulette transition through compatibility wrappers;
- a local title-frame project produces validated SVG, PNG, and manifest without network access.

### Milestone 3 — accessible chart foundation and two pilots

Deliver:

- generic data, scale, mark, layout, theme, and accessibility models;
- semantic table, line, bar, scatter, area, histogram, and uncertainty support;
- chart package output: SVG + HTML/table + manifest;
- hardened namespaced web CSS/JS with progressive enhancement;
- cartofreako Pages coverage and inventory charts;
- Devastation Pacific pinned house-style profile and bounded `special-topics` fixture;
- browser accessibility test harness and documented manual matrix.

Exit criteria:

- both pilots consume the same generic chart core;
- charts are meaningful with JavaScript disabled and in table-only form;
- keyboard, focus, tooltip, reduced-motion, reflow, contrast, and forced-color checks pass under the published policy;
- neither integration makes an unsupported whole-site/document conformance claim;
- final pilot artifacts record unverified items.

### Milestone 4 — graph breadth, print hardening, WASM, and agent polish

Deliver:

- grouped/stacked/diverging bars, dot/box/interval, heatmap, small multiples, slope/bump, and annotations;
- generic relational model before chord reintroduction;
- large-format print profiles and converter manifests;
- WASM config-to-SVG/manifest API and browser smoke tests;
- `izzi capabilities`, schema discovery, agent guide, and `llms.txt`;
- optional delivery adapter for the current seedance2ai.io API;
- performance budgets and compilation benchmarks.

Exit criteria:

- native and WASM resolve the same fixture to equivalent canonical semantics;
- reference CLI behavior and side effects are fully described;
- print/page/resource manifests reproduce representative output;
- vendor adapter is isolated, redacts secrets, supports dry-run/idempotency, and can be disabled without affecting rendering.

### Milestone 5 — v2 release and v1 retirement schedule

Deliver:

- semantic version and compatibility statement;
- generated API/schema/capability documentation;
- migration status for izzi examples, alpha60, and cartofreako;
- release artifacts and checksums;
- published deprecation window for `a60-svg*.h` and legacy namespace/API;
- retained golden and downstream compatibility matrix.

Exit criteria:

- a fresh consumer can install/use izzi without relative include paths;
- the documented C++20, header-only, JSON, SVG, WASM, sequence, graph, and accessibility-support claims are all backed by automated or named manual checks;
- no required consumer depends on an undocumented v1 behavior.

## Priority order

| Priority | Work | Reason |
|---|---|---|
| P0 | ODR, isolated headers, serializer escaping/root defect, explicit I/O, test harness | Every new feature otherwise rests on invalid distribution and unsafe user input |
| P1 | typed render model, config diagnostics/schema, deterministic timeline | Shared prerequisite for title frames, agents, WASM, graphs, and transitions |
| P1 | Hamonshu/roulette transitions and title-frame workflow | Direct user value with strong existing geometry and alpha60 precedent |
| P1 | accessible chart package foundation | New central project goal and prerequisite for honest WCAG support |
| P2 | cartofreako and Devastation Pacific pilots | Proves web/site and editorial/PDF use without synthetic demos |
| P2 | expanded graph styles | Valuable once scales, data, themes, semantics, and tests are shared |
| P2 | agent CLI/capability manifest | Makes stable internals discoverable and automatable |
| P3 | optional Seedance delivery client, MCP/skill wrappers, specialized graph forms | Vendor/framework-specific work should follow stable offline contracts |

## Key risks and controls

| Risk | Control |
|---|---|
| Namespace/include migration breaks alpha60 | compatibility headers, pinned consumer CI, coordinated release |
| “WCAG 2.2 style” becomes a false compliance claim | say “supports/targets,” ship semantic alternatives, test full integrations, retain limitations |
| SVG looks correct but has poor assistive semantics | HTML/table companion, accessibility-tree/manual tests, conservative SVG fallback |
| PDF is visually correct but untagged | tagged-capable report path, structural preflight, rendered-page inspection |
| User JSON injects markup, paths, commands, or secrets | typed values, escaping, strict schemas, output-root confinement, no secret fields |
| Floating point or RNG changes break reproducibility | canonical formatting, explicit algorithm/seed, normalized semantic goldens, manifest toolchain |
| Curve animation creates huge files or unstable frames | sample caps, stable topology, performance budgets, mask/transform modes |
| Seedance provider changes fields or pricing/limits | isolated versioned adapter, capability check, no provider logic in core |
| Font metrics differ across systems | explicit resolver, recorded asset hash/version, overflow preflight |
| Devastation Pacific spec evolves during integration | consume accepted profile version/hash; reject registry mismatch; no silent normalization |
| Generated docs inflate and stale the repository | publish CI artifacts/Pages output, keep source docs authoritative |
| GPL terms are unclear to consumers | add authoritative license file and document dependency/license boundaries |

## Recommended first change series

Keep the first pull requests small enough to review semantically:

1. Add CMake/CTest, header-isolation tests, and the failing two-TU test without changing behavior.
2. Fix the root literal `s`, add XML text/attribute escaping, and add hostile-input tests.
3. Make core free functions and variables ODR-safe; turn the two-TU test green.
4. Break include cycles and make all current public headers compile alone.
5. Introduce explicit `serialize()`/`write_file()` and deprecate destructor output.
6. Add structured diagnostics and replace JSON sentinel behavior.
7. Introduce the exact-frame timeline and migrate fade/blink/wink behind compatibility wrappers.
8. Add Hamonshu and roulette transition tests and examples.
9. Publish the title-sequence schema plus offline render/manifest CLI.
10. Begin the accessible chart model and cartofreako coverage pilot.

This order produces visible correctness improvements early and creates a clean seam for every requested feature.

## Definition of success

Izzi's evolution is successful when a human or agent can provide a small versioned JSON document, receive either precise structured diagnostics or deterministic SVG/HTML/frame artifacts, and understand:

- what was rendered;
- which inputs, fonts, seeds, and tool versions were used;
- which accessibility checks passed;
- which claims remain unverified;
- how to reproduce PNG, PDF, MKV, or browser output;
- how alpha60, cartofreako, or Devastation Pacific supplied domain/style context without contaminating the core.

At that point “clean C++20,” “header-only,” “agent-useful,” and “accessible data visualization” will be enforceable contracts rather than aspirations, while the generative-art vocabulary remains free to grow.

## Primary external references

- [WCAG 2.2, W3C Recommendation](https://www.w3.org/TR/WCAG22/)
- [W3C complex images tutorial](https://www.w3.org/WAI/tutorials/images/complex/)
- [W3C Understanding Use of Color](https://www.w3.org/WAI/WCAG22/Understanding/use-of-color.html)
- [W3C Understanding Content on Hover or Focus](https://www.w3.org/WAI/WCAG22/Understanding/content-on-hover-or-focus.html)
- [W3C APG sortable-table example and warning](https://www.w3.org/WAI/ARIA/apg/patterns/table/examples/sortable-table/)
- [SVG 2 Candidate Recommendation](https://www.w3.org/TR/SVG2/)
- [SVG Accessibility API Mappings Working Draft](https://www.w3.org/TR/svg-aam-1.0/)
- [JSON Schema Draft 2020-12](https://json-schema.org/draft/2020-12)
- [ByteDance Seedance 2.0 official launch](https://seed.bytedance.com/en/blog/seedance-2-0-official-launch)
- [seedance2ai.io independent platform API](https://www.seedance2ai.io/developers)
