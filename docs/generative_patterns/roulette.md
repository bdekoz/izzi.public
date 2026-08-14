# Family: roulette

Header: `src/izzi-svg-curves-roulette.h` (352 lines).

## API surface

- `svg::roulette::roulette_kind` (enum); `trochoid_config`,
  `roulette_config`; `minimum_samples_per_turn = 8`,
  `maximum_sample_count = 2'000'000`.

## Config

Trochoid and roulette configurations with explicit sampling bounds.

## Tests and examples

- Examples: `examples/guilloche-plates.cc`,
  `examples/guilloche-transition-frames.cc`, `examples/curves-roulette.cc`
  (roulette feeds guilloche plates); also
  `examples/seedance2/title-transition-study.cc`.
- Tests: no dedicated file; exercised via `tests/guilloche.cc` and the
  package-consumer canonical test.

## Usage

- Roulette sub-generator inside the guilloche family; also referenced by
  `svg::guilloche::roulette_kind`.

## Assessment

Status: `ASSESSED-FIRST-PASS`.
Open items: dedicated `tests/` file; confirm the mapping between
`svg::roulette::roulette_kind` and `svg::guilloche::roulette_kind`.
