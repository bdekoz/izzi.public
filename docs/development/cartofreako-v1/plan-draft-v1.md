# cartofreako-v1 — plan draft v1 (exploratory)

Title: `cartofreako-v1`

Status: `PLAN-DRAFT 2026-08-26; EXPLORATORY; ZERO-SPEND`

## Idea

A 1080p vertical short built from the 32 myriahedral WebP plates shown on
the cartofreako gallery page, in that page's presentation order. Each plate
is rotated 90° CCW to an exact-fit portrait, holds for 2 seconds, then
slides out left in 1 second. Letterboxing, when it occurs on any side, and
the wipe reveal use the plate's own sampled background color — never black;
the sequence runs plate by plate and ends on the final plate's background
color.

## Verified source set

- Path: `resources.rizal/cartofreako/webp/*.webp`
- Count: 32 WebP plates, all 1920×1080 RGB (landscape 16:9)
- Transform: rotate 90° CCW to 1080×1920 — exact fit, no letterboxing
- Order: cartofreako Myriahedral gallery page order
  (`geometry` → `graticules` → `earth` → `water` → `cloud-atmosphere` →
  `astro-…` → `orbital-…` → `network-…` → `anthropocene-…` →
  `resources-…`), frozen into a manifest when the proof is built
- Cloud-atmosphere exception: the page includes this plate but the local
  webp set does not, so it is read from the cartofreako repo's full tier
  and downscaled 3840×2160 → 1920×1080 to match

## Short form (as briefed)

- Resolution: 1080p vertical — 1080×1920
- Channels: 1, portrait
- Output: exactly one video file
- Timeline: per plate, 2.0 s hold then 1.0 s slide left —
  32 × 3.0 s = 96.0 s total

## Proposed decisions

- **Master framerate.** 30 fps, the repo vertical-form standard. Each
  2.0 s hold is 60 frames and each 1.0 s wipe is 30 frames. Locked: 30 fps
  (approved).
- **Fit policy.** Rotate 90° CCW to exact-fit 1080×1920, so no bars
  appear. If letterboxing ever appears on any side (left-right or
  top-bottom), it uses the plate's own sampled background color instead of
  black — per the cartofreako document-background convention (most common
  border color). Locked: plate-background letterboxing (approved). No
  crop-to-fill, no designed extension.
- **Slide.** Each held plate translates left by its full width over
  1.0 s (ease-in), revealing that plate's own sampled background color —
  a slide-out wipe. Locked: slide-out wipe (approved); no simultaneous
  fade. The wipe after the final plate ends the video on that plate's
  background color, not black.
- **Audio.** `Sounds of Nature - Walk at the Beach.mp3`
  (`resources.rizal/here-lies-trouble/seed-audio-nature/`, sha256
  `00da53fb28e8868e05a48507f6271a9e90039896557cd95caebba285aa806d8a`).
  Source is MP3 44.1 kHz stereo, 204.42 s; trim the first 96.0 s to match
  the timeline, fade out over the final wipe, normalize to
  speech-friendly peak rather than forcing loudness on the waves. Trim
  point locked at 0.0 s — no voice, open on the ambience as it plays.
- **Encode.** H.264 high profile, yuv420p, CRF 18, one `.mp4`; probe,
  hashes, and a contact sheet retained beside it.
- **Ending.** The video stops exactly at 96.0 s on the final plate's
  background color after the final wipe — no extra hold.

## Decisions before rendering

All locked. Title: `cartofreako-v1` (matches the approved slug; adjust
freely if a different display title is wanted).

## Proof render (complete 2026-08-26, local, zero-spend)

- Video: `outputs/ad-hoc/cartofreako-v1/cartofreako-v1.1080p.mp4` —
  H.264 high, 1080×1920, 30 fps, 2880 frames, 96.000 s, AAC 48 kHz
  stereo.
- Frames verified: each plate holds full-frame for 60 frames at exact
  1080×1920 fit, the wipe steps leftward over 30 frames and ends on that
  plate's own sampled background color (first plate geometry
  `80,80,255`, cloud-atmosphere `255,255,255`, last plate
  `241,241,241`), the next plate starts a fresh hold, and the final frame
  lands on the last plate's background color.
- Audio: source trimmed at 0.0 s to 96.0 s, fade-out 95.0→96.0 s across
  the final wipe. Peak brought to -12 dBFS (+0.8 dB gain).
- Retained beside the video: `cartofreako-v1.plan.json` (frozen order,
  hold/wipe structure, per-plate background RGB, decisions, per-source
  sha256),
  `cartofreako-v1.probe.json`, `cartofreako-v1.contact-sheet.png` (8×5),
  `cartofreako-v1.sha256.txt`.
- Build script: `scripts/render-cartofreako-v1-proof.py` (deterministic,
  re-runnable; frames streamed straight to the encoder, no intermediate
  PNGs retained).

## Next iteration

Formality arrives by iteration, not by fiat. Later drafts lock the
resolution, channel count, orientation or arrangement, and duration,
then add estimates, gates, and orchestration. The formal plan is the
first artifact that requires the full contract; every earlier stage is
reviewed as-is and advances on explicit KEEP.
