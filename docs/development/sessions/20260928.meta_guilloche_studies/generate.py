#!/usr/bin/env python3
"""Draw original survey studies, independently of the Izzi implementation.

These are explanatory SVG constructions, not production presets or reference
tracings. Run with --output PATH to reproduce without replacing the checked-in
sheet. Python standard library only; coordinates are SVG screen coordinates.
"""

import argparse
from html import escape
import math
from pathlib import Path
import xml.etree.ElementTree as ET


TAU = 2 * math.pi
INK = "#23464d"
SECONDARY = "#668a8a"
parts = []
path_count = 0
point_count = 0


def path(points, closed=False, color=INK, width=0.8):
    global path_count, point_count
    assert len(points) >= 2
    assert all(math.isfinite(v) for p in points for v in p)
    assert all(0 <= x <= 400 and 0 <= y <= 280 for x, y in points)
    data = "M" + " L".join(f"{x:.3f},{y:.3f}" for x, y in points)
    if closed:
        data += " Z"
    parts.append(f'<path d="{data}" fill="none" stroke="{color}" '
                 f'stroke-width="{width}" stroke-linejoin="round"/>')
    path_count += 1
    point_count += len(points)


def line(a, b, transform=lambda x, y: (x, y), samples=16, **style):
    path([transform(a[0] + (b[0] - a[0]) * i / samples,
                    a[1] + (b[1] - a[1]) * i / samples)
          for i in range(samples + 1)], **style)


def panel(x, y, label, subtitle):
    parts.append(f'<g transform="translate({x},{y})">')
    parts.append('<rect width="440" height="420" rx="8" '
                 'fill="#ffffff" stroke="#d7dedb"/>')
    parts.append(f'<text x="20" y="31" class="label">{escape(label)}</text>')
    parts.append(f'<text x="20" y="54" class="note">{escape(subtitle)}</text>')
    parts.append('<g transform="translate(20,78)">')


def end_panel(note):
    parts.append('</g>')
    parts.append(f'<text x="20" y="395" class="note">{escape(note)}</text>')
    parts.append('</g>')


def contours(modes):
    assert sum(abs(amplitude) for _, amplitude, _ in modes) < 1
    for j in range(19):
        radius = 22 + j * 5
        points = []
        for i in range(721):
            theta = TAU * i / 720
            h = 1 + sum(a * math.cos(k * theta + phase)
                        for k, a, phase in modes)
            points.append((200 + radius * h * math.cos(theta),
                           140 + radius * h * math.sin(theta)))
        points[-1] = points[0]
        path(points, closed=True)


def waves():
    for j in range(19):
        points = []
        for i in range(401):
            u = i / 400
            envelope = 0.25 + 0.75 * u
            phase = TAU * (1.7 * u + 0.25 * u * u)
            displacement = 27 * envelope * (
                math.sin(phase) + 0.23 * math.sin(TAU * 3.4 * u + 0.9))
            points.append((18 + 364 * u, 53 + 8.5 * j + displacement))
        path(points)


def hobnail(warp=False):
    def transform(x, y):
        if warp:
            x += 15 * math.sin(y / 42 + 0.5) + 0.0009 * (y - 90)**2 - 12
        return x, y

    size, cols, rows = 25, 11, 7
    ox, oy = (400 - cols * size) / 2, (280 - rows * size) / 2
    for col in range(cols + 1):
        x = ox + col * size
        line((x, oy), (x, oy + rows * size), transform, samples=96)
    for row in range(rows + 1):
        y = oy + row * size
        line((ox, y), (ox + cols * size, y), transform)
    for row in range(rows):
        for col in range(cols):
            cx = ox + (col + 0.5) * size
            cy = oy + (row + 0.5) * size
            for dx, dy in [(-1, -1), (1, -1), (1, 1), (-1, 1)]:
                line((cx, cy), (cx + dx * size / 2, cy + dy * size / 2),
                     transform, samples=8, color=SECONDARY, width=0.65)
            for fraction in [0.42, 0.72]:
                half = size * fraction / 2
                corners = [(cx - half, cy - half), (cx + half, cy - half),
                           (cx + half, cy + half), (cx - half, cy + half)]
                for a, b in zip(corners, corners[1:] + corners[:1]):
                    line(a, b, transform, samples=8, width=0.55)


def bezier(controls, t):
    weights = [(1 - t)**3, 3 * (1 - t)**2 * t,
               3 * (1 - t) * t * t, t**3]
    return tuple(sum(w * p[axis] for w, p in zip(weights, controls))
                 for axis in [0, 1])


def tangent(controls, t):
    weights = [3 * (1 - t)**2, 6 * (1 - t) * t, 3 * t * t]
    vector = tuple(sum(w * (b[axis] - a[axis])
                       for w, a, b in zip(weights, controls, controls[1:]))
                   for axis in [0, 1])
    length = math.hypot(*vector)
    assert length > 0
    return vector[0] / length, vector[1] / length


def foliage():
    stem = [(74, 254), (270, 195), (73, 100), (236, 22)]
    for j in range(-3, 4):
        points = []
        for i in range(301):
            t = i / 300
            x, y = bezier(stem, t)
            dx, dy = tangent(stem, t)
            points.append((x - dy * j * 1.35, y + dx * j * 1.35))
        path(points, width=0.65)
    for t, tip, width in [(0.25, (361, 174), 24),
                          (0.51, (42, 83), 18),
                          (0.73, (335, 46), 13)]:
        start = bezier(stem, t)
        tx, ty = tangent(stem, t)
        controls = [start, (start[0] + 49 * tx, start[1] + 49 * ty),
                    (tip[0] - (tip[0] - start[0]) * 0.35, tip[1] + 27), tip]
        path([bezier(controls, i / 240) for i in range(241)], width=0.7)
        for ring in range(1, 8):
            sides = [[], []]
            for i in range(241):
                u = i / 240
                x, y = bezier(controls, u)
                dx, dy = tangent(controls, u)
                offset = width * ring / 7 * math.sin(math.pi * u)**1.25
                sides[0].append((x - dy * offset, y + dx * offset))
                sides[1].append((x + dy * offset, y - dx * offset))
            path(sides[0] + sides[1][::-1], closed=True, width=0.65)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output', type=Path,
                        default=Path(__file__).with_name('asymmetry.svg'))
    args = parser.parse_args()
    parts.extend([
        '<svg xmlns="http://www.w3.org/2000/svg" width="1440" height="1060" '
        'viewBox="0 0 1440 1060" role="img" aria-labelledby="title desc">',
        '<title id="title">Guilloche enrichment: six geometry studies</title>',
        '<desc id="desc">Two contour rosettes compare regular lobes with mixed '
        'angular harmonics. A flowing wave field changes amplitude and wavelength. '
        'Two faceted lattices compare regular cells with a smooth invertible warp. '
        'An unequal three-branch leaf study explores foliate guilloche. These '
        'are original mathematical sketches, not implemented Izzi presets.</desc>',
        '<style>text{font-family:DejaVu Sans,sans-serif;fill:#23464d}'
        '.label{font-size:18px;font-weight:600}.note{font-size:13px;fill:#516e71}'
        '</style>',
        '<rect width="1440" height="1060" fill="#f5f6f2"/>',
        '<text x="36" y="52" font-size="29" font-weight="600">'
        'Guilloche beyond equal repetition</text>',
        '<text x="36" y="86" font-size="16">'
        'Original construction studies · 28 September 2026 · proposals, not Izzi output</text>',
    ])
    panel(36, 126, 'A · Regular contour control', 'One angular mode; six equal lobes')
    contours([(6, 0.13, 0)])
    end_panel('19 nested contours; open center; exact closure')
    panel(500, 126, 'B · Compound contour', 'Modes 3, 5 and 7 with different phases')
    contours([(3, 0.11, 0), (5, 0.075, 0.6), (7, 0.05, 1.7)])
    end_panel('Same ring count; unequal lobes; shared radial profile')
    panel(964, 126, 'C · Directional current', 'Amplitude envelope and changing wavelength')
    waves()
    end_panel('19 translated curves; preserved vertical ordering')
    panel(36, 570, 'D · Hobnail line study', 'Shared grid edges, ridges and two contour levels')
    hobnail()
    end_panel('11 × 7 cells; linework suggests faceting')
    panel(500, 570, 'E · Deformed hobnail', 'Every line shares the map (x + f(y), y)')
    hobnail(warp=True)
    end_panel('Same cell topology; nonlinear spatial asymmetry')
    panel(964, 570, 'F · Unequal foliate growth', 'A curved stem and three differently sized branches')
    foliage()
    end_panel('Intentional shared tips; junction spacing needs review')
    parts.append('<text x="36" y="1032" class="note">'
                 'See 20260928.meta_guilloche_survey.md for equations, source distinctions '
                 'and the staged implementation plan.</text></svg>')
    svg = '\n'.join(parts) + '\n'
    ET.fromstring(svg)
    args.output.write_text(svg, encoding='utf-8')
    print(f'{args.output}: {path_count} paths; {point_count} sampled vertices; '
          'finite coordinates, panel bounds and SVG XML checked')


if __name__ == '__main__':
    main()
