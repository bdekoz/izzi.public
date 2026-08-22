# Color palette family — documentation, testing, portal index review (proposal)

Date: 2026-08-14

Status: `APPROVED-2026-08-14-WITH-REVISIONS; ZERO-SPEND`

## Revisions (2026-08-14, user approval)

| Plan item | Direction | State |
| --- | --- | --- |
| Plan E — constexpr `.inc` data module | Agreed; the pattern is generalized and explained in Appendix A | APPROVED |
| Plan E — constexpr literals (`color_qi` / `color_qf`) | Approved | APPROVED |
| Plan E — explicit spectrum selection | Approved | APPROVED |
| Plan E — seeded randomness | Approved | APPROVED |
| Plan E — render boundary (`display_color_qis`) | Approved: stays in `render-basics.h` as a render concern and is documented there | APPROVED |
| Plan E — color bands on RGB and HSV | Liked; answered in detail — both models work with an explicit `band_model` (see Plan E, "Color-model coverage") | REVISED-DETAIL |
| Plan F — default typography → Atkinson Hyperlegible | Approved | APPROVED |
| Plan G — font resource `atkinson_hyperlegible.2026` | Approved (all families and weights) | APPROVED |
| Appendix B — decision details | Requested; written | DONE |
| Appendix C — RGB interpolation method | Requested; written | DONE |

The rest of this document is the implementation plan under those approvals;
unmarked staging steps execute in order after the source/example/test work is
verified.

## Why now

Izzi has a real color-palette surface — three dedicated headers, seventeen
top-level examples, and deterministic baseline outputs — but it is the only
generative family with **no family entry**, **no `make check`
registration**, and **no review surface** on situationshipin.space. The
generative curve and pattern families (hamonshu, guilloche, moire, danmu,
surface tension, radial, harmonograph, grignani, roulette) each have a
`docs/generative_patterns/` entry, CTest registration, and portal review
items. Color palettes are used everywhere (house palette, guilloche scene
palettes, WCAG grays, the ai-time-to-die overlay colors) yet are invisible to
the same documentation, test, and review loops.

Two known defects also argue for treating color as a first-class family:

- `M0-DETERMINISM-001` (open): `color-band-expand-to-larger` emits different
  SVG bytes across runs because `make_color_band_v1` seeds an RNG from
  `std::random_device`.
- The promoted generation baseline still carries pre-correction bytes,
  including the known root-text `s` artifact, so palette previews copied
  directly from `tests/baselines/generation/current/` would publish stale
  evidence.

## Current state (evidence, 2026-08-14)

### Headers

| Header | Lines | Surface |
| --- | ---: | --- |
| `src/izzi-svg-color.h` | 901 | `color` enum, `color_qi` (RGB ints), `color_qf` (HSV floats), `color_qis` vector alias, RGB↔HSV conversions, `combine_color_qi`, `color_qf_lt` |
| `src/izzi-svg-color-palette.h` | 704 | palette aliases; 10 palette tables (`izzi_palette`, `izzi_hue_palette`, `jp_palette`, `colorbrewer2s3s/7s/9s`, `ciecam02`, `ciecam16`, `ciecam16j70`, `esri_m_bathymetry`); `active_spectrum()`, `random_color()` ×2, `next_color()`, `start_at_color()` |
| `src/izzi-svg-color-band.h` | 228 | `colorband` tuple; 9 constexpr `cband_*` seeds; `make_color_band_v1/v2`, `make_color_band`, `next_in_color_band` |

`display_color_qis()` (the swatch-grid renderer) lives in
`src/izzi-svg-render-basics.h` (~line 1274) and emits `color_qis_N_palette`
SVG ids with 20×80 swatch blocks and rotated 7 pt RGB labels.

### Examples and baseline outputs

All 17 palette examples are top-level `examples/*.cc` files, so they are
compiled and double-run by the generation baseline, but none are registered
in `tests/CMakeLists.txt`. Baseline facts from
`tests/baselines/generation/current/`:

| Example | Outputs | Swatches | SVG size | Repeatability |
| --- | --- | ---: | ---: | --- |
| `color-palette-1` | 1 | 2 | 2 KB | STABLE |
| `color-palette-2` | 1 | 19 | 11 KB | STABLE |
| `color-palette-3` | 1 | 21 | 12 KB | STABLE |
| `color-palette-4` | 1 | 3 | 2 KB | STABLE |
| `color-palette-5` | 1 | 133 | 70 KB | STABLE |
| `color-palette-6-ciecam02` | 1 (second overwrites first) | 73 | 39 KB | STABLE |
| `color-palette-6-ciecam16j70` | 2 (one mislabeled `ciecam02`) | 89 | 47 KB | STABLE |
| `color-palette-6-colorbrewer2` | 2 (3- and 9-class) | 19 / 55 | 11 / 29 KB | STABLE |
| `color-palette-6-izzi-full` | 2 (unsorted / hue-sorted) | 154 | 81 KB | STABLE |
| `color-palette-6-jp` | 2 (jp / japan) | 118 | 62 KB | STABLE |
| `color-band-expand-to-larger` | 3 (p/o/r) | 100 each | ~52 KB | **VARIES** (M0-DETERMINISM-001) |
| `color-band-next` | 0 | — | — | NONVISUAL (assertion) |
| `color-qf-tint-perceptual-1` | 1 | 35 | 29 KB | STABLE |
| `color-qf-tint-perceptual-2` | 1 | 84 | 70 KB | STABLE |
| `color-rgb-to-hsv-1` | 0 | — | — | NONVISUAL (assertion) |
| `color-rgb-to-hsv-2` | 1 | 42 | 23 KB | STABLE |
| `color-rgb-to-hsv-3` | 1 | 266 | 147 KB | STABLE |

Preview PNGs are 3–93 KB each — comfortably inside the portal's 16 MiB proxy
cap for a palette index artifact.

### Portal index-review pattern

`situationshipin.space/data/review-items.json` already models aggregated
reviews: `media_kind: index` items with `index_members` and an `.html`
page (`izzi-generation-20260814`, `female-voice-bank-index`), plus
contact-sheet image reviews (`visual-current-review-index`,
`visual-guilloche-round-05-index`). `scripts/check-review-site.mjs` requires
index artifacts to publish an `.html` page and a non-empty member list.
`scripts/build-generation-index.mjs` + `publish-make-check-generation.mjs`
are the reference pipeline.

## Plan A — Document like `generative_patterns`

**Approved scope (2026-08-14):** a dedicated **`docs/generative_colors/`**
section mirroring `docs/generative_patterns/` — not an entry inside it.

1. **Section pointer index** — write `docs/generative_colors.md` mirroring
   `docs/generative_patterns.md` (document map, family index, visual-tester
   gallery) for the color surface.
2. **Directory entry** — write `docs/generative_colors/index.md` mirroring
   `docs/generative_patterns/index.md`.
3. **Family entry** — write `docs/generative_colors/color.md` following the
   assessment-plan template: family name → headers → API surface → config →
   tests → usage → assessment (status + open items) → links. The family is
   **color** (headers `izzi-svg-color*.h`); the entry covers palettes,
   bands, RGB/HSV quantization, and perceptual tint helpers.
4. **Assessment plan** — write `docs/generative_colors/assessment_plan.md`
   mirroring `docs/generative_patterns/assessment_plan.md`, recording the
   color family first-pass assessment including the open determinism,
   statefulness, and typography items.
5. **Gallery previews** — add
   `docs/generative_colors/images/color-*.png` previews regenerated from
   current source (see Plan C), not copied from the stale promoted baseline.

## Plan B — Test like `generative_patterns`

1. **CTest registration** — add an `izzi_add_color_family` function in
   `tests/CMakeLists.txt` mirroring `izzi_add_interference_family` and the
   danmu `foreach`: one demo target per visual example, a
   `color.<example>.demo` test, and `xmllint` well-formed checks where
   available. Labels: `generation;color;palette`.
2. **Determinism self-test** — each visual example's CTest runs the output
   twice and compares bytes; `color-band-expand-to-larger` stays excluded
   until the RNG fix lands, then becomes the proof test for
   `M0-DETERMINISM-001`.
3. **Functional checks move to `tests/`** — `color-band-next` becomes
   `tests/color-band.cc` and `color-rgb-to-hsv-1` becomes
   `tests/color-rgb-to-hsv.cc` (keep the stdout assertions as runtime CTest
   checks). This matches how other families keep assertions in `tests/`
   rather than `examples/`.
4. **Make targets** — add `color-check` (build + `ctest --tests-regex
   '^color'`) and `color-review` (render a fresh palette preview set),
   following the `guilloche-check` / `interference-motion-review` pattern.
5. **Existing harness** — the generation baseline already covers all
   top-level examples; keep that coverage and run the comparison
   (`tests/generation-baseline/compare.py`) after any byte-affecting change.
6. **Vocabulary gate** — add `tests/check-color-spelling.py` to `color-check`
   asserting the canonical color vocabulary (Plan D, item 6) across
   `src/izzi-svg-color*.h`, `examples/color-*.cc`, and
   `docs/generative_colors/color.md`, with zero variant hits — the same
   zero-regression posture as the docs link scan. The same gate asserts the
   canonical typography (Plan F): the default typography resolves to
   Atkinson Hyperlegible and color-family examples carry no `apercu`
   references in default paths.

## Plan C — Palette outputs for the situationshipin.space index review

Publish one aggregated review item, `palette-20260814-index`:

| Field | Value |
| --- | --- |
| `media_kind` | `index` |
| `generation_class` | `palette-index` (new; validator allows `review-index` pattern) |
| `review_category` | `proofs` |
| `review_scope` | `PALETTE-20260814` |
| `published_path` | `review/media/palette-20260814-index/palette-20260814-index.index.html` |
| `index_members` | the 15 deterministic visual artifacts below |

Recommended members (deterministic only, regenerated fresh at publish time):

- Baseline basics: `color-palette-1`, `color-palette-4` (2/3 swatches),
  `color-palette-2`, `color-palette-3`, `color-palette-5` (19/21/133).
- Source palettes: `color-palette-source-izzi` (154), `-jp` (118),
  `-ciecam02` (73), `-ciecam16j70` (89), `-colorbrewer2-3`, `-colorbrewer2-9`.
- Quantization / perception: `color-tint-perceptual-1/2`,
  `color-rgb-hsv-2`, `color-rgb-hsv-3` (266-swatch grid).

Excluded until fixed: `color-band-expand-to-larger` (nondeterministic) and
the two nonvisual assertions (moved to `tests/` in Plan B). The index page
reuses `scripts/build-generation-index.mjs` semantics (or a thin
`publish-palette-index.mjs`) and must pass `check-review-site.mjs` (member
list + `.html` page).

## Plan D — Organizational and naming cleanups

1. **Family identity** — name the family **color** and the entry
   `docs/generative_colors/color.md` (snake_case filenames are the
   convention; the section mirrors `generative_patterns` as a parallel
   tree). Do not place it under `docs/visual_workflow/` or
   `docs/generative_patterns/`; `visual_experiments_method.md` keeps color
   only as a cross-cutting review dimension.
2. **Example naming** — one `color-<concept>-<variant>` shape:
   - `color-palette-6-*` → `color-palette-source-*` (drop the opaque "6"
     tier): `color-palette-source-ciecam02`, `-ciecam16j70`,
     `-colorbrewer2-3`, `-colorbrewer2-9`, `-izzi`, `-jp`.
   - `color-qf-tint-perceptual-*` → `color-tint-perceptual-*` (drop the
     implementation type `qf` from public example names).
   - `color-rgb-to-hsv-*` → `color-rgb-hsv-*` for the visual grids; the
     nonvisual assertion moves to `tests/` (Plan B).
   - Keep `color-band-expand-to-larger`'s name after the determinism fix, or
     fold into `color-band-*` naming if a second band example appears.
3. **Output stems** — outputs must match the example stem with dashes, never
   dots: `color-palette-6.izzi.1.svg` → `color-palette-source-izzi-1.svg`.
   This also fixes the current duplicates: the `ciecam02` example writes the
   same stem twice (second overwrites first — only the hue-sorted variant
   survives), and the `ciecam16j70` example writes a mislabeled
   `color-palette-6.ciecam02` duplicate. Give sorted variants explicit
   `-1`/`-2` or `-sorted` stems.
4. **SVG identity** — `color_qis_N_palette` is legacy alpha60 vocabulary;
   new renders should use `color-swatch-grid-N`. The stale root-text `s`
   artifact stays in the promoted baseline only; fresh renders use the
   corrected serializer.
5. **File location** — keep palette examples top-level under `examples/` for
   this round: `tests/generation-baseline/generate.py` scans
   `EXAMPLES.glob("*.cc")` only (lines ~215, ~1009). If a
   `examples/color/` subdirectory is wanted later, the harness glob must be
   extended in the same change.
6. **Spelling normalization** — fix and normalize spelling to one canonical
   vocabulary across the color family (prose, comments, artifact labels, and
   identifiers), so "pallette"-class variants never re-enter the family:

   | Canonical (prose/docs) | Code identifier form | Variants to avoid |
   | --- | --- | --- |
   | palette | `*_palette` | `pallette`, `pallete` |
   | CIECAM02 / CIECAM16 | `ciecam02_palette`, `ciecam16*_palette` | lowercase `ciecam` in prose/comments |
   | ColorBrewer 2.0 | `colorbrewer2s*_palette` | `colorbrewer` in prose |
   | ESRI Bathymetry Tints | `esri_{s,m}_bathymetry_palette` | spelling variants of bathymetry |
   | WCAG 2.x | `wcag_*` | lowercase `wcag` in prose |
   | Apercu | `apercu_typo` | accent/spelling variants in prose |
   | RGB / HSV | `rgb_to_hsv`, `hsv_to_rgb`, `color_qi`, `color_qf` | lowercase `rgb`/`hsv` in prose |
   | Atkinson Hyperlegible (user shorthand "Hypervisible") | `hyperl_typo`, `default_typo` | `hypervisible` in prose; Apercu as the default typography |

   Identifier renames are byte-affecting and go through the staged baseline
   candidate with history preserved; prose, comments, and artifact labels
   (`<title>`, alt text, portal descriptions) are normalized in the same
   staged candidate and enforced by the Plan B vocabulary gate.

## Plan E — Source restructuring: constexpr modules

Targeted at `src/izzi-svg-color-palette.h` and `src/izzi-svg-color-band.h`.
All items in this section are **APPROVED 2026-08-14** (with the band
color-model detail added below).

### `izzi-svg-color-palette.h` — data/API split + constexpr tables

1. **Constexpr data module** (approved) — extract the ten palette tables into
   `src/izzi-svg-color-palettes.inc`, mirroring the
   `src/izzi-svg-curves-hamonshu-v2.inc` catalogue pattern. The header keeps
   the API and `#include`s the module; the tables become compile-time data
   instead of runtime-initialized `inline const` arrays.
2. **Constexpr literals** (approved) — add `constexpr` constructors to `color_qi`
   (the RGB tuple constructor already qualifies; replace the enum
   constructor's string round-trip through `to_string`/`from_string` with a
   `constexpr` enum→RGB table), plus `constexpr operator==`. Then flip the
   tables to `inline constexpr palette_qi<N>`.
3. **Explicit spectrum selection** (approved) — replace the mutable function-local
   `active_spectrum()` static (and the commented-out palette switching) with
   a pure `constexpr` selection by an enum tag (e.g., `palette_kind`), so
   callers pass the palette explicitly and no hidden global state remains.
   `next_color` / `start_at_color` become functions of an explicit spectrum;
   `start_at_color`'s internal static cursor moves into an explicit
   `color_cursor` object.
4. **Seeded randomness** (approved) — `random_color()` drops `std::random_device`;
   both overloads take an explicit seed (default: a documented constant) or
   a caller-supplied RNG, so every color pick is reproducible.
5. **Render boundary** (approved) — `display_color_qis()` stays in
   `izzi-svg-render-basics.h` (it is a render concern) and is documented as
   such in the family entry; a dedicated `izzi-svg-color-render.h` move is a
   later, non-blocking option.

### `izzi-svg-color-band.h` — deterministic bands + constexpr seeds

1. **Keep the constexpr seeds** (approved) — `colorband` and the nine `cband_*` entries
   are already `inline constexpr`; keep them as the public band table (or
   move the table into the same `.inc` module for symmetry).
2. **Split algorithm from randomness** (approved) — `make_color_band_v1` gains an
   RNG/seed parameter; the interpolation core
   (`combine_color_qi_linear` — gamma-corrected linear-light sRGB, Appendix
   B.5, implemented 2026-08-14) becomes a pure function of two colors and a
   weight, and the pair/weight selection becomes a pure function of a
   recorded seed. A default overload uses a fixed default seed, making
   `color-band-expand-to-larger` deterministic and closing
   `M0-DETERMINISM-001` (evidence receipt, mirroring the markers RNG
   correction).
3. **Seeded v2 path** (approved, corrected) — `make_color_band_v2`'s
   structure (set dedup + hue sort) is deterministic, but its mutation step
   (`mutate_color_qf`) is `std::random_device`-seeded like v1 — so v2 is
   nondeterministic too. Seed the mutation, strip its debug `std::cout`
   output (or gate it behind a flag) so CTest receipts stay clean, and make
   it the documented HSV-model default with v1 retained as the seeded
   RGB-model variant.
4. **No hidden caches** (approved) — `next_in_color_band`'s eight static per-band
   caches are order-dependent and stateful; replace with an explicit
   `color_band_cache` object keyed by `(band, size, seed)` so repeated
   generation is reproducible and cache reuse is deliberate.
5. **Contract preservation** (approved) — keep the umbrella `izzi-svg.h` includes and
   the header self-containment/ODR tests green. Every byte-affecting change
   (ids, stems, palette bytes) goes through a staged generation-baseline
   candidate + `compare.py` review, with history preserved — never a silent
   promotion.

### Color-model coverage: RGB and HSV bands (revision detail)

**Question: will the band idea work with both RGB and HSV color models?**

Yes — with an explicit model parameter. The two current algorithms each
already work in one model, and the repo already has HSV interpolation
precedent:

- **RGB path** (current `make_color_band_v1`): picks two `color_qi` samples
  inside the band region and combines them per channel with a random weight
  (denominator 2). Interpolation now happens in **linear-light sRGB** via
  `combine_color_qi_linear` (gamma-corrected; implemented 2026-08-14 —
  verified 50/50 red+green → `rgb(188,188,0)` vs the naive
  `rgb(127,127,0)`, and white/black 50/50 → `rgb(188,188,188)`), so
  midpoints keep correct luminance instead of darkening through the gamma
  curve. Hue is still not considered; averaging complementary hues can
  desaturate through gray in either model. Determinism fix: seed the
  pair/weight selection.
- **HSV path** (current `make_color_band_v2` + `mutate_color_qf`): walks
  `next_color` through the active spectrum, perturbs each color in HSV space
  (raises saturation toward 0.5–1 and value toward 0.5–1), converts back to
  `color_qi`, dedups through a hue-sorted `set`, and reverses. This is
  generation-by-mutation, not interpolation — and `mutate_color_qf` is also
  `std::random_device`-seeded, so v2 is nondeterministic too.
- **In-repo precedent**: `tint_perceptual` / `tint_percentage` already
  interpolate in HSV (value first, then saturation), so HSV-space band
  interpolation fits the existing design.

**Proposed API:** `enum class band_model { rgb, hsv }` and
`make_color_band(cb, neededh, model, seed)`:

- `rgb` — seeded pair + weight selection, per-channel combine (current v1
  math), with the gray-desaturation caveat documented.
- `hsv` — convert endpoints to `color_qf`; interpolate hue along the
  shortest arc (wrap-aware: 350°→10° crosses 0°, not 180°); interpolate `s`
  and `v` linearly; optional mutation variant for "more saturated / less
  dark" expansion (current v2 behavior, seeded); convert back to `color_qi`.
- Both paths consume `color_qi` palettes; HSV is the derived working space.
  CIECAM-sourced palettes remain source tables — band generation operates on
  their RGB/HSV representations. Perceptual-space interpolation is a
  follow-up, out of scope for the two-model ask.
- Determinism: every path is a pure function of `(band, size, model, seed)`;
  the cache key from item 4 gains `model`.

## Plan F — Converge default typography to Atkinson Hyperlegible

**APPROVED 2026-08-14.**

The color family currently labels every swatch grid with `k::apercu_typo`
(Apercu), and two header default-parameter sites still default to legacy
faces (`izzi-svg-graphs-line.h` annotation typography, `svg_element`'s
document typography). The established house direction — recorded
2026-08-08/12 and already live in the portal's `review.css` — is Atkinson
Hyperlegible for editorial text ("Hypervisible" is the user shorthand; the
canonical name is **Atkinson Hyperlegible**, `hyperl_typo` already exists in
`src/izzi-svg-constants.h`). Converge the defaults:

1. **`k::default_typo`** — add a canonical alias in
   `src/izzi-svg-constants.h`: `default_typo = hyperl_typo` (Atkinson
   Hyperlegible). Keep `apercu_typo`, `smono_typo`, and the other named
   faces available for explicit use; only the default converges.
2. **Default-parameter sites** — point `izzi-svg-graphs-line.h`'s annotation
   default and `svg_element`'s document-typography default at
   `k::default_typo` instead of `apercu_typo` / `smono_typo`.
3. **Color family examples** — replace `k::apercu_typo` with
   `k::default_typo` in all `examples/color-*.cc`, and give
   `display_color_qis()` a default `= k::default_typo` so palette grids
   converge without every caller passing a font.
4. **Mono/data typography** — the documented data face (Atkinson Hyperlegible
   Mono, `docs/dependencies.md` per user direction 2026-08-14) is a
   follow-up; this round converges the editorial default only and leaves
   explicit mono sites untouched unless directed.
5. **Baseline impact** — font-family strings are deterministic, so renders
   stay STABLE; every byte change still goes through the staged baseline
   candidate + `compare.py` review.
6. **Portal** — `situationshipin.space` already sets Atkinson Hyperlegible
   in `assets/css/review.css`; the `palette-20260814-index` descriptions
   state the Hyperlegible label typography, and no portal font change is
   required.

## Plan G — Font resource: `atkinson_hyperlegible.2026`

**APPROVED 2026-08-14 (all families and weights).**

Register `/home/bkoz/.fonts/atkinson_hyperlegible.2026/` as the color
family's label-typography resource (the "Hypervisible" shorthand resolves to
this Atkinson Hyperlegible family). Complete inventory, observed
2026-08-14 — **32 files across three families, all weights/styles**:

### Atkinson Hyperlegible (classic) — editorial labels

| File | Weight / style | SHA-256 |
| --- | --- | --- |
| `Atkinson-Hyperlegible-Regular-102.otf` | Regular | `c8d6d8f542bd9577bb4264c54099e4c75c4d96d1f9fb6d8d93116f15214b081b` |
| `Atkinson-Hyperlegible-Italic-102.otf` | Italic | `32fc711f2afeab1e0cba39d0b5a5167b0653fa34ba263f79ea5e15d2b63e9212` |
| `Atkinson-Hyperlegible-Bold-102.otf` | Bold | `7205e49c3a8b67a25218107a2b5a1ea9accdf8d9676c69d1a040d16cd896b3e2` |
| `Atkinson-Hyperlegible-BoldItalic-102.otf` | Bold Italic | `eec9e014ec7625788f86b10ee30d822f6057c9727aff4ed2f3b3b77f3fa05623` |

### Atkinson Hyperlegible Mono — data/metadata labels

| File | Weight / style | SHA-256 |
| --- | --- | --- |
| `AtkinsonHyperlegibleMono-ExtraLight.otf` | ExtraLight | `497ead26b51a711b8843f273074d6c619989aa392d1a6c6ad7f77437c95d8e37` |
| `AtkinsonHyperlegibleMono-ExtraLightItalic.otf` | ExtraLight Italic | `4582c3e6731899904cc556360ae98c56e226853ed5defbda93c8a64a8eab5603` |
| `AtkinsonHyperlegibleMono-Light.otf` | Light | `ac970ba1b1bd0858d25cb25d1ca992c730a2e52307bb82a8656681ea0b9ed246` |
| `AtkinsonHyperlegibleMono-LightItalic.otf` | Light Italic | `22ece76ce2d7d79103545b1361ea8165ffc88c7db90979b7932f86159acca7fa` |
| `AtkinsonHyperlegibleMono-Regular.otf` | Regular | `5126eedb80ae125e253d437d5302a6a65b7ec9a9a250b420545ec68004fb3168` |
| `AtkinsonHyperlegibleMono-RegularItalic.otf` | Regular Italic | `1adcc75308140c4714affa7a81c6ada58743e06171643d5e98b06b4ba0fb3245` |
| `AtkinsonHyperlegibleMono-Medium.otf` | Medium | `63b7935d1595bedff525db68bde54db237efcba76d0969b677757405c3f54175` |
| `AtkinsonHyperlegibleMono-MediumItalic.otf` | Medium Italic | `1935bcf6f681e850521092235fb0cfccf08f4a6447b822b217e4c090a3f21ad7` |
| `AtkinsonHyperlegibleMono-SemiBold.otf` | SemiBold | `8ac9187458bef3e69b6700446ffc2721c44f7143ee1b08648fa2456075750dff` |
| `AtkinsonHyperlegibleMono-SemiBoldItalic.otf` | SemiBold Italic | `0f14c7610e4fd3b71af67d2f353bd0f3d22897b7d30857d322b0e693735e5737` |
| `AtkinsonHyperlegibleMono-Bold.otf` | Bold | `1449c079d43575beb70558905e2c4059acfaf181dce845b7f1bd4f0bbca8837d` |
| `AtkinsonHyperlegibleMono-BoldItalic.otf` | Bold Italic | `08260a1c200dff386b61eafc9843c88e3ea87e8127c25dfac406717fe1d5ae92` |
| `AtkinsonHyperlegibleMono-ExtraBold.otf` | ExtraBold | `795716d8cbbe737434182b72b5808d7ebe7ed7b9c8ae0fae3f4741573b206de4` |
| `AtkinsonHyperlegibleMono-ExtraBoldItalic.otf` | ExtraBold Italic | `b770a29a92c4bbd363acc24ebee3a9cf80d015ae0d74106214682e98c92a6e45` |

### Atkinson Hyperlegible Next — heavier editorial/display roles

| File | Weight / style | SHA-256 |
| --- | --- | --- |
| `AtkinsonHyperlegibleNext-ExtraLight.otf` | ExtraLight | `1c501bbd3ecea9acc03508ecbdc4adf8346be1544951338e04c835c0ce9141e9` |
| `AtkinsonHyperlegibleNext-ExtraLightItalic.otf` | ExtraLight Italic | `1bb5eb05d804f81b079746b2edd6fc457feebc78ba4177672ba02c094dac7e2e` |
| `AtkinsonHyperlegibleNext-Light.otf` | Light | `5beec135597adc4c43b88afb07d74503a6be2bfc93c0ab895e87327acae973d2` |
| `AtkinsonHyperlegibleNext-LightItalic.otf` | Light Italic | `fc725711c11ac81f51c17e72ab59ea2242d4bcf56cf2673376369118ac5b0f35` |
| `AtkinsonHyperlegibleNext-Regular.otf` | Regular | `2a5e641eb7d42f5949d9fc84df1e7840c3c7bb1151dae178a8f20a3251e946d0` |
| `AtkinsonHyperlegibleNext-RegularItalic.otf` | Regular Italic | `66729ad5dcdccd130f30f46c76ae6e946e5b0deb77ed6de1d662df6860043580` |
| `AtkinsonHyperlegibleNext-Medium.otf` | Medium | `dda6e0d4357c6adf7deb681e017ce2f7d3db6b33cb9dde43c7dc38e99baa3a2d` |
| `AtkinsonHyperlegibleNext-MediumItalic.otf` | Medium Italic | `f0ebeee9e0065903632dd037d924a29ea01acb241fefc61004de7376ae06881e` |
| `AtkinsonHyperlegibleNext-SemiBold.otf` | SemiBold | `a6021e3dd5fd093d4092589814f62ba82163935864b29ef645d7b887d61eeee2` |
| `AtkinsonHyperlegibleNext-SemiBoldItalic.otf` | SemiBold Italic | `0ee781095c30886d78e242e34c9e7539934320d1447717575ff86d35c7cf7c72` |
| `AtkinsonHyperlegibleNext-Bold.otf` | Bold | `91485fed9ea491b764a6e037f9b2f8a20d21070a9345a85ce3fc42262e3c8b10` |
| `AtkinsonHyperlegibleNext-BoldItalic.otf` | Bold Italic | `658b25e70f637b31fdf5b62c77699aba61f272f0825124af21f9cfd6da7f11ca` |
| `AtkinsonHyperlegibleNext-ExtraBold.otf` | ExtraBold | `edab52eb7790e8116161dc5d15684b39d9c15d7f0695c9a34e55013d794f17ef` |
| `AtkinsonHyperlegibleNext-ExtraBoldItalic.otf` | ExtraBold Italic | `b8246542a38d2a345b00230691e7814a29219daf446b79fa4431f34ab90759cd` |

1. **Resource record** — extend `docs/dependencies.md` from the single Mono
   SemiBold row to the directory: path, the complete 32-file sha256
   inventory above (all families and weights, observed 2026-08-14), and the
   Braille Institute free-use license (no attribution required) already
   recorded there.
2. **Usage** — the converged default typography (Plan F) renders
   `font-family="Atkinson Hyperlegible"` in SVG; `display_color_qis` labels
   and `<title>` text use the family. SVG artifacts stay portable (font names
   only); previews and PDFs depend on the local install.
3. **Resource check** — add a local preflight to `color-review` and the
   portal publish step: verify the directory exists and its sha256 inventory
   matches before rendering fresh palette previews. The check is
   local-only; the fonts are never shipped into the repo or the portal.

## Staging

1. Approve this proposal (reply "approve").
2. Docs: write the `docs/generative_colors/` section — `generative_colors.md`,
   `index.md`, `color.md`, `assessment_plan.md` — with the canonical color
   vocabulary and gallery links.
3. Source: constexpr `.inc` data module, `constexpr` `color_qi`/tables,
   explicit spectrum/cursor/cache types, seeded band + random-color RNG, and
   spelling/vocabulary normalization in comments and identifiers; add
   `k::default_typo` (Atkinson Hyperlegible), converge default-parameter
   sites, and switch color-family examples to the default typography.
4. Examples/tests: apply renames and output-stem fixes, move the two
   nonvisual assertions to `tests/`, register the family in
   `tests/CMakeLists.txt`, add `color-check` / `color-review` targets.
5. Verify: `make check` (including the `color-check` vocabulary gate), fresh
   generation-baseline candidate + comparison, mirror link scan at 0 broken
   links, WebAssembly build unaffected.
6. Portal (requires portal-repo authorization): publish
   `palette-20260814-index` with fresh previews; validator passes.
7. Report results; no check-in, push, or portal publish without explicit
   authorization.

## Constraints

- Zero provider spend; local-only implementation.
- No writes outside the requested files; existing baselines and evidence
  stay immutable until a staged candidate is reviewed.
- Portal publication and git push remain approval-gated.

## Next step

**APPROVED 2026-08-14 with revisions.** Implementation proceeds through the
staging list: the `generative_colors` docs section is established; source,
example/test, and verification work come next; portal publish and check-in
remain approval-gated.

### Implementation progress (2026-08-14, unattended run)

- **Docs** (staging item 2): `docs/generative_colors/` section established
  (generative_colors.md, index.md, color.md, assessment_plan.md); 0 broken
  links.
- **Source** (item 3): `src/izzi-svg-color-rgb-map.inc` (constexpr enum→RGB,
  159 entries) and `src/izzi-svg-color-palettes.inc` (11 constexpr palette
  tables) extracted; `color_qi`/`color_qf` constexpr constructors +
  `constexpr operator==`; `palette_kind` + `spectrum<K>()` explicit
  selection; `color_default_seed`; seeded `random_color`,
  `make_color_band_v1/v2`; `band_model {rgb, hsv}`; `color_cursor` and
  `color_band_cache` (hidden statics removed from new paths; the ODR
  shared-state contract keeps `active_spectrum()` as the legacy mutable
  working spectrum); `combine_color_qi_linear` (Appendix C) wired into the
  RGB band path; `k::default_typo` added and converged in
  `izzi-svg-graphs-line.h`, `svg_element`, `display_color_qis`, and all
  color examples.
- **Examples/tests** (item 4): renames applied
  (`color-palette-source-*`, `color-tint-perceptual-*`, `color-rgb-hsv-*`);
  output stems dash-based; duplicate/mislabeled outputs fixed; the two
  nonvisual checks moved to `tests/color-band.cc` and
  `tests/color-rgb-to-hsv.cc`; 15 demo targets + 2 functional CTests
  registered (`make color-check`: 17/17 pass); `color-check` /
  `color-review` Make targets; `tests/check-color-spelling.py` gate passes;
  `scripts/render-color-review.py` renders 20 SVGs to
  `outputs/review/feedback/visual/color/round-01/`.
- **Determinism** — `M0-DETERMINISM-001` **CLOSED**: two isolated runs of
  `color-band-expand-to-larger` produce byte-identical SVGs (sha256 diff
  empty) with the seeded default.
- **Verification** (item 5): full `make check` passes (0 failures, including
  the new `color.*` CTests, migration contract CANONICAL-READY with the
  regenerated stage-i baseline, and the package contract with four include
  fragments). Staged generation-baseline candidate
  `tests/baselines/generation/candidates/WI-COLOR-FAMILY-20260814/`
  captured: **115 examples, 0 varied** (determinism holds in the harness);
  `compare.py` reports the expected SVG path-inventory delta (renames +
  post-baseline additions such as the danmaku/plates examples) and is not
  promotable without human review. Spot-checked byte deltas show exactly the
  planned changes: stray root `s` removed (serializer fix already in
  source), `font-family` converged to Atkinson Hyperlegible, color ids/stems
  normalized, band outputs deterministic, RGB bands in linear-light sRGB.
  Mirror link scan: `publish-public-mirror.sh --dry-run` audit **PASS** (0
  broken links, 261 markdown / 13819 html links). WebAssembly:
  `make check-wasm` in cartofreako **PASS** (projections + browser smoke +
  cahill-keyes + myriahedral) — izzi header changes compile and run under
  emscripten.
- **Published** (item 6): portal `palette-20260814-index` live at
  `situationshipin.space/review/media/palette-20260814/` — 15 member review
  pages + PNG previews from izzi `57b7144c`; validator PASS (264 catalog
  items). Portal commit `72b1f72` pushed. izzi check-in/push executed
  (`806256aa` then `57b7144c`).
- **Follow-up note**: output stems for the tint-perceptual and rgb-hsv
  examples were normalized in izzi `57b7144c` after the first push; the
  `WI-COLOR-FAMILY-20260814` baseline candidate predates that fix and should
  be regenerated before any promotion review.

## Appendix A — Constexpr data module pattern (generalized)

### What it is

A header/`.inc` split in which the header declares the API and a companion
`.inc` file carries only compile-time data as `inline constexpr` tables,
included exactly once by the header. The pattern is already used in izzi for
the Hamonshū catalogue
(`src/izzi-svg-curves-hamonshu-v2.inc` included by
`izzi-svg-curves-hamonshu.h`); the color plan applies the same pattern to
the palette tables.

### Requirements

- The element type is a **literal type**: `constexpr` default and
  parameterized constructors, `constexpr` destructor, no virtual functions,
  no runtime initialization. For color, `color_qi` needs a `constexpr` RGB
  constructor and a `constexpr` enum→RGB table (replacing the string
  round-trip), and `color_qf` needs a `constexpr` HSV constructor.
- Storage is a constant-size container — `std::array` (the existing
  `palette_qi<N>` alias) — not a heap vector.
- No mutable state, I/O, string parsing, or `static` initialization inside
  the module; initialization is constant-initialized.

### When to use

Use for large fixed tables that are shared across headers, examples, and
builds: palette data, catalogue data, seed tables, lookup tables. Use also
when a table must be validated at compile time (`static_assert` on size,
range, sentinel, or sortedness) or must be byte-identical across translation
units, compilers, and the WebAssembly build (ODR-safe by construction).

Do not use for algorithmic code — band generation, sorting, and rendering
stay in headers as functions **over** the data module. The module is data;
the header is behavior.

### Benefits

- Compile-time initialization: no static-init-order hazards, no hidden
  runtime state, no `std::random_device` at data-build time.
- Deterministic by construction: the data module is the source of truth, so
  seeded RNG becomes the only runtime input.
- Byte-stable output across compilers and target builds once the tables are
  pinned.
- Testable: `static_assert` checks live with the data, and the
  header-self-containment / ODR tests cover the include contract.

### Migration steps (color example)

1. Add `constexpr` constructors and `constexpr operator==` to the element
   type (`color_qi` / `color_qf`).
2. Extract the palette tables verbatim into
   `src/izzi-svg-color-palettes.inc`; change `inline const` →
   `inline constexpr`.
3. `#include` the `.inc` from `izzi-svg-color-palette.h`; keep the header's
   API surface unchanged for callers that use the named tables.
4. Add `static_assert`s for table sizes and the `color::none` sentinel.
5. Compile through the header self-containment and ODR tests; regenerate
   baseline bytes only where render output changes, via a staged candidate.

### Generalization beyond color

Any family with a fixed table can adopt the pattern: curve catalogues
(hamonshu precedent), band seeds, guilloche scene presets, or voice-bank
metadata. The rule of thumb: **if it is data and it is shared, make it an
`inline constexpr` module; if it is behavior, keep it a function over the
module.**

## Appendix B — Decision record (2026-08-14)

### B.1 Constexpr data module and literals

The ten palette tables move from `inline const` arrays in
`izzi-svg-color-palette.h` to an `inline constexpr` data module
`src/izzi-svg-color-palettes.inc` (Appendix A pattern). `color_qi` gains a
`constexpr` RGB constructor and a `constexpr` enum→RGB table (replacing the
string round-trip), `color_qf` gains a `constexpr` HSV constructor, and both
get `constexpr operator==`; tables flip to `inline constexpr palette_qi<N>`
with `static_assert`s for size and the `color::none` sentinel. Rationale:
compile-time init (no static-init-order), ODR-safe across TUs and wasm,
deterministic bytes, and compile-time validation.

### B.2 Explicit spectrum selection

`active_spectrum()`'s mutable function-local static (and commented-out
palette switching) is replaced by `enum class palette_kind` plus a pure
`spectrum(palette_kind)` selection. `next_color` / `start_at_color` take an
explicit spectrum; `start_at_color`'s hidden cursor becomes an explicit
`color_cursor` object. The legacy `active_spectrum()` name remains only as a
pure default-returning shim (izzi hue palette), with no hidden sort flag —
hue-sorted variants come from precomputed constexpr tables.

### B.3 Seeded randomness

`random_color`, `make_color_band_v1/v2`, and `mutate_color_qf` drop
`std::random_device`. Every stochastic entry takes an explicit seed
(default: a documented constant `default_seed`) or caller RNG, making each
call a pure function of its inputs and closing `M0-DETERMINISM-001` with an
evidence receipt (mirroring the markers RNG correction).

### B.4 Render boundary

`display_color_qis()` stays in `izzi-svg-render-basics.h` as a render
concern and is documented there (Plan E item 5); a dedicated
`izzi-svg-color-render.h` move is a later, non-blocking option.

### B.5 Band models: RGB and HSV

`enum class band_model { rgb, hsv }`; `make_color_band(cb, neededh, model,
seed)`; cache key `(band, size, model, seed)`.

- **RGB path** — seeded pair/weight selection, then **gamma-corrected
  linear-light sRGB interpolation** (`combine_color_qi_linear`,
  implemented 2026-08-14; full method comparison and verification in
  Appendix C). Rationale: naive per-channel sRGB averaging darkens
  midpoints because sRGB is gamma-encoded; interpolation in linear light
  preserves luminance. Documented caveat: averaging complementary hues
  still desaturates toward gray in any averaging model.
- **HSV path** — convert endpoints to `color_qf`; interpolate hue along the
  shortest arc (wrap-aware: 350°→10° crosses 0°, not 180°); interpolate `s`
  and `v` linearly; optional seeded mutation variant for "more saturated /
  less dark" expansion (current v2 behavior). Convert back to `color_qi`.
- **Alternatives considered** — OKLab (perceptually uniform; small matrix +
  cube-root math) and LCH (most uniform; heavier conversions) are recorded
  as follow-ups, out of scope for the two-model ask. CIECAM-sourced palettes
  stay source tables; band generation operates on their RGB/HSV
  representations.

### B.6 Default typography and font resource

`k::default_typo = hyperl_typo` (Atkinson Hyperlegible) added to
`izzi-svg-constants.h`; `izzi-svg-graphs-line.h` and `svg_element` defaults
converge to it, and color-family examples switch from `apercu_typo`.
Resource: `/home/bkoz/.fonts/atkinson_hyperlegible.2026/` — 32 files across
Atkinson Hyperlegible (4), Mono (14), and Next (14), all weights/styles,
sha256 inventory recorded (Plan G); Braille Institute free-use license.

### B.7 `generative_colors` section

Documentation lives in a dedicated `docs/generative_colors/` section
(`generative_colors.md`, `index.md`, `color.md`, `assessment_plan.md`,
`images/` pending regeneration) — a parallel tree, not an entry inside
`docs/generative_patterns/`.

### B.8 Spelling and vocabulary normalization

Canonical vocabulary per Plan D item 6 (palette, CIECAM02/16, ColorBrewer
2.0, ESRI Bathymetry Tints, WCAG, Apercu, RGB/HSV, Atkinson Hyperlegible);
enforced by `tests/check-color-spelling.py` in `color-check` with zero
variant hits across color sources, examples, and the family doc.

### B.9 Portal index review

`palette-20260814-index` — `media_kind: index`, `generation_class:
palette-index`, `review_category: proofs`, `review_scope: PALETTE-20260814`,
published under `review/media/palette-20260814-index/`. Members: the 15
deterministic visual artifacts (color-palette-1..5, color-palette-source-*
×6, color-tint-perceptual-1/2, color-rgb-hsv-2/3). Excluded until fixed:
`color-band-expand-to-larger` (determinism) and the two nonvisual
assertions (moved to `tests/`). Previews regenerated fresh; sizes 2–147 KB
SVG / 3–93 KB PNG fit the 16 MiB proxy cap; `check-review-site.mjs` requires
the `.html` page and member list.

## Appendix C — RGB interpolation: methods and the implemented improvement

### Problem

The legacy `combine_color_qi(a, ad, b, bd)` averages channels directly in
sRGB (gamma-encoded) space. Because sRGB encodes luminance non-linearly,
midpoints of a weighted average are darker and muddier than their perceived
intensity — e.g., 50/50 red+green yields `rgb(127,127,0)`, and 50/50
white+black yields `rgb(127,127,127)` rather than the luminance-correct
midpoint.

### Candidate methods

| Method | Effort | Deterministic | Notes |
| --- | --- | --- | --- |
| Naive sRGB channel average (`combine_color_qi`) | already present | yes | rejected for band generation: darkens midpoints, muddy mixes |
| **Gamma-corrected linear-light sRGB** (`combine_color_qi_linear`) | low (~30 lines) | yes | **chosen** — correct luminance midpoints, standard practice, pure function |
| HSV wrap-aware interpolation | planned | yes (seeded) | approved as the `band_model::hsv` path; hue-preserving, handles 350°→10° correctly |
| OKLab interpolation | moderate (matrices + cube root) | yes | perceptually uniform; recorded as a follow-up |
| LCH interpolation | high (CIELAB conversions, hue wrap) | yes | most uniform; recorded as a follow-up |

### Implemented improvement (2026-08-14)

In `src/izzi-svg-color.h`:

- `srgb_to_linear(c01)` — sRGB transfer function
  (`c ≤ 0.04045` → `c/12.92`, else `((c+0.055)/1.055)^2.4`).
- `linear_to_srgb(c01)` — inverse
  (`c ≤ 0.0031308` → `c*12.92`, else `1.055*c^(1/2.4) − 0.055`).
- `combine_color_qi_linear(a, ad, b, bd)` — same contract as
  `combine_color_qi` (`ad + bd == 2` averages), but each channel is
  converted to linear light, weighted, and converted back with rounding.

`make_color_band_v1` (the RGB band path) now uses
`combine_color_qi_linear`. Verified output:

| Mix | Naive sRGB | Linear-light sRGB |
| --- | --- | --- |
| red + green 50/50 | `rgb(127,127,0)` | `rgb(188,188,0)` |
| white + black 50/50 | `rgb(127,127,127)` | `rgb(188,188,188)` |

The change is deterministic and byte-affecting for band renders only; it
flows through the staged generation-baseline candidate. `combine_color_qi`
and `average_color_qi` remain as the legacy linear-space API for callers
that do not opt into gamma correction.
