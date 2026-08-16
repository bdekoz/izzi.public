# Audio workflow — ETL

First-pass sort of the extract/transform/load steps for Here Lies Trouble
audio.

## Extract

- Raw speech: `resources.rizal/here-lies-trouble/seed-audio-corpus/*.20260415.mp3`
  (16 kHz mono), hash-bound in manifests (e.g., episode-01 source
  SHA-256 `32e98463…`).
- Transcripts: `seed-audio-corpus/*.20260415.txt` — speaker-labeled turns with
  MM:SS timestamps; transcript quotations are kept out of published manifests.
- Ambience: `resources.rizal/here-lies-trouble/seed-audio-nature/` (river selected:
  SHA-256 `12027e87…`, offset 2 s, decode-once policy).

## Transform

- Scene cuts / VAD windows:
  `scripts/izzi-audio-excerpt-select.py` + candidates JSON under
  `outputs/ad-hoc/here-lies-trouble/scene-cuts/`; windowing method
  `LOW-ENERGY-BOUNDARY-WINDOWS-WITH-WEBRTC-VAD`, nominal 15 s.
- MeanVC2 transformation: `scripts/izzi-meanvc-audition-batch.py` and
  `scripts/izzi-meanvc-full-episode.py` (pinned engine under
  `build/private/voice-audition/`); outputs 16 kHz mono with peak ceiling −1 dBFS.
- Mix and normalize: `scripts/render-hlt-frogtown-v2-audio-canary.py` and
  `scripts/build-hlt-frogtown-v2-higgs-30s-review.py` (loudnorm two-pass +
  correction loop; see `docs/audio_workflow/defaults.md`).

## Load / record

- Manifests with SHA-256, durations, codecs, and loudness measurements under
  `outputs/…/episode-01-local-trial/audio/` and `…/full-length-scene/v2/higgs-audio/`.
- Failure loops and receipts under `docs/training-conversion/`.

Status: `FIRST-PASS-SORT; NO-REMOVAL`
