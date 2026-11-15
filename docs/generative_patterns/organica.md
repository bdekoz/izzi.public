# Organica

Organica collects the organic and botanical constructions into one review
group: plants, foliage, corals and anemones. The native catalog registers
31 forms under `group: organica`, organized into three subfamilies.

| Subfamily | Forms | Constructions |
| --- | ---: | --- |
| Botanical | 18 | Vines, scrolls, palmettes, palms, ginkgo, cannabis, papyrus, saguaro, willow and wisteria; line constructions and detailed botanical forms |
| Coral | 11 | Staghorn, lettuce, carnation, bubble, sun, fox, grooved brain and sea fan; line constructions and detailed reef forms |
| Anemone | 2 | Pink-tipped and saddle-carpet constructions |

These are construction counts; a species can have both a line construction
and a detailed form. Cannabis and papyrus retain their plant-count, growth,
branching and detail controls. See the [plant construction guide](../generative_patterns.md#foliate-and-botanical-forms),
[reef construction guide](../generative_patterns.md#reef-forms), and
[ten wild botanical compositions](botanical-wild/index.html).

## Review and rendering

The examples books place these forms and their controls together in an
**Organica** chapter. Plates retain their subfamily labels, and the HTML
filter accepts `organica`, a subfamily name, or an individual form ID.

```sh
python3 scripts/render-examples.py study --group organica \
  --output build/organica-study --strict --png

python3 scripts/render-examples.py composition --group organica \
  --output build/organica-composition --strict --png
```

The composition selector includes authored scenes whose components all
belong to Organica, including botanical/coral/anemone crossovers. Compositions
with another group, such as moiré or guilloché, appear in the full book's
Crossovers chapter. Repeat `--group` to include several groups and the
compositions contained within them. Group builds record `scope: selected`.
See [examples workflows](../development/examples-workflows.md) for exports
and individual form or scene selection.

## Catalog and recipe compatibility

The native review descriptor supplies the `group` field. The resolved catalog
also lists each group's member families and form IDs. Existing `family`
values, form IDs, scene JSON and C++ construction types remain valid, including
the `organic-*`, `botanical-*` and `coral-*` IDs. Authored crossover roles
continue to describe the participating subfamilies.

Geometry is implemented in [izzi-svg-guilloche.h](../html/izzi-svg-guilloche_8h.html)
and [izzi-svg-guilloche-meta.h](../html/izzi-svg-guilloche-meta_8h.html), with native
review registration in [izzi-review-catalog.h](../html/izzi-review-catalog_8h.html).
