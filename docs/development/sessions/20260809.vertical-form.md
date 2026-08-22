# Vertical-series visual form

- Status: confirmed production range and local synthetic-form study
- Updated: 2026-08-09
- Provider state: excluded from form development

## Confirmed form

The verticals project is a series of **60–90 episodes**. Each episode contains
**1–3 minutes of visual content**. Episode duration is independent of title-card
duration. A title card, when enabled, precedes the content and does not shorten
its slot.

The earlier ten-scene, ten-second model remains useful as a provider canary and
transition fixture, but it is not the production vertical-series form. Local
form, timing, accessibility, and editorial decisions must be locked before a
future Seedance request is prepared.

## Range variants

With a three-second title card before every episode, the principal range
variants are:

| Variant | Episodes | Content per episode | Content runtime | Cards | Review runtime |
|---|---:|---:|---:|---:|---:|
| Compact | 60 | 1 minute | 1:00:00 | 3:00 | 1:03:00 |
| Balanced | 75 | 2 minutes | 2:30:00 | 3:45 | 2:33:45 |
| Extended | 90 | 3 minutes | 4:30:00 | 4:30 | 4:34:30 |

Those anchors are not three separate formats. A project may use any integer
episode count from 60 through 90 and any episode content duration from 60
through 180 seconds. Useful scheduling policies include:

- fixed slots, when rhythm and distribution predictability matter;
- uniformly varied slots, for broad mechanical form testing;
- short-weighted slots, when mobile completion rate matters more than total
  duration; and
- editorially assigned slots, where the subject determines length and the
  range is only a guardrail.

The final production policy should be editorially assigned. Uniform randomness
is suitable for a synthetic test, not a claim about audience preference.

At a nominal 5 Mbit/s video bitrate, the compact, balanced, and extended review
forms are approximately 2.36 GB, 5.77 GB, and 10.29 GB. At 8 Mbit/s they are
approximately 3.78 GB, 9.23 GB, and 16.47 GB. These are planning estimates, not
encoder guarantees.

## Vertical composition variants

The mastering canvas remains 1080×1920 at 30 fps. Source imagery may enter it
through one of four explicit policies:

| Policy | Strength | Risk | Recommended use |
|---|---|---|---|
| Native portrait | Uses the full canvas without intervention | Limits the source pool | Preferred production source |
| Contain on black | Preserves every source pixel | Landscape work occupies a short central band | Conservative form studies and archival fidelity |
| Crop to fill | Maximizes the screen image | Can destroy lateral composition and notation | Only with a reviewed crop per source |
| Designed extension | Uses a static field, blurred extension, or layout around a contained source | Adds visual decisions and potentially more motion | Production derivative after accessibility review |

The first synthetic check uses **contain on black**. It is intentionally
non-destructive and makes orientation problems visible. A later comparison
should test designed extension and reviewed crop as separate candidates, never
silently replace the source framing.

For publication, retain 9:16 as the master. Derive 4:5 or square previews from
a reviewed layout rather than cropping essential visual information. Preserve
a static poster and meaningful accessible name for every episode.

## Synthetic form check 01

The confirmed test contains ten ordered episodes. Every episode is:

1. a three-second black title card with a white serif episode number; then
2. one randomly selected file from `resources.rizal/asama-loops/*.mkv`, looped when
   necessary and trimmed to one randomly selected 60–180-second content slot.

Random selection is frozen by seed `20260809` and recorded in
`outputs/ad-hoc/synthetic/synthetic-form-check-01.plan.json`. Sources are sampled without
replacement for coverage; ten of the eleven available loops appear. The test
is silent, uses straight cuts, conforms all imagery to 1080×1920 at 30 fps,
and contains exactly 34,260 frames over 19:02.

The requested deliverable is `outputs/ad-hoc/synthetic/synthetic-form-check-01.mkv`. Preserve
the numbered cards, episode-level intermediates, probe, hashes, and contact
sheet beside it or in its retained asset directory. Provider media, API calls,
credentials, and provider spending are outside this test.

## Gates before provider engagement

- Human review of title-card duration, numeral legibility, and episode rhythm.
- Human comparison of contain, designed-extension, and reviewed-crop policies.
- Motion and flash review, plus a reduced-motion/static alternative.
- A reviewed accessible name and video-only alternative for each episode.
- A locked episode count, duration, source or prompt, crop/layout, and seed.
- A separate provider-transfer and spend authorization after the local form is
  accepted.

Seedance should execute a previously accepted form. It should not be used to
discover the form through paid trial and error.
