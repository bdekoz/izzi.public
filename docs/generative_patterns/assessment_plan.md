# Generative pattern families — assessment plan

Recorded: 2026-08-12 America/Los_Angeles
Status: `IMPLEMENTED-2026-08-12; ALL-FAMILIES-ASSESSED-FIRST-PASS;
NO-REMOVAL; NOT-SHARED`

This plan assesses the generative curve and pattern families implemented in
`izzi/src/` and records each assessment as an entry under
`docs/generative_patterns/`.

## Families

| Family | Headers | Entry |
| --- | --- | --- |
| hamonshu | `izzi-svg-curves-hamonshu.h`, `-v2.inc` | `hamonshu.md` |
| guilloche | `izzi-svg-guilloche.h`, `-json.h`, `graph-guilloche.h` | `guilloche.md` |
| moire | `izzi-svg-moire.h` | `moire.md` |
| surface tension | `izzi-svg-surface-tension.h` | `surface_tension.md` |
| radial | `izzi-svg-radial*.h` (6 headers) | `radial.md` |
| damped harmonograph | `izzi-svg-curves-damped-harmonograph.h` | `harmonograph.md` |
| grignani | `izzi-svg-curves-grignani.h` | `grignani.md` |
| roulette | `izzi-svg-curves-roulette.h` | `roulette.md` |

## Assessment method

Each family entry records, as a first pass from the headers:

1. **API surface** — public types, enums, and functions observed in the
   headers.
2. **Config / parameter space** — spec structs, enums, and JSON schemas.
3. **Tests and examples** — dedicated tests under `tests/`, examples under
   `examples/`, and indirect usage (e.g., radial feeding graph synthesis).
4. **Usage wiring** — which workflows or review surfaces consume the family.
5. **Assessment status** — `DOCUMENTED-FIRST-PASS`, plus open items that need
   deeper review (missing tests, undocumented parameters, unused surfaces).

The goal is a pointer-level assessment: each entry says where the family
implements, tests, and uses its features, not a reimplementation of the
headers.

## Entry template

Each entry follows: family name → headers → API surface → config → tests →
usage → assessment (status + open items) → links.

## Assessment results (first pass, 2026-08-12)

- **Dedicated tests missing**: hamonshu, harmonograph, and grignani have no
  dedicated `tests/` file; radial has no dedicated test either (it is covered
  indirectly through graph synthesis and moire tests). All three are covered
  by examples; hamonshu and grignani are additionally exercised through the
  package-consumer canonical consumer test.
- **Legacy aliases**: no legacy compatibility headers exist. The former
  alpha60-era compatibility forwarding headers and their install option were
  removed (2026-08-12); consumers include canonical `izzi-*` headers only,
  exercised by `tests/package-consumer/canonical.cc`.
- **Usage wiring**: guilloche feeds the title-card/proof workflow
  (`docs/visual_workflow/tool_guilloche.md`); moire and surface tension feed
  the visual experiments method and have plate examples; radial feeds graph
  synthesis and guilloche scene radial layouts; hamonshu feeds the cartofreako
  ocean suite; roulette feeds guilloche plates; harmonograph and grignani are
  example-level (library-only) so far.

Status: `IMPLEMENTED; ENTRIES-UPDATED-WITH-EVIDENCE; NO-REMOVAL`
