# Portrait engraving

Izzi's installed C++20 portrait API converts decoded photographs into physical
vector marks. It supports authored subject masks and complete photographs.
The native engine generates geometry; adapters handle decoding, color, layout,
lettering and print delivery.

## Choose a route

| Task | Start here |
| --- | --- |
| Prepare image fields, resolve marks, emit SVG | [Core contract and declarations](development/20261113_portrait_rendering_api.md) |
| Browse generated declarations | [Core API reference](html/izzi-portrait_8h.html), [SVG adapter](html/izzi-svg-portrait_8h.html) |
| Control dots, lines, crosshatching, contours and flow | [Banknote controls and parameter exploration](development/20261114_banknote_portrait_api.md) |
| Reproduce holiday cards or anniversary currency | [Portrait and print workflow](visual_workflow/tool_portrait_print.md) |
| Inspect current development and deferred features | [November documentation review](development/sessions/20261114.documentation_review.md) |

The current banknote path uses `tensor-flow/1` preparation and `banknote/1`
engraving. The earlier `tone-flow/1` and `tone-flow/2` paths remain supported;
the latter serves `hedcut/2`. DLH means **dot / line / hash (crosshatch)**.
Its values are relative tone allocations, not percentages of final mark count
or occupied sheet area. A 33/33/33 recipe is the equal allocation.

## Boundaries

- `prepare` owns normalized tone, support and direction fields. Whole-photo
  adapters use rectangular support; anniversary adapters persist their crops
  and authored masks.
- `resolve` generates marks at the intended width in millimetres, with a seed,
  resource budget and coverage diagnostics. Changing physical width requires
  regeneration; paint and placement can reuse geometry.
- `render_svg` emits the marks with independent paint. Sheet-wide guilloche,
  photo fades, typography and page composition belong to the adapters.

Installed public headers are [izzi-portrait.h](html/izzi-portrait_8h.html) and
[izzi-svg-portrait.h](html/izzi-svg-portrait_8h.html). The banknote implementation
include is installed with them. The proposed `izzi-portrait-json.h` remains
unimplemented; current Python recipe JSON and native receipts do not provide
that C++ serialization API. Solid shadow masses are also a possible extension,
not an implemented banknote control.

## Build and check

From the repository root:

```sh
make configure
cmake --build build/cmake --target izzi_portrait izzi_portrait_banknote izzi_portrait_plates izzi_engraving_patterns
ctest --test-dir build/cmake --output-on-failure -R '^(portrait\.|engraving\.)'
```

The optional `engraving.lettering` check is registered when fontTools, Shapely
and HarfBuzz are available. Full gallery exports additionally need the adapter
dependencies listed in [Dependencies](dependencies.md#portrait-and-print-adapters-november-2026).
The generic form/composition books and these photograph-based editions have
separate reproduction commands and coverage records.
