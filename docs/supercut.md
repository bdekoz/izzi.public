# Tool: supercut

Derives deterministic review cuts from a source media file with recorded
provenance. This is the strict-CLI successor to
`MiL.git/scripts/ffmpeg-supercut-123-dupe.sh`.

## Contracts

- CLI: `scripts/izzi-supercut.py` (`--input --begin --length --cut-number`,
  optional `--tiers`, `--accurate`, `--fps`, `--output-root`, `--dry-run`,
  `--resume`, or `--batch cuts.json`).
- C++ types: `src/izzi-media-supercut.h`
  (`izzi::media::supercut::cut_spec`, `cut_manifest`, validators, and the
  naming/serialization helpers), `api_version = "izzi-supercut-1"`.
- Batch schema: `schemas/izzi.supercut-1.schema.json`.

## Output tiers

| Tier | Directory | Name | Encoding |
|---|---|---|---|
| `orig` | `ssk.orig` | `FILE-cut-N.mkv` | stream copy (keyframe-aligned) |
| `1080p` | `ssk.1080p` | `FILE-cut-N-1080p.1.mkv` | 1920×1080 letterbox/pillarbox, libx264 qp18 slow, FLAC |
| `proxy` | `ssk.proxy` | `FILE-cut-N-360p.2.mkv` | 640×360, libx264 crf23, AAC 128k |

The `.1` and `.2` suffixes reproduce the historical MiL.git naming so the
existing Blade Runner `time-to-die-ai` seed corpus stays name-compatible.

## Behavior

- Inputs are probed with ffprobe before cutting; a cut that extends past the
  source duration is rejected.
- `--accurate` seeks after the input for frame-accurate re-encoded tiers
  (stream copy remains keyframe-bound by design).
- Outputs are written to a `.tmp` sibling and atomically renamed; failed
  encodes are removed.
- Every run writes `manifest.json` (input SHA-256, filter graphs, codecs,
  geometry, per-output SHA-256/bytes/duration) and a `SHA256SUMS` file.
- `--resume` skips outputs whose recorded SHA-256 still matches.

## Checks

- `scripts/check-izzi-supercut.py` runs an end-to-end smoke test on a lavfi
  `testsrc2` input (no media corpus required) and verifies naming, geometry,
  duration, hashes, and resume idempotency.
- `tests/supercut-header.cc` pins the C++ naming/serialization contract.
- `make supercut-check` runs both.

## Provenance

This tool is a derivation helper only; it reads and remuxes user-supplied
media and records the exact filter graph and hashes it used. It does not
author new content and it never transcribes or embeds photographed text.
