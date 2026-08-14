# Izzi futures — ai-time-to-die draft 8 (phase 3, round 8) — plan

Status: `DRAFT-8-IMPLEMENTED 2026-08-14; AWAITING-REVIEW (G4); ZERO-SPEND`

## Decision summary

Draft 7 was reviewed via `bdekoz/situationshipin.space` issue #25 with a
**REVISE** decision and the note that the visual parts are "almost perfect".
This plan proposes the draft-8 attempted render: keep the accepted draft-7
visuals (orientation-fixed danmaku, full-height lanes capability, wild 50%
opacity masks, audit usage, draft-1 HLT T5, full-motion review video), spread
the bullet text across both halves of the 1920 px frame using two OCR'd BOC
blocks, and correct the audio state machine so canonical source segments play
their own source audio (shinkansen bed everywhere else).  No provider
submission; zero spend.

## GitHub poll results (2026-08-14)

Issue #25 (open) — **Decision: REVISE**:

- Danmaku text readable: **PASS**.  Still only in the top half: get text all
  the way down the 1920 side by OCR'ing **two text regions** from the page
  and using one in the top half and one in the bottom half.
- Audio: shinkansen throughout is wrong.  When the canonical source is
  playing, it is that source video **and audio and subtitles**; every other
  part is shinkansen.

## Review findings → draft-8 work items

| Finding | Draft-8 change | Executor | Verification |
|---|---|---|---|
| Danmaku top-half only | Extract **two BOC `CANONICAL-MARKER` blocks** from the seeded page (block 1 and block 2; second page fallback if only one block).  Block 1 feeds top-half lanes (lane packing from y=0), block 2 feeds bottom-half lanes (lane packing from y=960 via a new `lane_offset_y` in `build_resolved`); runs merged into one izzi overlay document | `scripts/compose-vector-overlay.py` + `scripts/render-time-to-die-draft8.py` | Red-text y-range spans both halves (text present above and below y=960) at T2 timecodes; OCR readable in both halves |
| Audio wrong | Canonical source audio (CUT4 mp3) plays during T1a (timeline 5–25 s, source 6–26 s) and T1b (timeline 36–46 s, source 45–55 s); shinkansen bed everywhere else.  Draft-1-style `adelay`/`amix` then loudnorm master | ffmpeg in draft-8 renderer | Waveform check at T1a/T1b boundaries; bed elsewhere confirmed |
| Subtitles (canonical parts) | Canonical source has an `hdmv_pgs_subtitle` (bitmap) track.  Bounded feasibility: extract with `mkvextract` and attempt burn via ffmpeg `pgssub` decode; if no local PGS burn path exists, record `NOT-BURNED` (track referenced) and defer to a dedicated subtitle pipeline | ffmpeg/mkvtoolnix | Subtitle pixels present in T1a/T1b frames, or explicit NOT-BURNED record |

## Naming and identity (draft 8)

| Role | Value |
|---|---|
| Artifact id | `ai-time-to-die-draft-8-attempted-render` |
| Family | `ai-time-to-die` |
| Generation class | `attempted-render` |
| Feedback round | `draft-8` |
| Review scope | `DRAFT-8-ATTEMPTED-RENDER` |
| Canonical | `outputs/review/feedback/visual/ai-time-to-die/draft-8/ai-time-to-die-draft-8-attempted-render.mkv` |
| Review format | full-motion 360x640 video proxy (draft-1 style) |
| Renderer | `scripts/render-time-to-die-draft8.py` (new) |

Draft 7 stays immutable; draft 8 is a new catalog entry so both remain
reviewable.

## Staging

### Phase 8a — Two-region bullet corpus

- Extend the PDF block extractor to return two blocks: the first two
  `BOC-???? CANONICAL-MARKER` blocks on the seeded page (16–57), falling back
  to a second page when the page has fewer than two blocks.
- Add `lane_offset_y` to `build_resolved`/`pack_lanes`; bottom-half runs get
  `lane_offset_y = H/2` so glyph baselines, bboxes, and background boxes land
  in the lower half.
- Merge the two resolved documents into one izzi emission per frame (motion
  unchanged: red-on-white 20 pt, W3C scroll/reverse, 0.2 s stagger, 2.5 s
  duration).

### Phase 8b — Audio state machine

- Bed: shinkansen WAVs (existing mix).
- Canonical windows: CUT4 mp3 trimmed to 6–26 s and 45–55 s, delayed to
  timeline 5–25 s and 36–46 s.
- Master: `amix` + `alimiter` + `loudnorm` (draft-1 style).

### Phase 8c — Subtitles (bounded)

- Confirm local PGS burn feasibility (mkvextract + ffmpeg `pgssub` decode).
- If a working path exists, burn subtitles into T1a/T1b with a house style;
  otherwise record `NOT-BURNED` in render-notes with the source track
  referenced.

### Phase 8d — Render, verify, publish

- Render canonical + full-motion proxy; write hashes and per-finding
  verification frames; publish via `publish-video-proof.mjs` (full-motion,
  `PROJECT-APPROVED` gate), commit + push izzi and portal.

## Kept from draft 7 (accepted)

- Bullet text orientation fix (izzi y-flip), red-on-white 20 pt style.
- Wild 50% opacity face masks with exact landmark transform (PASS #23).
- Audit usage pages 3/s (PASS #23).
- T5 = draft-1 here-lies-trouble pilot stand-in.
- A/V bed discipline for all non-canonical segments.
- Full-motion 360x640 review video.

## Implementation record (2026-08-14)

- `scripts/compose-vector-overlay.py`: `build_resolved`/`pack_lanes` gained
  `lane_offset_y` so a second lane family packs from y = H/2.
- `scripts/render-time-to-die-draft8.py`: two-region corpus (top block page
  38 `BOC-9956` 14 lines; bottom block page 33 `BOC-9942` 9 lines), one
  merged izzi document per frame; canonical CUT4 audio during T1a/T1b with
  shinkansen bed elsewhere; subtitles `NOT-BURNED` (PGS bitmap, no local burn
  path).
- Verified: red bullet text spans both halves (y 3..1849; 11.6k px top /
  36.3k px bottom), OCR reads `BOC-9956` top and `BOC-9942` bottom;
  canonical audio RMS 5022 at t=10 s vs 2001 bed-only at t=27 s; masks and
  T5 unchanged.
