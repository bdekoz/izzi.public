# Family: grignani

Header: `src/izzi-svg-curves-grignani.h`.

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
Open items: add focused geometry coverage; document ribbon vs ripple
parameter semantics and projection. Two native examples already exist.

`make_rolling_ribbon` constructs a modulated 3D loop, applies a twisting
strand frame, rotates it by the two view angles (radians), and projects by
discarding depth. `make_ripple_ribbon` uses a sinusoidal height along a
straight spine, with a linear amplitude taper when `decay > 0`, and the same
view rotation. These are projected ribbons; neither function computes
visibility or physically based shading.

The [September 28 survey](../development/sessions/20260928.meta_guilloche_survey.md#8-documentation-and-example-improvements)
proposes matched projection controls, explicit closure conditions, sampling
guidance and references alongside the optical-pattern studies.

## Units, closure and cost

For rolling ribbons, `fold_amplitude` and the fixed radial modulation
`0.4*cos(fold_frequency*t)` are local lengths; `torsion` is local depth.
`scale` multiplies every projected coordinate, including strand width.
`fold_frequency` counts spine oscillations over a full turn.
The strand-frame angle is `2*roll_speed*t`, so integer fold frequency and
integer `2*roll_speed` are sufficient periodicity conditions for labeled
edges, provided the frame is continuous. SVG polygon closure alone does not
establish a smooth ribbon seam. The frame's fallback up-vector switch can
introduce a discontinuity near vertical tangents.

For ripples, `length`, `amplitude` and `ribbon_width` are drawing-unit
lengths; there is no extra scale argument. `frequency` counts cycles over
the open spine and `phase_shift` is radians. Positive `decay` multiplies
height by `max(0, 1-decay*progress)`; negative decay has no effect.
Both view tilts are radians, applied about x and then y. Projection discards
z. A zero-tilt ripple therefore removes the visible height cue.

Rolling output samples each edge `steps+1` times (default 360, minimum 8);
ripple output uses 181 samples per edge. Construction cost is linear in
strands × samples. These older helpers do not share the guilloché scene
budget contract: callers must bound strands, dimensions and finite inputs.
Use higher sampling when increasing folds/twist, and compare at the actual
export size rather than accepting a smooth thumbnail as sufficient.

Useful next examples are matched zero/nonzero view-tilt pairs, integer
versus nonintegral closure cases and a resolution sweep. A stronger future
implementation would expose structured 3D edges, a continuous transported
frame, explicit validation and an optional visibility stage. These remain
documented improvements to the older ribbon module. The
[new optical controls](../generative_patterns.md#water-forms-and-optical-controls)
are already available as native guilloché studies.
