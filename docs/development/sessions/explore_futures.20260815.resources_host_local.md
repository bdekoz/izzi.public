# Resources host-local split — proposal

Recorded: 2026-08-15 America/Los_Angeles
Status: `PROPOSAL; ZERO-SPEND; NO-CHANGES-APPLIED`

Requested direction: make the izzi `resources/` directory host-local
(`resources.<hostname>`, e.g. `resources.rizal`), identify what would have to
change and what must stay in `resources/`, and compare a `resources.static`
rename against the cartofreako `assets.static` / `assets.generated` model
(with an `assets.<hostname>` variant).

## 1. Current state

`resources/` is a 3.1 GB mixed tree with 44 subdirectories. 332 files are
tracked in Git; the heavy media is untracked (most video/audio is already
ignored by extension; several scan corpora are untracked and **not** ignored,
which pollutes `git status` and risks accidental commits).

Current `.gitignore` rules for resources:

```gitignore
/resources/.private/
/resources/**/*.mkv
/resources/**/*.mp4
/resources/**/*.wav
/resources/**/*.mp3
/resources/**/*.flac
/resources/**/*.pdf
/resources/**/*.xlsx
/resources/duotone-111/*.asr-draft.*
/resources/duotone-111/*.asr-partial-vad.*
```

## 2. Content classification

### 2.1 Static — stays in Git (candidate `resources.static/`)

| Subdirectory | Files | Role |
|---|---|---|
| `neon-addict/` | 146 tracked | Style-processing reference corpus (portal `neon-addict` collection) |
| `noir-vibezz/` | 58 tracked | Style reference frames; `Makefile SEED_IMAGE_STAGE0_SPEC_ROOT` + `scripts/izzi-seed-image-stage0.py` default |
| `tokyo-psychedelic/` | 30 tracked | Style reference collection |
| `typeFONTBAT/` | 30 tracked | Fonts consumed by `render-guilloche-v4-static.py`, title-card renderers, `compose-vector-overlay.py` |
| `mmrl/` | 19 tracked | Graph data + kusama SVG; `tests/CMakeLists.txt` fixtures |
| `here-lies-trouble/` | 14 tracked | Transcripts/seeds (audio corpus itself is host-local) |
| `time-to-die-ai/` | 14 tracked | Danmaku text, prompts, seeds (video/audio corpus is host-local) |
| `duotone-111/` | 12 tracked | Style frames (audio + ASR drafts are host-local) |
| `blade-runner/` | 6 tracked | Cut lists, motion seeds (clip media is host-local) |
| `izzi-review-*.json` | 4 | Review-catalog dumps (portal build provenance) |
| `moire-index.tar.xz`, `edge-tension-please-01.tar.xz` | 2 | Reference archives |

### 2.2 Host-local — moves out of Git (candidate `resources.<hostname>/`, e.g. `resources.rizal/`)

| Subdirectory | Size | Content |
|---|---:|---|
| `blade-runner/` clips | 1.1 GB | `*.mkv` cuts (already ignored by extension) |
| `time-to-die-ai/` video/audio | 804 MB | `seed-video-corpus/*.mkv`, `seed-audio-ambient/*.wav` (ignored) |
| `hamonshu/` | 412 MB | `hamonshu_{1,2,3}.pdf` + 86 PNG scans — **untracked, not ignored** |
| `duotone-111/` audio | 248 MB | `*.wav`, `*.asr-*` (ignored) |
| `asama-loops/` | 181 MB | 11 `*.mkv` camera clips (ignored) |
| `here-lies-trouble/` audio | 86 MB | `seed-audio-nature/*`, `seed-audio-corpus/*.mp3` (ignored) |
| `astronomia-playing-cards.1829/` | 59 MB | PDF + JPG scans — **untracked, not ignored** |
| `mmrl/` media | small | `*.mp4`/`*.mkv` inside `mmrl/` (ignored) |
| `guilloche/`, `devastation-pacific/` | <1 MB | Reference PDFs (ignored) |

The untracked-not-ignored problem children today are **`hamonshu/`** (412 MB)
and **`astronomia-playing-cards.1829/`** (59 MB): they appear as `??` in every
status and could be committed by accident.

## 3. Target layout

```text
resources.static/            # committed: everything tracked today (2.1)
  neon-addict/  noir-vibezz/  tokyo-psychedelic/  typeFONTBAT/
  mmrl/  here-lies-trouble/  time-to-die-ai/  duotone-111/  blade-runner/
  izzi-review-*.json  *.tar.xz
resources.rizal/             # host-local, gitignored: heavy corpora (2.2)
  blade-runner/  time-to-die-ai/  hamonshu/  asama-loops/  duotone-111/
  here-lies-trouble/  astronautia-playing-cards.1829/  mmrl/  guilloche/
```

`resources.<hostname>` naming future-proofs multi-host work: today only
`rizal` (control plane) holds the corpus; `eureka`/`ord` never need izzi's
media. A resolver lets code prefer the host-local copy and fall back to
`resources.static/`:

```text
$RESOURCES_ROOT (env override)
  -> resources.$(hostname)/  (when present)
  -> resources.static/
```

## 4. What would have to change

1. **Git**
   - `git mv resources resources.static` for the 332 tracked files.
   - New ignore rules: `/resources.rizal/` (or `/resources.*/` with
     `!/resources.static/`), keep the extension rules scoped to
     `resources.static/`, and keep `/resources.static/.private/`.
   - No clone-weight change (heavy media was never committed), but `git
     status` becomes clean and accidental commits become impossible.
2. **Makefile** — `SEED_IMAGE_STAGE0_SPEC_ROOT ?= resources/noir-vibezz` →
   resolved root (static path or `$RESOURCES_ROOT`).
3. **Scripts with `resources/` defaults/paths**
   - `scripts/izzi-seed-image-stage0.py` (`resources/noir-vibezz`)
   - `scripts/render-guilloche-v4-static.py`,
     `scripts/render-guilloche-v4-title-cards.py`,
     `scripts/render-hlt-title-card-madredeus.py` (`resources/typeFONTBAT`)
   - `scripts/compose-vector-overlay.py` (`resources/time-to-die-ai/...`)
   - `scripts/build-hlt-voice-reference-bank.py`
     (`resources/here-lies-trouble/seed-audio-corpus/...`)
   - `scripts/check-hlt-frogtown-v2-canaries.py`,
     `scripts/proof-local-outputs.py`, `scripts/extract-movement.py`
     (blade-runner/asama-loops clips)
   - `scripts/generate-hlt-frogtown-v2-higgs-audio.py` and the
     render-time-to-die-draft*.py family (seed video/audio)
   - Hamonshu style-processing publish flow (`resources/hamonshu` →
     situationshipin.space), plus the seed-image/style-processing
     publish paths for the committed corpora
4. **Tests** — `tests/CMakeLists.txt` (3 `resources/mmrl` references) and any
   test fixtures/baselines that embed `resources/` paths; determinism tests
   that render from host-local clips need a documented
   `resources.<hostname>` prerequisite (skip-or-fail contract).
5. **Docs** — ~70 files reference `resources/`; bulk path updates plus the
   new layout section in `docs/dependencies.md`.
6. **CI/checks** — `scripts/check-tracked-secrets.py` scans paths; verify it
   handles the new layout; add a guard that `resources.*/` (host-local) is
   never tracked.

## 5. What stays in `resources/` (i.e. `resources.static/`)

Everything that is committed today (section 2.1): the three style reference
corpora, fonts, graph data, seeds, transcripts, review dumps, and the two
reference archives. Nothing in 2.1 depends on a hostname.

## 6. Cartofreako comparison

| Concept | cartofreako today | izzi proposal |
|---|---|---|
| Committed source/reference assets | `assets.static/` (heavy downloads kept out via per-domain `.raw/`/`.prepared/` ignores) | `resources.static/` (rename of `resources/`) |
| Build outputs | `assets.generated/` (fully ignored, archived as `assets.generated.v*.tar.xz`) | `outputs/` (already gitignored) — no change needed |
| Host-local working media | implicit inside `assets.static/*/.raw` | explicit `resources.<hostname>/` (e.g. `resources.rizal/`) |

The cartofreako model keeps host-local downloads **inside** `assets.static/`
as ignored `.raw` dirs; the proposed `resources.<hostname>` variant is a
cleaner separation because the host-local tree is not a subdirectory of the
committed tree, `git status` cannot show it at all, and future hosts
(eureka/ord) get their own sidecar without touching the committed tree.
Naming: `resources.static` + `resources.<hostname>` matches the user's
proposal and izzi's existing vocabulary; `assets.static` +
`assets.<hostname>` is the equivalent if izzi adopts cartofreako's `assets`
prefix. Either pair works; the important invariant is
**static = committed, hostname = ignored sidecar**.

## 7. Recommendation

1. Rename `resources/` → `resources.static/` (`git mv`; keeps 332 tracked
   files and all history).
2. Create `resources.rizal/` (gitignored) and move the section 2.2 corpora
   there, preserving relative subpaths so scripts only change their root.
3. Introduce a small resolver (`$RESOURCES_ROOT` →
   `resources.$(hostname)` → `resources.static`) and update the ~10 code
   consumers + `tests/CMakeLists.txt`; bulk-update docs last.
4. Tighten `.gitignore`: `/resources.rizal/`, scope extension ignores to
   `resources.static/`, and add a check that `resources.*/` is never tracked.
5. Add a `resources-check` make target that fails if any host-local dir
   contains tracked files or if required host-local media is missing
   (documented skip list for CI).

This is a proposal only; no renames, moves, or ignore changes were applied.

## Evidence

- `du -sh resources/*`, `git ls-files resources`, per-subdir tracked counts,
  and `.gitignore` rules (2026-08-15).
- Consumers: `Makefile:37`, `tests/CMakeLists.txt:1618-1666`, and the scripts
  listed in section 4 (defaults verified by grep).
- cartofreako model: `README.md:33-34` (`assets.static/`,
  `assets.generated/`), `.gitignore:73-97` (`.raw`/`.prepared` ignores,
  `/assets.generated/`, archives).
