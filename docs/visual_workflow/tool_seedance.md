# Visual workflow — tool: Seedance image-to-video

First-pass sort for the configured provider video path.

- Adapter scripts (private): `build/private/seedance2-submit-one.sh`,
  `build/private/seedance2-poll-one.sh`, `build/private/seedance2-download-one.sh`.
- Canary specs: `examples/seedance2/here-lies-trouble-episode-01-frogtown.v2.motion-canary-0{1,2,3}.json`.
- Delivery contract: image-to-video, 5 s (canaries) or segments (scenes),
  720p expected (delivered 496x864 in practice — recorded in failure loops
  `TC-LOOP-SEEDANCE-PRO-720P-RETURNED-496X864-001.json`), `generate_audio: false`.
- Cost gate: prior 5 s pro 1080p job 120 credits; 720p estimate 60 credits;
  hard-stop and quote verification before any submission; see
  `docs/development/explore_futures_here_lies_trouble_motion_provider_auth.20260812.md`.

Status: `FIRST-PASS-SORT; NO-REMOVAL`
