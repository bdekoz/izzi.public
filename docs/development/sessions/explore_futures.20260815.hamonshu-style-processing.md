# Izzi futures — hamonshu Pages style-processing review (proposal)

Date: 2026-08-15

Status: `APPROVED; IMPLEMENTED 2026-08-15; PUBLISHED`

## Objective

Put the content of the three Apple Pages documents in
`resources/hamonshu/` (`hamonshu_1`, `hamonshu_2`, `hamonshu_3`) onto
`situationshipin.space` as a `style-processing` review round, with an
invited human reviewer.

## Source inventory

| Source | Pages | PNG export (3479x5058) | PDF |
|---|---|---|---|
| `hamonshu_1` | 29 | `hamonshu_1.pages/hamonshu_1-001.png` … `-029` | `hamonshu_1.pdf` (6.5 MiB) |
| `hamonshu_2` | 28 | `hamonshu_2.pages/hamonshu_2-001.png` … `-028` | `hamonshu_2.pdf` (6.8 MiB) |
| `hamonshu_3` | 29 | `hamonshu_3.pages/hamonshu_3-001.png` … `-029` | `hamonshu_3.pdf` (7.6 MiB) |

86 page images total, roughly 4–7 MiB each as PNG (~394 MiB all together).
The PDFs are canonical source; the PNG exports are the page renders.

## Proposed approach

1. **Canonicalize.** Keep the PDFs and PNG exports as the source of record
   under `resources/hamonshu/`. Do not commit the Pages application files
   themselves.
2. **Derive compact review proxies.** Downscale each page to a bounded
   review JPEG (e.g. ~1600 px long edge, quality-tuned to stay well under
   1 MiB/page). 86 pages at a few hundred KiB each fits the portal's
   bounded build budget (currently 96 MiB total, 16 MiB per artifact).
   Full-resolution PNGs stay local and are linked from the review pages as
   the proofing source, mirroring the movement-family convention.
3. **Publish one catalog family.** `resource-hamonshu-20260815`, one
   `media_kind: index` aggregate per Pages document plus 86
   `image-reference` members, all `review_category: style-processing`,
   using the existing portal publisher pattern (neon-addict style:
   `review/izzi/<date>/resources/hamonshu/…`).
4. **Invited reviewer.** `coolart@avis.ne.jp` is the invited human reviewer
   for this round. The plan records the invitation; the review pages and
   catalog description name the invited reviewer so the round is clearly
   directed. The portal's reviewer label remains self-supplied on
   submission (that is by design), so the invitation lives in the plan and
   the published item descriptions rather than in a roster.
5. **Verify and hand off.** Run `check-review-site.mjs` locally, then hand
   the review link to the invited reviewer.

## Reviewer note (2026-08-15)

`coolart@avis.ne.jp` added as the invited reviewer for the hamonshu
style-processing round in this plan. If they also need GitHub collaborator
access to open issues/PRs against `bdekoz/situationshipin.space`, a GitHub
**username** (not just an email) is required — an email alone cannot be
invited through GitHub. Current collaborators: `bdekoz`, `stargirl-jpeg`.

**RESOLVED 2026-08-15:** the owner added the reviewer on the GitHub side, so
collaborator access to `bdekoz/situationshipin.space` is handled outside the
repo; the invite is pending their acceptance of the GitHub invitation.

## Open items

- **DONE** Approve the publish path and proxy sizing (2026-08-15).
- **DONE** Publish the family to `situationshipin.space`:
  `resource-hamonshu-20260815` — 86 image-reference members plus one index
  per document (`resource-hamonshu-20260815-<d>-index`), all
  `review_category: style-processing`, with `invited_reviewer:
  coolart@avis.ne.jp` recorded on every item. Portal commit `d493864`
  (izzi commit `6d1c8529ad33dfbb7199ff50bfd4b87b25b6103f`);
  `check-review-site.mjs` passes (355 items, payload bounded at
  93 139 092 bytes).
- Hand the review link to the invited reviewer (links below).
- **DONE** GitHub collaborator access — the owner added the reviewer on the
  GitHub side (2026-08-15); the review is no longer link-only once the
  invite is accepted.

## Review links (2026-08-15)

- Catalog: https://situationshipin.space/style.html
- Hamonshu 1 index: https://situationshipin.space/review/media/resource-hamonshu-20260815/hamonshu_1.index.html
- Hamonshu 2 index: https://situationshipin.space/review/media/resource-hamonshu-20260815/hamonshu_2.index.html
- Hamonshu 3 index: https://situationshipin.space/review/media/resource-hamonshu-20260815/hamonshu_3.index.html

## Publisher

`scripts/publish-hamonshu-style-index.mjs` in `bdekoz/situationshipin.space`
regenerates the family from the proxy directory and izzi source root:

```sh
node scripts/publish-hamonshu-style-index.mjs \
  --proxies <dir of hamonshu_<d>-<NNN>.jpg> \
  --source-root /home/bkoz/src/izzi/resources/hamonshu \
  --izzi-commit <40-hex>
```
