# Generative movement families — assessment plan

Recorded: 2026-08-14 America/Los_Angeles
Status: `FIRST-PASS; API-EXAMPLE-IMPLEMENTED; NO-REMOVAL; NOT-SHARED`

This plan assesses the generative movement surface implemented in
`izzi/src/` and records the assessment as the family entry under
`docs/generative_movement/`. It mirrors the pattern and color assessment
plans.

## Families

| Family | Headers | Entry |
| --- | --- | --- |
| movement | `izzi-svg-movement.h` | `movement.md` |

## Assessment method

Each family entry records, as a first pass: API surface, config /
parameter space, tests and examples, usage wiring, and assessment status
with open items.

## Assessment results (first pass, 2026-08-14)

- **API**: `easing`, `movement_path`, `movement_segment`,
  `movement_document`, `generate_path_points`, `sample_path`,
  `sample_segment`, `emit_movement_json` (schema `izzi.movement/1`).
- **Path wiring**: roulette trochoid sampling implemented via
  `sample_trochoid`; surface-tension and hamonshu kinds currently use the
  deterministic straight-line fallback (open item: wire the named families).
- **Examples**: `examples/movement-asama-loops.cc` emits the movement JSON
  plus an SVG camera-path preview; registered as `movement.asama-loops.demo`
  and `make movement-check`.
- **Video extraction** (`scripts/extract-movement.py`) is a planned follow-up
  for here-lies-trouble / Blade Runner cut-5 camera descriptions.
- **Determinism**: path sampling and JSON emission are pure and seeded;
  the example output is byte-stable.

Status: `FIRST-PASS-COMPLETE; API-EXAMPLE-IMPLEMENTED; OPEN-ITEMS-RECORDED`
