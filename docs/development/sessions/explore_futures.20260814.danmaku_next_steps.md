# Izzi futures — danmaku text-overlay generative pass (next steps)

Date: 2026-08-14

Status: `PROPOSAL; AWAITING-APPROVAL; ZERO-SPEND`

## Why now

`src/izzi-svg-text-overlay.h` (345 lines, `izzi::text_overlay`) is a pure,
deterministic string serializer for vector text overlays — `mode` enum
(`scroll` / `reverse` / `top` / `bottom`), `glyph`, `run` (id, source, mode,
lane, start, duration, delay, font_size, width, lane_y, fill,
fill_opacity, stroke, stroke_width, font/glyph provenance, bbox,
background, background_opacity, padding, radius, glyphs), and `document`
(width, height, area_percent, allow_overlap, seed, generator,
generator_version, runs).  It is already wired into the ai-time-to-die
vertical (`scripts/compose-vector-overlay.py`,
`scripts/render-time-to-die-draft*.py` for T2/T3 bullet text) but has no
generative-family documentation and no danmaku-style examples.

Requested scope (full-page discussion in
`explore_futures.20260814.planning_review_class.md`):

1. Add `docs/generative_patterns/danmu.md` describing
   `izzi-svg-text-overlay.h` and its relation to the W3C proposal.
2. Write 1–5 danmaku-style examples in `examples/text-danmu-[1-5].cc`,
   varying speed, size, and color per the YouTube reference (0:31–0:32).
3. Settle the standard term: **danmu**, **danmaku**, or **bullet-chat**.
4. Propose a plan and await human approval.

Drafting philosophy approved for this session: first draft cheap and
relaxed, constraints applied by iteration.

## Terminology recommendation

- **Canonical term: danmaku** — matches the W3C Bullet Chatting API
  Proposal and the existing `mode` enum in the overlay header.
- **Accepted alias: danmu** — used in the requested filenames
  (`danmu.md`, `text-danmu-[1-5].cc`).
- **Plain-English descriptor: bullet-chat / bullet chatting.**

The mapping is recorded in `danmu.md` so both names resolve to the same
pattern family.

## W3C relation (source of truth)

Proposal: https://w3c.github.io/danmaku/api.html (W3C Bullet Chatting
Community Group).

| W3C construct | Meaning | izzi mapping |
|---|---|---|
| `bulletchatlist` `area` (0–100) | % of display height reserved for bullets | `document.area_percent` |
| `bulletchatlist` `allowOverlap` | whether bullets may overlap | `document.allow_overlap` |
| `bulletchat` `mode` | scroll / top / bottom / reverse | `run.mode` |
| CSS `bulletchat-duration` | travel speed | `run.duration` |
| CSS `bulletchat-delay` | start offset; negative starts mid-lane | `run.delay` |
| CSS font / color / alpha / shadow | glyph styling | `run.font_size`, `fill`, `fill_opacity`, `stroke`, `stroke_width` |
| Events `bulletchatstart` / `bulletchatend` / `bulletchatcancel` | lifecycle | run `start` / `duration` / `delay` metadata in the manifest |

izzi implements the same lane/timing metadata in its manifest and adds two
things the W3C proposal leaves open: deterministic lane packing with a
recorded `seed`, and glyph/font provenance on every run.

## Deliverables

1. **`docs/generative_patterns/danmu.md`** — family doc in the house style
   of `moire.md`: Header → API surface → Config → Tests and examples →
   Usage → Assessment.  Includes the terminology mapping, the W3C relation
   table above, a note on determinism (seed + provenance), and the existing
   ai-time-to-die wiring.
2. **`examples/text-danmu-[1-5].cc`** — five examples modeled on
   `examples/text-overlay.cc` (CLI emitting overlay SVG + manifest JSON,
   with a byte-identical determinism self-test):
   1. scroll, mid (~46 px), medium (~10 s), white.
   2. scroll, dense lanes, small (~28 px), fast (~3.5 s), bright
      white/yellow.
   3. scroll, large (~72 px), slow (~13 s), saturated colors + stroke.
   4. reverse + top/bottom, mixed sizes, varied colors.
   5. layered composite with `area`/`allow_overlap` policy, deterministic
      seed and z-order.
3. **Build registration** — register all five in `tests/CMakeLists.txt`
   (same pattern as `examples/moire-plates.cc`, line ~398) so `make check`
   compiles and runs them.
4. **Index links** — add the family to `docs/generative_patterns.md` and
   `docs/generative_patterns/index.md`.
5. **Verification** — build and run the examples, `make check` passes,
   link-integrity scan stays at 0 broken links.

## Staging

1. Write `danmu.md` + index links.
2. Write the five examples + CMake registration.
3. `make check` and the link scan; fix any failures.
4. Report results and notify on completion; await explicit
   check-in/push/portal-publish authorization — none of those happen
   without it.

## Constraints

- Zero provider spend; local-only implementation.
- No writes outside the requested files (no changes to
  `proposed_vertical_v4.md`, portal content, or release state).
- Snapshot-dyad ledger remains dirty (SD-20260814-4c366cf9, local-only);
  the end marker and check-in happen when the user requests them.

## Next step

**Awaiting human approval.**  Reply "approve" to begin implementation
through the staging list above.
