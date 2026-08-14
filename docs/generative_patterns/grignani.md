# Family: grignani

Header: `src/izzi-svg-curves-grignani.h` (333 lines).

## API surface

- `point_2d`, `point_3d`; `ribbon_config`, `ripple_config` spec structs.

## Config

Ribbon and ripple configuration structs.

## Tests and examples

- No dedicated `tests/` file; grignani is exercised through the
  package-consumer canonical test (`tests/package-consumer/canonical.cc`).
- Examples: `examples/curves-grignani-1.cc`, `examples/curves-grignani-2.cc`.

## Usage

- Library-level curve family; not yet wired into a named workflow surface.

## Assessment

Status: `ASSESSED-FIRST-PASS`.
Open items: add test coverage and an example; document ribbon vs ripple
parameter semantics.
