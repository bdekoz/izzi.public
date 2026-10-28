# Guilloche, water, leaf, coral and optical studies

These are original explanatory constructions for the
[September 28 survey](../20260928.meta_guilloche_survey.md), not new native
Izzi presets. Source-derived Hamonshū thumbnails are explicitly separate.

- [Asymmetric guilloche](asymmetry.svg): six comparisons of contours, waves,
  faceted cells and unequal foliage.
- [Leaf and coral structures](organic.svg): four leaf and four coral forms.
- [Optical treatments](optical.svg): six static constructions.
- [Interactive optical lab](optical-lab.html): parameters, comparison
  controls, primary references and individual SVG downloads. Open locally
  in a browser; no server or network connection is required for the drawings.
- [Hamonshū source gallery](../../../generative_patterns/hamonshu-water-gallery.html)
  and [complete index](../../../generative_patterns/hamonshu.md#graphic-water-catalogue):
  seven form groups across all 86 scans, excluding writing examples.

## Reproduction

From this directory:

```sh
python3 generate.py
python3 generate_organic.py
node render_optical.mjs
python3 build_catalogue.py
```

The first two scripts use the Python standard library. The optical exporter
uses Node.js without external packages and shares geometry functions with the
HTML lab. The catalogue builder requires Pillow and the existing local
`resources.rizal/hamonshu` scans. Its input JSON contains manually reviewed
classifications; the builder does not infer them from English names.

The geometry scripts accept `--output PATH`; the catalogue builder accepts
`--output-dir DIRECTORY`. It updates the marked catalogue section inside
`docs/generative_patterns/hamonshu.md`, preserving the surrounding API and
consumer documentation. Scratch output includes the classification JSON and
adjusts local links; the default location is the canonical repository guide.
Source scans are read only. The seven JPEGs are literal crops, with no tracing
or generative image processing.

## Verification

The survey records the completed checks: deterministic regeneration, SVG XML,
finite coordinates/bounds, complete catalogue coverage, local links,
JavaScript control behavior and raster inspection. Browser engines could not
launch in the execution sandbox, so browser layout and download UI were not
verified there. Node checks exercised geometry and callbacks with a minimal
DOM substitute; they are not browser tests. Perceptual efficacy remains a
visual-review question rather than an automated-test result.
