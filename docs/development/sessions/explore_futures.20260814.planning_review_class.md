# Izzi futures — plan vertical review class (proposal)

Date: 2026-08-14

Status: `PROPOSAL; AWAITING-APPROVAL; ZERO-SPEND`

## Why now

We walked a new vertical — **ai-time-to-die** — from proposal to complete
episodes in one continuous arc (`proposal_vertical_v1.md` through
`proposal_vertical_v3.md`).  The walkthrough exposed a structural gap: the
portal's review classes (`proofs`, `style-processing`, `location scouting`)
review artifacts, not plans.  The ai-time-to-die plan itself was reviewed
through the draft loop instead of being reviewable up front, which cost
several round trips.  A dedicated **plan vertical** review class makes the
production plan the first reviewable artifact of every new vertical.

## What the class covers

- **Artifact:** the complete production plan for a vertical — template,
  segment list, measured estimates, pilot options, gates with defaults, and
  orchestration (rizal/ord/eureka).  Reference shape:
  `docs/development/time-to-die-ai/proposal_vertical_v2.md` plus the
  validated special-topics PDF variant.
- **Review vocabulary:** the existing `KEEP` / `KEEP-PARTS` / `REVISE` /
  `REJECT` decisions, recorded via GitHub issue and mirrored to the
  local-object store.  A `KEEP` unlocks the draft loop for that vertical.
- **Portal surface:** one review page per plan vertical with the standard
  review form; the artifact is rendered as a document (linked PDF/markdown
  rather than a video/image player).

## Proposed catalog contract

| Field | Value |
|---|---|
| `media_kind` | `plan` (new; document render) |
| `review_category` | `planning` (new) |
| `generation_class` | `plan-vertical` |
| `review_scope` | `PLAN-VERTICAL-REVIEW` |
| `review_mode` | `output` |
| `source_path` | the plan markdown (and PDF, when generated) |
| `published_path` | the review artifact under `review/` |
| Required content | template, segments, estimates, pilot options, gates, orchestration |

The validator gains a `planning` contract: presence of the required content
in the source, bounded payload, and (when the PDF variant is supplied) the
existing devastation-pacific-house-style special-topics checks.

### Drafting philosophy — relaxed first, constraints later (2026-08-14)

The first plan draft must be cheap to produce and publish: a starter
markdown carrying the vertical idea is enough — no required sections, no
PDF, no house-style checks, and one low-ceremony publish command.  Formality
is added by iteration, not demanded up front.  Validation is graduated by
plan stage:

- `plan-draft` (exploratory): bounded payload, provenance, review page, and
  the standard decision vocabulary only.
- `plan-vertical` (structured, v2 shape): requires the plan content skeleton
  — template, segments, estimates, pilot options, gates.
- `plan-formal` (final, v3 shape): full required content including
  orchestration, plus the existing house-style special-topics checks when
  the PDF variant is supplied.

A `KEEP` at any stage advances the plan; a `KEEP` on the formal plan unlocks
the draft loop for that vertical.

## Staging

1. **Portal:** add `media_kind: plan` rendering (document link/embed + review
   form), `review_category: planning`, and the validator contract.  **Surface
   decision (2026-08-14):** the plan class gets a full catalog page
   (`plans.html`) mirroring `proofs.html` — same handoff/issue section and
   required IDs, `data-review-category="planning"` — rather than a minimal
   document-list page.  The plan publisher accepts a `--stage` flag and only
   enforces the content contract for the stage being published.
2. **izzi:** emit a plan-vertical package per new vertical (markdown summary
   + optional special-topics PDF), using the ai-time-to-die v2/v3 documents
   as the reference template.  A scaffold command turns a vertical name plus
   a one-line concept into the starter `plan-draft` markdown, so the first
   reviewable plan costs one command and a few sentences.
3. **Gate flow:** plan `KEEP` -> draft loop (existing classes) -> episode
   review (existing classes) -> baseline promotion.  A `REVISE` returns to
   the plan with findings, exactly like the draft loop but one stage earlier.
4. **Defaults:** a new vertical defaults to one bounded 57 s episode template
   unless the plan says otherwise; zero provider spend at plan stage.

## Evidence from the walkthrough

- Draft-9 KEEP (issue #28) and Phase 6 (v2 synthesis + special-topics PDF)
  show the plan artifact is producible and reviewable in house style.
- The measured render/voice/provider facts in `proposal_vertical_v2/v3.md`
  are the estimate inputs a plan review needs.
- The corpus bound (79.0 s -> 2-3 episodes) is the kind of finding a plan
  review surfaces before production starts.

## Next step

**IMPLEMENTED 2026-08-14** (approved; local and zero-spend; not pushed):

- **Portal (situationshipin.space):** full `plans.html` catalog page
  (`data-review-category="planning"`), `media_kind: plan` document rendering
  in `scripts/review-page.mjs`, planning category support in
  `assets/js/review.js`, approval-gated `scripts/publish-plan-proof.mjs`
  with `--stage draft|vertical|formal`, and validator + build-receipt
  contract (`.pdf`/`.md` allowlist, planning category counts, per-stage
  required content, formal-PDF house-style `PASS` requirement).
- **izzi:** `scripts/scaffold-plan-vertical.mjs` and `make
  plan-vertical-scaffold NAME=… CONCEPT=…` produce the one-command first
  draft.
- **First reviewed plans (portal catalog, local):**
  `plan-vertical-ai-time-to-die-v1` (PLAN-DRAFT) and
  `plan-vertical-ai-time-to-die-v2` (PLAN-FORMAL, with the special-topics
  PDF; special-topics and coda-share checks PASS in source and PDF modes).
- `check-review-site.mjs` passes (315 catalog items; planning: 2).

Next: human plan review — a `KEEP` on v1 advances the plan, a `KEEP` on v2
unlocks the ai-time-to-die draft loop; push/check-in still requires
authorization.

## Generation-group refresh — implementation note (2026-08-14)

The `izzi-generation-YYYYMMDD` family aggregates the make-check reference
images (26 `check-image` proofs from the guilloche/moire/surface-tension
generators) into one review group.

**Design decision:** a refresh produces a NEW date-based group
(`izzi-generation-<new date>`) rather than updating the existing group in
place. The previous group keeps its human review decisions and is marked
`SUPERSEDED`.

**`generation_state` is informative only.** Values are `CURRENT`, `STALE`,
and `SUPERSEDED`, shown as a card pill. `STALE` means izzi HEAD has moved
past the recorded `generation_commit` and a re-run is recommended; it does
not gate, hide, or block anything.

**Scoped implementation (2026-08-14):** the 26 initial items carry
`generation_state: CURRENT` with `generation_commit`
`d8cfd8b18fbee62e5a163ec40f085a47731992d7` (best-known izzi HEAD at
generation time, 2026-08-14 ~04:00 PDT); the portal card renders the state
pill and generation-commit provenance; the validator enforces the state
enum and 40-hex commit format.

**Deferred (full refresh pipeline):** an izzi make target that re-runs the
generators at current HEAD and writes a snapshot manifest, a
portal-side `publish-make-check-generation.mjs` that rebuilds the WebP
review copies and publishes the new group, STALE detection by comparing
izzi HEAD to `generation_commit`, and SUPERSEDED marking of the prior group.
