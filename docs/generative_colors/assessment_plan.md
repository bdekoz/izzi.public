# Generative color families — assessment plan

Recorded: 2026-08-14 America/Los_Angeles
Status: `FIRST-PASS; PLANNED-ITEMS-20260814; NO-REMOVAL; NOT-SHARED`

This plan assesses the generative color surface implemented in `izzi/src/`
and records the assessment as the family entry under
`docs/generative_colors/`. It mirrors
[`docs/generative_patterns/assessment_plan.md`](../generative_patterns/assessment_plan.md).

## Families

| Family | Headers | Entry |
| --- | --- | --- |
| color | `izzi-svg-color.h`, `izzi-svg-color-palette.h`, `izzi-svg-color-band.h` | `color.md` |

## Assessment method

Each family entry records, as a first pass from the headers:

1. **API surface** — public types, enums, and functions observed in the
   headers.
2. **Config / parameter space** — palette tables, band seeds, spec structs,
   and enums.
3. **Tests and examples** — dedicated tests under `tests/`, examples under
   `examples/`, and indirect usage.
4. **Usage wiring** — which workflows or review surfaces consume the family.
5. **Assessment status** — `DOCUMENTED-FIRST-PASS`, plus open items that need
   deeper review.

The goal is a pointer-level assessment: each entry says where the family
implements, tests, and uses its features, not a reimplementation of the
headers.

## Entry template

Each entry follows: family name → headers → API surface → config → tests →
usage → assessment (status + open items) → links.

## Assessment results (first pass, 2026-08-14)

- **Examples**: 17 top-level `examples/color-*.cc` files, all covered by the
  generation baseline (double-run, `xmllint`, previews, grid); none are
  registered in `tests/CMakeLists.txt` yet.
- **Determinism**: `color-band-expand-to-larger` varies across runs
  (`M0-DETERMINISM-001`); a seeded-RNG fix is planned.
- **Dedicated tests**: the nonvisual assertions in `color-band-next` and
  `color-rgb-to-hsv-1` belong in `tests/`; visual examples get CTest
  registration (`color.<example>.demo`) plus a vocabulary/typography gate.
- **Statefulness**: `active_spectrum`, `random_color`, and
  `next_in_color_band` hold hidden mutable state; constexpr data modules and
  explicit state are planned for `izzi-svg-color-palette.h` and
  `izzi-svg-color-band.h`.
- **Naming**: output stems use dots, `color_qis_N_palette` ids are legacy,
  and the `ciecam02`/`ciecam16j70` examples emit duplicate or mislabeled
  outputs; renames are planned.
- **Typography**: default labels converge to Atkinson Hyperlegible
  (`k::default_typo`), resource
  `/home/bkoz/.fonts/atkinson_hyperlegible.2026/` (32 files, all families
  and weights).
- **Usage wiring**: izzi house palette, guilloche scene palettes, WCAG gray
  ramp, and ai-time-to-die overlay colors.

Status: `FIRST-PASS-COMPLETE; PLANNED-ITEMS-RECORDED; NO-REMOVAL`
