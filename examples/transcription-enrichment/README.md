# Transcription enrichment example

Smallest working example for `scripts/transcribe-to-final-draft.py` and the
`izzi::transcription_enrichment` serializer.

```sh
python3 scripts/transcribe-to-final-draft.py \
  --transcript examples/transcription-enrichment/fixture-transcript.txt \
  --show "FIXTURE SHOW" \
  --slug "INT. KITCHEN -- DAY" \
  --outdir /tmp/transcription-enrichment-example
```

`fixture-transcript.txt` is the two-turn input. The golden outputs under
`golden/` show the expected `.fdx`, `.fountain`, and `.provenance.json` for
that input; regeneration must be byte-identical (except timestamps in the
provenance, which the example omits).
