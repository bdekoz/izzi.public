# Audio workflow — tool: Higgs TTS 2 (high compute)

First-pass sort for the high-compute TTS path.

## Where it runs

- Host: `eureka` (LAN, AMD Strix Halo gfx1151, unified memory) — full v2 model
  fits in its 96 GiB working set; the local Quadro RTX 4000 (8 GB) cannot.
- Runtime: uv Python 3.11 venv at `/home/bkoz/higgs-audio-v2/.venv` on eureka;
  torch `2.12.0a0+rocm7.13.0a…` (ROCm nightly gfx1151), transformers 5.15.

## Model and license

- `bosonai/higgs-tts-2-3b-base` (v2, 3.6B LLM + 2.2B DualFFN, bf16, 11 GB) +
  `bosonai/higgs-audio-v2-tokenizer` (769 MB). Boson Community License — user
  accepted the gate ("license pass", 2026-08-12).

## Usage notes

- Zero-shot cloning: reference text + reference audio in the chat template;
  greedy decoding produced the cleanest window-1 take.
- Generation script: `scripts/generate-hlt-frogtown-v2-higgs-audio.py`
  (supports per-window variants).
- Verification: local Whisper transcription before publication
  (faster-whisper small, CPU).
- Full investigation: `docs/development/seedance/audio-higgs.md`.

Status: `FIRST-PASS-SORT; NO-REMOVAL`
