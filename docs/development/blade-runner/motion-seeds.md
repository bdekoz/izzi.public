# Blade Runner 1080p cuts as motion-aesthetic seeds

Recorded: 2026-08-12 America/Los_Angeles
Status: `INVESTIGATION-COMPLETE; SEED-RECORDS-DRAFTED; SCHEMA-EXTENDED;
CANARY-SPECS-WIRED; CHECKER-PASS; HUMAN-REVIEW-PENDING; NOT-SHARED`

This document investigates whether the five local 1080p Blade Runner Final Cut
clips in `resources/blade-runner/output/ssk.1080p/` can serve as seeds for
describing motion aesthetics in the Izzi motion-canary workflow. It records
what the clips measurably are, derives a per-clip motion-aesthetic reading from
observed metadata, proposes a hash-bound seed contract, and keeps the film
itself strictly local.

## Source inventory

All five files are 1920x1080 H.264 High 10 (yuv420p10le, bt.2020 with SMPTE
2084 mastering metadata retained), 23.976 fps, MP3 48 kHz stereo audio, PGS
subtitle track, Matroska container. Each is an unbroken single take: the
0.35 scene-change detector found zero hard cuts in every clip.

| Clip | Source time (s) | Duration (s) | Frames | SHA-256 |
| --- | --- | --- | --- | --- |
| cut-1 | 2170 | 8.023 | 192 | `1864690809c75fbdced98b9feeb4bd2ed8ab5696b07cc3b0d4bb92fb36efc2da` |
| cut-2 | 2171 | 10.523 | 252 | `380fc8cff7697c107c0dfdfabad32f5eee48b1fe023a5d2fdbb9067d2b65821a` |
| cut-3 | 2215 | 19.023 | 456 | `7d785c7c1a7ddcd7eda091241418898dff728b92582374bb33a7a81002fb8e72` |
| cut-4 | 6373 | 79.038 | 1894 | `7c0652f78d7d8135ba1f054e611cf70667e3e086977bcd9905ebf3f0054bb530` |
| cut-5 | 6588 | 16.023 | 383 | `83f51e7484e93b56a66e2c5ee2f9d08bd4b7b775c44b9de23747376855f68514` |

Paths: `resources/blade-runner/output/ssk.1080p/Blade.Runner.Final-Cut.4k-cut-N-1080p.1.mkv`.

## Measurement method

- Stream and duration metadata: `ffprobe` and `-count_frames`.
- Scene changes: ffmpeg `select='gt(scene,0.35)'`, counted selected frames.
- Motion energy: 4 fps sampling, `scale=160:90`, `format=gray`,
  `tblend=all_mode=difference`, `signalstats` YAVG (mean absolute luma change).
- Change coverage (camera vs subject proxy): same chain plus
  `lut='if(gte(val,12),255,0)'`; coverage is the percentage of pixels whose
  luma changes by more than 12/255 between sampled frames. High coverage means
  whole-frame motion (camera or a large moving mass); low coverage with nonzero
  energy means localized subject motion.
- Luma: mean `signalstats` YAVG per clip.
- Saturation: ImageMagick HSL mean of the mid-duration frame.

## Measured profiles

| Clip | Coverage mean | Coverage peak | Motion mean | Motion peak | Luma mean /255 | Saturation (mid) | Per-second coverage shape |
| --- | --- | --- | --- | --- | --- | --- | --- |
| cut-1 | 0.60% | 3.08% | 2.2 | 4.4 @ 3 s | 125.6 | 41.8% | near-static; tiny rise at second 3 |
| cut-2 | 2.26% | 51.4% | 5.5 | 83.7 @ 9 s | 134.6 | 42.4% | calm through second 8, one violent whole-frame burst at second 9 |
| cut-3 | 14.78% | 61.6% | 20.5 | 104.9 @ 1 s | 159.0 | 40.6% | sustained high energy; surges at 1, 5, 13, and 18-20 s |
| cut-4 | 5.95% | 70.0% | 13.1 | 145.1 @ 5 s | 246.6 | 34.4% | rhythmic: activity bursts alternating with near-freeze seconds (28-29, 55-57, 62, 66-67, 70, 72-73) |
| cut-5 | 8.46% | 14.3% | 13.9 | 23.5 @ 14 s | 163.0 | 41.5% | steady ramp from 2% to 13%, moderate sustained energy |

## Per-clip motion-aesthetic reading

These readings describe observable motion qualities only, not scene identity,
composition, or visual content.

**cut-1 — near-static tableau with micro-drift.** Locked-off or extremely slow
camera; nothing crosses the frame. Tempo: held stillness. Use as the reference
for "the camera is never the primary source of motion."

**cut-2 — held calm, then one decisive burst.** Almost eight seconds of quiet
localized motion, then a single whole-frame move that briefly covers half the
frame. Tempo: restraint followed by a sharp accent. Use for "one gesture per
beat" and for isolating a single deliberate camera action.

**cut-3 — continuous dense motion.** High sustained coverage with periodic
surges. The frame is never still. Tempo: urgent, unbroken forward energy. Use
for group locomotion, crowd density, and continuous walking.

**cut-4 — rhythmic tableau motion.** Moderate bursts of whole-frame motion
separated by near-freeze moments, over a very bright, low-saturation frame.
Tempo: episodic; action, pause, action. Use for exploration that advances in
stages, and for keeping environmental motion subordinate during holds.

**cut-5 — steady build.** Motion rises gradually and stays moderate, with no
violent peak. Tempo: measured acceleration. Use for confident, unhurried
walking with coherent body-driven progress.

## Why these clips work as motion seeds

1. **Unbroken takes**: every clip is one continuous shot, so the measured motion
   is shot behavior rather than edit rhythm.
2. **Varied tempo vocabulary**: the set spans stillness (cut-1), calm-plus-accent
   (cut-2), dense continuous (cut-3), episodic (cut-4), and steady build
   (cut-5).
3. **Camera vs subject separation is measurable**: low-coverage energy indicates
   subject motion; high-coverage bursts indicate camera or whole-frame motion.
4. **10-bit HDR metadata is retained**, so temporal behavior was not distorted by
   the 1080p re-encode (bt.2020/SMPTE 2084 mastering data preserved).

## Proposed seed contract

A motion-aesthetic seed is a hash-bound local reference clip whose *derived
descriptor text* may enter prompts and motion-canary JSON. The clip bytes never
leave the local machine.

Suggested per-clip seed record:

```json
{
  "seed_id": "br-final-cut-cut-1",
  "source": {
    "path": "resources/blade-runner/output/ssk.1080p/Blade.Runner.Final-Cut.4k-cut-1-1080p.1.mkv",
    "sha256": "1864690809c75fbdced98b9feeb4bd2ed8ab5696b07cc3b0d4bb92fb36efc2da"
  },
  "measured": {
    "duration_seconds": 8.023,
    "fps": 23.976,
    "coverage_mean_percent": 0.6,
    "coverage_peak_percent": 3.08,
    "luma_mean_255": 125.6,
    "single_take": true
  },
  "motion_grammar": {
    "camera": ["near-static", "micro-drift", "never the primary mover"],
    "subject_motion": ["localized", "contained"],
    "environmental_motion": ["minimal"],
    "tempo": ["held stillness"]
  },
  "review": { "status": "PENDING" }
}
```

The `motion_grammar` fields reuse the vocabulary already present in
`docs/development/seedance/visual-method.md` (camera, subject_motion,
environmental_motion, tempo). Only these descriptor strings, never frames or
film-derived imagery, are eligible for provider prompts.

## Wiring into the motion-canary contract

Two compatible options were proposed:

1. **Minimal**: add `motion_aesthetic_seed` to `sources` in each HLT canary
   spec using the existing `hashed_source` shape (`path` + `sha256`), with the
   seed descriptor living in a companion `motion_grammar` block of the same
   spec.
2. **Schema**: extend
   `schemas/izzi.here-lies-trouble-motion-canary.schema.json` with an optional
   `motion_aesthetic_seeds` array (seed_id, source, measured, motion_grammar)
   while preserving `additionalProperties: false`.

The schema option was implemented. The motion-canary schema now defines
`motion_aesthetic_seed_reference`, `motion_aesthetic_seed`, and
`motion_grammar` under `$defs`, and each canary specification may carry an
optional `motion_aesthetic_seeds` array plus a derived `motion_grammar` block.

Illustrative mapping for the three Frogtown v2 canaries:

| Canary | Seed reference | Derived motion vocabulary |
| --- | --- | --- |
| 01 GROUP-WALK | cut-1 (camera), cut-5 (locomotion) | unhurried weighty stride; camera a restrained lateral drift, never the mover; steady build without violence |
| 02 CONVERSATION-REACTIONS | cut-1, cut-2 | held stillness between beats; one deliberate gesture or reaction per beat; no continuous whole-frame motion |
| 03 RIVER-EDGE EXPLORATION | cut-4 | episodic progress: advance, pause, look; bright ambient field; environmental glint and sway during holds |

## Implementation status

- `resources/blade-runner/motion-aesthetic-seeds.json` contains the five
  hash-bound seed records (source, measured metadata, `motion_grammar`,
  `review.status`).
- `schemas/izzi.here-lies-trouble-motion-canary.schema.json` was extended with
  the optional seed and grammar definitions described above.
- All three Frogtown v2 motion-canary specifications reference the seeds and
  carry their derived `motion_grammar`:
  - `examples/seedance2/here-lies-trouble-episode-01-frogtown.v2.motion-canary-01.json`
    (GROUP-WALK; cut-1 camera-reference, cut-5 locomotion-reference);
  - `...-02.json` (CONVERSATION-AND-LISTENER-REACTIONS; cut-1 camera-reference,
    cut-2 gesture-accent-reference);
  - `...-03.json` (LOCATION-EXPLORATION; cut-4 exploration-tempo-reference).
- `scripts/check-hlt-frogtown-v2-canaries.py` validates the seed records
  against the schema, re-hashes all five source MKVs, resolves every canary
  seed reference, and rejects unknown or duplicate seed ids.
- Verification: `python3 scripts/check-hlt-frogtown-v2-canaries.py` passes
  (v1 identity, audio canary, listening MKV, three motion specifications, and
  the five seed records).

These specifications are the motion version of here-lies-trouble at the
specification boundary. Actual motion generation remains a separate,
unauthorized gate: provider, model, cost, and submission authorization are
still required before generating even the first silent motion canary.

## Authority and privacy boundary

- Blade Runner is a commercial film. The clips are local analysis references
  only; they are not inputs to any generator, are not uploaded, and their
  frames are not published to situationshipin.space or anywhere else.
- No provider request, model, seed value, cost, motion generation, or
  submission is authorized or performed by this investigation.
- Training-conversion transfer remains `NOT-SHARED`.
- Human acceptance of the derived motion vocabulary is not inferred.
