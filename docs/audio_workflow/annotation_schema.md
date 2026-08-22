# Response annotations and MeanVC2 timing

The motion track is written into the transcript as inline text annotations of
the form:

```text
RESPONSE_BEGIN [action] RESPONSE_END
```

Example actions: `snort`, `look at animal [gecko]`, `pause`,
`sniff a flower`, `watch a butterfly`. The normalized machine-readable form
is `docs/audio_workflow/annotation_schema.json`
(`izzi-response-annotation/1`); the validator is
`scripts/check-response-annotations.py`.

## Grammar

- `RESPONSE_BEGIN` opens an annotation; the next marker must be its matching
  `RESPONSE_END`. Nesting is not allowed and every marker must pair.
- The action between the markers is one of the bounded verbs `snort`,
  `pause`, `sniff a flower`, `watch a butterfly`, or `look at animal`.
- `look at animal` requires a bracketed name argument:
  `look at animal [name]`, where `name` is a lowercase token such as
  `gecko`. Other verbs take no argument.
- The verb vocabulary is bounded by default; `check-response-annotations.py
  --extra-verbs` accepts an extension allow list of one-verb-per-line
  entries, recorded in the normalized manifest's provenance.
- Each annotation resolves to the MM:SS timestamp of the transcript line it
  opens on (continuation lines inherit the last speaker timestamp), so the
  motion event stays glued to the speech that triggers it.

## VAD window alignment

- `check-response-annotations.py --vad` reads VAD windows in the shape
  `{"vad_windows": [{"index": 0, "begin_s": 0.0, "end_s": 15.0}, ...]}` and
  requires every annotation timestamp to fall inside exactly one window.
  The matched window index is written to the normalized manifest so the
  motion track can be re-aligned if scene boundaries shift.
- An annotation is a point in time: it carries `begin_s` and a null `end_s`
  by default. Durations are production parameters resolved at assembly from
  the action table, not re-derived from the transcript.

## MeanVC2 timing notes

- MeanVC2 conversion is duration-preserving: transformed output stays
  16 kHz mono at the source length, so annotation timestamps measured
  against the canonical transcript remain valid on the converted voice
  track without rescaling.
- The windowing method recorded in
  `outputs/ad-hoc/here-lies-trouble/scene-cuts/`
  (`LOW-ENERGY-BOUNDARY-WINDOWS-WITH-WEBRTC-VAD`, nominal 15 s) is the
  alignment reference. If new windows are cut, re-run the validator with
  the new window set before assembly.
- The motion track is timing metadata consumed at assembly: it never
  changes transcript text, voice content, or audio samples.

Status: `WORKFLOW-V2-W4; DRAFT-AWAITING-REVIEW`
