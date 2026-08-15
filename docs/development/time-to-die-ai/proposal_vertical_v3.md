# ai-time-to-die — proposed vertical v3 summary

Status: `COMPLETE-VERTICAL-WALKTHROUGH 2026-08-14; episodes 01-03 published; awaiting G4 episode review`

## What was walked through (top to bottom)

The ai-time-to-die vertical was taken from proposal to complete episodes in
one continuous arc:

1. **Draft loop (drafts 1-9):** the episode template locked at 8 segments /
   57 s / 1080x1920@24.  Review vocabulary `KEEP` / `KEEP-PARTS` / `REVISE` /
   `REJECT` via GitHub issues on the portal.  Draft-9 received **KEEP**
   (issue #28).
2. **izzi vector text overlay:** red-on-white 20 pt two-region bullet text
   from OCR'd BOC CANONICAL-MARKER PDF blocks; orientation fixed (font y-up
   vs SVG y-down).
3. **Face masks:** MediaPipe FaceLandmarker landmarks mapped through the
   exact vertical transform; wild guilloche/moire/surface-tension textures at
   50 % opacity.
4. **Voice:** 29 Kokoro female stock voices materialized and reviewed;
   **zf_xiaoyi (V29)** selected (issue #27) and wired into every canonical
   window via MeanVC2.
5. **Audio layout (issue #26):** canonical segments carry the voice-remapped
   dialogue, the here-lies-trouble T5 carries its original audio, shinkansen
   bed everywhere else.
6. **T5:** tiling abandoned; the accepted draft-1 HLT stand-in is the
   default, with gated provider renders (110-240 credits each) for fresh
   locations (episode 3's Koreatown night-market scene).
7. **Portal:** full-motion video reviews, a 29-voice audio bank, a
   26-image guilloche/moire/surface-tension reference set, and a payload
   budget that grew 48 -> 96 MiB to host the series.
8. **Phase 6:** v2 synthesis plus a validated devastation-pacific-house-style
   special-topics PDF.
9. **Complete vertical:** episode masters 01-03 rendered, verified, and
   published as full-motion `episode-master` reviews.

## Learnings

- **Process:** per-step rendering beats long single sessions (sandbox CPU
  throttling); deterministic reuse is a legitimate accelerator (draft-9 was a
  ~0.1 min audio-only remux because the video was byte-identical to draft-8).
- **Provider:** the resolution cap persists (1080p requested, 720p or
  496x864 returned).  Plan for contain-fit composition, never tiling.
- **izzi text:** the glyph y-flip bug was only caught by OCR-verifying the
  rendered overlay; orientation verification should be a standing gate.
- **Subtitles:** PGS bitmap tracks have no local burn path; the policy is
  burn-if-possible-else-skip, recorded per episode.
- **Corpus bounds:** measure source durations early.  A 79.0 s canonical
  corpus supports exactly 2 distinct episodes; episode 3 was a documented
  callback plus a new provider location.
- **Voice:** Kokoro stock voices are enough for production selection; custom
  accents (Asian-American, Southern) require human reference recordings.
- **Budget:** full-motion reviews accumulate; portal payload caps need to be
  planned per series, not raised reactively.

## Measured facts

| Fact | Value |
|---|---|
| Episode template | 8 segments / 57 s / 1080x1920@24 |
| Whole-episode local render | 3-5 min (draft-4..8); ~0.1 min audio-only remux |
| Provider spend (round total) | 590 credits (110 + 240 + 240) |
| Voice conversion | ~1-2 min per 30 s canonical window, local CPU |
| Canonical corpus | 79.0 s (bounds series to 2-3 episodes) |
| Episodes produced | 01-03, published and live |
| Portal payload | 96 MiB cap; ~86 MiB used |

## Next steps

- G4-style human review of episodes 01-03; a `KEEP` promotes them to
  baseline.
- Use this walkthrough as the template for the **plan vertical** review class
  (proposed in `docs/development/sessions/explore_futures.20260814.planning_review_class.md`), so the next
  vertical starts with a reviewed plan instead of a draft loop.
- Optional follow-ups: a real PGS subtitle pipeline, WASM byte-parity for the
  overlay header, and Higgs TTS where authorized.
