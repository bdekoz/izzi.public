# Kusama and chord graphs

Izzi provides Kusama and chord renderers at two levels:

- the generic `visualization_document` facade in
  `izzi-svg-visualization.h`; and
- typed alpha60-swarm-metadata adapters in `izzi-swarm-metadata.h`,
  `izzi-svg-radial-swarm-metadata.h`, and `izzi-svg-graphs-chord.h`.

The metadata adapters read JSON files directly. They do not depend on the
Alpha60 C++ runtime.

## Choosing a graph

| Question | Graph | Encoding |
| --- | --- | --- |
| How frequent is each category? | radial Kusama | angle and satellite size represent the count; equal counts share an orbit cluster |
| Which actor-age bucket is related to which playing-age bucket? | flattened age chord | the connector joins the two age positions; color identifies the playing-age bucket |
| Which arbitrary entities are connected? | generic circular chord | each row is a source, target, and optional weight |

Kusama is a marginal-frequency view: it intentionally discards pairwise
relationships. Chord is a relationship view: a connector must be backed by an
actual source/target pair and must not be inferred from two unrelated totals.

## Radial Kusama

Include `izzi-svg-radial-swarm-metadata.h` for the metadata adapter.

```cpp
#include "izzi-svg-radial-swarm-metadata.h"

const auto data = svg::load_swarm_metadata_visualization_data(
  metadata_directory, { "succession-301" });
svg::radial_kusama_swarm_metadata(
  data, "radial-kusama-swarm-age", svg::k::v4k_h,
  240, 32, false);
```

`swarm_metadata_kusama_values(counts)` converts a metadata count map to the
`id_value_umap` accepted by the radial renderer. Compound age labels use
`actor age -> playing age` in the SVG. Use
`draw_swarm_metadata_kusama(...)` to place an orbit in an existing document,
which is useful for multi-panel comparisons. Its
`grouping_centerlines` option draws one white centerline from the origin to
each unique grouping's numeric marker, including groupings moved into a high
orbit by collision avoidance. `center_orbit` can hide the center circle and
direction arrow for a ray-only layout. `sort_strings_by_size` selects
length-based or alphabetical ordering inside an equal-value grouping, and
`grouping_centerline_width` scales the rays for larger viewports.

The renderer requires at least one positive value. With weighted glyphs,
angle and satellite radius both follow the value. Collision avoidance can
move adjacent values into a high orbit; allow roughly five base radii of
clearance when it is enabled. Reduce the radius or disable collision avoidance
when a small multiple supplies enough separation on its own.

The metadata examples demonstrate these mappings, all on 4K viewports:

- `visualization-radial-kusama-swarm-age` counts normalized
  `identity.cast_leads[].ages` values for one record;
- `visualization-radial-kusama-swarm-asian-countries` counts actor-side
  nationality codes over the `asian-led-global` candidate slice and shows all
  27 boundary countries;
- the `black-countries` and `latinx-countries` variants show all countries in
  their explicit 60- and 20-country slice boundaries;
- the `multi-countries` and `white-countries` variants show observed
  actor-side countries because those slice definitions do not supply a finite
  country boundary; their 1–5, 6–20, and 21+ panels preserve exact grouping
  numbers while scaling sphere radius within each band; and
- `visualization-radial-kusama-swarm-ethnicities` joins complete principal
  credits to confirmed person-review ethnicity tags for `ironheart-01` and
  `altered-carbon-02`.

The boundary-country examples use a one-unit marker only when a country has zero
observations, so the country remains visible while every positive value retains
its exact radius. A sphere label contains only `Country (code)`; the grouping
number supplies the count once, and equal-value country labels are alphabetized.
Nationality metadata is descriptive and is not evidence that a work qualifies
for the slice.

All five country pages follow
`20260915_metadata_v7.4_slice_candidates.md`. That method document identifies
the checked-in `reports/candidates/h15-v7.generated.json` artifact as the
frozen cohort and candidate-ID registry, so the examples validate the method
document and read slice membership from that canonical machine-readable
report. By default the Alpha60 and `alpha60-swarm-metadata` repositories are
expected to be siblings; pass the method-file path as the optional second
argument when they are not.

The ethnicity example counts confirmed review tags, not inferred identities.
One principal lead can contribute more than one tag. A lead without a
confirmed ethnicity assertion contributes to the explicit
`no confirmed ethnicity` bucket.

The generic `emit_kusama_graph(document, output)` facade is intentionally
simpler. It places `document.headers`—or generated point IDs—on a low orbit.
Use the typed adapter when the value, clustering, and collision behavior must
come from metadata counts.

## Flattened age chord

Include `izzi-svg-graphs-chord.h` for the swarm-metadata age chord.

```cpp
#include "izzi-svg-graphs-chord.h"

const auto data = svg::load_swarm_metadata_chord_data(
  metadata_directory, { "game-of-thrones-610" });
svg::h_chord_graph(data, "game-of-thrones-610");
```

The horizontal axis is the normalized age domain. Plain actor-age counts rise
above it. A compound value such as `60-69 playing 30-39` produces a connector
from the actor-age position to the playing-age position below the axis.
Connector color identifies the playing-age bucket. An unknown or ambiguous
playing-age endpoint uses a dashed connector. When actor and playing ages
occupy the same bucket, the occurrence is folded into the marginal count
instead of drawing a zero-length connector.

Partial numeric ranges such as `40-45` and `40-47` normalize to `40-49`.
Counts are metadata occurrences, not distinct people or canonical works, so a
record containing multiple values can contribute to multiple buckets.

`h_chord_graph` accepts two layout controls after the viewport:

- `y_scale`, in `(0, 1]`, controls the available vertical graph extent; and
- `bar_width`, greater than zero, controls the marginal and playing bars.

`render_swarm_metadata_chord(...)` is the convenience entry point that loads
selected collection keys and renders in one call. Passing an empty key list to
the loader aggregates every JSON record in the metadata directory.

The generic `emit_chord_graph(document, output)` API is a separate circular
layout. It reads each `document.rows` entry as `source`, `target`, and optional
numeric `weight`; `document.headers` can add isolated or explicitly ordered
node labels. It does not apply age normalization or swarm-metadata semantics.

## Building and running the examples

Build the visualization targets and focused tests with:

```sh
make visualization-check
```

The example programs are emitted under `build/cmake/tests/`. They write SVGs
to the current directory.

```sh
build/cmake/tests/izzi_visualization_chord-swarm-record \
  /path/to/alpha60-swarm-metadata/metadata game-of-thrones-610

build/cmake/tests/izzi_visualization_chord-swarm-aggregate \
  /path/to/alpha60-swarm-metadata/metadata

build/cmake/tests/izzi_visualization_radial-kusama-swarm-age \
  /path/to/alpha60-swarm-metadata/metadata succession-301

build/cmake/tests/izzi_visualization_radial-kusama-swarm-asian-countries \
  /path/to/alpha60-swarm-metadata

build/cmake/tests/izzi_visualization_radial-kusama-swarm-black-countries \
  /path/to/alpha60-swarm-metadata

build/cmake/tests/izzi_visualization_radial-kusama-swarm-latinx-countries \
  /path/to/alpha60-swarm-metadata

build/cmake/tests/izzi_visualization_radial-kusama-swarm-multi-countries \
  /path/to/alpha60-swarm-metadata

build/cmake/tests/izzi_visualization_radial-kusama-swarm-white-countries \
  /path/to/alpha60-swarm-metadata

build/cmake/tests/izzi_visualization_radial-kusama-swarm-ethnicities \
  /path/to/alpha60-swarm-metadata
```

The focused CTests are
`visualization.swarm-metadata-chord` and
`visualization.swarm-metadata-radial-kusama`.

## Interpretation and accessibility

- Read orbit angle and glyph size as value encodings, not time or geography.
- Read a chord connector as a recorded relation, not as causation.
- Preserve explicit unknown, ambiguous, zero, and missing-review states.
- Put the data source, unit of count, and any display-only baseline in the SVG
  description or subtitle.
- Do not rely on connector color alone; labels and line styles carry the
  bucket and uncertainty semantics.

Both metadata renderers add an SVG description and use Atkinson Hyperlegible
text. Callers composing a larger document remain responsible for a meaningful
document title, description, source note, and sufficient viewport clearance.

## Round 3 country associations — 2026-09-23

The eight swarm country examples prefer the canonical
`reports/candidates/h15-v3.generated.json` when present, then fall back to the
Round 2 artifact. The optional explicit legacy method argument still selects
the historical path. The reader accepts schema versions 2 and 3, uses the
Round 3 country catalogue, and renders the measure-specific explanation.

Round 3 counts one distinct country association per actor cast entry per sampled
work, combining citizenship, nationality, descent, category counting context,
and preserved legacy annotations. Repeated sources for the same entry/country
count once; repeated appearances across works count again. Country associations
do not assign identity or equal unique citizens. White and Mixed retain three
separate occurrence-band pages. Stable plate filenames and IDs are preserved.

Canonical metadata supplies all counts and the artifact digest. Its
`scripts/build-h15-round3-plates.py` builds these eight programs, verifies two
byte-identical SVG runs, and exports the twelve-page review. This bounded build
and the ten complete-example invocation tests pass; it does not claim a new
full-suite run of unrelated Izzi examples.
