# Changelog

## v4.20261116 — 2026-11-16

This release brings together the native graphics, media, print and workflow
development since `v3.20260816`. It is the first published GitHub release
after `v2.20260812`; the two v3 versions were distributed as Git tags.

### Native engraving and generative graphics

- Added an installed C++20 [portrait engraving API](docs/portrait.md): prepare
  photograph fields, resolve physical vector marks and emit SVG. Banknote
  controls include dot/line/crosshatch allocation, contours, tensor flow,
  contrast and maximum line width.
- Added [portrait and print edition workflows](docs/visual_workflow/tool_portrait_print.md)
  for anniversary currency and whole-photo holiday cards, including isolated
  candidate directories, dependency preflight, deterministic replay, galleries,
  600 dpi PNG and print PDF exports.
- Expanded [generative patterns](docs/generative_patterns.md) with guilloche
  enrichment, botanical and reef forms grouped as Organica, arabesque,
  muqarnas, optical treatments and lenticular interlacing/calibration.
- Added [edge-tension cubic bundles](docs/generative_patterns/edge_tension.md),
  expanded moiré and surface-tension fields, Dutch security-panel/medallion
  reconstructions, and an accompanying script font.
- Added deterministic keyframes and route-derived framing, reference-motion
  studies, and expanded danmaku text motion.

### Screen media and animation

- Added the unified [Screen Glitch composition API](docs/generative_movement/screen-glitch.md)
  and strict JSON authoring for ordered film, VCR, TV-broadcast, satellite,
  receiver, buffering and CRT stages, with explicit seeds, clocks, source
  history, resource limits and receipts.
- Added [film decay](docs/generative_movement/film-decay.md) and
  [video glitch](docs/generative_movement/video-glitch.md) engines, native
  controls and parameter studies, plus FFmpeg production adapters.
- Added compact guilloche and moiré reel workflows, media-cut derivation with
  [Supercut](docs/supercut.md), rotation/retiming utilities and documented
  distributed rendering and delivery checks.
- Documented the [MOSS appearance-to-local-animation workflow](docs/visual_workflow/tool_moss_local.md),
  including pinned appearance provenance, saved poses/masks, deterministic CPU
  replay, restart boundaries and full-episode review.

### Visualization and reproducible examples

- Expanded line graphs with axis annotations, decimated ticks, dashed secondary
  series and tooltip markers; corrected constant/nonzero domains and
  single-point rendering.
- Added categorical bars, adaptive histograms, paired-triangle count/share
  matrices, GeoJSON examples and richer swarm-metadata Kusama/chord views.
- Split [native form studies and authored compositions](docs/development/examples-workflows.md)
  into independent workflows with pilot/selected/full scopes, native catalog
  discovery, parameter and crossover coverage, source fingerprints and export
  receipts. Each produces HTML/SVG/PNG books, compact/vector PDFs and reels.
- Added canonical native review descriptors and source-ownership checks across
  registered graphics and media families.
- Fixed GCC/Clang build failures from a narrowing viewport initializer and
  misleading indentation in the video-glitch XML escape helper.
- Moved the six committed Fuller reference/example assets into
  `resources.static/`, preserving their content and documentation links while
  restoring the host-local resource split.

### Publication, audio and text

- Added [manifest-driven publication tools](docs/visual_workflow/tool_publication.md)
  for portrait, landscape-facing and full-bleed A4 interiors, reflowable EPUB,
  mobile PDFs, retailer ebook covers and typographic print-cover masters.
  Includes source hashes, explicit color profiles, layout checks, EPUBCheck
  integration and page/reader previews.
- Added [local audio/video transcription](docs/audio_workflow/tool_transcribe_audio.md)
  with an explicit Whisper model, stream/channel/excerpt selection, retained
  raw hypotheses, timestamped TXT/Markdown/JSON/SRT/VTT and hash-bound reruns.
- Added [DOCX word title videos](docs/visual_workflow/tool_docx_word_titles.md),
  paragraph clips, layered review reels and vertical Instagram derivatives.
- Added [transcription enrichment](docs/transcription-enrichment_workflow.md)
  for reviewed transcripts to Final Draft, Fountain and PDF, plus project-specific
  speech, voice audition and redubbing workflows.

### Documentation and upgrading

- Reorganized navigation into sibling General, Visual and Audio workflow
  groups. [Using](docs/using.md) describes every workflow's scope and links to
  detailed task guides, examples and production records.
- Refreshed generated Doxygen references, added a focused Screen Glitch
  reference, and added Markdown path/anchor and shared-diagram checks.
- Refreshed the canonical source inventory for 80 installed headers and nine
  include fragments. Font reproducibility checks retain exact artifact
  comparisons while validating runtime provenance separately; native catalog
  checks allow enough time for the complete Debug study grid.
- Aligned CMake's dense screen renderer with the production worker's `-O2`
  in GNU/Clang Debug builds, retaining debug symbols and assertions so the
  complete pixel-control checks can finish within their existing time limit.
- Updated the CMake package to `4.20261116`. Consumers should request
  `find_package(izzi 4 CONFIG REQUIRED)` and link `izzi::izzi`; the package
  retains `SameMajorVersion` compatibility. Schema versions remain independent.
- Users upgrading from the last published v2 release also receive the v3
  typography and color changes documented below: Atkinson Hyperlegible
  defaults, explicit color seeding, deterministic bands and constexpr modules.
- This is a source release. Optional production tools have additional media,
  model, font and Python dependencies; generated project deliveries and private
  source assets are not attached. Physical print proofing and production-specific
  human review remain separate from automated verification.

## v3.20260816 — 2026-08-16

### Added

- Guilloche vocabulary: `pattern_class` and `pattern_family` enums;
  `radial_ray_spec`, `harmonic_wave_spec`, `fish_scale_spec`,
  `barleycorn_spec`, `vignette_spec`, `medallion_spec`,
  `multicolor_line_spec`, and `flinque_overlay_spec`; new
  `composition_kind` values (`sunburst`, `fish_scale_field`,
  `barleycorn_field`, `band_guilloche`, `medallion`, `vignette`,
  `multicolor_line`, `flinque`); primitive-only shape rendering via
  `resolved_shape_kind`; recursive medallion insets; and a graph-layer
  composite mode via `graph_composite_spec`.
- JSON authoring: optional top-level `pattern {class, family}` enums.
- `examples/guilloche-capability-plates.cc` and `make
  guilloche-capability-review` generate 27 parameter-space plates; portal
  family `generation-guilloche-20260816`.

### Notes

- `make guilloche-check` now covers 23 tests, including the capability
  generator.
- Public header count unchanged (43).

## v3.20260815 — 2026-08-15

New major version after `v2.20260812`.

### Breaking changes

- Default typography converged to Atkinson Hyperlegible
  (`k::default_typo`); legacy faces remain available as named constants.
- Color family: `active_spectrum(sortbyhuep)` signature removed; RNG
  seeding is explicit (`color_default_seed`); `band_model {rgb, hsv}`;
  `color_band_cache` replaces `next_in_color_band`; palette tables and the
  enum→RGB map are constexpr data modules.
- Generation-baseline bytes change for every example that renders text
  (font-family) and for the color family (deterministic bands,
  linear-light sRGB interpolation).

### Added

- `generation_color` surface: `docs/generative_colors/`, portal
  `palette-20260814-index`.
- `generation_movement` surface: `izzi-svg-movement.h` (`movement_document`,
  `sample_path`/`sample_segment`, `izzi.movement/1`),
  `scripts/extract-movement.py`, portal `generation-movement-20260814`.
- `generation_visualization` surface: `izzi-svg-visualization.h`
  (grid/table/line/chord/kusama emitters, `izzi.visualization/1`),
  `build-alpha60-visualizations.py`, `generate-alpha60-includes.mjs`,
  WebAssembly driver (`src.wasm/`), portal
  `generation-visualization-20260814`.
- Subtitle/OCR pipeline (`scripts/build-subtitles.py`); local
  full-resolution proof index (`scripts/proof-local-outputs.py`).
- Public header count: 43 (was 42); package contract updated.

### Notes

- Generation-baseline candidates regenerated (0 varied); promotion stays
  gated on human review.
- Alpha60 producer migration is a separate project; the alpha60-results site
  `_includes` are now izzi-generated (cold cutover).
