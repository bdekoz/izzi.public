# Audio-to-transcript examples

[`fixture.wav`](fixture.wav) is 5.227 seconds of synthetic English speech in
16 kHz mono PCM16. Its spoken source is [`fixture-text.txt`](fixture-text.txt):

> This is a transcription example. Every word comes from the recording.

The waveform was generated locally with eSpeak NG and FFmpeg.
[`create-fixture.py`](create-fixture.py) regenerates it using the installed
voice/build:

```sh
python3 examples/audio-transcription/create-fixture.py
```

Use the included WAV for repeatable input bytes. Regeneration can differ
between eSpeak versions; receipts bind the exact file/model used.
Punctuation and segmentation are recognition outputs, so the spoken text
is a listening reference rather than a byte-exact ASR golden file.

Run commands from the repository root with Python 3.11+, FFmpeg, ffprobe,
whisper.cpp `whisper-cli`, and a local GGML model. The first examples use
this workspace's existing sibling checkout/model; substitute your model
path elsewhere. See the [usage guide](../../docs/audio_workflow/tool_transcribe_audio.md)
for all options, binary discovery, clocks, and verification.

## Example 1: speech fixture

```sh
python3 scripts/transcribe-audio.py examples/audio-transcription/fixture.wav \
  --model ../whisper.cpp/models/ggml-large-v3-turbo.bin \
  --language en \
  --output outputs/transcription/audio-example
```

Argument 1 is the media path. Deliveries are `transcript.txt`, `.md`, `.json`,
`.srt`, and `.vtt` under `deliveries/`; native hypotheses and logs stay under
`raw/`. All text is an ASR draft with unknown speakers. Compare it to the
recording and source text before acceptance.

## Example 2: separate preparation, recognition, and verification

```sh
python3 scripts/transcribe-audio.py examples/audio-transcription/fixture.wav \
  --model ../whisper.cpp/models/ggml-large-v3-turbo.bin --language en \
  --output outputs/transcription/audio-staged-example --stage prepare

python3 scripts/transcribe-audio.py examples/audio-transcription/fixture.wav \
  --model ../whisper.cpp/models/ggml-large-v3-turbo.bin --language en \
  --output outputs/transcription/audio-staged-example --stage transcribe

python3 scripts/transcribe-audio.py examples/audio-transcription/fixture.wav \
  --model ../whisper.cpp/models/ggml-large-v3-turbo.bin --language en \
  --output outputs/transcription/audio-staged-example --stage verify
```

Preparation produces the selected WAV without ASR. Normal reruns reuse
matching completed deliveries; verification checks existing files and
estimated timestamps without running recognition again.

## Example 3: video right channel and excerpt

```sh
python3 scripts/transcribe-audio.py /path/to/recording.mp4 \
  --model /path/to/ggml-large-v3.bin --whisper-cli /path/to/whisper-cli \
  --language en --channel right --start 1:00 --duration 30 \
  --output outputs/transcription/video-right-excerpt
```

This selects 30 seconds from the first audio stream, starting one minute
into the video. JSON/Markdown and default subtitles retain the source clock.
Add `--subtitle-clock clip` with a new output directory for subtitles aligned
to the extracted WAV. Add `--audio-stream 1` to select a second audio track.
A missing track/channel is an error.

## Example 4: retain an independent retry hypothesis

```sh
python3 scripts/transcribe-audio.py /path/to/recording.wav \
  --model /path/to/ggml-large-v2.bin \
  --language en --channel left --beam-size 8 --no-context \
  --prompt 'Names: Nakamura, Whisper.' --start 1:00 --duration 30 \
  --output outputs/transcription/left-channel-v2-retry
```

The prompt is user-supplied spelling context, not proof those names were
spoken. Changing source, model, channel, excerpt, beam, context, or other
frozen settings requires a new output directory. Compare alternative
hypotheses against the audio, keeping listening corrections separate.

## Verification and actual Whisper run

```sh
python3 tests/audio-transcription.py --integration
```

Deterministic pipeline tests use a test decoder for actual FFmpeg channel/
excerpt extraction, both clocks, reruns, tamper rejection, failure recovery,
and no-text hypotheses. They are distinct from real speech recognition.

On October 13, 2026, the included WAV was recognized with local whisper.cpp
1.9.2, `ggml-large-v3-turbo.bin`, English, CPU, eight threads, and beam 5.
Both fixture sentences were recovered in two segments. Measured recognition
time was 13.386 seconds; this is a fixture measurement, not a general
throughput estimate. Results and receipts are retained under
`outputs/transcription/examples/20261013-general-audio-fixture/`.
