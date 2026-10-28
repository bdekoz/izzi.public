# Audio and video to transcript

`scripts/transcribe-audio.py` is the
general-purpose local Whisper entry point. **Argument 1 is an audio or video
file path.** It prepares recognition audio, preserves raw hypotheses, and
delivers TXT, Markdown, JSON, SRT, and VTT with verified provenance. Supply
an explicit local whisper.cpp GGML model with `--model`.

This generalizes the preparation, hash-bound recognition, and validation
from `transcribe-nakamura.py`. The
Nakamura coordinator retains its recording-specific multi-host workflow;
the generic command runs locally and starts no downloads or remote jobs.

## Requirements and quick start

- Python 3.11+ on Linux; only its standard library is needed.
- FFmpeg and ffprobe on `PATH`.
- A whisper.cpp `whisper-cli` supporting full JSON, TXT, SRT, VTT, beam
  search, `--version`, and `-ng`/CPU selection.
- A local GGML model file. Models are read and hashed, never downloaded
  automatically.

The binary is discovered on `PATH`, then in the sibling checkout
`../whisper.cpp/build/bin/whisper-cli` relative to the repository location.
Override discovery with `--whisper-cli /path/to/whisper-cli`.

From the `izzi` repository root, use the included synthetic-speech fixture
and the model already available in this workspace:

```sh
python3 scripts/transcribe-audio.py examples/audio-transcription/fixture.wav \
  --model ../whisper.cpp/models/ggml-large-v3-turbo.bin \
  --language en \
  --output outputs/transcription/audio-example
```

Substitute your local model path on another machine. The
[examples](../../examples/audio-transcription/README.md) cover staged runs,
video input, channels, excerpts, retries, and verification. The fixture's
[spoken text](../../examples/audio-transcription/fixture-text.txt) is a
listening reference rather than a byte-exact ASR golden file.

## Input preparation

Any audio/video format FFmpeg can decode is accepted, including WAV, MP3,
FLAC, M4A, MP4, and MKV. `--audio-stream` selects a zero-based index among
audio streams; the default is 0. Media without the selected audio stream
is an error. The source file remains unchanged.

The selected audio is decoded once to **16 kHz mono PCM16 WAV**. `mix`
averages all channels with equal weights; `left`, `right`, or a zero-based
numeric channel selects one directly. Missing channels are errors. No
denoising, normalization, VAD filtering, translation, or speed change is
applied.

Select a video's second audio track and right channel:

```sh
python3 scripts/transcribe-audio.py /path/to/recording.mp4 \
  --model /path/to/ggml-large-v3.bin \
  --audio-stream 1 --channel right --language en \
  --output outputs/transcription/video-track-1-right
```

For stereo recordings with different speech per channel, retain independent
left/right hypotheses in separate output directories and compare by
listening. Channels are not verified speaker identities; the tool does not
merge channel transcripts or infer speaker names.

## Arguments

```text
python3 scripts/transcribe-audio.py MEDIA --model MODEL.bin [options]
```

| Argument | Default | Meaning |
| --- | --- | --- |
| `MEDIA` | Required | First positional argument: audio or video file path. |
| `--model PATH` | Required | Local whisper.cpp GGML model; its hash is recorded. |
| `--whisper-cli PATH` | Discovery above | Select an explicit executable. |
| `--output PATH` | `outputs/transcription/<sanitized-media-stem>` | Frozen production directory. |
| `--stage prepare\|transcribe\|verify\|all` | `all` | Prepare, recognize, or reverify. |
| `--language CODE` | `auto` | Runtime language code, such as `en`, or automatic detection. |
| `--channel NAME` | `mix` | Equal average, `left`, `right`, or zero-based channel number. |
| `--audio-stream N` | `0` | Zero-based audio-stream index. |
| `--start TIME` | `0` | Excerpt start: seconds, `MM:SS`, or `HH:MM:SS`, including fractional seconds. |
| `--duration TIME` | Remaining audio | Positive excerpt length in the same time syntax. |
| `--subtitle-clock source\|clip` | `source` | Original-source or excerpt-relative SRT/VTT timestamps. |
| `--beam-size N` | `5` | Beam size 1–8, matching the supported original workflow range. |
| `--threads N` | `8` | Positive recognition worker thread count. |
| `--device cpu\|auto` | `cpu` | CPU disables GPU; `auto` lets the runtime select its backend. |
| `--no-context` | Off | Disable prior text context for a separately retained retry. |
| `--prompt TEXT` | None | User-supplied spelling glossary, recorded verbatim. |

Recognition uses temperature 0, a 0.2 fallback increment, and best-of 5.
Full native JSON and decode logs retain available model, token, and runtime
diagnostics. `auto` is a backend request, not proof of GPU use. A non-CPU
run is marked with GPU evidence only when device allocation appears in the
decode log; otherwise its backend is unverified. CPU mode explicitly
disables GPU.

## Stages and reruns

`prepare` probes the source and freezes source/model/runtime/script hashes,
tool versions, settings, and exact commands. It writes the selected WAV and
its receipt without recognition:

```sh
python3 scripts/transcribe-audio.py examples/audio-transcription/fixture.wav \
  --model ../whisper.cpp/models/ggml-large-v3-turbo.bin --language en \
  --output outputs/transcription/audio-example --stage prepare
```

`transcribe` and `all` prepare if needed, recognize, assemble deliveries,
and verify their hashes and timestamps. Use the same source, model, output,
and settings:

```sh
python3 scripts/transcribe-audio.py examples/audio-transcription/fixture.wav \
  --model ../whisper.cpp/models/ggml-large-v3-turbo.bin --language en \
  --output outputs/transcription/audio-example --stage transcribe
```

`verify` checks an existing production without decoding or recognizing again:

```sh
python3 scripts/transcribe-audio.py examples/audio-transcription/fixture.wav \
  --model ../whisper.cpp/models/ggml-large-v3-turbo.bin --language en \
  --output outputs/transcription/audio-example --stage verify
```

Normal reruns reuse completed raw results/deliveries only when their hashes
and provenance match. Changes to input, model, runtime, script, tool
versions, channel, excerpt, or settings require a new output directory.
Missing/corrupt recorded artifacts cause an error instead of silent
replacement. A file lock permits one process per production directory.

Interrupted recognition restarts as a complete pass; partial JSON is not a
checkpoint. Previous partial results, failure records, and logs are archived
under `raw/failed-attempt-*/`. Raw hypotheses stay distinct from deliveries.
Changing model, beam, context, or prompt for a retry requires a separate
output directory.

## Excerpts and clocks

```sh
python3 scripts/transcribe-audio.py /path/to/recording.wav \
  --model /path/to/ggml-large-v3.bin --language en \
  --start 00:01:00 --duration 30 \
  --output outputs/transcription/minute-1-excerpt
```

Prepared audio and native Whisper outputs begin at zero. Delivered JSON
contains source-relative `start`/`end` and excerpt-relative
`clip_start`/`clip_end`. Markdown always uses the source clock. SRT/VTT
default to source times, so this excerpt's subtitles begin near 00:01:00
when paired with the original recording. Add `--subtitle-clock clip`, using
a new output directory, for subtitles aligned to the prepared excerpt WAV.
Plain TXT contains recognized segment text only.

Checks reject nonfinite, negative, reversed, nonmonotonic, or out-of-range
segment intervals. An end timestamp up to 250 ms past the actual WAV may be
clamped; the adjustment is recorded in delivered JSON while raw timestamps
remain untouched. Empty recognition is preserved as a no-text hypothesis
with `no_text_detected=true`, rather than proof that no speech was present.

## Outputs and review

```text
OUTPUT/
  production.json             # identities, source probe, settings and commands
  verification.json           # delivery/raw/audio hashes and machine checks
  audio/
    prepared.wav              # selected 16 kHz mono PCM16 audio
    receipt.json              # actual duration, sample count and hash
  raw/
    result.json               # native full JSON, including available tokens
    result.txt
    result.srt
    result.vtt                 # native clocks are excerpt-relative
    decode.log
    receipt.json              # language, elapsed time, backend evidence and hashes
    failed-attempt-*/          # only after failed/interrupted recognition
  deliveries/
    transcript.txt            # recognized text only
    transcript.md             # source timestamps and review status
    transcript.json           # both clocks, unknown speakers, timing adjustments
    transcript.srt
    transcript.vtt
  logs/
    prepare.log
```

Status is **`ASR_DRAFT_PENDING_LISTENING_REVIEW`**. Machine verification
checks identities and estimated timestamp structure, not transcription
accuracy. Speaker fields stay `null`/`UNVERIFIED`, and `human_verified=false`.
Check words, names, and speaker assignments against the recording before
treating them as authoritative. Token diagnostics stay in raw JSON; this
command does not claim aligned or human-verified word timestamps.

[Transcription enrichment](../transcription-enrichment_workflow.md) is a
later formatting step. Its converter expects reviewed speaker turns such
as `Speaker 1 MM:SS` / `Unknown Speaker MM:SS`, rather than this tool's plain
TXT. Review and structure speaker turns before making a screenplay.

## Tests and existing specialized workflow

```sh
python3 tests/audio-transcription.py
python3 tests/audio-transcription.py --integration
```

The integration test uses a clearly identified test decoder for actual
FFmpeg extraction, channels, clocks, subtitles, path quoting, reuse, tamper
rejection, failure recovery, and empty hypotheses without requiring model
weights. The speech fixture separately exercises real local Whisper.

The [Nakamura session](../development/sessions/20261005.transcription_nakamura.md)
retains its multi-model/channel pilot, remote scheduling, VAD evidence,
disagreement selection, and listening-review plan. Those specialized stages
remain separate from the general-purpose single-pass local command.
