# Family: damped harmonograph

Header: `src/izzi-svg-curves-damped-harmonograph.h` (144 lines).

## API surface

- `point_2t = std::tuple<double, double>`; damped-harmonograph curve
  generation with decay parameters.

## Config

Curve parameters (frequencies, phases, decay) in the generator call; no
public spec struct found in the first pass.

## Tests and examples

- Examples: `examples/curves-harmonic-1.cc`, `-3.cc`, `-4.cc`.
- Also `examples/curves-harmonic-2.cc` (four harmonic examples total).
- No dedicated `tests/` file.

## Usage

- Harmonic curve examples; part of the curves family consumed by visual
  proofs.

## Assessment

Status: `ASSESSED-FIRST-PASS`.
Open items: no dedicated `tests/` file found; confirm parameter bounds and
sampling behavior.
