# Publication manifests and production

Izzi's optional Python production tools make A4 print interiors, reflowable
EPUBs, ebook covers, and mobile PDFs. They accept explicit assets and text;
they do not acquire data or know about a particular creative domain.

The [workflow](visual_workflow/tool_publication.md) covers production and
review. The [small example](../examples/publication/README.md) runs without
Cartofreako or any geographic datasets. These tools are separate from the
installed C++20 headers and `portrait::print_spec`.

## Commands

```sh
python3 scripts/build-publication.py build publication.manifest.json \
  --output outputs/my-book --stem my-book.v1 \
  --epubcheck /path/to/epubcheck.jar

python3 scripts/build-publication.py validate \
  --pdf outputs/my-book/my-book.v1.a4.ingramspark.pdf \
  --epub outputs/my-book/my-book.v1.epub \
  --epubcheck /path/to/epubcheck.jar

python3 scripts/build-mobile-pdf.py input.pdf output.mobile.pdf \
  --max-edge 2400 --jpeg-quality 85 --receipt output.mobile.json --resume

python3 scripts/review-publication.py outputs/my-book/validation/my-book.v1.json --render
```

`build` also writes `my-book.v1.ebook-cover.jpg` and
`validation/my-book.v1.json`, with Ghostscript, page-decode and EPUBCheck logs.
`--profile` selects an explicit dated JSON profile. `--resume` verifies an
already completed delivery's inputs, tool hashes and output hashes; it never
silently replaces an edition. Missing inputs, changed assets, invalid content
order, or invalid geometry fail before a delivery is accepted.

## Manifest v1

The [example preparer](../examples/publication/prepare-example.py) writes a
complete runnable manifest. Its top-level fields are:

| Field | Meaning |
| --- | --- |
| `schema` | Exactly `izzi-publication-v1`. |
| `id` | Lowercase letter followed by lowercase letters, digits or hyphens. |
| `metadata` | `title`, `creator`, BCP-47-style `language`, `identifier`, UTC `modified` (`YYYY-MM-DDTHH:MM:SSZ`), and `edition`; optional `subtitle` and short `running_title`. |
| `assets` | Object keyed by safe asset IDs. Each entry has `path` and SHA-256 `sha256`. Relative paths resolve beside the manifest, not the working directory. |
| `fonts` | Asset IDs for `regular` and `bold` TrueType fonts; optional `fallback_regular` and `fallback_bold`. Missing print glyphs fail instead of becoming silent empty boxes. |
| `cmyk_profile` | Asset ID for the explicit CMYK ICC file. |
| `introduction`, `reading_guide`, `colophon` | Ordered arrays of plain-text paragraphs. |
| `chapter_order` | Frozen list of chapter IDs; must exactly equal the chapter array order. |
| `chapters` | Ordered chapter objects described below. |
| `cover_image` | Optional image descriptor for the title page and ebook cover; defaults to the first chapter's overview. |

Each chapter has `id`, `title`, optional `subtitle`, an `image` descriptor,
and `sections`. An optional `ebook_image` supplies an existing mobile overview
without replacing the original print source. Each section has a `title`,
`paragraphs`, and optionally `keys` (objects with `label` and an `image`
descriptor). A section with `role: sources` uses the smaller print source-note
type size. `details` is an array of additional image descriptors with optional
`title` and `caption`; each starts a new print page in the flowing and landscape
layouts. The four-page portrait-spread layout retains these optional details
in EPUB while the complete print artwork spans two pages.

Optional `print_layout` pins the intended profile layout (`flow-v1`,
`facing-spreads-v1`, or `portrait-map-spreads-v1`); a mismatch fails admission. For facing spreads, a
chapter's optional `facing_context` is an array of `[section_index,
paragraph_index]` pairs, both zero-based. These original paragraphs must fit
on the primary legend page alongside every key. Other paragraphs continue
after the key in two columns. References must exist and be unique. The
underlying sections stay intact for EPUB and content auditing.

An image descriptor looks like:

```json
{
  "asset": "original-artwork",
  "page": 1,
  "crop": [0.30, 0.20, 0.25, 0.50],
  "alt": "A description of the visible artwork and its meaning.",
  "caption": "Enlarged detail from the original composition."
}
```

`page` is one-based, defaults to 1, and applies to PDF assets. `crop` defaults
to `[0, 0, 1, 1]`. It is `[x, y, width, height]` in normalized coordinates
from the top-left of the visible, rotated page. PDF CropBox/MediaBox
intersection, rotation and UserUnit determine that frame. SVG sources require
a viewBox; scripts, foreignObject and external href resources are rejected.
Bitmap input is flattened on white. Print bitmaps must already have enough
pixels for their placed size; the tool rejects enlargement beyond those
pixels. SVG/PDF detail views are rendered directly from their original source.

Text fields are plain text, not arbitrary XHTML or ReportLab markup. EPUB
source URLs using HTTP(S) become links. EPUB retains source paragraph/key
ordering; facing print layouts group the complete key and selected context
on the primary legend, then flow the remaining paragraphs in source order.
Source-specific interpretations and meaningful image
descriptions belong in the manifest, supplied by its content owner.

## Physical and color behavior

The initial profile is
[`ingramspark-a4-20261114`](../config/publication/ingramspark-a4-20261114.json):
210 x 297 mm portrait, no bleed, mirrored 20 mm inner / 15 mm outer margins,
20 mm top/bottom text margins, 10.5 pt body and 9.5 pt source text. Running
footers sit 14 mm from the lower edge. All type is embedded, with K-only small
text. Artwork is proportionally contained at 300 ppi; legends remain live
text with original symbol swatches. Contents page numbers are generated in
two layout passes and checked for convergence.

Key sections with six or more entries use two columns, ordered across each
row. Row height follows the longest wrapped label; text and 8 mm symbol
sizes stay unchanged. Shorter keys remain in a single column.

Pillow/LittleCMS converts sRGB artwork through the manifest's ICC profile.
After conversion, the engine caps C+M+Y+K at 240%, retaining K and reducing
C/M/Y proportionally where necessary. It records the maximum before/after
coverage and affected pixel count. The exercised profile is Ghostscript's
Artifex CMYK SWOP profile. The cap is a separate numerical operation, not a
claim that the ICC profile itself is a 240% press profile. Saturated dark
colors can lighten; review the wine/amber colors and order a physical proof.

CMYK images use lossless Flate in the print PDF so JPEG quantization cannot
push samples back over the ink limit. Ghostscript produces PDF/X-3:2002 with a
CMYK output intent. Structural preflight independently checks syntax, page
boxes, embedded fonts, image/color resources, transparency and actual ink
samples. Ghostscript then decodes every final page. These checks do not
constitute certification by an independent PDF/X conformance product; the
receipt explicitly records when that validator is unavailable.

## Landscape facing spreads

Select [`a4-landscape-facing-20261114`](../config/publication/a4-landscape-facing-20261114.json)
explicitly. It produces `STEM.a4-landscape.pdf`, with 297 x 210 mm pages:

```sh
python3 scripts/build-publication.py build publication.manifest.json \
  --profile config/publication/a4-landscape-facing-20261114.json \
  --output outputs/my-landscape-book --stem my-book.v2 \
  --epubcheck /path/to/epubcheck.jar --resume
python3 scripts/review-publication.py \
  outputs/my-landscape-book/validation/my-book.v2.json --render
```

Every complete artwork occupies an even left page without publication text,
headers or folios. Its title, subtitle, all symbol keys and selected context
occupy the next odd right page. Physical PDF parity includes frontmatter and
blanks; alignment blanks are explicit in `layout.page_roles`. Contents point
to the artwork page. A legend cannot silently spill or shrink its type:
the engine measures two and then three key columns at 10.5 pt and rejects
unresolved primary-legend overflow. Remaining explanations/sources use two
columns, followed by detail pages.

Artwork uses the entire safe frame with uniform scale and centered placement.
There is no 95 mm overview cap. Source crop bounds remain explicit in each
descriptor; the exporter does not guess which source marks can be discarded.
The top/bottom/outer margins are 13 mm. `inner_margin_by_pages` selects a
total inside margin of 13, 16, 25 or 29 mm for up to 60, 150, 400 or 600 pages.
These bands follow the dated Lulu binding guidance. Layout resolves contents
and binding margins using inexpensive measurement passes before rasterizing.
Text pages reserve 22 mm below the text frame for footer clearance; artwork
pages retain the full 13 mm bottom allowance.

The receipt's `requested_profile` preserves the input profile and `profile`
records the effective binding margin. `layout.spreads` links each chapter,
source image, map page, legend page and key count/column count. Independent
PDF review checks the actual image matrix against source aspect ratio and
the largest centered placement within 0.1 mm; it requires exactly one image
and zero text operations on each artwork page. `--render` also makes paired
spread previews and contact sheets.

This is a neutral interior master, with a 2-600 generation range rather than
a retailer minimum. Lulu's guide lists exact A4 landscape; binding, paper,
distribution and final product limits still need title setup. It is not an
IngramSpark or KDP landscape submission claim. Select provider templates and
export requirements separately; retain the existing CMYK/PDF/X production
baseline and conservative EPUB limits. See the profile's dated sources.

## Full-bleed portrait spreads

Select [`a4-portrait-spread-20261114`](../config/publication/a4-portrait-spread-20261114.json)
and set `print_layout` to `portrait-map-spreads-v1`. The output is
`STEM.a4-portrait-spread.pdf`. Every chapter occupies exactly four pages:
an even/odd artwork spread, then an even/odd legend-and-notes spread.
Body/key text stays at 10.5 pt and source notes at 9.5 pt. Text that cannot
fit these two pages fails during measurement, before rasterization.

TrimBox is 210 x 297 mm; MediaBox and BleedBox are 216 x 303 mm, with 3 mm
bleed on all four edges. Artwork pages have no publication text or margins
inside the trim. Complete artwork is proportionally contained within the
420 x 297 mm spread. Its most frequent border color extends across the
remaining background. No source coverage is removed or stretched. Text pages
retain mirrored binding margins and clear the footer.

One complete CMYK spread is split at integral pixel coordinates, using
12 pixels/mm (304.8 ppi) for the default profile. Outer bleed extends edge
pixels; fold bleed contains adjacent pixels from the other page. The receipt
records the artwork bounds, background RGB, pixel dimensions and complete
trim-raster hash. Independent PDF review reconstructs the spread from the
actual page images, verifies its pixel hash and matching fold bleed, and
checks the full-bleed placements and four-page sequence. Review contact sheets
show trimmed pages and both paired spreads, without a false gap at the fold.

```sh
python3 scripts/build-publication.py build publication.manifest.json \
  --profile config/publication/a4-portrait-spread-20261114.json \
  --output outputs/my-spread-book --stem my-book.v4 \
  --epubcheck /path/to/epubcheck.jar --resume
python3 scripts/review-publication.py \
  outputs/my-spread-book/validation/my-book.v4.json --render
```

This is a neutral full-bleed interior master. The physical binding can hide
fine marks at the fold; a provider's bleed template and binding-specific
compensation must be checked for a submission derivative. The source remains
complete in this master and in the reflowable EPUB, which imposes no split.

## EPUB and mobile behavior

EPUB uses real XHTML headings, paragraphs and source links, chapter
navigation, cover declarations and a reading-order spine. It uses RGB images
and does not force body foreground/background colors, preserving reader
themes. Every image has alternative text; this does not establish full
accessibility conformance. The initial conservative image limit is 3.2 million
pixels, with a 100 MB archive limit. The original profiles' 1400 x 2240
cover is a historical build asset; it is too narrow for an IngramSpark upload.
Run the retailer-cover export below to obtain a 1600 x 2560 external JPEG
and an EPUB with a proportional embedded copy below the 3.2-million-pixel
cap. EPUBCheck must pass before delivery is accepted.

### Retailer ebook-cover export

```sh
python3 scripts/export-ebook-cover.py outputs/my-book/validation/my-book.v1.json \
  --manifest publication.manifest.json --epubcheck /path/to/epubcheck.jar --resume
```

The separate `config/publication/ebook-cover-20261114.json` profile records
1600 x 2560 RGB upload dimensions, a conservative 5 MB file limit and the
3.2-million-pixel embedded-image limit. Output names are
`my-book.v1.ebook-cover.1600.jpg`, `my-book.v1.cover1600.epub` and
`validation/my-book.v1.cover1600.json`. The external JPEG carries 350 dpi
metadata; its embedded proportional copy is 1414 x 2262 pixels.

The exporter renders the original artwork and typography anew. It updates
only the EPUB cover image and modification timestamp, checks byte-for-byte
preservation of every other resource, and runs EPUBCheck. Source book assets
and receipts remain unchanged. `--resume` verifies an existing derivative;
use another output directory if inputs or production code change. Inspect
both the new external image and its proportional embedded copy.

IngramSpark requires the external cover's shortest side to be at least 1600
pixels and longest side at least 1873; its EPUB image cap applies separately.
KDP recommends 1600 x 2560 for the external cover. The dated profile links
both providers' primary requirements.

The mobile exporter performs full-page rasterization: it deliberately removes
text selection, vector paths, annotations and links, uses one opaque RGB
image per page, preserves visible page geometry and linearizes PDF 1.4.
Per-page encoding chooses the smaller of JPEG 4:4:4 and lossless Flate. This is
different from the print tool's selective artwork rasterization with live
text. A mobile PDF cannot supply the reflowable text needed by the EPUB
writer, and must not supply enlarged print details.

## Evidence, reruns and recovery

Receipts record asset hashes, manifest/profile/tool hashes, dependency
versions, page inventory, placement boxes, effective ppi, color statistics,
output hashes and validator results. Output generation occurs in a private
temporary directory inside the destination. Publication uses exclusive links
and writes the complete receipt after the three artifacts. Ordinary failures
roll back only files created by that invocation. An abrupt process kill during
the final publication step can leave files without a receipt: the next run
refuses to treat them as complete. Inspect or move those partial files before
rerunning. A failed render does not replace an earlier valid edition.

Freeze a new profile filename when requirements change. Keep the old profile,
manifest and receipt with the old delivery. `--resume` is intentionally strict:
a changed production tool or profile requires a new build directory. Local
validation, visual review, retailer preview, upload validation and physical
proofing are distinct evidence; record only the steps actually performed.

The exercised example reproduced byte-identical EPUB and JPEG cover files.
Ghostscript refreshes PDF creation/modification dates and document identifiers;
the two print PDFs were identical after excluding those fields. Fresh print
builds therefore promise the same content and layout, not byte-identical files.
`--resume` verifies the exact bytes of an already completed delivery.

`review-publication.py` checks effective ppi from the final PDF's actual image
matrices, mirrored image margins, K-only text, and the bounds of every extracted
word. `--render` decodes every page into a contact-sheet set for inspection.
It records visual review as pending until an actual reviewer inspects it.

## Provider boundary

The landscape and full-bleed spread profiles are neutral interior masters.
Their generation limits do not establish a selected retailer product,
binding or distribution route. Verify the provider's template separately.


The `ingramspark-a4-20261114` portrait profile targets IngramSpark A4
premium-color paperback interiors.
It uses a conservative 24-590 page range compatible with KDP's A4 premium
color option; A4 standard color is unavailable there. Lulu's Amazon color
distribution requires at least 72 pages, and Lulu has its own selected-product
bleed/template rules. A passing IngramSpark profile is not a universal Lulu
submission claim. See the dated primary links inside the profile.

IngramSpark's file guide states 3.2 million pixels for EPUB images while its
ebook web page states 5.6 million; the stricter value is deliberate. Neither
the external ebook cover nor an interior PDF replaces the separate print
wraparound cover made for final page count, paper, binding and ISBN. This tool
does not upload, publish, assign an ISBN, set a price or place an order.


## Typographic print-cover masters

`scripts/build-type-cover.py` makes a single-page paperback wrap PDF, a
separate two-page front/back panel PDF, a PNG preview and a JSON receipt:

```sh
python3 scripts/build-type-cover.py cover.json outputs/book.print-cover.pdf
```

The `izzi-type-cover-v1` JSON specification supplies `title`, `creator`,
`edition_date` (ISO date), `trim_mm` (width/height), `spine_mm`, `bleed_mm`,
two `front_lines`, two `back_lines`, and four-channel CMYK fractions in
`background_cmyk` and `type_cmyk`. `font` and `icc` objects have explicit
`path` and `sha256` fields; relative paths resolve beside the specification.
An optional `interior` object adds `path`, `sha256` and `pages`, checked
against the source PDF and every page's TrimBox. A `spine_basis` object can
record the provider calculation and template status without printing it.

The front is on the right of the wrap, the back on the left, and the spine
is plain. Panel masters are front first, back second, each with its own
bleed. Text is embedded vector type; fills are opaque CMYK with at most
240% total ink. All colored type is larger than 24 pt. The exporter reuses
the publication PDF/X conversion and structural preflight, decodes every
page and checks all requested text. It refuses existing output names.
Inspect the wrap and panel renders, then record that review in the receipt.

The caller supplies a spine width from the selected paper, binding and final
processed page count. This tool does not invent an ISBN/barcode or obtain a
printer template. Preserve barcode space in the design and confirm the wrap
against the final provider template before submission. The separate panel
masters allow the same artwork to be placed on that template.
