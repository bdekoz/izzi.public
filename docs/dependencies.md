# Izzi runtime and build dependencies

Records of third-party software and model assets required by izzi examples,
renderers, and tooling.  New dependencies are appended here when a round
introduces them; render-notes.json files record the exact versions observed
at render time.

## Portrait and print adapters (November 2026)

The installed [portrait core](portrait.md) is standard-library C++20. The
holiday/anniversary drivers and delivery scripts add the following optional
adapter dependencies; they are not requirements for all Izzi consumers.

| Layer | Dependencies | Evidence |
| --- | --- | --- |
| Decode and field preparation | Pillow/ImageCms, NumPy and SciPy | Whole-photo adapter and shared anniversary helpers |
| Vector layout and lettering | fontTools, Shapely, contourpy and system HarfBuzz | Outlined type, ink bounds and vector fade partitions |
| SVG/PNG/PDF delivery | ReportLab, Pycairo, PyGObject/librsvg, Poppler tools | Candidate preflight and edition review receipts |
| Original photos and fonts | Local source directories and recorded font files | Each edition's `sources.json` and source/font hashes |

Run `python3 scripts/portrait-edition.py preflight --edition fall-crazy-pants
--json build/portrait-preflight.json` to check the complete holiday v3/v4
environment before rendering. The runner requires Python 3.11 or later and
records versions plus photo, font, code, recipe and dependency hashes. It does
not install packages or supply missing local resources. See the
[candidate and restore workflow](visual_workflow/tool_portrait_print.md#isolated-holiday-v3v4-candidates).

The November 14 preflight observed Python 3.14.10, NumPy 2.4.6, SciPy 1.16.2,
Pillow 12.3.0 with LittleCMS 2.16, fontTools 4.62.1, Shapely 2.2.0,
contourpy 1.4.0, ReportLab 4.5.1, Pycairo 1.28.0, PyGObject 3.56.3,
Cairo 1.18.6, librsvg 2.62.4 and HarfBuzz 14.1.0. GCC was 16.2.1;
`pdfinfo`, `pdfimages`, `pdftocairo` and `pdftoppm` were Poppler 26.01.0.
This is a checked reproduction environment, not a portable package lock.
The original v3 environment receipt
remains the unchanged historical render record.

## MOSS and compiled local film (October–November 2026)

[MOSS appearance and local animation](visual_workflow/tool_moss_local.md)
separates the isolated historical training runtime from the system-Python CPU
renderer. Model/trainer revisions, offload settings and complete package pins
belong to the [saved appearance recipe](development/sessions/20261029.local_rendering_exploration_stage_4_durable_workflow.md#historical-moss-appearance-recipe).
The [Stage-5 software table](development/sessions/20261030.local_rendering_exploration_stage_5_durable_workflow.md#software-hosts-ai-and-data-usage)
records NumPy, SciPy, Pillow, OpenCV, FFmpeg and font/shaping dependencies for
compiled replay. Existing MOSS pixels and SAM masks are inputs; replay does
not need their model runtimes, a GPU or network access.

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

- Seed corpus: `resources.rizal/time-to-die-ai/` (~803 MiB; videos, screenshots,
  danmu-text-dyads, PDFs, WAVs).
- Provider render: `build/private/seedance2-runs/ai-time-to-die-t5-round-01-20260814/`.
- MediaPipe model asset: `build/private/mediapipe/face_landmarker.task`.
- Kokoro-82M female voice bank (29 stock voices, pinned revision
  `f3ff3571791e39611d31c381e3a41a3af07b4987`):
  `build/private/voice-audition/models/kokoro-82m/voices/`; target references
  in `build/private/voice-audition/ai-time-to-die/female-references/`
  (`provenance-female-all.json`); published as audio review items on
  situationshipin.space (media_kind `audio`, 64 k mono mp3s).

## Decasia-agesia distributed delivery (added 2026-10-02)

The **devp.ac** compute resources comprise rizal, ord, and eureka. Rizal
runs the shared dependency queue and completion notification; ord and
eureka execute delivery stages. See [devp.ac compute topology](workflows.md#devpac-compute-resources-2026-10-27)
and the [approved production plan](development/sessions/20261001.short_center_frame_2_screen_decay.md).

| Dependency | Observed version / location | Purpose |
| --- | --- | --- |
| Python 3 standard library | Existing host installations | Durable job queue, locks, SSH dispatch, checksums, and stage receipts. |
| FFmpeg / ffprobe | 8.1.2 on ord and eureka | FFV1/FLAC masters, clockwise rotation, retiming, HEVC encoding, probing, and decoded verification. |
| libx265 and zscale | Available in both host FFmpeg builds | Approved veryslow two-pass Main10 HEVC and sRGB-to-BT.709 conversion. |
| Pillow and NumPy | Existing worker installations | Imported by the existing production helpers. |
| fontTools | 4.62.1; isolated worker `.deps/fontTools` | Existing helper imports; copied from the approved local dependency, without modifying system packages. |
| OpenSSH and rsync | Existing host installations | Authenticated resumable worker-to-worker transfers over USB4; agent forwarded for the active transfer session. |
| systemd user services | rizal | Persistent coordinator and desktop completion watcher after the interactive turn ends. |
| notify-send / freedesktop notification service | rizal desktop | Requested completion or pipeline-failure alert. |

The verified eureka GPU allocation is 103,079,215,104 bytes (96 GiB), with
approximately 32 GiB host-visible RAM. The approved libx265 encoder is a CPU
workload; this delivery does not install or substitute a GPU encoder.

## Publication production (November 2026)

The [publication workflow](visual_workflow/tool_publication.md) adds optional
Python tools; these are not dependencies of Izzi's installed C++ headers.

| Component | Required for | Exercised version |
| --- | --- | --- |
| Python, Pillow/ImageCms, NumPy, ReportLab, pikepdf | Book layout, image/color conversion and structural checks | Python 3.14; Pillow 13.0.0; NumPy 2.4.6; ReportLab 4.5.1; pikepdf 10.17.0 |
| Poppler (`pdftoppm`, `pdftotext`) | Original PDF rasterization, page reviews, text extraction | 26.01.0 |
| Ghostscript (`gs`) and a CMYK ICC profile | PDF/X-3 conversion and complete-page decoding | 10.06.0; Artifex CMYK SWOP profile, hash in each manifest |
| Java and EPUBCheck | Required final EPUB validation | EPUBCheck 5.4.0 |
| Pycairo, PyGObject/librsvg and the existing review font environment | Only when the manifest contains SVG artwork | Shared `scripts/review_raster.py` adapter |
| Explicit TrueType fonts | Searchable print type and glyph fallback | Bundled Atkinson Hyperlegible example fonts; system DejaVu Sans fallback |

Install the optional Python packages in an environment that can also access
system `gi` and `cairo` when SVG rendering is needed:

```sh
python3 -m venv --system-site-packages build/publication-python
build/publication-python/bin/python -m pip install Pillow numpy reportlab pikepdf
```

Install Poppler, Ghostscript and Java through the operating system. Obtain the
[official EPUBCheck release](https://github.com/w3c/epubcheck/releases/tag/v5.4.0)
and pass its jar with `--epubcheck` or `EPUBCHECK_JAR`. The tools do not install
software, fetch artwork or contact a publisher. An existing isolated package
installation may instead be supplied through `PYTHONPATH`.

The default example ICC location is
`/usr/share/ghostscript/iccprofiles/default_cmyk.icc`; the manifest hashes the
actual file. Record a different profile as a new input/edition. For SVGs the
shared renderer requires installed Atkinson Hyperlegible Next Regular/Bold
OTFs, using its isolated fontconfig. The PDF/bitmap path does not need that
SVG font environment. See [manifest reference](publication.md) for output
limits, ink limiting, evidence and the independent PDF/X validator limitation.
