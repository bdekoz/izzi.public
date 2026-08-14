# Audio workflow — tool: MeanVC2 + Kokoro (low compute)

First-pass sort for the low-compute local voice pass (`tool_XX.md` in the
plan; named for the actual tools used).

## MeanVC2

- Engine: pinned commit `86c201e5d9ac98f8a66abadbdd8df20067afc928` under
  `build/private/voice-audition/vendor/meanvc2/`; model `120ms`, 3 steps, CPU.
- Runtime venv: `build/private/voice-audition/runtime/venv/`
  (Python 3.11.13, torch 2.5.1+cpu).
- Checkpoints hash-bound in `build/private/voice-audition/audition-six-scenes.batch.json`.
- Entry points: `scripts/izzi-meanvc-audition-batch.py`,
  `scripts/izzi-meanvc-full-episode.py`.

## Kokoro targets

- Target voices `af_river`/`am_echo`/`bf_emma` generated from Kokoro-82M
  (`hexgrad/Kokoro-82M` pinned revision) into
  `build/private/voice-audition/target-references/V0X.wav`; provenance in the
  same directory.

Status: `FIRST-PASS-SORT; NO-REMOVAL`
