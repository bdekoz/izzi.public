#!/usr/bin/env python3
"""Update the graphic-water catalogue inside the canonical Hamonshu guide.

Run from any directory. Pillow is used only for literal source-image crops;
the historical artwork is neither traced nor restyled. --output-dir permits
regeneration elsewhere for comparison. The JSON is the classification source;
only the marked catalogue section of hamonshu.md is generated.
"""

import argparse
from collections import Counter
from html import escape
import json
import os
from pathlib import Path
import re

from PIL import Image


ROOT = Path(__file__).resolve().parents[4]
SOURCE = ROOT / 'docs/generative_patterns/hamonshu-water-catalogue.json'
GUIDE = SOURCE.with_name('hamonshu.md')
BEGIN_CATALOGUE = '<!-- BEGIN generated-hamonshu-water-catalogue -->'
END_CATALOGUE = '<!-- END generated-hamonshu-water-catalogue -->'


def write_guide(out, catalogue):
    document = GUIDE.read_text()
    if document.count(BEGIN_CATALOGUE) != 1 or document.count(END_CATALOGUE) != 1:
        raise ValueError('hamonshu.md must contain exactly one catalogue marker pair')
    before, remaining = document.split(BEGIN_CATALOGUE)
    _, after = remaining.split(END_CATALOGUE)
    if out != GUIDE.parent:
        # Keep hand-written documentation usable in a scratch regeneration.
        # Gallery and JSON are generated beside that copy.
        def relocate(match):
            target = match.group(1)
            file, separator, fragment = target.partition('#')
            if target.startswith(('http:', 'https:', 'mailto:', '#')) or file in {
                'hamonshu-water-gallery.html', 'hamonshu-water-catalogue.json'
            }:
                return match.group(0)
            path = Path(os.path.relpath(GUIDE.parent / file, out)).as_posix()
            return '](' + path + (separator + fragment if separator else '') + ')'
        before = re.sub(r'\]\(([^)]+)\)', relocate, before)
        after = re.sub(r'\]\(([^)]+)\)', relocate, after)
        (out / SOURCE.name).write_bytes(SOURCE.read_bytes())
    (out / GUIDE.name).write_text(
        before + BEGIN_CATALOGUE + '\n\n' + catalogue.rstrip()
        + '\n\n' + END_CATALOGUE + after
    )


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output-dir', type=Path, default=SOURCE.parent)
    args = parser.parse_args()
    out = args.output_dir.resolve()
    out.mkdir(parents=True, exist_ok=True)
    data = json.loads(SOURCE.read_text())
    records, groups = data['records'], data['groups']
    group_ids = {g['id'] for g in groups}
    assert len(records) == len({r['id'] for r in records}) == 86
    actual = {str(p.relative_to(ROOT)) for p in
              (ROOT / 'resources.rizal/hamonshu').glob('*.pages/*.png')}
    assert {r['source'] for r in records} == actual
    assert Counter(r['volume'] for r in records) == {1: 29, 2: 28, 3: 29}
    for record in records:
        assert record['content_status'] in data['content_statuses']
        assert set(record['form_groups']) <= group_ids
        assert set(record['arrangements']) <= data['arrangements'].keys()
        assert bool(record['form_groups']) == (record['content_status'] != 'excluded_text')
        assert record['id'] == f"v{record['volume']}-s{record['scan']:03}"
    for pdf in data['pdfs']:
        assert (ROOT / pdf['source']).is_file()

    def link(source):
        return Path(os.path.relpath(ROOT / source, out)).as_posix()

    image_dir = out / 'images/hamonshu-groups'
    image_dir.mkdir(parents=True, exist_ok=True)
    crops = {}
    for group in groups:
        ref = group['representative']
        source = next(r for r in records if (r['volume'], r['scan']) ==
                      (ref['volume'], ref['scan']))
        assert group['id'] in source['form_groups']
        box = ref['box']
        assert 0 <= box[0] < box[2] <= 1 and 0 <= box[1] < box[3] <= 1
        with Image.open(ROOT / source['source']) as scan:
            w, h = scan.size
            crop = scan.crop(tuple(round(v * (w if i % 2 == 0 else h))
                                   for i, v in enumerate(box))).convert('RGB')
            crop.thumbnail((720, 540), Image.Resampling.LANCZOS)
            crop.save(image_dir / f"{group['id']}.jpg", quality=90)
        crops[group['id']] = source

    md = [
        '## Graphic-water catalogue', '',
        'Reviewed 2026-09-28. **86 scans across three volumes; 84 contain water '
        'ornament, two are text-only exclusions.** This is a scan-level, '
        'multi-label index. A scan can appear in several groups; these counts '
        'are not counts of individual motifs.', '',
        '[Open the filterable gallery](hamonshu-water-gallery.html) · '
        '[Machine-readable classifications](hamonshu-water-catalogue.json)', '',
        'Only graphic water forms are classified. Titles, calligraphy, colophons, '
        'marginal writing and writing equipment are excluded. Mixed pages remain '
        'in the inventory with directions to their water region. Front and back '
        'covers contribute repeated spiral ornament, not new families. Originals '
        'remain in place.', '',
        'Group names are modern descriptions, not translated historical captions. '
        'The seven thumbnails are literal water-only crops from volume 3. The '
        'full-source links retain page context and may include writing.', '',
        '| Form group | Scans including this form | Geometric starting point |',
        '| --- | ---: | --- |',
    ]
    for group in groups:
        members = [r for r in records if group['id'] in r['form_groups']]
        md.append(f"| [{group['name']}](#{group['id']}) | {len(members)} | {group['construction']} |")
    md += ['', '### How to use the grouping', '',
           'Select a **form** first, then an **arrangement**: band, field, '
           'medallion, cluster, border or spread. For example, an eddy can '
           'occur inside a medallion or in a continuous band. Keep these axes '
           'separate in future preset metadata.', '',
           'The existing [153-entry C++ catalogue](' + link('src/izzi-svg-curves-hamonshu-v2.inc') +
           ') covers volume 2 individual specimens. This index adds reviewed '
           'scan coverage for all three volumes; it does not invent per-motif '
           'IDs for volumes 1 and 3. Scan numbers are the PNG filename suffixes, '
           'not printed page numbers. Do not substitute them in current C++ '
           '`pattern_spec` page fields.', '']
    for group in groups:
        source = crops[group['id']]
        md += [f"<a id=\"{group['id']}\"></a>", '', f"### {group['name']}", '',
               group['description'], '',
               f"![{group['name']}: graphic source crop](images/hamonshu-groups/{group['id']}.jpg)", '',
               f"Representative: [{source['id']}]({link(source['source'])}). "
               + group['construction'], '']
        for volume in (1, 2, 3):
            selected = [r for r in records if r['volume'] == volume and
                        group['id'] in r['form_groups']]
            md.append(f"- Volume {volume}: " + ', '.join(
                f"[{r['scan']:03}]({link(r['source'])})" for r in selected) + '.')
        md.append('')
    md += ['### Scan inventory', '',
           'These notes describe visible water forms only. Classification was '
           'manual, using contact sheets and selected full-size spreads. A '
           'future motif-level catalogue should record individual crop bounds '
           'and review ambiguous forms at full resolution.', '',
           '| Scan | Content | Form groups | Arrangements | Graphic observations |',
           '| --- | --- | --- | --- | --- |']
    for r in records:
        md.append(f"| [{r['id']}]({link(r['source'])}) | {r['content_status']} | "
                  + ', '.join(r['form_groups']) + ' | '
                  + ', '.join(r['arrangements']) + f" | {r['notes']} |")
    md += ['', '### Catalogue sources and regeneration', '',
           'Local source PDFs: ' + ', '.join(f"[volume {p['volume']}]({link(p['source'])})"
                                            for p in data['pdfs']) + '.', '',
           'See [source provenance](#sources-and-provenance) and the '
           '[revision map](#revision-map) in this document. The '
           '[guilloche survey](' + link('docs/development/sessions/20260928.meta_guilloche_survey.md') +
           ') explains structured-path integration, stable motif identities, '
           'asymmetric water fields, leaves, coral and optical treatments.', '',
           'Regenerate with `python3 docs/development/sessions/'
           '20260928.meta_guilloche_studies/build_catalogue.py` from the '
           'repository root (Pillow required). The generator validates complete '
           'scan coverage and source links before updating this marked catalogue '
           'section, thumbnails and the gallery; it preserves the surrounding '
           'API, construction and consumer documentation.', '']
    write_guide(out, '\n'.join(md))

    cards = []
    for group in groups:
        source = crops[group['id']]
        items = []
        for r in records:
            if group['id'] in r['form_groups']:
                items.append(f'<li data-volume="{r["volume"]}"><a href="{escape(link(r["source"]))}">'
                             f'{r["id"]}</a> — {escape(r["notes"])}</li>')
        cards.append(f'<section data-group="{group["id"]}"><h2>{escape(group["name"])}</h2>'
                     f'<img src="images/hamonshu-groups/{group["id"]}.jpg" '
                     f'alt="Water-only crop: {escape(group["name"])}">'
                     f'<p>{escape(group["description"])}</p><p class="muted">'
                     f'Crop: {source["id"]}. {escape(group["construction"])}</p>'
                     f'<details><summary>Source scans</summary><ul>{"".join(items)}</ul></details></section>')
    options = ''.join(f'<option value="{g["id"]}">{escape(g["name"])}</option>' for g in groups)
    html = '''<!doctype html>
<html lang="en"><meta charset="utf-8"><meta name="viewport" content="width=device-width,initial-scale=1">
<title>Hamonshū · graphic water forms</title>
<style>
*{box-sizing:border-box}body{margin:0;background:#f4f3ed;color:#203e42;font:17px/1.5 system-ui,sans-serif}
header,main,footer{max-width:1180px;margin:auto;padding:24px}h1{font-size:36px;margin:8px 0}
p{max-width:850px}a{color:#12636c}nav{display:flex;gap:24px;flex-wrap:wrap;padding:16px 0}
select{font:inherit;padding:7px;border:1px solid #708985;border-radius:4px;background:white}
main{display:grid;grid-template-columns:repeat(auto-fit,minmax(min(100%,340px),1fr));gap:24px}
section{background:white;border:1px solid #d8ddd6;border-radius:8px;padding:22px}h2{font-size:23px;margin-top:0}
img{width:100%;height:245px;object-fit:contain;background:#ece8dc}.muted{font-size:14px;color:#526966}
summary{cursor:pointer;padding:8px 0}li{margin:10px 0;font-size:14px}[hidden]{display:none!important}
</style>
<header><p class="muted">Mori Yūzan · three volumes · reviewed 28 September 2026</p>
<h1>Hamonshū: graphic water forms</h1>
<p>Seven groups for browsing currents, waves and water ornament. The previews contain only water drawings.
Group names are modern descriptions. Source scans may include writing, which is excluded from classification.</p>
<p><a href="hamonshu.md#graphic-water-catalogue">Hamonshū guide and complete 86-scan index</a> ·
<a href="hamonshu-water-catalogue.json">Classification data</a></p>
<nav><label>Form <select id="form"><option value="all">All seven groups</option>''' + options + '''</select></label>
<label>Source volume <select id="volume"><option value="all">All volumes</option><option value="1">1</option>
<option value="2">2</option><option value="3">3</option></select></label></nav>
<p class="muted" id="status" role="status">Seven groups. Open a source list to inspect its scans.</p></header>
<main>''' + '\n'.join(cards) + '''</main>
<footer class="muted">84 scans contain water ornament; two text-only scans are excluded.
Forms can belong to several groups. Counts refer to scans, not individual motifs. Cover spirals repeat.</footer>
<script>
const form=document.querySelector('#form'), volume=document.querySelector('#volume');
function filter(){let visible=0, memberships=0;document.querySelectorAll('section[data-group]').forEach(s=>{
  let count=0;s.querySelectorAll('li[data-volume]').forEach(li=>{li.hidden=volume.value!=='all'&&li.dataset.volume!==volume.value;if(!li.hidden)count++;});
  s.hidden=(form.value!=='all'&&s.dataset.group!==form.value)||count===0;
  if(!s.hidden){visible++;memberships+=count;}s.querySelector('summary').textContent=`Source scans (${count})`;
});document.querySelector('#status').textContent=`${visible} groups; ${memberships} scan memberships. Previews are volume 3 crops; filters affect source lists.`;}
form.addEventListener('change',filter);volume.addEventListener('change',filter);filter();
</script></html>
'''
    (out / 'hamonshu-water-gallery.html').write_text(html)
    print(f'{len(records)} unique scans validated; 7 water crops, Markdown and HTML written to {out}')


if __name__ == '__main__':
    main()
