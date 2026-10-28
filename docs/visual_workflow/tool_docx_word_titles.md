# DOCX word title videos

`render-docx-word-titles.py`
turns the words of a DOCX into centered white titles on solid black. **The
first positional argument is the DOCX file path.** Each word appears for
`--vsec` seconds (default **1.0**), followed by three seconds of black after each nonempty
paragraph. Use `--split-paragraphs` to make a separate video for each
paragraph. Both 4K portrait and 1080p landscape are produced by default.

The canonical script location is `izzi/scripts/render-docx-word-titles.py`;
the commands below run from the `izzi` repository root. Use
`--skip-first-paragraph` to omit a document title and `--review-reel` to
concatenate separate paragraph clips into one review reel per orientation.
The [layered-video workflow](#layer-the-words-over-a-background-video) below
also covers the completed Telemundo composite and its 1080p vertical
Instagram MP4 derivative.

## Requirements

- Python 3.11 or later and Pillow (`python3 -c 'import PIL'` checks Pillow).
- FFmpeg with the `libx264` encoder and ffprobe on `PATH`.
- An OTF/TTF font file. The local default is
  `/home/bkoz/.fonts/atkinson_hyperlegible.2026/AtkinsonHyperlegibleNext-Regular.otf`.
  On another machine, supply `--font /path/to/AtkinsonHyperlegibleNext-Regular.otf`.

The default face is **Atkinson Hyperlegible Next Regular**. The source session
called it “Atkinson Hypervisible New”; that spelling is interpreted as the
installed Next family. An unavailable font is an error, with no fallback.
The default size is **72 points at 96 DPI = 96 pixels** in both orientations.
The frame size does not change that conversion. Override `--dpi` or
`--font-points` explicitly when a different pixel size is desired.

## Quick start: separate paragraph videos

Run these commands from the repository root. A small, portable input is
included at [`examples/docx-word-titles/fixture.docx`](../../examples/docx-word-titles/fixture.docx).
It contains two nonempty paragraphs and one empty separator paragraph.

Plan and inspect previews:

```sh
python3 scripts/render-docx-word-titles.py examples/docx-word-titles/fixture.docx \
  --split-paragraphs --stage plan \
  --output outputs/docx-word-titles-example/paragraphs
```

Render and fully verify the same production:

```sh
python3 scripts/render-docx-word-titles.py examples/docx-word-titles/fixture.docx \
  --split-paragraphs \
  --output outputs/docx-word-titles-example/paragraphs
```

This example makes **four MKVs**: paragraph 001 is eight seconds / 240 frames
in each orientation; paragraph 002 is five seconds / 150 frames. The final
three seconds of every clip are black. Open the generated `index.md` for
links to all clips. The [example README](../../examples/docx-word-titles/README.md)
also shows whole-document and landscape-only runs.

## Options and production stages

```text
python3 scripts/render-docx-word-titles.py DOCUMENT.docx [options]
```

| Option | Default | Effect |
| --- | --- | --- |
| `--split-paragraphs` | Off | Separate clips under `paragraphs/001/`, `paragraphs/002/`, etc. |
| `--vsec NUMBER` | `1.0` | Floating-point seconds per word, rounded to the nearest frame at 30 fps. |
| `--skip-first-paragraph` | Off | Omit the first nonempty paragraph; retain original paragraph numbering. |
| `--review-reel` | Off | With `--split-paragraphs`, also stream-copy the clips into one reel per selected orientation. |
| `--output PATH` | `outputs/text-document-to-video/<sanitized-DOCX-stem>` | Production directory, independent of the input directory. |
| `--orientation both\|portrait\|landscape` | `both` | Choose the output formats. |
| `--font PATH` | Local Atkinson Next Regular OTF | Select an explicit font file. |
| `--font-points NUMBER` | `72` | Point size before conversion to pixels. |
| `--dpi NUMBER` | `96` | Pixels per inch for point conversion. |
| `--stage plan\|render\|verify\|all` | `all` | Select planning, encoding, or reverification. |
| `--threads NUMBER` | `4` | Encoder and full-decode worker thread count. |

`plan` writes a frozen manifest and native-size first-word/widest-word PNG
previews; it does not encode. `render` and `all` prepare, encode, and verify
each delivery. `verify` checks existing files without encoding. For a split
production, it checks every paragraph in both selected orientations:

```sh
python3 scripts/render-docx-word-titles.py examples/docx-word-titles/fixture.docx \
  --split-paragraphs --stage verify \
  --output outputs/docx-word-titles-example/paragraphs
```

Use the same source, output, font settings, and format selection across the
stages. A repeated `all`/`render` run reuses a completed file only if its
SHA-256 and successful verification receipt match the frozen production.
Changing source contents, word duration, title skipping, review reel selection,
font, orientation, split mode, or the renderer
script requires a new output directory. There is no overwrite flag.
An interrupted encode leaves a `.partial.mkv`; rerunning the same command
restarts that encode and retains already verified deliveries. An existing
final MKV with no valid receipt is an error; choose a new output directory.
Run one process per production directory.

## Half-second contents and review reels

The titled fixture adds a heading before the two example content paragraphs.
Skip that heading and render every content word for 0.5 seconds:

```sh
python3 scripts/render-docx-word-titles.py examples/docx-word-titles/fixture-with-title.docx \
  --split-paragraphs --vsec 0.5 --skip-first-paragraph --review-reel \
  --output outputs/docx-word-titles-example/half-second-body
```

The output retains original paragraph numbers **002 and 003**. Paragraph
002 is 5.5 seconds / 165 frames; paragraph 003 is four seconds / 120 frames.
The two review reels are 9.5 seconds / 285 frames each. Reels concatenate
clips in source order without re-encoding or removing paragraph pauses.
Every paragraph and reel receives the same full metadata/frame-count/decode
verification. `--review-reel` requires `--split-paragraphs`; it also works
with a single selected orientation. `--stage verify` checks the reels as well
as the clips, and a normal rerun reuses matching verified files.

## Layer the words over a background video

After rendering a portrait title reel, use it as the top layer over a
background video of the same size and frame rate. The completed Shotwell
production uses **2160 × 3840 / 30 fps** inputs, both beginning at zero:

- Top: the half-second content reel, paragraphs 002–010, now named
  `outputs/text-document-to-video/20261012-shotwell-paragraphs-v2-half-second/deliveries/review-reel-v.mkv`.
- Bottom:
  `outputs/telemundo-ota/20261011/detail-v3-notices/deliveries/telemundo-ota-detail-v3-4k-vertical-receiver-notices.mkv`.

Use **screen blending in RGB**, rather than applying the screen formula
directly to YUV planes. For 8-bit components the formula is
`out = 255 - (255 - top) * (255 - bottom) / 255`. The grayscale title reel's
black pixels reveal the background, white pixels produce white words, and
antialiased edges retain their coverage. During each paragraph pause the
background continues with no added word.

The essential FFmpeg filter graph is:

```text
[0:v]setpts=PTS-STARTPTS,format=gbrp[top];
[1:v]setpts=PTS-STARTPTS,format=gbrp[bottom];
[top][bottom]blend=all_mode=screen:shortest=1:repeatlast=0,
scale=in_range=full:out_range=tv:out_color_matrix=bt709,format=yuv420p,
setparams=range=limited:color_primaries=bt709:color_trc=bt709:colorspace=bt709,
setsar=1[out]
```

This production ends with the **133-second / 3,990-frame** title reel.
The 189-second background contributes its first 133 seconds; neither input
is looped or retimed. Receiver notices from the final Telemundo pass occur
after 180 seconds and are outside this composite's duration. The output is
a silent 4K portrait H.264 MKV with the same CRF-14 / 60M–120M delivery
settings as the original portrait profile.

Reproduce the completed composite from its frozen manifest:

```sh
python3 outputs/text-document-to-video/20261012-shotwell-over-telemundo-v1/render.py
```

The composite production
contains actual input paths/hashes, probes, timing policy, and the complete
encoding command. The verification receipt
records exact frame counts, full decoding, output identity, and the layer
proof. These production runners are separate from the DOCX renderer;
`--vsec` and paragraph-selection flags apply when creating the source reel.

## Additional 1080p vertical Instagram export

Downscale the verified 4K composite uniformly by half with Lanczos to
**1080 × 1920**, preserving 9:16 framing, centered text, the complete
**2:13 / 3,990-frame** timeline, and silence. The export contains the full
composite without cropping or changing word timing. This is a portrait
derivative; the DOCX renderer's `--orientation landscape` option produces
a separate 1920 × 1080 horizontal title video.

| Setting | Instagram derivative |
| --- | --- |
| Container / encoder | MP4 / `libx264` |
| Dimensions / aspect / rate | 1080 × 1920 / 9:16, square pixels / 30 fps |
| Profile / level / scan | High / 4.0 / progressive |
| Preset / CRF | `medium` / 18 |
| VBV maxrate / buffer | 12M / 24M |
| GOP / B frames | Closed 60-frame GOP / none |
| Pixel format / color | 8-bit `yuv420p` / BT.709 limited range |
| Audio | No audio track, preserving the silent source |
| MP4 packaging | `+faststart`; `moov` before `mdat`; no edit lists |

The MP4, H.264, progressive 4:2:0 video, closed GOP, and front-loaded
metadata choices follow [Meta's Reels video requirements](https://github.com/fbsamples/reels_publishing_apis/blob/main/insta_reels_publishing_api_sample/README.md).
The verified local file is **180,948,478 bytes (180.9 MB)**. Its output frame
count, timing, dimensions, codec/profile, color tags, MP4 atom order,
absence of edit lists, and full decoding all passed verification.

Reproduce the frozen Instagram export:

```sh
python3 outputs/text-document-to-video/20261012-shotwell-over-telemundo-v1/derivatives/instagram-1080p-v1/render.py
```

The export production
records the source identity, scale, output settings, and exact FFmpeg
command. Its verification receipt
and native-size preview
are saved beside it. Both frozen runners reuse existing deliveries only
when the input, output, and production hashes match their records.

## Current Shotwell deliveries

All four reels/exports below are silent, 30 fps, **2:13 / 3,990 frames**.
The paragraph title is omitted, and each content word is held for 0.5 seconds.

| Output | Dimensions | File |
| --- | --- | --- |
| White words on black, vertical reel | 2160 × 3840 | MKV |
| White words on black, horizontal reel | 1920 × 1080 | MKV |
| Words over Telemundo, 4K vertical master | 2160 × 3840 | MKV |
| Words over Telemundo, Instagram portrait export | 1080 × 1920 | MP4 |

The paragraph index
links all 18 individual content clips. The
[session record](../development/sessions/20261012.text_document_to_video_one_sec_titles.md)
preserves prior revisions, layer proofs, acceptance, and delivery receipts.
The source reels were manually renamed to `review-reel-v.mkv` and
`review-reel-h.mkv`; a fresh DOCX render still produces the standard
`review-reel-portrait.mkv` and `review-reel-landscape.mkv` names.

## Text and timing rules

The extractor reads DOCX `word/document.xml` with no office application.
It includes body paragraphs, table paragraphs, hyperlink display text,
inserted revisions, and displayed field text in document order. Formatting
runs are joined before whitespace tokenization, so a word split across
multiple runs stays one word. Punctuation and Unicode text are retained;
tabs and manual line breaks separate words within their paragraph.

Empty paragraphs are skipped. Title paragraphs and short standalone body
paragraphs are included by default. `--skip-first-paragraph` explicitly
omits the first nonempty paragraph without inferring its style or title;
the omitted paragraph is recorded in `production.json`. Original nonempty
paragraph numbers are retained, so skipping 001 makes the first output 002.
Headers, footers, automatic list numbers, deleted
revisions, and field instructions are excluded. Images, equations, symbolic
runs, and footnote/endnote references cause an explicit error because this
renderer cannot represent them faithfully. It does not preserve DOCX style,
color, bold, or italic formatting; all words use the selected font.

At 30 fps, each word occupies a half-open interval of `word_frames` frames,
and each paragraph pause occupies 90 frames. `--vsec 1.0` gives 30 frames
per word; `--vsec 0.5` gives 15. Other positive finite values are rounded to
the nearest whole frame (half frames round up). Values that round to zero
frames are rejected. The manifest records requested `vsec`, actual
`word_seconds`, and `word_frames`; durations remain floating point rather
than being truncated to whole seconds. A paragraph with `N` words has
`N * word_seconds + 3` seconds / `N * word_frames + 90` frames.
In split mode, each clip starts at
frame zero and ends with its own black pause. In whole-document mode, the
same paragraphs and pauses form one continuous timeline.

Titles are upright in both formats and centered by visible ink bounds,
including descenders and side bearings. The renderer rejects words that
would clip; it does not shrink or wrap them.

## Video settings

The delivery profile follows the [October 11 production settings](../development/sessions/20261011.telemundo_ota_glitch.md#assembly-encoding-and-reproduction).

| Setting | Portrait | Landscape |
| --- | --- | --- |
| Dimensions | 2160 × 3840 | 1920 × 1080 |
| H.264 profile / level | High / 5.1 | High / 4.0 |
| VBV maxrate / buffer | 60M / 120M | 18M / 36M |

Both use Matroska MKV, `libx264`, `medium`, CRF 14, 8-bit `yuv420p`, 30 fps,
square pixels, a 60-frame closed GOP, and no B frames or audio. RGB titles are
converted to limited-range BT.709, with BT.709 matrix, primaries, and transfer
tags. Landscape text is rendered directly at landscape resolution, rather
than rotating or scaling the portrait's lettering.

## Files and verification

A split production has this layout:

```text
OUTPUT/
  production.json                  # source/font/script hashes and full timeline
  verification.json                # all deliveries, local durations and hashes
  index.md                         # links to the completed paragraph videos
  deliveries/                      # optional review reels and their receipts
    review-reel-portrait.mkv
    review-reel-landscape.mkv
  assets/                          # optional review reel concat lists
  logs/                            # optional reel assembly/decode logs
  paragraphs/
    001/
      production.json              # local paragraph timeline, beginning at zero
      verification.json
      previews/                    # first and widest word at each native size
      assets/portrait/             # unique word PNGs and FFmpeg concat timeline
      assets/landscape/
      logs/                        # encode and full-decode logs
      deliveries/
        word-titles-portrait.mkv
        word-titles-portrait.verification.json
        word-titles-landscape.mkv
        word-titles-landscape.verification.json
    002/
      ...
```

In whole-document mode, `previews/`, `assets/`, `logs/`, and `deliveries/`
live directly under `OUTPUT/`, with the same filenames. The split index is
written when all selected clips pass verification. Plan-only runs do not
create a delivery index.

With `--skip-first-paragraph`, the directories begin at `002/`. With
`--review-reel`, the root verification receipt adds a `review_reels` list
alongside the paragraph `deliveries`, and the index links the reels first.

Verification checks the container, single video stream, codec, profile/level,
dimensions, frame rate, exact decoded frame count, duration, square pixels,
pixel format, B-frame setting, and all color metadata with ffprobe. Every
delivery is also decoded completely with FFmpeg error checking. Receipts
record file hashes and probe results; `READY_FOR_HUMAN_REVIEW` means these
technical checks passed. Typography and target-device playback remain visual
review tasks.

Run the focused tests, or add the full-size encode/decode integration test:

```sh
python3 tests/docx-word-titles.py
python3 tests/docx-word-titles.py --integration
```

The integration test covers both output sizes, every word/pause boundary,
separate paragraph timelines, fractional word durations, skipped titles,
retained paragraph numbering, paths containing quotes and shell characters,
and hash-verified reruns. Native-size decoded frame hashes verify that the
stream-copied review reels preserve each source clip's pixels.
