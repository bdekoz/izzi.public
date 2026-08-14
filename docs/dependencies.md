# Izzi runtime and build dependencies

Records of third-party software and model assets required by izzi examples,
renderers, and tooling.  New dependencies are appended here when a round
introduces them; render-notes.json files record the exact versions observed
at render time.

## ai-time-to-die draft-4 pipeline (added 2026-08-14)

The draft-4 renderer adds MediaPipe face landmarks (T1 detector swap) on top
of the existing draft-3 toolchain.  The vector text overlay pipeline
(`scripts/compose-vector-overlay.py`) depends on the Python and font assets
listed below.

| Dependency | Version / asset | Purpose | License | Notes |
|---|---|---|---|---|
| MediaPipe (Python) | 1.0.0 (`mediapipe`) | FaceLandmarker face/eye/mouth landmarks for T1 masks | Apache-2.0 | Installed in the local Python 3.14 site-packages; Tasks API only (no legacy `solutions`) |
| MediaPipe face_landmarker model | `face_landmarker.task` (float16, 3,758,596 B, sha256 `64184e229b263107bc2b804c6625db1341ff2bb731874b0bcc2fe6544e0bc9ff`) | FaceLandmarker model asset | Apache-2.0 | Downloaded from `storage.googleapis.com/mediapipe-models/face_landmarker/face_landmarker/float16/1/` into `build/private/mediapipe/` (gitignored); not bundled with the pip package |
| Atkinson Hyperlegible Mono SemiBold | `/home/bkoz/.fonts/atkinson_hyperlegible.2026/AtkinsonHyperlegibleMono-SemiBold.otf` (sha256 `8ac9187458bef3e69b6700446ffc2721c44f7143ee1b08648fa2456075750dff`) | Vector text overlay outline source (T2/T3 bullet text) | Braille Institute free-use license (no attribution required) | Replaces DejaVu Sans Mono per user direction 2026-08-14 |
| fontTools | 4.61.0 | Glyph outline extraction (`SVGPathPen`, `BoundsPen`, `TransformPen`) | MIT | Python |
| cairosvg | installed (Pillow 11.3.0, numpy present) | Overlay SVG rasterization at target resolution | LGPL-3.0+ | Python; falls back to inkscape/headless Chrome |
| rapidjson | system header (`/usr/include/rapidjson`) | Resolved-runs JSON parsing in `examples/text-overlay.cc` | MIT | C++ example builds with `-std=gnu++20 -O2 -Werror` |
| tesseract | system (`tesseract`, `--psm 6`) | Terminal-filtered OCR for T2 lines | Apache-2.0 | Optional: OCR runs are skipped if absent |
| ffmpeg / ffprobe | system | Frame extraction, compositing, encode, proxy | GPL/LGPL | Core render pipeline |

## Baseline render toolchain (draft-1 through draft-4)

- OpenCV Haar cascades (`/usr/share/opencv4/haarcascades`): retained as the
  zero-dependency fallback detector when MediaPipe finds no face.
- pdftoppm: audit-PDF page extraction for T4.
- Python 3, Pillow, numpy: mask/line textures and compositing.
- DejaVu font family (`/usr/share/fonts/dejavu-*`): draft-1..3 title/drawtext
  text; superseded for T2/T3 by Atkinson Mono outlines in draft-4.

## Media assets (gitignored, not committed)

- Seed corpus: `resources/time-to-die-ai/` (~803 MiB; videos, screenshots,
  danmu-text-dyads, PDFs, WAVs).
- Provider render: `build/private/seedance2-runs/ai-time-to-die-t5-round-01-20260814/`.
- MediaPipe model asset: `build/private/mediapipe/face_landmarker.task`.
- Kokoro-82M female voice bank (29 stock voices, pinned revision
  `f3ff3571791e39611d31c381e3a41a3af07b4987`):
  `build/private/voice-audition/models/kokoro-82m/voices/`; target references
  in `build/private/voice-audition/ai-time-to-die/female-references/`
  (`provenance-female-all.json`); published as audio review items on
  situationshipin.space (media_kind `audio`, 64 k mono mp3s).
