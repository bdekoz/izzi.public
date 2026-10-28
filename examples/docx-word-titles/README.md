# DOCX word title examples

[`fixture.docx`](fixture.docx) is a small valid DOCX containing:

1. `Every word gets one second.` — five words.
2. An empty separator paragraph, skipped by the renderer.
3. `Then black.` — two words.

The first paragraph deliberately splits `second.` across two formatting
runs. The extractor rejoins those runs before finding words.
[`fixture-with-title.docx`](fixture-with-title.docx) adds a first paragraph,
`Word title demo`, to the same content for the title-skipping example below.
[`create-fixture.py`](create-fixture.py) regenerates both DOCX files deterministically
using only Python's standard library:

```sh
python3 examples/docx-word-titles/create-fixture.py
```

Run all following commands from the repository root. They require Python
3.11+, Pillow, FFmpeg/libx264, ffprobe, and the default local Atkinson Next
Regular font. Add `--font /path/to/font.otf` consistently to every stage on
other machines. See the [full usage guide](../../docs/visual_workflow/tool_docx_word_titles.md)
for typography, extraction, encoding, and provenance details.

## Example 1: separate videos for each paragraph

```sh
python3 scripts/render-docx-word-titles.py examples/docx-word-titles/fixture.docx \
  --split-paragraphs \
  --output outputs/docx-word-titles-example/paragraphs
```

| Paragraph | Words | Duration | Frames per video | Outputs |
| --- | --- | --- | --- | --- |
| 001 | 5 | 8 seconds | 240 | 4K portrait and 1080p landscape |
| 002 | 2 | 5 seconds | 150 | 4K portrait and 1080p landscape |

The four clips are linked from the generated `index.md`. Each clip ends
with three seconds of black.

## Example 2: whole-document videos

```sh
python3 scripts/render-docx-word-titles.py examples/docx-word-titles/fixture.docx \
  --output outputs/docx-word-titles-example/document
```

This creates `deliveries/word-titles-portrait.mkv` and
`deliveries/word-titles-landscape.mkv`: **13 seconds / 390 frames each**,
including the pause between paragraphs and the final pause.

## Example 3: plan and render landscape only

```sh
python3 scripts/render-docx-word-titles.py examples/docx-word-titles/fixture.docx \
  --split-paragraphs --orientation landscape --stage plan \
  --output outputs/docx-word-titles-example/landscape

python3 scripts/render-docx-word-titles.py examples/docx-word-titles/fixture.docx \
  --split-paragraphs --orientation landscape \
  --output outputs/docx-word-titles-example/landscape
```

The first command writes manifests and native-size PNG previews without
encoding. The second makes two landscape clips and fully verifies them.
Rerunning the second command reuses matching verified files. To check the
existing clips again without rendering:

```sh
python3 scripts/render-docx-word-titles.py examples/docx-word-titles/fixture.docx \
  --split-paragraphs --orientation landscape --stage verify \
  --output outputs/docx-word-titles-example/landscape
```

Use a new output directory if the DOCX, font settings, orientation, split
mode, or renderer script changes.

## Example 4: half-second words, omit title, and concatenate review reels

```sh
python3 scripts/render-docx-word-titles.py examples/docx-word-titles/fixture-with-title.docx \
  --split-paragraphs --vsec 0.5 --skip-first-paragraph --review-reel \
  --output outputs/docx-word-titles-example/half-second-body
```

`--vsec` is a floating-point duration in seconds; the default is `1.0`.
At 30 fps, `0.5` holds each word for exactly 15 frames. The first nonempty
paragraph is omitted, and the remaining paragraphs retain their original
numbers **002 and 003**. This makes four paragraph MKVs and two stream-copied
review reels:

| Content | Duration | Frames in each orientation |
| --- | --- | --- |
| Paragraph 002 — five words + final pause | 5.5 seconds | 165 |
| Paragraph 003 — two words + final pause | 4 seconds | 120 |
| Review reel — 002 followed by 003 | 9.5 seconds | 285 |

The generated `index.md` links both reels and every paragraph clip.
Reels live in `deliveries/review-reel-portrait.mkv` and
`deliveries/review-reel-landscape.mkv`; each retains the three-second pause
at the end of every paragraph, including the final one. Reverify all six
files by repeating the command with `--stage verify`. Use a different
output directory for a subsequent timing revision.

## Example 5: reproduce the completed layered 4K production

The completed Shotwell production layers the half-second portrait reel over
the Telemundo OTA master using RGB screen blending. Black title pixels reveal
the footage and white words remain white; the background continues during
paragraph pauses. This example uses the existing production inputs and its
frozen manifest, rather than the small DOCX fixture:

```sh
python3 outputs/text-document-to-video/20261012-shotwell-over-telemundo-v1/render.py
```

The output is a 2160 × 3840 composite MKV,
**2:13 / 3,990 frames / 30 fps**, with no audio. It uses the first 133
seconds of the background, without looping or retiming. The
[layering guide](../../docs/visual_workflow/tool_docx_word_titles.md#layer-the-words-over-a-background-video)
explains the filter graph and links the complete command, source identities,
and verification receipts.

## Example 6: reproduce the 1080p vertical Instagram MP4

```sh
python3 outputs/text-document-to-video/20261012-shotwell-over-telemundo-v1/derivatives/instagram-1080p-v1/render.py
```

This uniformly downscales the verified 4K composite with Lanczos and creates
the 1080 × 1920 Instagram MP4.
It preserves the complete **2:13 / 3,990-frame** timeline, 9:16 framing,
word timing, and silence without cropping. H.264 High / level 4.0,
`yuv420p`, 30 fps, CRF 18, 12M/24M VBV, and `+faststart` are recorded in
the export manifest. Verification checks exact frame count, full decoding,
front-loaded MP4 metadata, and absence of edit lists. The completed file is
**180.9 MB**.

These two production runners check source hashes and reuse a completed
matching export; they do not take the DOCX renderer's CLI arguments.
See the [Instagram export guide](../../docs/visual_workflow/tool_docx_word_titles.md#additional-1080p-vertical-instagram-export)
for settings and receipts. The original 4K master and title-only reels are
preserved alongside the derivative.
