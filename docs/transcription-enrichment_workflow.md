# Transcription enrichment

Post-transcription formatting layer: takes the canonical
here-lies-trouble transcript and emits a Final Draft 13 screenplay (FDX +
rendered PDF) and a production-fit assessment against the May 2026 network
needs document.

Status: stage-1 plan approved 2026-08-20
([`20260820.transcribe_enrichment_stage_1.md`](development/sessions/20260820.transcribe_enrichment_stage_1.md)).

## Principles

- **Format-only enrichment.** Dialogue text is verbatim from the transcript;
  no words are invented. Speaker labels become `SPEAKER 1`-`6` and
  `UNKNOWN SPEAKER` cues; no character names are invented.
- **Model roles.** sol-5.6 reads page images and returns per-page format
  rules over the Batch API (cheapest available tier). The orchestrator
  aggregates rules and applies every repository edit locally with
  `apply_patch`; the model never emits diffs (izzi issue #1).
- **Deterministic output.** The izzi serializer and the Python transform
  emit byte-stable output from the same input, native C++ and WASM alike.

## Pipeline

See [`workflow.md`](development/transcription-enrichment/workflow.md) for the
mermaid diagrams and the FDX subset contract.

## E1 — transcript to Final Draft 13

```sh
python3 scripts/izzi-build-format-extraction-batch.py \
  --pages-dir build/private/transcription-enrichment/g2/pages \
  --output build/private/transcription-enrichment/g2/requests.jsonl \
  --tier standard

python3 scripts/izzi-openai-batch-client.py submit \
  --jsonl build/private/transcription-enrichment/g2/requests.jsonl \
  --outdir build/private/transcription-enrichment/g2 \
  --metadata transcription-enrichment-g2 --cap-usd 25

python3 scripts/transcribe-to-final-draft.py \
  --transcript review/media/audio-transcript/here-lies-trouble-canonical-source-transcript.20260818.txt \
  --show "HERE LIES TROUBLE" \
  --slug "INT. MANUELA -- LOS ANGELES" \
  --outdir outputs/transcription-enrichment/here-lies-trouble \
  --rules build/private/transcription-enrichment/g2/format-rules.json
```

Outputs: `.fdx`, `.fountain`, `.pdf`, `.pages/*.png` page proofs, and
`.provenance.json` (turn -> speaker/timing mapping and every mechanical
cleanup). The `.fdx` (interpolated, rendered) and `.fountain` (open
screenplay markup) are published side-by-side for the human reviewers to
choose the preferred style.

## E2 — production fit

```sh
python3 scripts/build-production-fit.py \
  --needs-text /tmp/network-needs.txt \
  --outdir resources.static/production-fit
```

The data form is `resources.static/production-fit/2026-05-production-needs.json`
(schema `izzi-production-needs-2`); the fit document compares the E1
screenplay's observable attributes against each house's `green` (wants) and
`red` (rejects) lists.

## Examples

- Input fixture and golden outputs:
  `examples/transcription-enrichment/` (fixture transcript, expected FDX,
  Fountain, and provenance).
- Native serializer test: `tests/transcription-enrichment.cc`
  (`ctest -R transcription-enrichment`).

## Authority

Spend requires the stage-1 approval and stays inside the `$25` hard cap.
No credentials are written to artifacts; batch receipts record only ids,
counts, and cost estimates.
