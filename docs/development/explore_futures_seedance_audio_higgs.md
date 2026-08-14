# Higgs Audio V2 investigation for the Here Lies Trouble 30s scene audio

Recorded: 2026-08-12 America/Los_Angeles
Status: `INVESTIGATION-COMPLETE; EUREKA-PROBE-COMPLETE;
30S-REVIEW-PACKAGE-PUBLISHED; ZERO-PROVIDER-SPEND; NOT-SHARED`

This document records the investigation requested in review issue
`bdekoz/situationshipin.space#6` (KEEP-PARTS): the 30-second Here Lies Trouble
Frogtown v2 scene motion passes, the cat pink-spike is a recorded negative
case, and the local audio mix is considered sub-par — Higgs Audio V2 was named
as the candidate to investigate for dialogue naturalness. The finding below is
that Higgs Audio V2 is real, capable, and a good technical fit for the dialogue
layer, but the full 3B-generation model does not fit this machine's 8 GB GPU;
the v2.5 lightweight (1B) sibling is the plausible local probe candidate, and
the hosted v3 API is the zero-GPU alternative that requires provider
authorization. Nothing has been downloaded, installed, or spent.

## Request and scope

- Review issue: https://github.com/bdekoz/situationshipin.space/issues/6
- Disposition: KEEP-PARTS. Motion passes. Cat pink-spike on body is a negative
  case (constraint recorded 2026-08-12, see
  `docs/training-conversion/failure-loops/TC-LOOP-HLT-FROGTOWN-V2-CAT-PINK-SPIKE-001.json`).
  Audio naturalness is sub-par and Higgs Audio V2 should be investigated.
- This investigation covers: what Higgs Audio V2 is, version/licensing
  landscape, fit for the existing scene audio, local hardware feasibility, and
  the bounded next-step options with their authorization gates.
- Not in scope: any model download, install, generation, provider transfer,
  spending, publication, or constraint update. Those remain gated.

## What Higgs Audio V2 is

Higgs Audio is a text-audio foundation model family from Boson AI
(`github.com/boson-ai/higgs-audio`, weights on Hugging Face under
`bosonai/`). The v2 generation checkpoint is approximately a 3.6B LLM plus a
2.2B audio "Dual FFN" adapter, pretrained on over 10 million hours of audio
with a large text corpus. Documented capabilities:

- expressive text-to-speech with emotion, style, and prosody control;
- zero-shot voice cloning from a short reference clip;
- multi-speaker dialog generation;
- speech plus background-music generation in a single pass;
- audio continuation / music and sound-effect generation (tokenizer and Dual
  FFN design).

Boson reports a 75.7% win rate over `gpt-4o-mini-tts` on emotion evaluations
and strong Seed-TTS Eval / ESD scores. Community deployment guidance (WebUI
projects and LocalAI issue #5905) consistently repeats the official
recommendation: a GPU with **at least 24 GB VRAM** for generation; CPU mode
exists but is described as much slower.

## Version landscape

| Version | Parameters | License | Local fit on this machine |
| --- | --- | --- | --- |
| v2 (3B base) | ~3.6B LLM + 2.2B audio adapter | Boson Community License (Boson AI USA) | No — 8 GB GPU is far below the 24 GB recommendation; CPU mode impractical for iterative 30s scenes |
| v2.5 | 1B (lightweight, production-oriented) | Boson v2 line community terms per model card | Plausible — 1B F16 can fit the 8 GB class; unverified on this box |
| v3 (TTS 4B) | 4B | Boson Higgs Audio v3 Research and Non-Commercial License; commercial/hosted/revenue use needs a separate commercial license | No — 4B and non-commercial-only terms for self-host |
| v3 hosted API (`api.boson.ai`) | — | API terms; free rate-limited public preview | Yes on hardware — but remote provider processing, so it needs provider authorization |

The current Boson repo headline release is v3; v2 and v2.5 remain available and
documented in `README_V2.md`. For the "Higgs Audio V2" the review named, the
local-probe candidate is the v2.5 1B model, which is part of the same v2
family and is the version small enough to be credible on this hardware.

## License status

The user accepted the license gate for this investigation ("license pass").
Recorded facts for the record:

- Higgs Audio v2 weights are distributed under the **Boson Community License
  Agreement** (Copyright Boson AI USA, Inc.), which includes additional
  commercial terms. The `higgs-tts-2-3b-base` Hugging Face repo historically
  carried an `apache-2.0` tag while shipping the Boson license file; the
  license file governs.
- Higgs Audio v3 self-host weights are research / non-commercial only;
  production, hosted, or revenue-generating use requires a separate Boson
  commercial license. The hosted API is a preview with its own terms.
- This project's use (internal creative experiment, review portal) is
  non-commercial, which is compatible with both the v2 community license and
  the v3 research terms, but any future public/commercial release would need
  the commercial license. This is noted, not resolved by "license pass".

## Fit for the existing 30s scene

Artifact being improved (measured locally):

- `outputs/review/feedback/visual/here-lies-trouble/neon-addict-stage-01/episode-01-local-trial/full-length-scene/v2/hlt-frogtown-v2-full-length-scene-30s.mkv`
- 30.125 s, 496x864 H.264 video + FLAC 48 kHz stereo audio; companion
  `hlt-frogtown-v2-full-length-scene-30s.audio.wav` is PCM s24le 48 kHz stereo.
- The audio is the first 30 s of the Frogtown v2 audio-canary mix: river
  ambience bed with 11 "engineering windows" of dialogue, sidechain-ducked,
  window-normalized. Voice codes are random engineering targets, not speaker
  IDs; no speaker diarization was performed.

How Higgs would slot in: replace (or augment) the dialogue layer. The existing
pipeline already has 18 local voice auditions and a Kokoro-based local TTS
path. Higgs v2/v2.5 zero-shot voice cloning could take the audition clips as
per-character reference voices and regenerate the 11 dialogue windows with
natural prosody, emotion, and multi-speaker turn-taking; the existing river
ambience bed, ducking, and −16 LUFS / −1.5 dBTP normalization could then be
reapplied unchanged. This directly targets the reviewer's "naturalness" note
without touching the accepted motion.

## Local feasibility assessment

Measured on this machine (2026-08-12):

- GPU: NVIDIA Quadro RTX 4000 (TU104GL), 8 GB GDDR6; driver 580.173.02 loaded
  and active (`GPU Excluded: No`), currently driving the display. No
  `nvidia-smi` binary in PATH; NVIDIA container toolkit (`nvidia-ctk`) not
  installed; `podman` present but no images.
- CPU/RAM: 62 GiB RAM (about 48 GiB available), plenty for CPU inference.
- Python: system default is 3.14.6 (too new for typical torch/Higgs stacks);
  a project-local Python 3.11.13 exists in the voice-audition runtime but has
  no torch installed.
- Disk: 204 GB free on /home (model fits easily); /tmp only 8.5 GB free.
- Network: shell access to GitHub/Hugging Face is currently blocked (DNS
  resolution fails), so any download requires an explicit network
  authorization step.

Assessment: the full v2 3B-generation model is not practical here — 8 GB VRAM
is a third of the recommendation and the model weight files alone exceed it at
F16. v2.5 (1B) is the only self-hosted model in the v2 family that is even
plausible on this GPU, and it must be verified (download + VRAM/runtime probe)
before promising anything. CPU inference for v2.5 is possible but expected to
be slow for iterative 30s scene work.

## Eureka rendering option

The user proposed the LAN host `eureka` as a rendering option because it has a
better video card. A read-only inventory over SSH (agent key, user `bkoz@`)
measured the following on 2026-08-12:

- Host: Fedora 43 Workstation, kernel 7.0.13, AMD Ryzen AI MAX+ 395 (Strix
  Halo), 32 cores.
- GPU: AMD Radeon 8060S integrated graphics (RDNA 3.5, gfx1151). No NVIDIA
  GPU and no `rocm-smi` — ROCm is not installed.
- Memory: 125 GiB unified RAM with 96 GiB available; 1.2 TiB free on /home;
  62 GiB free in /tmp.
- Software: Python 3.14.6 only, no torch, no conda/uv in PATH, `podman`
  present. Outbound internet works: Hugging Face, ROCm nightly index, PyPI,
  and GitHub all reachable (HTTP 200).

Why this matters for Higgs: the Radeon 8060S exposes the machine's unified
memory, so the practical working set for a model is the 96 GiB free RAM, not
an 8 GB VRAM cap. The full Higgs v2 (3B base, ~12-16 GB at F16) fits easily on
eureka, which the Quadro RTX 4000 cannot hold. Compute is roughly 2x the RTX
4000's FP32 class, and ROCm officially supports gfx1151 (Strix Halo) since
ROCm 6.4.4, with nightly PyTorch ROCm builds for gfx1151 published on
`rocm.nightlies.amd.com` and community reports of working torch.compile /
ComfyUI on this exact APU. Known caveats: community reports describe occasional
bf16 correctness bugs and memory-op hangs on gfx1151 (ROCm issue #6034,
amd-gfx list), and Higgs's audio-processing code (xcodec-derived) may carry
CUDA assumptions that need verification under ROCm's HIP shim. The 32-core CPU
with 96 GiB RAM is a serviceable fallback for a v2.5 probe.

Net assessment: eureka is the preferred self-hosted render host for the Higgs
audio work — it is the only local option that can realistically run the full
v2 model. This changes the earlier recommendation: probe the full v2 on
eureka, with v2.5 as the fallback if ROCm stability or install friction
blocks the 3B run. Installing ROCm, a Python 3.10/3.11 environment, and the
Higgs stack on eureka is a system-level change on that machine and remains an
explicit gate.

## Recommendation

Primary path (no provider spend, data stays local): a bounded local probe of
the full **Higgs Audio v2 (3B base)** on eureka — install ROCm for gfx1151,
create a Python 3.10 environment, download weights, and run one short dialogue
regeneration of a single engineering window to compare naturalness against the
current window. **Higgs Audio v2.5 (1B)** is the fallback if ROCm on gfx1151
proves unstable. The hosted v3 API remains the zero-setup alternative but
requires provider authorization and a NOT-SHARED review.

## Probe result (2026-08-12)

The authorized local probe ran end to end on eureka: ROCm-nightly PyTorch
(gfx1151), the full v2 weights (11 GB), and zero-shot regeneration of the
three dialogue windows visible in the 30-second scene (V03/V01/V02), fitted to
their engineering-window slots and rebuilt with the Frogtown v2 ambience mix
(−16 LUFS / −1.5 dBTP, verified). The review package is published at
https://situationshipin.space/review/hlt-episode-01-frogtown-v2-full-length-scene-30s-higgs-proxy/
(site commit `7614011`), with the original-mix proxy linked for comparison.
Details, hashes, and the authority states are in
`docs/training-conversion/receipts/WI-HLT-FROGTOWN-V2-HIGGS-AUDIO-30S-COMPLETION-2026-08-12.md`
and the completion dyad. The cat pink-spike constraint update from issue #6
was recorded on 2026-08-12 with the round-2 inputs reassessed; it is no longer
pending.

## Authorization gates for any next step

No next step is authorized yet. Each gate is separate:

1. `eureka_ssh_rendering_authorized: false` — read-only inventory was done;
   installing ROCm, Python, and the Higgs stack on eureka is a system-level
   change on that host and needs explicit authorization.
2. `higgs_model_download_authorized: false` — authorize downloading the v2
   (or v2.5) weights from Hugging Face on eureka.
3. `higgs_local_probe_authorized: false` — authorize installing the runtime
   deps and generating one bounded test utterance on eureka.
4. `provider_submission_authorized: false` — authorize any Boson API call or
   cloud-GPU run, including its cost ceiling and the data-transfer scope.
5. `audio_mix_regeneration_authorized: false` — authorize rebuilding the 30s
   scene audio with a Higgs dialogue layer and re-review.
6. `constraint_update_authorized: true` (updated 2026-08-12) — the cat
   pink-spike negative case is recorded as a constraint and the round-2 model
   inputs were reassessed; resubmission remains unauthorized.

Suggested approval phrase if the local probe is desired:
`AUTHORIZE HLT HIGGS V2 EUREKA LOCAL PROBE`
(or the same phrase plus a cost ceiling for a hosted path).

## Evidence

- Boson repo: https://github.com/boson-ai/higgs-audio (README, README_V2.md)
- v2 weights card: https://huggingface.co/bosonai/higgs-audio-v2-generation-3B-base
- v2.5 catalog entry (Azure AI): https://ai.azure.com/catalog/models/Higgs-Audio-v2.5
- v3 card: https://huggingface.co/bosonai/higgs-tts-3-4b
- Local measurements: GPU inventory from `/proc/driver/nvidia/gpus/0000:09:00.0`,
  `ffprobe` on the 30s mkv, audio-canary manifest
  (`.../audio/here-lies-trouble-episode-01-frogtown.v2.audio-canary.manifest.json`),
  review dyads `DYAD-2026-08-12-HLT-FULL-LENGTH-SCENE-REVIEW-{RECEIVED,END}-001.json`.
- Eureka inventory: read-only SSH (agent key) to `bkoz@eureka`
  (172.31.200.55): `lspci`, `free -h`, `df -h`, `cat /etc/os-release`,
  `uname -r`, and curl HEAD checks to Hugging Face / ROCm nightly / PyPI /
  GitHub; recorded in `DYAD-2026-08-12-HIGGS-AUDIO-V2-EUREKA-RENDERING-001.json`.
- ROCm gfx1151 support: AMD compatibility matrix (ROCm 6.4.4 adds gfx1150/
  gfx1151), TheRock nightly torch builds issue #3065, ROCm/ROCm issue #6034
  (Strix Halo stability notes), vLLM PR #38455 (gfx1151 device IDs).
- Report file: `docs/development/explore_futures_seedance_audio_higgs.md`
