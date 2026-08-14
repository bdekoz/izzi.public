# Audio workflow — analyze and expand the voice reference bank

First-pass sort for the expanded voice-reference bank (Higgs zero-shot
cloning input).

## What exists

- Original 9-clip audition matrix:
  `outputs/ad-hoc/here-lies-trouble/audio-auditions/audition/`
  (`HLT-EPISODE-01/05/09--V01/V02/V03.wav`), manifest with hashes alongside.
- Expanded 18-clip bank (option 2, 2026-08-12):
  `outputs/ad-hoc/here-lies-trouble/voice-reference-bank/audition/`
  (`HLT-REF-E1-01..03--V0X.wav`, `HLT-REF-E2-01..03--V0X.wav`), manifest
  with hashes; reference turn texts kept private in
  `build/private/voice-audition/reference-bank/ref-texts.json`.

## How to expand

1. Select clean single-speaker transcript turns (both episodes).
2. Cut + edge-trim: `scripts/build-hlt-voice-reference-bank.py`
   (validates ≥ 75% speech fraction).
3. Transform to each target voice with the pinned MeanVC2 batch pipeline
   (`scripts/izzi-meanvc-audition-batch.py` + reference-bank spec).
4. Use as Higgs zero-shot reference pairs (reference text + audio).

## Lesson learned (r2)

Window 1 garble was diagnosed with local Whisper (faster-whisper small, CPU on
eureka): the original V03 audition reference (multi-turn excerpt) produced
hallucinated speech. A single-speaker corpus turn transformed to V03 and
greedy decoding produced a clean take (W1-V03-F). See
`docs/training-conversion/receipts/WI-HLT-FROGTOWN-V2-HIGGS-AUDIO-30S-REVISION-R2-2026-08-12.md`.

Status: `FIRST-PASS-SORT; NO-REMOVAL`
