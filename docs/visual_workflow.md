# Visual workflow

Parent workflow: [Workflows](workflows.md).

First-pass index for the Here Lies Trouble visual workflow, approved under
`docs/visual_workflow/reorganization_proposal.md`. The directory pages stay
pointer-only; the Supercut usage guide was moved here from
`docs/workflows.md` in the 2026-08-22 documentation pass.

## Supercut usage

`scripts/izzi-supercut.py` derives deterministic seed/review cuts from a
source file and records their provenance. Times are accepted as seconds,
`MM:SS`, or `HH:MM:SS`.

Single cut:

```bash
python3 scripts/izzi-supercut.py \
  --input resources.rizal/blade-runner/source.mkv \
  --begin 1:02:03.5 --length 30 --cut-number 4 \
  --tiers orig,1080p,proxy \
  --output-root outputs/ad-hoc/supercut
```

Batch (see `schemas/izzi.supercut-1.schema.json` for the `cuts` contract):

```bash
python3 scripts/izzi-supercut.py --batch cuts.json \
  --output-root outputs/ad-hoc/supercut --resume
```

Options: `--accurate` seeks after the input for frame-accurate re-encoded
tiers; `--fps` sets an output frame-rate target; `--dry-run` prints the
ffmpeg commands without running them; `--resume` skips outputs whose
recorded SHA-256 already matches.

Outputs land under the output root in `ssk.orig` (stream copy),
`ssk.1080p` (1920×1080 x264 qp18 + FLAC), and `ssk.proxy` (640×360 x264
crf23 + AAC), with the historical `-cut-N.mkv`, `-1080p.1.mkv`, and
`-360p.2.mkv` suffixes. Each run writes `manifest.json` (input hash, filter
graphs, codecs, geometry, per-output hashes) and `SHA256SUMS`.

The same contract is available in C++ through
`src/izzi-media-supercut.h` (`izzi::media::supercut`). Verify the toolchain
with `make supercut-check`, which runs the header contract test and a lavfi
smoke derivation.

## Document map

| File | Bucket |
| --- | --- |
| `docs/supercut.md` | Supercut usage: deterministic seed/review cuts with recorded provenance |
| `docs/visual_workflow/index.md` | Directory entry page mirroring this index |
| `docs/visual_workflow/method.md` | Visual method passes and experiments method |
| `docs/visual_workflow/style_packs.md` | Style packs, style frames, aesthetic proofs |
| `docs/visual_workflow/motion_grammar.md` | Motion seeds, canary grammar, camera tracks |
| `docs/visual_workflow/camera_tracking_routes.md` | Route waypoints and derived camera tracks |
| `docs/visual_workflow/tool_seedance.md` | Provider image-to-video adapter and delivery contract |
| `docs/visual_workflow/tool_guilloche.md` | Guilloche v4 renderer and title cards |
| `docs/visual_workflow/etl.md` | Keyframes, filmstrips, proxies, hashing, episode assembly |
| `docs/visual_workflow/proofs.md` | Review surface pattern (filmstrip + proxy pages) |
| `docs/visual_workflow/visual_workflow_experiments.md` | First-pass unclassified material |

Status: `FIRST-PASS-SORT; NO-REMOVAL; NOT-SHARED`
