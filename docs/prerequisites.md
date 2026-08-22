# Prerequisites

Back: [izzi](../README.md)

Development and use is hosted on top-of-tree Linux. At the moment,
this means Fedora 41 or Alma 9 operating systems on multi-core
x86_64 hardware.

Some familiarity with Linux, contemporary C++20, and SVG is assumed.

**Text / subtitle / OCR machinery** (used by the time-to-die vertical and the
subtitle capability):

- `tesseract` 5.x — OCR of frames and PDFs into bullet-text and subtitle
  content (already the default OCR engine in the time-to-die draft scripts).
- `ffmpeg` / `ffprobe` with `libass` — probe clip geometry/duration, extract
  frames, and burn SRT/WebVTT subtitles.
- `python3` (stdlib) and `node` — OCR orchestration and subtitle
  serialization helpers.
- Atkinson Hyperlegible fonts installed locally
  (`/home/bkoz/.fonts/atkinson_hyperlegible.2026/`) for legible subtitle and
  label rendering.
