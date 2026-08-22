# Typed graph boundary

Back: [izzi](../README.md)

[`src/izzi-svg-graph.h`](https://bdekoz.github.io/izzi.public/docs/html/izzi-svg-graph_8h.html)
separates graph data, layout
specification, deterministic layout, and rendering. It currently provides
typed radial-orbit and flattened linear-chord data/specifications and produces
a renderer-neutral scene with stable IDs, geometry, marginal totals, reading
order, accessibility intent, and structured diagnostics. The retained compact
MMRL orbit language is available as
`svg::graph::radial_orbit_spec::mmrl_defaults()` and the named `kusama`
preset.

[`src/izzi-svg-graph-json.h`](https://bdekoz.github.io/izzi.public/docs/html/izzi-svg-graph-json_8h.html)
strictly parses
`izzi.graph/1` authoring JSON from a caller-supplied string. It performs no file
I/O, rejects unknown members and incompatible data/layout kinds, and emits an
`izzi.graph-resolved/1` document in deterministic member order. Every default,
resolved scene coordinate, semantic reading order, and caller-supplied input
and schema hash is materialized. See `tests/data/graph/` for compact radial and
linear-chord inputs. The public schemas are
`schemas/izzi.graph-1.schema.json` and
`schemas/izzi.graph-resolved-1.schema.json`.

This boundary does not replace the existing radial/Kusama and chord renderers
yet. Their compatibility adapters and visual parity work are a separate
migration stage, so a valid scene is not a claim that historical output has
already been reproduced or that a graph conforms to WCAG 2.2.
