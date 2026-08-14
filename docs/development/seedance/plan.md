Now that the audio baseline is approved, let's formulate next steps.

1. clean up audio development workflow files, and put them into
 
 - audio_workflow.md
   - defaults.md
   - analyze_expand_voice_reference_bank.md
   - tool_higgs.md (high compute)
   - tool_XX.md  (whatever name from previous low-compute pass was)
   - etl.md
   - audio_workflow_experiments.md 
   
   Just do a first pass sort now, no removal. Unable to classify goes into audio_workflow_experiments.md

2. implement a new "review" surface scope on situationshipin.space for "location scouting" and have it be a first-tier scope like existing.

when you get to https://situationshipin.space/location.html, 

for the project "here-lies-love" you'll see a list of the previously decided location names (like "dtla" and "frogtown", but all. There will be a way to select frogtown and then enter points on a google map that track paths that the four animals will walk.

For frogtown, use the intial google maps point (34.105547608596765, -118.24361721594703) and suggest next point (34.10771761395043, -118.24676789715588) and final point (34.107936983024416, -118.25265394226766).

that might be the length of all the episodes, check for suitability

3. For the tile sequence, use the style from 
https://situationshipin.space/review/izzi/2026-08-10/video/4d-guilloche-episode-08/frame-02.jpg

but light gray background, and the episode number as the number, and "here/nlies/trouble" as the text.

use madredeus font for type 

---

## Implementation status (2026-08-12)

1. Audio workflow first-pass sort: `docs/audio_workflow.md` plus
   `docs/audio_workflow/{defaults,etl,analyze_expand_voice_reference_bank,
   tool_higgs,tool_meanvc2_kokoro,audio_workflow_experiments}.md` — pointer
   index only, no removal.
2. Location scouting review scope: `location.html` on situationshipin.space
   (first-tier card on the landing page), `data/locations.json` registry with
   the Frogtown three-point route (straight-line 920 m: start→next 377 m,
   next→final 542 m — suitable as a multi-episode corridor; a 165 s episode at
   walking pace covers ~165–248 m). Map is Leaflet/OpenStreetMap with
   draggable markers; Google Maps editing would require an API key.
3. Title card: `scripts/render-hlt-title-card-madredeus.py` renders the
   guilloche-style card (light gray background, episode number, madredeus
   type) to `title-card-v5-madredeus.png` (SHA-256
   `6a112554ec31954f4b5be10e27d3e68d886fbd045273e59f89c94929834ad4a7`).
   **WIRED 2026-08-14:** `scripts/build-hlt-frogtown-v2-episode-master.py`
   now defaults `--title-card` to the madredeus card, and the episode-01
   Frogtown v1 master was rebuilt with the card as its 3 s title segment
   (168 s preserved; original kept as `*.v1.pre-madredeus.mkv`; manifest
   sha updated).  Verified light-gray title frame in the rebuilt master.

Project naming note: the plan says "here-lies-love"; the implemented registry
and pages use the established project id `here-lies-trouble`. Renaming is a
one-line data change if the plan's spelling is intentional.
