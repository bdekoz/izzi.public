# Camera tracking routes

Recorded: 2026-08-12 America/Los_Angeles
Status: `ROUTE-REVIEW-PENDING; FROGTOWN-CONFIRMED; NO-SPEND; NOT-SHARED`

Camera tracking needs direction, not just coordinates. A single confirmed
coordinate per location tells us *where*; ordered waypoints let us compute
heading, segment distance, pace, and camera-relative motion for each beat.
This document defines the waypoint model, the derived camera track, the
confirmed and pending routes, and how routes flow into motion-canary specs.

## Waypoint model

Each location route is a small ordered set of waypoints (3–6 is enough):

| Waypoint | Role | Needed for |
| --- | --- | --- |
| entry | first frame of the episode; establishes approach | approach heading, establishing camera |
| anchor | the beat's primary sightline; animals regroup here | hold/pause-and-look timing, framing |
| exit | final frame; hands off to the next episode | continuing heading, exit reframe |

Extra intermediate waypoints are added when the path turns (e.g., a corner or
overlook) so bearings stay meaningful.

## Derived camera track

From the reviewed waypoints we compute, per segment:

- bearing (great-circle or projected azimuth between consecutive waypoints);
- distance (segment length, meters);
- implied duration at walking pace (1.0–1.5 m/s) versus the episode content
  window (165 s);
- camera motion language: `advance`, `pause-and-look`, `reframe`, `exit`
  mapped onto the established motion grammar.

The computed track is then folded into each location's motion-canary spec
(camera is never the primary mover; body-driven motion first).

## Routes

### Frogtown (confirmed)

The Frogtown corridor is the reference route and is already plotted in the
location-scouting scope (`data/locations.json` on situationshipin.space):

| Waypoint | Coordinates | Segment | Distance |
| --- | --- | --- | --- |
| start | 34.1055476, -118.2436172 | start → next | 377 m |
| next | 34.1077176, -118.2467679 | next → final | 542 m |
| final | 34.1079369, -118.2526539 | — | total 920 m |

Suitability: at walking pace a 165 s episode covers ~165–248 m, so Frogtown is
a multi-episode corridor (five beats, episodes 01–05).

### Palette locations (pending route review)

The five non-Frogtown locations need a human route review so camera tracking
can be computed. Candidate coordinates below are UNVERIFIED and must be
confirmed in the location map before any track is computed.

| Location | Episode | Candidate coordinates (UNVERIFIED) | Route status |
| --- | --- | --- | --- |
| DTLA Central Library | 06 | ≈ 34.0505, -118.2541 | PENDING-REVIEW |
| MOCA Geffen | 07 | ≈ 34.0502, -118.2388 | PENDING-REVIEW |
| Hauser & Wirth | 08 | ≈ 34.0347, -118.2323 | PENDING-REVIEW |
| Hammer Museum | 09 | ≈ 34.0597, -118.4440 | PENDING-REVIEW |
| Sunset pass (4810 Sunset Blvd) | 10 | ≈ 34.0990, -118.2960 | PENDING-REVIEW |

Review priority matches the arc order: DTLA Central Library first, then MOCA
Geffen, Hauser & Wirth, Hammer Museum, Sunset pass.

## Scope notes

- Museums and the library use exterior/approach/transit routes unless access
  is explicitly authorized — the same "stylized exterior/transit, not
  entering" treatment as the Sunset pass.
- The walking order across the vertical is editorial (Frogtown → DTLA
  Library → MOCA Geffen → Hauser & Wirth → Hammer → Sunset), not strictly
  geodesic.
- Route review is read-only: adjust waypoints in `location.html`, save the
  review JSON, and KEEP before tracks are computed.

## Workflow

1. Scout: confirm/adjust waypoints per location in the location map.
2. KEEP the route (GitHub issue or saved review JSON).
3. Compute the camera track (bearing, distance, pace, motion language).
4. Fold the track into the location's motion-canary spec and episode prompt.
5. Render proofs; review filmstrip + proxy as usual.

## Gates

- `location_scouting_approval` — KEEP per location before track computation.
- `provider_submission_authorized` — still false; no renders from these
  routes until authorized.
