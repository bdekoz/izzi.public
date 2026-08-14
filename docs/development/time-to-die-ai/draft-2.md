# Izzi futures — ai-time-to-die draft 2 (phase 3, round 2) — plan

Status: `DRAFT-2-PLAN; AWAITING-APPROVAL (G1); ZERO-SPEND`

## Decision summary

Draft 1 was published to the situationshipin.space proofs surface and
reviewed. The review decision is **REVISE** (recorded 2026-08-14T07:02:04Z in
`bdekoz/situationshipin.space` issue #18), with five timecode-specific
findings. This plan proposes the draft-2 attempted render: the same canonical
57-second vertical timeline, re-cut and re-rendered with a fix for each
finding, published as a new immutable artifact
`ai-time-to-die-draft-2-attempted-render`. Everything is local and zero-spend
unless a new gate authorizes otherwise.

## GitHub poll results (2026-08-14)

- Public izzi site: Pages build **built** for `bdekoz/izzi.public` commit
  `f2ed48f` (07:07:05Z); https://bdekoz.github.io/izzi.public/ live.
- Portal live checks: draft-1 review page, its `manifest.json`, and
  `proofs.html` all return HTTP 200.
- Portal catalog (`data/review-items.json`, 261 items):
  `ai-time-to-die-draft-1-attempted-render` is `human_review_state:
  UNREVIEWED`, `baseline_state: NOT-PROMOTED` (the decision was recorded via
  the issue, not yet mirrored into the catalog fields).
- Review issue #18 (open): **Decision: REVISE** with the findings below.

## Review findings → draft-2 work items

| Timecode | Finding | Draft-2 change | Executor | Verification |
|---|---|---|---|---|
| 0:02 | Transition good | Keep as-is | — | Frame check |
| 0:03 | PDF page clipped on left; scale "fit page"; overlap filled with randoma11y-accent color if found, else default house colors; sequence should move faster, 3 pages/s | T4 rework: fit-page (contain) scaling, overlap fill from the randoma11y-accent palette with house-color fallback, interjection rate 3 pages/s | pdftoppm + izzi + ffmpeg | Frames at 0:03 + page-rate check |
| 0:06 | Guilloche/surface-tension layer: white layer must be transparent, lines varied sizes/opacities of white; placement must obscure the face; use facial recognition; triangle eye1–eye2–mouth: blue guilloche wild @ eye1, green moire wild @ eye2, moire @ mouth; colors/patterns change every 1 s | T1 mask rework: transparent compositing; add local face/eye/mouth detection (default OpenCV Haar cascades; fallback center-weighted triangle); per-second color/pattern assignments | izzi render + local detector | Frames 0:06–0:26 + detection manifest |
| 0:29 | Transform clipped; should be legible: OCR the text and colors, scroll bottom→up at one line / 0.3 s; fit vertical frame width; scroll speed is good | T2/T3 rework: fit screenshot to frame width; OCR (default tesseract) into line overlays scrolling bottom-up at 0.3 s/line, colors from the frame palette | ffmpeg/izzi + tesseract | Frames 0:29–0:37 |
| 0:44 | Canonical soundtrack still audible in transform sections; during transforms the transform owns the scene (canonical A/V stops); after the transition ends, canonical video+audio resume | Segment state machine: mute/pause canonical A/V across each transform, resume at the exact post-transition timecode | ffmpeg concat/filter with cut-point manifest | Waveform + frames at each transform boundary |
| 0:55 | Two-frame tiling confirmed as the way forward; "cat has two tails" FAIL | T5: keep 720p×2 tiling + letterboxing; forensics on the 0:55 frame, remove the duplicated-object artifact (likely tiling overlap or duplicated stand-in frame) | ffmpeg/izzi | Frame at 0:55 |

## Naming and identity (draft 2)

| Role | Value |
|---|---|
| Artifact id | `ai-time-to-die-draft-2-attempted-render` |
| Family | `ai-time-to-die` |
| Generation class | `attempted-render` |
| Feedback round | `draft-2` |
| Review scope | `DRAFT-2-ATTEMPTED-RENDER` |
| Canonical | `outputs/review/feedback/visual/ai-time-to-die/draft-2/ai-time-to-die-draft-2-attempted-render.mkv` |
| Review proxy | `review/media/ai-time-to-die/ai-time-to-die-draft-2-attempted-render.review.mp4` (360×640, ≤16 MiB) |
| Renderer | `scripts/render-time-to-die-draft2.py` (new) |

Draft 1 stays immutable; draft 2 is a new catalog entry so both are
reviewable.

## Staging

### Phase 3.2a — Forensics (local, zero-spend)

- Extract frames at 0:03, 0:06, 0:29, 0:44, 0:55 plus the waveform around
  every transform boundary.
- Root-cause the 0:55 "cat has two tails" artifact (tiling overlap vs.
  duplicated stand-in frame) before re-rendering T5.

### Phase 3.2b — Tooling defaults

- Confirm local availability: tesseract (OCR), OpenCV Haar cascades
  (face/eye/mouth), randoma11y-accent palette source (or house-style
  fallback), pdftoppm page extraction.
- Record tool versions and fallbacks in
  `outputs/review/feedback/visual/ai-time-to-die/draft-2/render-notes.json`.

### Phase 3.2c — T1 mask rework

- Transparent guilloche/surface-tension compositing (white layer alpha, lines
  at varied sizes/opacities).
- Face/eye/mouth detection per frame with the eye1–eye2–mouth triangle and
  per-second color/pattern rotation (blue guilloche wild / green moire wild /
  moire). Detection results recorded in the render manifest.

### Phase 3.2d — T2/T3, T4, T5, and A/V state machine

- T2/T3: width-fit screenshot + OCR line overlays scrolling bottom-up at
  0.3 s/line with frame-derived colors.
- T4: fit-page scaling, randoma11y-accent overlap fill, 3 pages/s.
- T5: keep tiling + letterboxing; apply the forensics fix.
- A/V state machine: canonical audio+video stop during transforms and resume
  exactly at the post-transition timecode.

### Phase 3.2e — Render and verify

- Render canonical 1080×1920@24 MKV + 360×640 MP4 proxy; write hashes,
  detection manifest, and per-finding verification frames.

### Phase 4.2 — Publish (human gate G3)

- `node scripts/publish-video-proof.mjs --dry-run …` then a real publish
  gated on an explicit `PROJECT-APPROVED`; commit + push the portal via the
  gcr SSH agent socket; validator must pass.

### Phase 5.2 — Human review (gate G4)

- Hand off the draft-2 review URL; decision vocabulary `KEEP` /
  `KEEP-PARTS` / `REVISE` / `REJECT` via a GitHub issue; mirror the decision
  to the dyad/local-object store.

## Gates with defaults

| Gate | Decision required | Default if not overridden |
|---|---|---|
| G1 | Approve this draft-2 plan and bound | Accept defaults; proceed |
| G2 | Draft-2 render bound | Same 57 s timeline with revised segments; one segment |
| G9 | Facial-detection model | OpenCV Haar cascades, local; fallback center-weighted triangle |
| G10 | OCR tool | tesseract, local; fallback manual transcription of the three screenshots |
| G3 | Publication approval | Refuse publication (requires `PROJECT-APPROVED`) |
| G4 | Draft-2 human review | `REVISE` → round 3; never auto-promote |

## Definition of done

- Draft-2 render published as a new catalog entry; draft-1 entry untouched;
  validator passes; live `proofs.html` equals local.
- Each finding verified against draft-2 output: fit-page + 3 pages/s (0:03),
  detection-triangle masks with transparent lines (0:06), OCR scroll fitted
  to width (0:29), A/V state machine (0:44), no duplicated-object artifact
  (0:55).
- Issue #18 updated with the draft-2 handoff (or a new issue); decision
  mirrored to the local store; `baseline_state` remains `NOT-PROMOTED` until
  an explicit `KEEP`.

## Risks and open items

- OCR quality on compressed screenshots and palette ambiguity — default to a
  contrast-considerate fallback and record per-frame OCR confidence.
- Haar cascade false positives on stylized frames — fallback to the
  center-weighted triangle; detections are manifest-visible for review.
- Frame-exact canonical resume depends on precise cut points — verify with
  waveform + frame diffs at every transform boundary.
- The 16 MiB proxy cap is unchanged; the ten-frame filmstrip fallback remains
  available.
- The "cat has two tails" root cause is unknown until forensics; if it is a
  source-material artifact, apply a localized de-duplication or re-crop
  rather than re-doing the tiling approach.

## Immediate next step

Human approval of this plan (gate G1), then Phase 3.2a forensics, the
draft-2 render, publication (G3), and the round-2 review (G4).
