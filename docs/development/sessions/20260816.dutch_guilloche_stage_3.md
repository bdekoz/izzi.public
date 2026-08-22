Document the expanded set of generation_guilloche capability.

Using existing examples,
izzi/examples/guilloche*.cc files that explores the parameter space.


Expand to fit the new capabilities, exploring the parameter space with
3 variations per category x option 1 x option 2 parameterization.

Then, re-generate generation_guilloche index for review on situationshipin.space and await feedback.

Estimate time to completion and a summary of any plan changes appended to the end of this file before starting unattended implementation work.

### Estimate and plan changes

Estimated completion: about one working day single-agent, or roughly 4–6
hours unattended with local SVG generation and eureka/ord available for PNG
preview rendering.

Plan changes relative to the Stage 2 pilot:

- Capability documentation is written into the stage document rather than a
  separate file to keep the review trail in one place.
- The new parameter-space generator is a separate example binary
  `examples/guilloche-capability-plates.cc`; the existing `guilloche-plates`
  output stays untouched so its accepted keepers remain stable.
- Categories are the eight new pattern families from the pilot:
  `sunburst`, `band_guilloche`, `fish_scale`, `barleycorn`, `medallion`,
  `vignette`, `multicolor_line`, and `flinque`, with `rosette` kept as the
  control.
- Each category produces 3 variations, sweeping option 1 (symmetry/density)
  and option 2 (scale/phase) across the variations: 24 new review plates.
- The portal family is a new `generation-guilloche-20260816` index; the
  existing `izzi-generation-20260814` family is not modified.
- SVG is generated locally; PNG previews are derived with Inkscape and
  published with the existing bounded review-site checks before the index
  goes live.

Implementation sequence:

1. Add the capability-plates example and a `guilloche-capability-review`
   Make target with a CMake smoke test.
2. Generate the 24 SVG plates and derive PNG previews.
3. Add `scripts/publish-guilloche-capability-index.mjs` on the portal,
   publish the family, refresh the build manifest, and run
   `check-review-site.mjs`.
4. Commit, push, verify the live index, then await human feedback.

### Implementation status

Implemented and published; awaiting human feedback.

- `examples/guilloche-capability-plates.cc` generates 27 SVG plates
  (9 categories, 3 variations each) sweeping option 1 (symmetry/density)
  and option 2 (scale/phase).
- `make guilloche-capability-review` builds the generator, writes SVG, and
  derives 27 Inkscape PNG previews via
  `scripts/render-guilloche-capability-png.py`.
- Portal: `scripts/publish-guilloche-capability-index.mjs` published family
  `generation-guilloche-20260816` with 27 review pages, media, index, and
  catalog entry; `check-review-site.mjs` passes with 351 catalog items.
- Live index:
  `https://situationshipin.space/review/media/generation-guilloche-20260816/generation-guilloche-20260816-index.index.html`.
- Commits: izzi `aeaefa65`, portal `8f0ac95`; both pushed.
