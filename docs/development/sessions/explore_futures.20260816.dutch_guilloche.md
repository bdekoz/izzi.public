Using the reference images located here:
/home/bkoz/src/izzi/resources.rizal/dutch-banknote-design-1814-2002

Suggest improvements to generation_family guilloche that allow more complex wave. sunburst, "Écailles De Poisson", "Flinqué", Barleycorn, rosettes, banding guilloches, medallions, vignette, multi-color-line patterns, latent image, venticular

Investigate, propose 1-3 ways to group patterns, and suggest advanced implementation for:
izzi-svg-graph-guilloche.h


Write results in
explore_futures.20260816.dutch_guilloche_refinement.md

## Console

```text
=== reference inventory ===
resources.rizal/dutch-banknote-design-1814-2002/AGC_20260815_192223548.jpg
resources.rizal/dutch-banknote-design-1814-2002/AGC_20260815_192247676.jpg
resources.rizal/dutch-banknote-design-1814-2002/AGC_20260815_192317697.jpg
resources.rizal/dutch-banknote-design-1814-2002/AGC_20260815_192353385.jpg
resources.rizal/dutch-banknote-design-1814-2002/AGC_20260815_192415710.jpg
resources.rizal/dutch-banknote-design-1814-2002/AGC_20260815_192435019.jpg
resources.rizal/dutch-banknote-design-1814-2002/AGC_20260815_192449327.jpg
resources.rizal/dutch-banknote-design-1814-2002/AGC_20260815_192510043.jpg
resources.rizal/dutch-banknote-design-1814-2002/AGC_20260815_192533174.jpg
resources.rizal/dutch-banknote-design-1814-2002/AGC_20260815_192553299.jpg
resources.rizal/dutch-banknote-design-1814-2002/AGC_20260815_192558884.jpg
resources.rizal/dutch-banknote-design-1814-2002/AGC_20260815_192618169.jpg
resources.rizal/dutch-banknote-design-1814-2002/AGC_20260815_192624421.jpg
resources.rizal/dutch-banknote-design-1814-2002/AGC_20260815_192651842.jpg
resources.rizal/dutch-banknote-design-1814-2002/AGC_20260815_192655156.jpg
resources.rizal/dutch-banknote-design-1814-2002/AGC_20260815_192708507.jpg
resources.rizal/dutch-banknote-design-1814-2002/AGC_20260815_192724103.jpg
resources.rizal/dutch-banknote-design-1814-2002/AGC_20260815_192807866.jpg

=== reference dimensions ===
AGC_20260815_192223548.jpg UHDR 2252x4000 2252x4000+0+0 16-bit sRGB 3.97863MiB 0.000u 0:00.007
AGC_20260815_192247676.jpg UHDR 2252x4000 2252x4000+0+0 16-bit sRGB 4.20314MiB 0.000u 0:00.006
AGC_20260815_192317697.jpg UHDR 4000x2252 4000x2252+0+0 16-bit sRGB 4.07139MiB 0.010u 0:00.005
AGC_20260815_192353385.jpg UHDR 4000x2252 4000x2252+0+0 16-bit sRGB 4.72252MiB 0.010u 0:00.005
AGC_20260815_192415710.jpg UHDR 2252x4000 2252x4000+0+0 16-bit sRGB 4.35602MiB 0.000u 0:00.002
AGC_20260815_192435019.jpg UHDR 2252x4000 2252x4000+0+0 16-bit sRGB 3.72294MiB 0.000u 0:00.001
AGC_20260815_192449327.jpg UHDR 2252x4000 2252x4000+0+0 16-bit sRGB 3.97062MiB 0.000u 0:00.001
AGC_20260815_192510043.jpg UHDR 2252x4000 2252x4000+0+0 16-bit sRGB 3.88143MiB 0.000u 0:00.001
AGC_20260815_192533174.jpg UHDR 2252x4000 2252x4000+0+0 16-bit sRGB 3.21513MiB 0.000u 0:00.001
AGC_20260815_192553299.jpg UHDR 2252x4000 2252x4000+0+0 16-bit sRGB 2.70239MiB 0.000u 0:00.001
AGC_20260815_192558884.jpg UHDR 2252x4000 2252x4000+0+0 16-bit sRGB 3.05772MiB 0.000u 0:00.001
AGC_20260815_192618169.jpg UHDR 2252x4000 2252x4000+0+0 16-bit sRGB 3.42352MiB 0.000u 0:00.001
AGC_20260815_192624421.jpg UHDR 2252x4000 2252x4000+0+0 16-bit sRGB 3.55569MiB 0.000u 0:00.001
AGC_20260815_192651842.jpg UHDR 2252x4000 2252x4000+0+0 16-bit sRGB 3.11546MiB 0.000u 0:00.001
AGC_20260815_192655156.jpg UHDR 2252x4000 2252x4000+0+0 16-bit sRGB 3.42799MiB 0.000u 0:00.001
AGC_20260815_192708507.jpg UHDR 2252x4000 2252x4000+0+0 16-bit sRGB 2.65919MiB 0.010u 0:00.001
AGC_20260815_192724103.jpg UHDR 2252x4000 2252x4000+0+0 16-bit sRGB 3.57891MiB 0.000u 0:00.001
AGC_20260815_192807866.jpg UHDR 4000x2252 4000x2252+0+0 16-bit sRGB 3.17286MiB 0.000u 0:00.001

=== guilloche source line counts ===
  2107 src/izzi-svg-guilloche.h
  1750 src/izzi-svg-guilloche-json.h
   639 src/izzi-svg-graph-guilloche.h
  4496 total

=== current geometry/composition vocabulary ===
70:enum class roulette_kind
76:enum class composition_kind
97:enum class glyph_ornament_kind
117:struct roulette_spec
129:struct rose_spec
137:struct lissajous_spec
147:struct wave_spec
173:struct family_spec
185:struct composition_spec
276:struct scene_spec

=== relevant source files ===
examples/generative/guilloche-scene.stage0.json
examples/guilloche-glyph-v3.cc
examples/guilloche-plates.cc
examples/guilloche-transition-frames.cc
src/izzi-svg-graph-guilloche.h
src/izzi-svg-graph-json.h
src/izzi-svg-graph.h
src/izzi-svg-graphs-bar.h
src/izzi-svg-graphs-chord.h
src/izzi-svg-graphs-line.h
src/izzi-svg-guilloche-json.h
src/izzi-svg-guilloche.h
tests/baselines/generation/current/previews/guilloche-glyph-v3/combination-01-duotone-111.png
tests/baselines/generation/current/previews/guilloche-glyph-v3/combination-02-izzi-20.png
tests/baselines/generation/current/previews/guilloche-glyph-v3/combination-03-alpha-60.png
tests/baselines/generation/current/previews/guilloche-glyph-v3/combination-04-orbit-08.png
tests/baselines/generation/current/previews/guilloche-glyph-v3/combination-05-radial-13.png
tests/baselines/generation/current/previews/guilloche-glyph-v3/combination-06-signal-01.png
tests/baselines/generation/current/previews/guilloche-glyph-v3/combination-07-glitch-03.png
tests/baselines/generation/current/previews/guilloche-glyph-v3/combination-08-silence-00.png
tests/baselines/generation/current/previews/guilloche-glyph-v3/combination-09-trace-22.png
tests/baselines/generation/current/previews/guilloche-glyph-v3/combination-10-wild-10.png
tests/baselines/generation/current/previews/guilloche-glyph-v3/number-01-digit-0.png
tests/baselines/generation/current/previews/guilloche-glyph-v3/number-02-digit-1.png
tests/baselines/generation/current/previews/guilloche-glyph-v3/number-03-digit-2.png
tests/baselines/generation/current/previews/guilloche-glyph-v3/number-04-digit-3.png
tests/baselines/generation/current/previews/guilloche-glyph-v3/number-05-digit-4.png
tests/baselines/generation/current/previews/guilloche-glyph-v3/number-06-digit-5.png
tests/baselines/generation/current/previews/guilloche-glyph-v3/number-07-digit-6.png
tests/baselines/generation/current/previews/guilloche-glyph-v3/number-08-digit-7.png
tests/baselines/generation/current/previews/guilloche-glyph-v3/number-09-digit-8.png
tests/baselines/generation/current/previews/guilloche-glyph-v3/number-10-digit-9.png
tests/baselines/generation/current/previews/guilloche-glyph-v3/word-01-izzi.png
tests/baselines/generation/current/previews/guilloche-glyph-v3/word-02-duotone.png
tests/baselines/generation/current/previews/guilloche-glyph-v3/word-03-orbit.png
tests/baselines/generation/current/previews/guilloche-glyph-v3/word-04-radial.png
tests/baselines/generation/current/previews/guilloche-glyph-v3/word-05-signal.png
tests/baselines/generation/current/previews/guilloche-glyph-v3/word-06-glitch.png
tests/baselines/generation/current/previews/guilloche-glyph-v3/word-07-silence.png
tests/baselines/generation/current/previews/guilloche-glyph-v3/word-08-trace.png
tests/baselines/generation/current/previews/guilloche-glyph-v3/word-09-form.png
tests/baselines/generation/current/previews/guilloche-glyph-v3/word-10-wild.png
tests/baselines/generation/current/previews/guilloche-plates/transition-frame-0.png
tests/baselines/generation/current/previews/guilloche-plates/transition-frame-12.png
tests/baselines/generation/current/previews/guilloche-plates/transition-frame-18.png
tests/baselines/generation/current/previews/guilloche-plates/transition-frame-24.png
tests/baselines/generation/current/previews/guilloche-plates/transition-frame-6.png
tests/baselines/generation/current/previews/guilloche-plates/variant-data-orbit-accessible-static.png
tests/baselines/generation/current/previews/guilloche-plates/variant-data-orbit-compatibility.png
tests/baselines/generation/current/previews/guilloche-plates/variant-data-orbit-experimental.png
tests/baselines/generation/current/previews/guilloche-plates/variant-data-orbit-motion-safe.png
tests/baselines/generation/current/previews/guilloche-plates/variant-inscription-accessible-static.png
tests/baselines/generation/current/previews/guilloche-plates/variant-inscription-compatibility.png
tests/baselines/generation/current/previews/guilloche-plates/variant-inscription-experimental.png
tests/baselines/generation/current/previews/guilloche-plates/variant-inscription-motion-safe.png
tests/baselines/generation/current/previews/guilloche-plates/variant-lissajous-moire-accessible-static.png
tests/baselines/generation/current/previews/guilloche-plates/variant-lissajous-moire-compatibility.png
tests/baselines/generation/current/previews/guilloche-plates/variant-lissajous-moire-experimental.png
tests/baselines/generation/current/previews/guilloche-plates/variant-lissajous-moire-motion-safe.png
tests/baselines/generation/current/previews/guilloche-plates/variant-rose-orbit-accessible-static.png
tests/baselines/generation/current/previews/guilloche-plates/variant-rose-orbit-compatibility.png
tests/baselines/generation/current/previews/guilloche-plates/variant-rose-orbit-experimental.png
tests/baselines/generation/current/previews/guilloche-plates/variant-rose-orbit-motion-safe.png
tests/baselines/generation/current/previews/guilloche-plates/variant-roulette-rosette-accessible-static.png
tests/baselines/generation/current/previews/guilloche-plates/variant-roulette-rosette-compatibility.png
tests/baselines/generation/current/previews/guilloche-plates/variant-roulette-rosette-experimental.png
tests/baselines/generation/current/previews/guilloche-plates/variant-roulette-rosette-motion-safe.png
tests/baselines/generation/current/previews/guilloche-plates/variant-wave-band-accessible-static.png
tests/baselines/generation/current/previews/guilloche-plates/variant-wave-band-compatibility.png
tests/baselines/generation/current/previews/guilloche-plates/variant-wave-band-experimental.png
tests/baselines/generation/current/previews/guilloche-plates/variant-wave-band-motion-safe.png
tests/baselines/generation/current/previews/guilloche-plates/wild-01-duotone-111.png
tests/baselines/generation/current/previews/guilloche-plates/wild-01-variation-01-centered-dual.png
tests/baselines/generation/current/previews/guilloche-plates/wild-01-variation-02-radial-fourfold.png
tests/baselines/generation/current/previews/guilloche-plates/wild-01-variation-03-orbital-twins.png
tests/baselines/generation/current/previews/guilloche-plates/wild-01-variation-04-radial-sixfold.png
tests/baselines/generation/current/previews/guilloche-plates/wild-01-variation-05-counter-orbit.png
tests/baselines/generation/current/previews/guilloche-plates/wild-01-variation-06-bilateral-hourglass.png
tests/baselines/generation/current/previews/guilloche-plates/wild-01-variation-07-radial-sunburst.png
tests/baselines/generation/current/previews/guilloche-plates/wild-01-variation-08-orbital-eclipse.png
tests/baselines/generation/current/previews/guilloche-plates/wild-01-variation-09-bilateral-mirror.png
tests/baselines/generation/current/previews/guilloche-plates/wild-01-variation-10-radial-orbit-hybrid.png
tests/baselines/generation/current/previews/guilloche-plates/wild-02-orbit-moire.png
tests/baselines/generation/current/previews/guilloche-plates/wild-02-variation-01-bilateral-nests.png
tests/baselines/generation/current/previews/guilloche-plates/wild-02-variation-02-triangular-orbits.png
tests/baselines/generation/current/previews/guilloche-plates/wild-02-variation-03-radial-triptych.png
tests/baselines/generation/current/previews/guilloche-plates/wild-03-registration-slip.png
tests/baselines/generation/current/previews/guilloche-plates/wild-03-variation-01-left-expression.png
tests/baselines/generation/current/receipts/runtime/guilloche-glyph-v3.run-1.stdout.txt
tests/baselines/generation/current/receipts/runtime/guilloche-glyph-v3.run-2.stdout.txt
tests/baselines/generation/current/receipts/runtime/guilloche-plates.run-1.stdout.txt
tests/baselines/generation/current/receipts/runtime/guilloche-plates.run-2.stdout.txt
tests/baselines/generation/current/receipts/runtime/guilloche-transition-frames.run-1.stderr.txt
tests/baselines/generation/current/receipts/runtime/guilloche-transition-frames.run-2.stderr.txt
tests/baselines/generation/current/svg/guilloche-glyph-v3/combination-01-duotone-111.svg
tests/baselines/generation/current/svg/guilloche-glyph-v3/combination-02-izzi-20.svg
tests/baselines/generation/current/svg/guilloche-glyph-v3/combination-03-alpha-60.svg
tests/baselines/generation/current/svg/guilloche-glyph-v3/combination-04-orbit-08.svg
tests/baselines/generation/current/svg/guilloche-glyph-v3/combination-05-radial-13.svg
tests/baselines/generation/current/svg/guilloche-glyph-v3/combination-06-signal-01.svg
tests/baselines/generation/current/svg/guilloche-glyph-v3/combination-07-glitch-03.svg
tests/baselines/generation/current/svg/guilloche-glyph-v3/combination-08-silence-00.svg
tests/baselines/generation/current/svg/guilloche-glyph-v3/combination-09-trace-22.svg
tests/baselines/generation/current/svg/guilloche-glyph-v3/combination-10-wild-10.svg
tests/baselines/generation/current/svg/guilloche-glyph-v3/number-01-digit-0.svg
tests/baselines/generation/current/svg/guilloche-glyph-v3/number-02-digit-1.svg
tests/baselines/generation/current/svg/guilloche-glyph-v3/number-03-digit-2.svg
tests/baselines/generation/current/svg/guilloche-glyph-v3/number-04-digit-3.svg
tests/baselines/generation/current/svg/guilloche-glyph-v3/number-05-digit-4.svg
tests/baselines/generation/current/svg/guilloche-glyph-v3/number-06-digit-5.svg
tests/baselines/generation/current/svg/guilloche-glyph-v3/number-07-digit-6.svg
tests/baselines/generation/current/svg/guilloche-glyph-v3/number-08-digit-7.svg
tests/baselines/generation/current/svg/guilloche-glyph-v3/number-09-digit-8.svg
tests/baselines/generation/current/svg/guilloche-glyph-v3/number-10-digit-9.svg
tests/baselines/generation/current/svg/guilloche-glyph-v3/word-01-izzi.svg
tests/baselines/generation/current/svg/guilloche-glyph-v3/word-02-duotone.svg
tests/baselines/generation/current/svg/guilloche-glyph-v3/word-03-orbit.svg
tests/baselines/generation/current/svg/guilloche-glyph-v3/word-04-radial.svg
tests/baselines/generation/current/svg/guilloche-glyph-v3/word-05-signal.svg
tests/baselines/generation/current/svg/guilloche-glyph-v3/word-06-glitch.svg
tests/baselines/generation/current/svg/guilloche-glyph-v3/word-07-silence.svg
tests/baselines/generation/current/svg/guilloche-glyph-v3/word-08-trace.svg
tests/baselines/generation/current/svg/guilloche-glyph-v3/word-09-form.svg
tests/baselines/generation/current/svg/guilloche-glyph-v3/word-10-wild.svg
tests/baselines/generation/current/svg/guilloche-plates/transition-frame-0.svg
tests/baselines/generation/current/svg/guilloche-plates/transition-frame-12.svg
tests/baselines/generation/current/svg/guilloche-plates/transition-frame-18.svg
tests/baselines/generation/current/svg/guilloche-plates/transition-frame-24.svg
tests/baselines/generation/current/svg/guilloche-plates/transition-frame-6.svg
tests/baselines/generation/current/svg/guilloche-plates/variant-data-orbit-accessible-static.svg
tests/baselines/generation/current/svg/guilloche-plates/variant-data-orbit-compatibility.svg
tests/baselines/generation/current/svg/guilloche-plates/variant-data-orbit-experimental.svg
tests/baselines/generation/current/svg/guilloche-plates/variant-data-orbit-motion-safe.svg
tests/baselines/generation/current/svg/guilloche-plates/variant-inscription-accessible-static.svg
tests/baselines/generation/current/svg/guilloche-plates/variant-inscription-compatibility.svg
tests/baselines/generation/current/svg/guilloche-plates/variant-inscription-experimental.svg
tests/baselines/generation/current/svg/guilloche-plates/variant-inscription-motion-safe.svg
tests/baselines/generation/current/svg/guilloche-plates/variant-lissajous-moire-accessible-static.svg
tests/baselines/generation/current/svg/guilloche-plates/variant-lissajous-moire-compatibility.svg
tests/baselines/generation/current/svg/guilloche-plates/variant-lissajous-moire-experimental.svg
tests/baselines/generation/current/svg/guilloche-plates/variant-lissajous-moire-motion-safe.svg
tests/baselines/generation/current/svg/guilloche-plates/variant-rose-orbit-accessible-static.svg
tests/baselines/generation/current/svg/guilloche-plates/variant-rose-orbit-compatibility.svg
tests/baselines/generation/current/svg/guilloche-plates/variant-rose-orbit-experimental.svg
tests/baselines/generation/current/svg/guilloche-plates/variant-rose-orbit-motion-safe.svg
tests/baselines/generation/current/svg/guilloche-plates/variant-roulette-rosette-accessible-static.svg
tests/baselines/generation/current/svg/guilloche-plates/variant-roulette-rosette-compatibility.svg
tests/baselines/generation/current/svg/guilloche-plates/variant-roulette-rosette-experimental.svg
tests/baselines/generation/current/svg/guilloche-plates/variant-roulette-rosette-motion-safe.svg
tests/baselines/generation/current/svg/guilloche-plates/variant-wave-band-accessible-static.svg
tests/baselines/generation/current/svg/guilloche-plates/variant-wave-band-compatibility.svg
tests/baselines/generation/current/svg/guilloche-plates/variant-wave-band-experimental.svg
tests/baselines/generation/current/svg/guilloche-plates/variant-wave-band-motion-safe.svg
tests/baselines/generation/current/svg/guilloche-plates/wild-01-duotone-111.svg
tests/baselines/generation/current/svg/guilloche-plates/wild-01-variation-01-centered-dual.svg
tests/baselines/generation/current/svg/guilloche-plates/wild-01-variation-02-radial-fourfold.svg
tests/baselines/generation/current/svg/guilloche-plates/wild-01-variation-03-orbital-twins.svg
tests/baselines/generation/current/svg/guilloche-plates/wild-01-variation-04-radial-sixfold.svg
tests/baselines/generation/current/svg/guilloche-plates/wild-01-variation-05-counter-orbit.svg
tests/baselines/generation/current/svg/guilloche-plates/wild-01-variation-06-bilateral-hourglass.svg
tests/baselines/generation/current/svg/guilloche-plates/wild-01-variation-07-radial-sunburst.svg
tests/baselines/generation/current/svg/guilloche-plates/wild-01-variation-08-orbital-eclipse.svg
tests/baselines/generation/current/svg/guilloche-plates/wild-01-variation-09-bilateral-mirror.svg
tests/baselines/generation/current/svg/guilloche-plates/wild-01-variation-10-radial-orbit-hybrid.svg
tests/baselines/generation/current/svg/guilloche-plates/wild-02-orbit-moire.svg
tests/baselines/generation/current/svg/guilloche-plates/wild-02-variation-01-bilateral-nests.svg
tests/baselines/generation/current/svg/guilloche-plates/wild-02-variation-02-triangular-orbits.svg
tests/baselines/generation/current/svg/guilloche-plates/wild-02-variation-03-radial-triptych.svg
tests/baselines/generation/current/svg/guilloche-plates/wild-03-registration-slip.svg
tests/baselines/generation/current/svg/guilloche-plates/wild-03-variation-01-left-expression.svg
tests/baselines/guilloche/guilloche-scene-profile-bundle-v1.json
tests/baselines/guilloche/guilloche-scene-resolved-v1.json
tests/graph-guilloche-synthesis.cc
tests/guilloche-odr/left.cc
tests/guilloche-odr/right.cc
tests/guilloche-profile-companions.cc
tests/guilloche-scene-json.cc
tests/guilloche-scene-schema-contract.py
tests/guilloche.cc
```
