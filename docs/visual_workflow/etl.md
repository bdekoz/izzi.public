# Visual workflow — ETL

First-pass sort of visual extract/transform/load steps.

- Episode assembly: `scripts/render-here-lies-trouble-local-episode.py`
  (title card + content, review MKV).
- Filmstrip hydration: ten-frame filmstrips under
  `outputs/…/episode-01-local-trial/review/` with manifests; review proxies
  at 360x626 H.264/AAC.
- Review artifacts and hashes: `outputs/review/feedback/visual/…` manifests
  with SHA-256 lineage; site catalog `data/review-items.json`.

Status: `FIRST-PASS-SORT; NO-REMOVAL`
