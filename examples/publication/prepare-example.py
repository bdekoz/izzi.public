#!/usr/bin/env python3
"""Freeze a small non-cartographic publication; no Cartofreako dependency."""
import argparse
import hashlib
import json
from pathlib import Path

HERE = Path(__file__).resolve().parent


def prepare(destination, icc, landscape=False, spread=False):
    paths = {'art': HERE/'sampler.svg', 'font-regular': HERE/'fonts/AtkinsonHyperlegible-Regular.ttf',
             'font-bold': HERE/'fonts/AtkinsonHyperlegible-Bold.ttf', 'cmyk': icc.resolve()}
    for role, filename in [('regular', 'DejaVuSans.ttf'), ('bold', 'DejaVuSans-Bold.ttf')]:
        paths['fallback-'+role] = next(Path('/usr/share/fonts').rglob(filename))
    assets = {k: {'path': str(p), 'sha256': hashlib.sha256(p.read_bytes()).hexdigest()} for k, p in paths.items()}
    overview = {'asset': 'art', 'alt': 'Two overlapping circles, dense diagonal lines and a white wave over a gold-to-blue gradient.',
                'caption': 'Transparency, line fields and a gradient in a small visual study.'}
    data = {
        'schema': 'izzi-publication-v1', 'id': 'izzi-visual-sampler',
        'metadata': {'title': 'Lines, light and layers', 'subtitle': 'An Izzi visual sampler', 'creator': 'Izzi',
                     'language': 'en', 'identifier': 'urn:uuid:4645a813-b482-5887-a924-a435893d8a67',
                     'modified': '2026-11-14T12:00:00Z', 'edition': 'Production example / November 2026'},
        'fonts': {'regular': 'font-regular', 'bold': 'font-bold', 'fallback_regular': 'fallback-regular', 'fallback_bold': 'fallback-bold'}, 'cmyk_profile': 'cmyk', 'assets': assets,
        'introduction': ['A small, self-contained art publication demonstrates the same layout and packaging used for larger visual books.'],
        'reading_guide': ['The overview presents the full composition. The enlarged detail separates thin lines and overlapping shapes.',
                          'Text remains adjustable in the ebook. The print edition uses readable text outside the artwork.'],
        'colophon': ['Original synthetic artwork made for the Izzi publication example.',
                     'Atkinson Hyperlegible is distributed under the SIL Open Font License; see the bundled OFL.txt.',
                     'Blank end pages bring this test interior to the selected paperback profile minimum.'],
        'chapters': [
            {'id': 'layers', 'title': 'Layers and interference', 'image': overview,
             'sections': [{'title': 'How to read the composition', 'paragraphs': [
                 'The wave crosses two translucent circles. Their overlap changes the local contrast without changing the line spacing.',
                 'Unicode typography: café, piñata, Ångström, 37° N, 50 m², and λ. Long links wrap without losing their target: https://www.w3.org/publishing/epub3/epub-spec.html.']}],
             'details': [{**overview, 'crop': [.33, .16, .33, .65], 'title': 'Linework at a larger scale',
                          'caption': 'An enlarged section of the same vector source.'}]},
            {'id': 'observation', 'title': 'Looking again: a deliberately long heading to exercise wrapping and page composition',
             'image': overview, 'sections': [{'title': 'Observation notes', 'paragraphs': [
                 'A repeated motif looks different as the surrounding field changes. The source contains gradients and opacity; the printed version is a flattened color image.']*12}]}
        ]
    }
    data['chapter_order'] = [x['id'] for x in data['chapters']]
    data['chapters'][0]['sections'].append({'title': 'Sample regions', 'keys': [
        {'label': label, 'image': {**overview, 'crop': [.06+i*.10, .25, .08, .14], 'alt': label}}
        for i, label in enumerate(['Warm field', 'First translucent layer', 'A deliberately long key label that wraps across several lines without touching its neighbor',
                                    'Fine diagonal lines', 'Crossing wave', 'Layer overlap / λ', 'Cool field', 'Outer boundary'])]})
    if landscape:
        data['print_layout'] = 'facing-spreads-v1'
        data['metadata']['edition'] = 'Landscape facing-spread example / November 2026'
        data['reading_guide'].append('Artwork fills each even left page; its key and primary context occupy the facing right page. Additional notes continue in columns after the key.')
        data['colophon'][-1] = 'The neutral landscape master has no retailer minimum-page padding. A final blank page keeps the interior count even when necessary.'
        for chapter in data['chapters']:
            chapter['facing_context'] = [[0,0]]
    if spread:
        data['print_layout'] = 'portrait-map-spreads-v1'
        data['metadata']['edition'] = 'Full-bleed portrait spread example / November 2026'
        data['reading_guide'] = ['Each complete composition spans two portrait pages. Its border background extends to the edges; keys and notes occupy the next two pages.',
                                 'Read key rows from left to right; read explanatory text down each column. Enlarged details are retained in the reflowable ebook.']
        data['colophon'][-1] = 'A4 portrait trim with 3 mm bleed. The complete map is preserved; the physical fold may hide fine details. This is a neutral interior master.'
        for chapter in data['chapters']:
            chapter['facing_context'] = [[0,0]]
    destination.parent.mkdir(parents=True, exist_ok=True)
    destination.write_text(json.dumps(data, indent=2, ensure_ascii=False)+'\n')
    print(destination)


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('destination', type=Path)
    parser.add_argument('--cmyk-profile', type=Path, default=Path('/usr/share/ghostscript/iccprofiles/default_cmyk.icc'))
    modes = parser.add_mutually_exclusive_group()
    modes.add_argument('--landscape', action='store_true')
    modes.add_argument('--spread', action='store_true')
    args = parser.parse_args(); prepare(args.destination, args.cmyk_profile, args.landscape, args.spread)
