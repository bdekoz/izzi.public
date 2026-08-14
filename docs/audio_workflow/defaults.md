# Audio workflow — defaults and contracts

First-pass sort of the default values and contracts used across the Here Lies
Trouble audio pipeline.

## Delivery and window defaults

- Delivery mix: 48 kHz stereo, −16 LUFS integrated, −1.5 dBTP, LRA ≤ 12 LU.
- Engineering window normalization: −18 LUFS integrated, −1.5 dBTP,
  dual-mono power-preserving, 48 kHz mono per window.
- Ambience: target −32 LUFS integrated, sidechain duck 5 dB under dialogue
  (`sidechaincompress threshold=0.05:ratio=4:attack=20:release=400:makeup=1:mix=1`).
- Source corpus: `resources/here-lies-trouble/seed-audio-corpus/`
  (`here-lies-trouble-1/2.20260415.mp3` + transcripts); ambience-only sources
  in `resources/here-lies-trouble/seed-audio-nature/`.

## Voice codes

- `V01` = Kokoro `af_river`, `V02` = `am_echo`, `V03` = `bf_emma`.
- Codes are engineering targets, not speaker identities
  (`RANDOM-ENGINEERING-TARGET-NOT-SPEAKER-ID`); speaker diarization is
  NOT-PERFORMED in the current canary.
- Target references: `build/private/voice-audition/target-references/V0X.wav`
  with hashes in `build/private/voice-audition/target-references/provenance.json`.

## Determinism

- MeanVC2 audition seed material:
  `sha256(selection_id + ":" + code + ":20260809")`; full-episode assignment
  seed `41320260809`.
- Higgs TTS 2 seed convention: `41320260809 + window_index (+ variant offset)`;
  temperature 0.8 / top-p 0.9 sampling default; greedy used for the selected
  window-1 revision.

Primary sources: `examples/seedance2/here-lies-trouble-episode-01-frogtown.v2.json`
(`audio_mix`), `docs/audio_workflow/human_voice_selection.md`,
`docs/development/explore_futures_seedance_audio_test_plan.20260809.md`.

Status: `FIRST-PASS-SORT; NO-REMOVAL`
