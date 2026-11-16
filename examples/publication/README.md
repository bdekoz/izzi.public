# Standalone publication example

This non-cartographic sampler exercises transparency, gradients, fine
linework, Unicode text, a long heading, source URLs and an enlarged detail.
It uses the same Izzi production engine as the World atlas and Starlink books.
No Cartofreako checkout or data services are needed.

![Synthetic artwork used by the example](sampler.svg)

The [manifest example](manifest.example.json) shows the actual schema, input
hashes, long-label key grid and source references. Its system font/profile
paths are the exercised Linux installation; the preparer resolves them for
the current machine. The generated contact sheet is under
`outputs/publication-example/validation/izzi-visual-sampler.pages/`.

Install the optional [production dependencies](../../docs/dependencies.md#publication-production-november-2026),
including an EPUBCheck jar, and set `EPUBCHECK_JAR` to its absolute path.
`prepare-example.py` uses bundled Atkinson Hyperlegible TrueType fonts and
system DejaVu Sans fallback fonts. The fonts' SIL license is in
[`fonts/OFL.txt`](fonts/OFL.txt). The default ICC path is Ghostscript's
`/usr/share/ghostscript/iccprofiles/default_cmyk.icc`; pass `--cmyk-profile`
to the preparer on another installation.

From the Izzi root:

```sh
make examples-publication EPUBCHECK_JAR=/path/to/epubcheck.jar
```

For the independent A4 landscape example:

```sh
make examples-publication-landscape EPUBCHECK_JAR=/path/to/epubcheck.jar
```

This prepares the same synthetic content with explicit facing-context
references and uses `config/publication/a4-landscape-facing-20261114.json`.
It writes `outputs/publication-example-landscape/izzi-visual-sampler.a4-landscape.pdf`,
an EPUB, ebook cover, receipts, page contact sheets and paired spread previews.
Artwork is maximally fitted on even pages, with complete keys/context on
the next odd pages. This neutral master has no retailer minimum-page padding.
To validate it separately, use `build-publication.py validate` with that
profile and the landscape filenames, then `review-publication.py` on its
receipt with `--render`. Both example targets are optional and independent
of Cartofreako. Cleanup is limited to the selected example output directory.

## Portrait stages and validation

The portrait target prepares a manifest, builds a print/EPUB pair, and makes a mobile
PDF under `outputs/publication-example`. To run the stages individually:

```sh
python3 examples/publication/prepare-example.py \
  outputs/publication-example/sources/manifest.json
python3 scripts/build-publication.py build \
  outputs/publication-example/sources/manifest.json \
  --output outputs/publication-example --epubcheck "$EPUBCHECK_JAR" --resume
python3 scripts/build-mobile-pdf.py \
  outputs/publication-example/izzi-visual-sampler.a4.ingramspark.pdf \
  outputs/publication-example/izzi-visual-sampler.mobile.pdf --resume
```

The mobile command with `--resume` checks the input, tool and output hashes,
geometry and linearization before reusing an existing destination. Generated outputs
are not source fixtures and should not be committed.

Separate validation:

```sh
python3 scripts/build-publication.py validate \
  --pdf outputs/publication-example/izzi-visual-sampler.a4.ingramspark.pdf \
  --epub outputs/publication-example/izzi-visual-sampler.epub \
  --epubcheck "$EPUBCHECK_JAR"
make check-publication-unit
make check-publication-integration EPUBCHECK_JAR="$EPUBCHECK_JAR"
```

Expected files are `izzi-visual-sampler.a4.ingramspark.pdf`,
`izzi-visual-sampler.epub`, `izzi-visual-sampler.ebook-cover.jpg`,
`izzi-visual-sampler.mobile.pdf`, its `.json` receipt, and `sources/` and
`validation/` records. The paperback profile produces 24 A4 pages, including
explicit blank end padding; this small fixture is not a commercial book.
EPUBCheck should report zero errors and warnings; print images must be CMYK
with no sample exceeding 240% total ink. A contact sheet and selected page
renders belong beside the local generated example, not in the source fixture.

`tests/publication.py` supplies deliberately invalid manifest/archive cases
and mixed crop/rotation fixtures. The integration tests use their own temporary
directories and clean up only those directories. To remove a manual example,
remove only the chosen `outputs/publication-example` directory after review.

## Full-bleed portrait spread example

```sh
make examples-publication-spread EPUBCHECK_JAR=/path/to/epubcheck.jar
```

`PUBLICATION_SPREAD_OUTPUT` defaults to `outputs/publication-spread-example`.
The same small artwork occupies two portrait A4 pages, followed by exactly
two pages of keys and notes. Output includes `izzi-visual-sampler.a4-portrait-spread.pdf`,
reflowable EPUB, ebook cover, receipts and trimmed map/legend spread previews.
The complete artwork is preserved, with its border background extended to
fill the spread. The exercised sampler has 14 pages. Physical pages have 3 mm bleed; matching
fold pixels and reconstructed full-map hashes are checked. See the [production reference](../../docs/publication.md#full-bleed-portrait-spreads).

To repeat structural and pixel validation separately, use the spread profile
with `build-publication.py validate`, then run `review-publication.py` against
the generated receipt. Visual inspection of the resulting contact sheets is
a separate step. The shared suite has eight fast and eight integration tests.
