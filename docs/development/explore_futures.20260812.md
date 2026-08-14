---
title: "Izzi futures: house-style web review for motion and audio"
date: "2026-08-12"
status: "PROPOSED"
observation_cutoff: "2026-08-11T16:11:15-07:00"
implementation_style: "house-style"
individual_review_page_style: "randoma11y-accent"
accessibility_target: "WCAG 2.2 AA; verification pending"
---

# Izzi futures: house-style web review for motion and audio

## Decision summary

Make the entire `situationshipin.space` review portal explicitly
`house-style`. Give each separate motion/audio review page one explicit
canonical style, normally `randoma11y-accent`, while retaining neutral reading
fields and the house typography, grid, evidence hierarchy, and interaction
rules.

The exact canonical names are `house-style` and `randoma11y-accent`.
`house-default` and `randomally-accent` are not persisted aliases. A page or
manifest must select one exact style; it must not merge the two names into one
style value or infer a default.

Use GitHub Pages for the lean interface and artifact-specific review pages.
For the first web-motion canary, use GitHub Release assets for the canonical
MKV and a browser-compatible review proxy. Keep Cloudflare R2 as a scale-up
option and an authenticated studio origin as a private-review option. Do not
put large source media directly in the Pages repository.

Every review handoff in Codex chat should point to one exact, verified page:

```text
https://situationshipin.space/review/<review_id>/
```

A handoff is not ready until that URL returns the intended page and its
manifest resolves to the expected source and derivative hashes.

This file is an implementation proposal, not a fully validated Devastation
Pacific report and not a WCAG conformance claim. The current design reference
is Devastation Pacific Summer 2026 version 3.9 at local commit
`0e9ce57d7f86044f88618141026e9372096163ba`. The separately pinned Izzi
`training-conversion` contract remains unchanged; this proposal does not
rewrite its historical or normative baseline.

## Observed current state

The current `situationshipin.space` checkout was clean at commit
`cb8b555662c5ed591a0fe8c9fbe2c55b4b4efbc7`. Its existing checker passed on
2026-08-11 with:

- 250 unique catalog items;
- 13,118,192 bytes of bounded public preview payload;
- no remote script or stylesheet dependency;
- hash-bound artifacts, source-image or source-MKV lineage, and ten-frame
  manifests;
- local draft storage, JSON export/import, public-issue preparation, and
  per-frame feedback; and
- an explicit rule that source MKV, MP4, WAV, MP3, and MOV files do not enter
  the current Pages tree.

The current interface already has useful accessibility foundations:

- one H1 and semantic sections;
- a skip link;
- visible `<label>` elements around the filter controls;
- native HTML `<select>` elements;
- a visible `:focus-visible` outline;
- controls approximately 46 CSS pixels high at the default root size;
- live status regions;
- native checkboxes and radio buttons; and
- no autoplay.

The current visual system is not Devastation Pacific house style. It uses a
dark field, radial and linear gradients, neon pink/cyan/yellow signals,
dashboard-like artifact cards, several translucent or mixed colors, and the
original Atkinson family plus generic monospace fallbacks. It also has no
artifact-specific route, browser video player, or exact deep-link contract.

The current `izzi` color source exposes these relevant literal values:

| Izzi token | Value | Existing comment |
|---|---:|---|
| `wcag_lgray` | `#949494` | 3:1 large-text target on white |
| `wcag_gray` | `#767676` | 4.5:1 normal-text target on white |
| `wcag_dgray` | `#2e2e2e` | 13.6:1 target on white |

These are color constants, not a complete WCAG 2.2 web-component system.
Their original comments assume pure white. Independent calculation against
the house paper `#fcfbf7` produces approximately 2.93:1, 4.39:1, and 13.11:1,
respectively. Therefore `wcag_lgray` and `wcag_gray` must not be copied into
normal house-paper form text merely because their enum names contain `wcag`.

The following observed house-palette pairings are suitable starting points,
subject to testing in the final rendered state:

| Foreground / background | Ratio |
|---|---:|
| House ink `#14171a` / paper `#fcfbf7` | 17.38:1 |
| House muted `#4d565d` / paper `#fcfbf7` | 7.23:1 |
| House Pacific blue `#173a55` / paper `#fcfbf7` | 11.44:1 |
| House rule `#9da8af` / paper `#fcfbf7` | 2.34:1 |
| Izzi `wcag_dgray` `#2e2e2e` / paper `#fcfbf7` | 13.11:1 |

The structural house rule is suitable for nonessential separators, but its
2.34:1 pairing is too weak for an essential control boundary that needs 3:1
non-text contrast. Use house muted, house ink, Pacific blue, or another
independently validated color for form boundaries and focus indicators.

## Proposed review architecture

### Option 1 — GitHub Pages plus GitHub Releases

**Recommendation: start here.**

Keep HTML, CSS, JavaScript, manifests, poster images, filmstrips, waveforms,
and small bounded proof images in `situationshipin.space`. Create one release
per review batch and attach:

- the canonical MKV;
- one H.264/AAC MP4 review proxy;
- optionally one VP9/Opus WebM proxy;
- captions or an authorized transcript derivative when available;
- the source/proxy manifest and checksums; and
- no material lacking publication authority.

The artifact page binds release URLs to exact hashes. The source MKV remains
the canonical production artifact; the MP4 or WebM is a separately identified
review derivative. The page may stream a proxy and offer the source MKV as an
explicit download.

GitHub documents a limit of less than 2 GiB per release asset, up to 1,000
assets per release, and no total release-size or bandwidth quota. GitHub Pages
has a one-gigabyte published-site limit and a soft bandwidth limit, and GitHub
itself recommends Releases or another host when a site outgrows Pages. See
[About releases](https://docs.github.com/en/repositories/releasing-projects-on-github/about-releases)
and [GitHub Pages limits](https://docs.github.com/en/pages/getting-started-with-github-pages/github-pages-limits).

Before adopting this route, deploy one short canary and verify seeking,
byte-range behavior, MIME type, mobile playback, and cross-origin behavior.
Release-hosted browser playback is a test requirement, not an assumption.

### Option 2 — GitHub Pages plus Cloudflare R2

Use this when the review corpus grows beyond a convenient Release workflow or
when predictable streaming behavior becomes more important than minimizing
infrastructure.

Store source MKVs and browser proxies under content-addressed object keys.
Serve them through a custom media domain, enable caching, and apply a narrowly
scoped CORS policy for the Pages origin. Keep the review interface and GitHub
issue handoff on `situationshipin.space`.

Cloudflare documents public delivery through a custom domain, optional access
controls, and browser CORS for public or presigned objects. See
[R2 public buckets](https://developers.cloudflare.com/r2/buckets/public-buckets/)
and [R2 CORS](https://developers.cloudflare.com/r2/buckets/cors/).

This option adds an account, storage configuration, cost boundary, privacy
decision, and upload workflow. No R2 upload is authorized by this proposal.

### Option 3 — authenticated studio media origin

For unreleased voices, private collaborators, or material that should remain
locally retained, serve content-addressed source and proxy files from `rizal`
through a separately reviewed authenticated tunnel or private network. The
public site may carry a non-sensitive review shell, but must not expose a
private origin URL, credential, or token.

This retains local control but depends on studio uptime and collaborator
onboarding. It should use the same `review_id`, hashes, review-page structure,
and JSON/GitHub feedback contract so that changing storage does not change the
human workflow.

## Browser media contract

Matroska is the production container; it should not be the only browser
playback source. MDN recommends WebM for modern web use and demonstrates
offering WebM and MP4 alternatives. See
[MDN media container formats](https://developer.mozilla.org/en-US/docs/Web/Media/Guides/Formats/Containers).

Each motion/audio review package should contain:

| Role | Required record |
|---|---|
| Canonical source | MKV path or release URL, SHA-256, bytes, duration, dimensions, frame rate, video codec, audio codec, and publication state |
| Primary review proxy | MP4 path or URL, SHA-256, bytes, dimensions, duration, H.264 profile, AAC channels/rate, and source-MKV SHA-256 |
| Optional open proxy | WebM path or URL, SHA-256, VP9/AV1 and Opus details, and source-MKV SHA-256 |
| Fast visual preview | Poster plus ten evenly spaced frames, each with ordinal, timestamp, dimensions, SHA-256, and source-MKV SHA-256 |
| Audio orientation | Waveform and measured loudness summary; neither substitutes for listening |
| Language access | Authorized captions, transcript, or explicit `UNAVAILABLE` state and reason |
| Human review | Whole-clip decision, motion notes, audio notes, frame-level decisions, and review identifier |

Proxy generation must preserve duration and frame geometry within declared
tolerances. It must not imply byte identity, mastering acceptance, or exact
audio fidelity. Review of the proxy is review of the proxy unless the page and
human decision explicitly extend the disposition to the canonical source.

Do not autoplay. Start on the poster/filmstrip state. For glitch, strobe, or
rapid interference work, add a plain-language motion/flashing warning and a
static-review path. Respect `prefers-reduced-motion` in interface movement
without changing or censoring the canonical artwork. A human may explicitly
play the full work after the warning. Any WCAG claim for flashing material
requires a separate three-flashes/safety evaluation; visual intensity must not
be silently removed from the art to make the interface easier to certify.

## Exact review pages and chat handoff

Generate a real static directory for each review item:

```text
review/<review_id>/index.html
review/<review_id>/manifest.json
```

The `review_id` should combine a readable stable slug and a short source hash:

```text
hlt-episode-01-frogtown-v2-audio-canary-65919a746572
```

The resulting canonical link is:

```text
https://situationshipin.space/review/hlt-episode-01-frogtown-v2-audio-canary-65919a746572/
```

That example is a proposed identity, not a currently published URL.

Frame-specific links append stable anchors:

```text
https://situationshipin.space/review/<review_id>/#frame-01
...
https://situationshipin.space/review/<review_id>/#frame-10
```

Each generated page reuses shared site CSS and JavaScript. It does not copy a
large application bundle. Its manifest must include:

```json
{
  "schema_version": "izzi-web-review-item-1",
  "review_id": "hlt-episode-01-frogtown-v2-audio-canary-65919a746572",
  "artifact_id": "here-lies-trouble-episode-01-frogtown.v2.audio-canary",
  "style": "randoma11y-accent",
  "source_mkv": {
    "sha256": "65919a746572475fb7b96621cbc858887da5ac9248a47f474a960ec08e000b61",
    "publication_state": "NOT-PUBLISHED"
  },
  "review_proxy": {
    "status": "PROPOSED",
    "sha256": null
  },
  "human_review": {
    "status": "PENDING"
  }
}
```

The generator must reject a route collision, hash mismatch, missing artifact,
unvalidated style, or publication state that does not authorize the selected
media.

When an exact page has been deployed and verified, the Codex handoff becomes:

```markdown
Review HLT Episode 1 — Frogtown v2 motion and audio:
[Open the exact review page](https://situationshipin.space/review/<review_id>/)
```

Do not send only `https://situationshipin.space/`. Do not announce review
readiness before an HTTP check and manifest/hash check pass. If deployment is
unavailable, provide the local artifact path and say the web page is
`UNAVAILABLE`; do not invent the URL state.

## House-style site system

### Devastation Pacific visual cue corpus

Site graphics should take their visual cues from the two current files in
`resources/devastation-pacific/`:

| Source | SHA-256 | Observed visual cues |
|---|---|---|
| `de-kosnikfront-door-sash-color.pdf` | `448836cb838b5359d5d718956b155e28ba44c8b76e12f81d05f6cc6ad9c641b0` | very tall sash, large white field, pale cyan and fluorescent-yellow edge rails, fine orbit/rosette clusters accumulating at the margins, and isolated central emblems |
| `house-of-dk-v9.2-tape.pdf` | `03366ad149bfb13a1d312ec1b0fd1a3e328d90a38687e42d7b29ab02991a4ced` | narrow vertical tape, centered black cycloid/floral linework, bilateral symmetry, small talismanic figures, tiny centered phrases, an infinity mark, and unusually generous vertical intervals |

Do not turn the interface into a literal PDF background. Translate the source
language into a small deterministic Izzi-generated SVG system:

- a narrow `dp-sash` edge rail for the catalog and artifact pages;
- fine roulette/cycloid rosettes as section-opening or terminal marks;
- bilateral or center-radial symmetry rather than generic iconography;
- sparse single emblems in large quiet fields;
- clustered line motifs at selected outer margins, never behind reading text;
- small Mono metadata phrases where text has a real interface meaning; and
- an optional content-hash seed so each artifact page receives a stable—not
  live-random—variation.

Generate new vectors through Izzi curve, roulette, and guilloche primitives
rather than rasterizing the PDFs into page furniture. Preserve each source PDF
and hash as provenance for the visual direction. The site graphics are
interface derivatives, not replacements for the source works.

Reconcile the colorful sash with the house-style anti-gradient rule by using
flat stepped rails or literal validated color fields in interface chrome.
Source-art gradients may remain visible when the source artwork itself is
being reviewed, but the review UI should not imitate them with unvalidated
transparency under text. On `house-style` pages, use ink, Pacific blue, and at
most one quiet pale field. On `randoma11y-accent` pages, map the rosette or
sash accent to the frozen daily pair only after every actual adjacency passes.

All decorative SVGs should be `aria-hidden="true"`, non-focusable, and
`pointer-events: none`. They must disappear safely in forced-colors mode,
remain still under `prefers-reduced-motion`, and never encode review status,
frame order, location, audio state, or acceptance. The ordinary headings,
labels, rules, and review controls remain the accessible representation.

### Global portal

The root catalog and shared site chrome should explicitly persist:

```json
{
  "style": "house-style"
}
```

Apply:

- warm paper `#fcfbf7`;
- near-black ink `#14171a`;
- Pacific blue `#173a55` for structural emphasis;
- muted text `#4d565d`;
- `#eef1f2` and `#f5f6f4` for flat secondary fields;
- square corners, hard left alignment, disciplined rules, and generous open
  space;
- Atkinson Hyperlegible Next for editorial text and Atkinson Hyperlegible Mono
  for IDs, hashes, timestamps, dimensions, codec data, and frame ordinals when
  available; and
- original Atkinson Hyperlegible and system faces only as disclosed fallbacks.

Remove interface gradients, shadows, decorative color glows, ornamental
noise, and dashboard-card styling. Keep artwork and video imagery unchanged;
the neutral interface is a frame for experimental material, not a palette
restriction on the material itself.

Because the house specification says not to commit or separately deliver font
binaries, first use verified local family names and the current fallback. A
later web-font delivery decision needs its own provenance, privacy, caching,
and repository-policy review.

### Individual MKV review pages

An individual review page may explicitly select:

```json
{
  "style": "randoma11y-accent",
  "style_day": "2026-08-12",
  "timezone": "America/Los_Angeles",
  "pair_manifest": "tokens/randoma11y/daily-pairs.json"
}
```

The pair changes accents, thick rules, active controls, frame ordinals, and a
small number of selected panels. Reading fields remain neutral. The pair is
selected once per Pacific date and reused for every artifact generated that
day. Every actual foreground/background adjacency is resolved to literal sRGB
and independently validated. If any required pairing fails or cannot be
tested, that page selects `house-style` and records the RandomA11y gap as
`UNAVAILABLE`.

Color never carries acceptance, rejection, source state, or audio state by
itself. Every state remains written in text and represented structurally.

## WCAG 2.2 form and dropdown contract

Use native `<select>` for the current finite filters and per-frame decisions.
Do not replace it with a custom ARIA combobox merely for visual styling. The
W3C describes a select-only combobox as functionally similar to HTML
`select`; a custom implementation would inherit a much larger keyboard and
state contract. See the
[WAI-ARIA combobox pattern](https://www.w3.org/WAI/ARIA/apg/patterns/combobox/).

Every dropdown must have:

- a persistent visible `<label>`; an `aria-label` alone is not the visual
  instruction required by WCAG 2.2 SC 3.3.2;
- a minimum 3rem control height, comfortably above the WCAG 2.2 AA 24 by 24
  CSS-pixel minimum target;
- `#14171a` or Izzi `wcag_dgray` text on a house soft/paper field;
- a two-pixel essential boundary using at least house muted, not the lighter
  structural rule;
- a clearly new, at least three-pixel focus outline in an independently
  validated color with an offset/gap that remains visible against both the
  control and page;
- native arrow-key, Home/End, typing, Escape, and platform behavior;
- meaningful option labels and `<optgroup>` labels when families become long;
- selected state expressed in text, not only through accent color;
- error and saved state announced in a nearby live region without moving
  focus; and
- usable reflow and no clipped focus at 200 percent zoom and a 320 CSS-pixel
  viewport.

The relevant W3C material includes
[Labels or Instructions](https://www.w3.org/WAI/WCAG22/Understanding/labels-or-instructions.html),
[Target Size (Minimum)](https://www.w3.org/WAI/WCAG22/Understanding/target-size-minimum),
and [Focus Appearance](https://www.w3.org/WAI/WCAG22/Understanding/focus-appearance).
WCAG 2.2 became a W3C Recommendation on 2023-10-05; see
[What is new in WCAG 2.2](https://www.w3.org/WAI/standards-guidelines/wcag/new-in-22/).

A proposed token boundary is:

```css
:root,
[data-dp-style="house-style"] {
  --dp-paper: #fcfbf7;
  --dp-ink: #14171a;
  --dp-accent: #173a55;
  --dp-muted: #4d565d;
  --dp-rule: #9da8af;
  --dp-soft: #eef1f2;
  --dp-soft-2: #f5f6f4;
  --izzi-wcag-dark: #2e2e2e;
}

select {
  min-height: 3rem;
  color: var(--dp-ink);
  background: var(--dp-soft-2);
  border: 2px solid var(--dp-muted);
  border-radius: 0;
}

select:focus-visible {
  outline: 3px solid var(--dp-accent);
  outline-offset: 3px;
}
```

This CSS is a proposal. Final delivery must test the rendered control in the
supported browser matrix; the source tokens alone do not prove WCAG
conformance.

## Artifact-page information architecture

Each separate page should present one task and follow this order:

1. **Identity and state** — project, episode, location, version, review ID,
   publication state, and explicit `CURRENT IS NOT ACCEPTED` boundary.
2. **Continuous playback** — poster-first video controls, duration, dimensions,
   audio availability, caption/transcript state, and flashing/motion warning.
3. **Whole-clip judgment** — accept, keep parts, revise, reject, discuss; motion
   and audio notes remain separate fields.
4. **Ten temporal samples** — numbered frame anchors with timestamp, thumbnail,
   decision, tags, and note.
5. **Audio evidence** — channel layout, sample rate, loudness measures,
   ambience source status, waveform, and the boundary between provisional
   window leveling and verified per-speaker normalization.
6. **Provenance** — canonical and proxy hashes, generation configuration,
   source commit, provider/release state, known limitations, and evidence
   status.
7. **Handoff** — save locally, download hash-bound JSON, or deliberately open a
   public GitHub issue draft.

The page must remain understandable with images disabled, sound unavailable,
color removed, or animation reduced. Captions and transcript handling must
respect the source's publication and privacy authority; when they cannot be
published, the page records `UNAVAILABLE` rather than reconstructing them.

## Implementation stages

### Stage 0 — local architecture proof

- Add a versioned review-item schema to `situationshipin.space`.
- Add explicit `style` metadata to the global portal and every artifact page.
- Replace the root neon UI with explicit `house-style` tokens and structure.
- Add one deterministic Izzi-generated `dp-sash`/rosette SVG study derived
  from the two hash-bound `resources/devastation-pacific` visual precedents.
- Implement one generated artifact page and exact local route.
- Generate a local MP4 proxy, filmstrip, poster, waveform, and manifest from a
  non-sensitive short canary.
- Add direct frame anchors and whole-clip motion/audio fields.
- Keep all media local and perform no upload or release.

Acceptance:

- schema and hashes pass;
- native form behavior works with keyboard only;
- the root and artifact page work at 200 percent zoom and 320 CSS pixels;
- every actual text, control-boundary, focus, and RandomA11y pairing has a
  retained contrast result;
- no source or proxy media enters the Pages source tree accidentally; and
- decorative source-derived graphics remain inert, non-semantic, outside form
  hit targets, and removable without losing information; and
- the local exact page resolves directly without first navigating the catalog.

### Stage 1 — one GitHub Release and Pages canary

After explicit publication authority for the selected media:

- create one review-batch release;
- upload one short canonical MKV and browser proxy;
- publish one exact `review/<review_id>/` page;
- verify desktop and mobile streaming, seeking, audio, captions state, hashes,
  and issue feedback;
- verify the exact URL over HTTPS after deployment; and
- send that exact link in chat.

This stage should use the smallest artifact that exercises motion and audio.
The HLT Frogtown v2 audio canary is technically suitable but must not be made
public merely because it exists locally.

### Stage 2 — full-episode review

- Publish one authorized 1–3 minute episode package.
- Add separate motion and audio decisions plus ten temporal anchors.
- Preserve the full source/proxy distinction and exact hashes.
- Test long seeking, mobile data behavior, issue payload size, and collaborator
  completion time.
- Decide whether Release hosting remains sufficient.

### Stage 3 — scale decision

Move media to R2 only if measured Release behavior, corpus size, access
control, or review throughput requires it. Choose the authenticated studio
origin when public transfer is not appropriate. Keep the public page and
review JSON stable across either storage change.

## Verification plan

Extend the existing site checker rather than replacing it.

Automated checks should fail on:

- an unknown or implicit style;
- `house-default`, `randomally-accent`, or another noncanonical alias;
- a missing or duplicate `review_id`;
- a source/proxy/filmstrip/frame hash mismatch;
- a Pages-tree source MKV or unallowlisted browser proxy;
- an unvalidated RandomA11y adjacency;
- normal text below 4.5:1;
- essential boundaries or focus indicators below 3:1 against adjacent colors;
- a review page without one H1, visible form labels, direct frame anchors, or
  a whole-clip decision field;
- autoplay;
- a missing flashing warning for a flagged artifact;
- a published-media URL whose manifest says `NOT-PUBLISHED`;
- an exact review link that does not resolve; or
- a chat handoff that links only to the portal root.

Retain these test layers:

1. `node scripts/check-review-site.mjs` for the bounded catalog and public
   payload;
2. a literal-token and actual-adjacency contrast checker;
3. HTML/ARIA and keyboard tests for native controls, dialogs, playback, and
   focus restoration;
4. responsive screenshots for desktop, portrait mobile, 200 percent zoom,
   forced colors, and reduced motion;
5. `ffprobe` and checksum receipts for MKV and proxies;
6. duration, frame-count, geometry, audio-channel, and caption-state checks;
7. an HTTPS deep-link and manifest fetch after deployment; and
8. a manual human pass of continuous motion and audio—the filmstrip alone
   cannot close either review gate.

Use the language “targets WCAG 2.2 AA” until the implemented site, every
actual color pairing, media alternatives, keyboard paths, zoom/reflow, and
manual review have been tested. Do not convert a successful color calculation
into a whole-site compliance claim.

## Authority and publication boundary

This proposal authorizes the local planning document and the associated local
trace snapshot. It does not by itself authorize:

- changing or publishing `situationshipin.space`;
- creating a GitHub Release;
- uploading an MKV, proxy, audio, caption, transcript, or image;
- configuring R2, a tunnel, or another storage service;
- making collaborator voices or private review material public;
- promoting a generated artifact to an accepted baseline; or
- transferring training-conversion material.

Those actions retain their existing repository, publication, privacy,
provider, cost, and human-review gates.
