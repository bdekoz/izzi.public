# Deterministic keyframes

`izzi::keyframes` resolves discrete SVG authoring frames from explicit
specifications and a scene seed.

## K1 style grammar

`k1_neon_grammar` supplies the initial neon-style preset:

- a four-color cyan, magenta, gold, and violet palette;
- a near-black radial background with a restrained violet core;
- rounded weighted lines with a wider translucent glow halo;
- sparse, seed-positioned luminous texture marks; and
- independently constructed radiant, orbital, and segmented bridge motifs.

The default preset authors five ordered frames. Every generated variation
comes from the scene seed and stable indices; time, addresses, and
environment state are not inputs.

Clean-room rule: geometry is authored only from mathematical primitives.
Reference plates inform visual character only; their paths, text, numbers,
signatures, and other typographic elements are not used.

## K2 location framing

K2 accepts a location ID and an ordered list of latitude/longitude points;
coordinates remain input geometry only.

The eight supplied locations are:

- `frogtown` — Frogtown;
- `dtla-central-library` — DTLA — Los Angeles Central Library;
- `moca-geffen` — MOCA Geffen Contemporary;
- `hauser-wirth` — Hauser & Wirth;
- `hammer-museum` — Hammer Museum;
- `hiho-to-runyon-canyon` — Hi-Ho to Runyon Canyon;
- `runyon-mulholland-drive` — Runyon Canyon Mulholland Drive; and
- `runyon-to-inspiration-point` — Runyon Canyon to Inspiration Point.

`make_k2_framing` validates the route and computes its centroid as the
arithmetic mean of all ordered points. Its extent comes from the
latitude/longitude bounding box, with longitude scaled by centroid latitude.
The location seed combines the caller seed with a stable hash of the ID.

`make_k2_scene` maps the centroid into a bounded canvas frame, uses extent
to control motif scale and separation, and uses the ordered first-to-last
route direction for motif rotation. The location seed supplies only small,
deterministic framing offsets and then becomes the K1 scene seed.

Clean-room and audio independence: route points are never rendered as a
path, copied into SVG geometry, or treated as source artwork. Keyframes
remain stills authored before any audio step; audio is not an input to
their geometry, framing, seed, or style grammar.

## K3 example and tests

`examples/keyframe-plates.cc` embeds the eight authoritative routes in a
static input table and builds one K2 SVG for each location.

- `keyframe-plates --list` prints exactly the eight location-derived stems.
- `keyframe-plates OUTPUT-DIRECTORY` writes one `<stem>.svg` per location.

The stable per-location base seeds are:

- `frogtown`: `2026120301`;
- `dtla-central-library`: `2026120302`;
- `moca-geffen`: `2026120303`;
- `hauser-wirth`: `2026120304`;
- `hammer-museum`: `2026120305`;
- `hiho-to-runyon-canyon`: `2026120306`;
- `runyon-mulholland-drive`: `2026120307`; and
- `runyon-to-inspiration-point`: `2026120308`.

K2 combines each base seed with the stable location-ID hash before
resolving the K1 scene. No time, environment, address, or audio value
participates.

`tests/keyframes.cc` checks byte-identical SVG output from repeated K1 and
K2 resolves, arithmetic-centroid and bounding-extent invariants, stable
location seeds and motif framing, resource-budget rejection, and the
clean-room, reference-boundary, and pending-human-review metadata markers.

The embedded routes remain input geometry only: no route or reference path
data enters the generated primitives. The tests also retain the explicit
note that these keyframes are stills authored without audio input.

## K4 asset map and review plan

Required assets and their allowed roles:

- The neon-addict PNG set at `resources.static/neon-addict/` is a required
  visual-direction reference only. Its source geometry is not copied into
  keyframes.
- The eight here-lies-trouble routes from situationshipin.space
  `data/locations.json` are the authoritative ordered input geometry. They
  remain framing inputs only and are not rendered as route paths.
- The provenance-corrected accepted `gpt-5.6-sol-max` lineage frames are
  required lineage and review assets; their handoff remains pending until
  the §5.5 items are complete.

Review plan:

1. Before K5, confirm availability and role boundaries for all three asset
   classes and confirm the lineage-frame provenance correction.
2. During still review, confirm coverage of all eight routes and verify
   that neon-addict material remains visual direction rather than copied
   geometry.
3. Keep the accepted lineage-frame handoff pending until the §5.5 items are
   complete and record that status with the reviewed still set.

Audio remains independent: keyframes are stills authored before any audio
step, and the `seed-audio-*` corpora are not keyframe inputs.

Handoff order:

1. K5 generates the stills locally.
2. fal.ai later renders 1080p 15 s chains from those stills.
3. Local MeanVC2 audio is muxed only afterwards.
