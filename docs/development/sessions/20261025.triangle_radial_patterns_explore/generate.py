#!/usr/bin/env python3
"""Original vector sketches for the triangle radial-pattern exploration.

Run with Python 3; only the standard library is required. The output is a
concept sheet, not a set of Izzi generators or historical reconstructions.
"""

from html import escape
from math import atan2, cos, exp, hypot, isclose, log, pi, sin, sqrt
from pathlib import Path
import colorsys
import json


TAU = 2 * pi
INKS = ("#183e53", "#407c78", "#c2934f", "#d8dfd5", "#ad6755")
PAPER = "#fcfaf4"
CELLS = []


def polar(radius, angle, center=(0, 0)):
    return center[0] + radius * cos(angle), center[1] + radius * sin(angle)


def mix(a, b, t):
    return (1 - t) * a[0] + t * b[0], (1 - t) * a[1] + t * b[1]


def centroid(vertices):
    return tuple(sum(v[k] for v in vertices) / len(vertices) for k in (0, 1))


def area(vertices):
    a, b, c = vertices
    return abs((b[0] - a[0]) * (c[1] - a[1])
               - (b[1] - a[1]) * (c[0] - a[0])) / 2


def triangle(vertices, color=0, inset=0.96):
    assert len(vertices) == 3 and area(vertices) > 1e-8
    CELLS.append((tuple(vertices), color % len(INKS), inset))


def stamp(center, radius, angle, color=0):
    triangle([polar(radius, angle + i * TAU / 3, center) for i in range(3)],
             color, 1)


def polygon(radius, count, phase=0, center=(0, 0)):
    return [polar(radius, phase + i * TAU / count, center)
            for i in range(count)]


def lattice(side, shells):
    def vertex(i, j):
        return side * (i + j / 2), side * sqrt(3) * j / 2

    def inside(p):
        i, j = p
        return max(abs(i), abs(j), abs(i + j)) <= shells

    faces = []
    for i in range(-shells, shells + 1):
        for j in range(-shells, shells + 1):
            for parity, indices in enumerate((
                ((i, j), (i + 1, j), (i, j + 1)),
                ((i + 1, j), (i + 1, j + 1), (i, j + 1)),
            )):
                if all(inside(p) for p in indices):
                    shell = max(max(abs(x), abs(y), abs(x + y))
                                for x, y in indices)
                    faces.append(([vertex(*p) for p in indices], parity, shell))
    assert len(faces) == 6 * shells * shells
    return faces


def children(vertices):
    a, b, c = vertices
    ab, bc, ca = mix(a, b, .5), mix(b, c, .5), mix(c, a, .5)
    return ((a, ab, ca), (ab, b, bc), (ca, bc, c), (ab, bc, ca))


def lattice_shells():
    for vertices, parity, shell in lattice(28, 4):
        triangle(vertices, (shell + parity) % 4)
    return 6 * 4**2 * sqrt(3) * 28**2 / 4


def triangular_bands():
    rings = [polygon(r, 3, -pi / 2) for r in (132, 105, 78, 51, 24)]
    for k, (outer, inner) in enumerate(zip(rings, rings[1:])):
        for i in range(3):
            j = (i + 1) % 3
            triangle((outer[i], outer[j], inner[j]), k % 3)
            triangle((outer[i], inner[j], inner[i]), (k + 1) % 3)
    triangle(rings[-1], 0)
    return area(rings[0])


def polar_wheel():
    vertices = polygon(117, 18, -pi / 2)
    for i in range(18):
        triangle(((0, 0), vertices[i], vertices[(i + 1) % 18]), i % 3, .975)
    return 18 * 117**2 * sin(TAU / 18) / 2


def annular_mesh():
    n = 18
    rings = [polygon(r, n, (k % 2) * pi / n - pi / 2)
             for k, r in enumerate((28, 57, 87, 117))]
    for i in range(n):
        triangle(((0, 0), rings[0][i], rings[0][(i + 1) % n]), i % 2)
    for k, (inner, outer) in enumerate(zip(rings, rings[1:])):
        for i in range(n):
            j = (i + 1) % n
            triangle((inner[i], outer[i], outer[j]), (k + 1) % 4)
            triangle((inner[i], outer[j], inner[j]), k % 4)
    return n * 117**2 * sin(TAU / n) / 2


def spiral_arms():
    for arm in range(6):
        for step in range(11):
            radius = 15 + 9.5 * step
            angle = log(radius / 15) / .46 + arm * TAU / 6
            stamp(polar(radius, angle), 3.1 + .058 * radius,
                  angle + atan2(1, .46), arm % 3)


def phyllotaxis():
    golden_angle = pi * (3 - sqrt(5))
    for i in range(210):
        radius = 7.8 * sqrt(i + .5)
        angle = i * golden_angle
        stamp(polar(radius, angle), 4.6, angle, int(radius / 28) % 3)


def asanoha():
    for vertices, parity, shell in lattice(54, 2):
        center = centroid(vertices)
        for i in range(3):
            triangle((vertices[i], vertices[(i + 1) % 3], center),
                     (i + parity) % 3, .955)
    return 6 * 2**2 * sqrt(3) * 54**2 / 4


def uroko_scales():
    stamp((0, 0), 12, -pi / 2, 0)
    for k, (radius, count) in enumerate(((29, 6), (56, 12), (83, 18), (110, 24))):
        for i in range(count):
            angle = TAU * (i + .5 * (k % 2)) / count - pi / 2
            stamp(polar(radius, angle), 13.5, angle, k % 3)


def kagome():
    side = 27

    def vertex(i, j):
        return side * (i + j / 2 - 1.5), side * sqrt(3) * (j - 1) / 2

    for i in range(-6, 7):
        for j in range(-6, 7):
            if i % 2 == 0 and j % 2 == 0:
                indices, color = ((i, j), (i + 1, j), (i, j + 1)), 1
            elif i % 2 and j % 2:
                indices, color = ((i + 1, j), (i + 1, j + 1), (i, j + 1)), 0
            else:
                continue
            points = [vertex(*p) for p in indices]
            if all(hypot(*p) <= 126 for p in points):
                triangle(points, color, .98)


def khatam_mosaic():
    radius = 39
    for q in range(-1, 2):
        for r in range(-1, 2):
            if max(abs(q), abs(r), abs(q + r)) > 1:
                continue
            center = sqrt(3) * radius * (q + r / 2), 1.5 * radius * r
            corners = polygon(radius, 6, -pi / 2, center)
            for sector in range(6):
                parent = (center, corners[sector], corners[(sector + 1) % 6])
                for a, first in enumerate(children(parent)):
                    for b, second in enumerate(children(first)):
                        color = 0 if b == 3 else (2 if a == 3 else (sector + b) % 4)
                        triangle(second, color, .955)
    return 7 * 6 * sqrt(3) * radius**2 / 4


def sierpinski():
    def recurse(vertices, depth, color):
        if depth == 0:
            triangle(vertices, color, .97)
        else:
            for child in children(vertices)[:3]:
                recurse(child, depth - 1, color)

    corners = polygon(119, 6)
    for i in range(6):
        recurse(((0, 0), corners[i], corners[(i + 1) % 6]), 3, i % 3)
    return 6 * sqrt(3) * 119**2 / 4 * (.75**3)


def rotating_triangles():
    corners = polygon(119, 6)
    for sector in range(6):
        outer = [(0, 0), corners[sector], corners[(sector + 1) % 6]]
        for depth in range(8):
            inner = [mix(outer[i], outer[(i + 1) % 3], .24) for i in range(3)]
            for i in range(3):
                triangle((outer[i], inner[i], inner[(i - 1) % 3]), depth % 3)
            outer = inner
        triangle(outer, 0)
    return 6 * sqrt(3) * 119**2 / 4


STUDIES = (
    (1, "Triangular lattice", "Exact equilateral shells", lattice_shells),
    (2, "Triangular bands", "Nested boundaries, split strips", triangular_bands),
    (3, "Polar wheel", "Eighteen common-apex sectors", polar_wheel),
    (4, "Annular triangulation", "Half-step offsets between rings", annular_mesh),
    (5, "Logarithmic spiral arms", "Separate equilateral motifs", spiral_arms),
    (6, "Phyllotactic field", "Golden-angle placement", phyllotaxis),
    (7, "Asanoha network", "Leaf divisions within carriers", asanoha),
    (8, "Uroko radial adaptation", "Outward scales, staggered bands", uroko_scales),
    (9, "Kagome abstraction", "Triangles with hexagonal voids", kagome),
    (10, "Khatam-inspired mosaic", "Original microtriangle color recipe", khatam_mosaic),
    (15, "Sierpinski rosette", "Three recursive omission levels", sierpinski),
    (20, "Rotating triangle dissection", "Eight steps, edge fraction 0.24", rotating_triangles),
)


def svg_triangle(vertices, fill, opacity=1, inset=.95):
    center = centroid(vertices)
    points = [mix(center, p, inset) for p in vertices]
    text = " ".join(f"{x:.4f},{y:.4f}" for x, y in points)
    return f'<polygon points="{text}" fill="{fill}" opacity="{opacity:.4f}"/>'


def rotate_point(point, angle, offset=(0, 0)):
    x, y = point
    return (x * cos(angle) - y * sin(angle) + offset[0],
            x * sin(angle) + y * cos(angle) + offset[1])


def hash_value(i, j, seed=20261025):
    value = (i * 374761393 + j * 668265263 + seed * 1274126177) & 0xffffffff
    value = ((value ^ (value >> 13)) * 1274126177) & 0xffffffff
    return ((value ^ (value >> 16)) / 0xffffffff) * 2 - 1


def noise(x, y, seed=20261025):
    from math import floor
    i, j = floor(x), floor(y)
    tx, ty = x - i, y - j
    sx, sy = tx * tx * (3 - 2 * tx), ty * ty * (3 - 2 * ty)
    top = (1 - sx) * hash_value(i, j, seed) + sx * hash_value(i + 1, j, seed)
    bottom = ((1 - sx) * hash_value(i, j + 1, seed)
              + sx * hash_value(i + 1, j + 1, seed))
    return (1 - sy) * top + sy * bottom


def fbm(x, y, seed):
    return sum(.62**j * noise(x * 2.6**j, y * 2.6**j, seed + j)
               for j in range(5)) / sum(.62**j for j in range(5))


def experimental_sheet():
    """Illustrate selected controls; this is not the proposed full effects API."""
    palette = ("#74eff0", "#b898ff", "#ff799d", "#ecea87", "#91e9bd")
    art = []

    # 21: centroid-selected sector groups; rigid rotation preserves each tile.
    slipped = []
    for vertices, parity, shell in lattice(20, 5):
        center = centroid(vertices)
        sector = int((atan2(center[1], center[0]) % TAU) / TAU * 9)
        angle = .38 * sin(sector * 1.9) if hypot(*center) > 16 else 0
        shift = polar(15 * sin(sector * 2.1), sector * TAU / 9)
        points = [rotate_point(p, angle, shift) for p in vertices]
        slipped.append(svg_triangle(points, palette[(shell + parity) % 5], .9))
    art.append("".join(slipped))

    # 22: deduplicated lattice edges, three registered color layers.
    edges = set()
    for vertices, _, _ in lattice(9, 12):
        for i in range(3):
            edge = tuple(sorted(tuple(round(v, 5) for v in p)
                                for p in (vertices[i], vertices[(i + 1) % 3])))
            edges.add(edge)
    path = " ".join(f"M{a[0]},{a[1]}L{b[0]},{b[1]}" for a, b in sorted(edges))
    layers = []
    for angle, scale, offset, color in ((-2.8, .985, -3, palette[0]),
                                      (0, 1, 0, palette[2]),
                                      (3.7, 1.018, 3, palette[3])):
        layers.append(f'<g transform="translate({offset},0) rotate({angle}) scale({scale})" '
                      f'opacity=".7"><path d="{path}" fill="none" '
                      f'stroke="{color}" stroke-width=".6"/></g>')
    art.append("".join(layers))

    # 23: radial bands, coherent missing runs, and rigid displaced copies.
    CELLS.clear()
    annular_mesh()
    torn = []
    for vertices, color, inset in CELLS:
        x, y = centroid(vertices)
        angle, radius = atan2(y, x) % TAU, hypot(x, y)
        if radius > 50 and .12 < angle < 1.0:
            continue
        shift = (-18, 6) if 45 < radius < 90 else (9, -5)
        points = [(px + shift[0], py + shift[1]) for px, py in vertices]
        if 45 < radius < 90 and 2.2 < angle < 4.8:
            for echo in (2, 1):
                echo_points = [(px + echo * 8, py - echo * 7) for px, py in points]
                torn.append(svg_triangle(echo_points, palette[2], .58**echo, inset))
        torn.append(svg_triangle(points, palette[color], .86, inset))
    art.append('<g transform="scale(.9)">' + "".join(torn) + '</g>')

    # 24: a deterministic value-noise sketch with warped sampling coordinates.
    # Samples a subset of the report's controls; individual shapes stay triangular.
    groups = [[], [], [], []]
    shadows = []
    for vertices, parity, shell in lattice(18, 6):
        x, y = centroid(vertices)
        nx = x / 48.6 + 2.4 * noise(x / 72, y / 72, 20261025)
        ny = y / 48.6 + 2.4 * noise(x / 72, y / 72, 20261026)
        u = fbm(nx, ny, 20261025)
        if u < -.18:
            continue
        angle = 155 * pi / 180 * fbm(nx + 11, ny - 8, 20261028)
        scale = max(.18, min(3.2, exp(.9 * fbm(nx - 5, ny + 4, 20261029))))
        offset = (x + 39.6 * fbm(nx + 3, ny, 20261030),
                  y + 39.6 * fbm(nx, ny + 3, 20261031))
        points = [rotate_point(((px - x) * scale, (py - y) * scale), angle, offset)
                  for px, py in vertices]
        rgb = colorsys.hsv_to_rgb((.5 + .78 * u + shell * .06) % 1, .63, 1)
        color = '#%02x%02x%02x' % tuple(round(255 * c) for c in rgb)
        alpha = .015 + .965 * ((u + 1) / 2)**.42
        group = (int(x / 35) + 2 * int(y / 35)) % 4
        groups[group].append(svg_triangle(points, color, alpha))
        shadows.append(svg_triangle(points, "#ee79d5", alpha * .62))
    body = ['<g transform="scale(.78)">',
            '<g transform="translate(25.2,-16.2)" filter="url(#noise-shadow)">',
            *shadows, '</g>']
    for i, group in enumerate(groups):
        filter_attr = '' if i < 2 else f' filter="url(#noise-soft-{i})"'
        body.extend([f'<g{filter_attr}>', *group, '</g>'])
    body.append('</g>')
    art.append("".join(body))

    # 25: sector-correlated decay, per-leaf scatter, earlier-state echoes.
    corners = polygon(106, 6)
    leaves = []
    for sector in range(6):
        branch = [((0, 0), corners[sector], corners[(sector + 1) % 6])]
        for _ in range(3):
            branch = [child for parent in branch for child in children(parent)]
        leaves.extend((p, sector, i) for i, p in enumerate(branch))

    def state(time, shadow=False):
        parts = []
        for vertices, sector, index in leaves:
            onset = 2.8 * (sector / 6)
            age = max(0, time - onset)
            x, y = centroid(vertices)
            selector = (hash_value(index, sector, 20261026) + 1) / 2
            if not shadow and age > .5 and selector < min(.78, .35 * age):
                continue
            alpha = max(.01, 2**(-age / (.7 * (2.3 if shadow else 1))))
            turn = min(120, 58 * age) * pi / 180 * sin(sector + .7)
            drift = 13.25 * 2.6 * min(1, age / 2)
            moved_center = rotate_point((x, y), turn,
                                        polar(drift * selector, sector * TAU / 6))
            size = max(.12, min(2.4, 1 + age * .55 * (2 * selector - 1)))
            points = [rotate_point(((px - x) * size, (py - y) * size), turn,
                                   moved_center) for px, py in vertices]
            color = '#fb6ca9' if shadow else palette[(sector + index % 3) % 5]
            parts.append(svg_triangle(points, color, alpha))
        return "".join(parts)

    body = ['<g transform="scale(.71)">',
            '<g transform="translate(23.85,14.575)" opacity=".62" '
            'filter="url(#decay-shadow)">', state(2.4, True), '</g>']
    for echo in range(8, 0, -1):
        body.extend([f'<g transform="translate({echo * 4.24},{-echo * 2.385}) '
                     f'rotate({echo * 19})" opacity="{.8 * .74**echo:.4f}">',
                     state(2.4 - .17 * echo), '</g>'])
    body.extend([state(2.4), '</g>'])
    art.append("".join(body))

    labels = (
        (21, 'Slipped radial sectors', 'Rigid groups / exposed seams'),
        (22, 'Lattice misregistration', 'Three colored edge networks'),
        (23, 'Fractured rings', 'Dropout / displacement / replay'),
        (24, 'Correlated triangle noise', 'Warp / opacity / blur / shadow'),
        (25, 'Group decay echoes', 'Fragmentation / delayed ghosts'),
    )
    svg = ['<svg xmlns="http://www.w3.org/2000/svg" width="1544" height="448" '
           'viewBox="0 0 1544 448" role="img" aria-labelledby="title desc">',
           '<title id="title">Triangle glitch, noise, and decay studies</title>',
           '<desc id="desc">Five static original sketches illustrating selected '
           'controls from candidates 21 through 25, not full implementations.</desc>',
           '<defs>']
    for name, sigma in (('noise-shadow', 14.4), ('noise-soft-2', 4.3),
                        ('noise-soft-3', 8.64), ('decay-shadow', 16.56)):
        svg.append(f'<filter id="{name}" filterUnits="userSpaceOnUse" '
                   'x="-250" y="-250" width="500" height="500" '
                   'color-interpolation-filters="sRGB">'
                   f'<feGaussianBlur stdDeviation="{sigma}"/></filter>')
    svg += ['</defs>', '<rect width="1544" height="448" fill="#0e1726"/>',
            '<g font-family="DejaVu Sans, sans-serif" fill="#f2f4ef">',
            '<text x="24" y="35" font-size="24">Triangle disruption studies</text>',
            '<text x="24" y="59" font-size="12" fill="#afc6d8">'
            'Five original static sketches / selected controls only / '
            'complete proposed parameter ranges are in the survey</text>']
    for i, (number, title, subtitle) in enumerate(labels):
        x = 24 + i * 302
        svg += [f'<g id="candidate-{number}">',
                f'<rect x="{x}" y="79" width="288" height="332" rx="5" '
                'fill="#142136" stroke="#3b5068"/>',
                f'<text x="{x + 12}" y="105" font-size="15" font-weight="bold">'
                f'{number}  {escape(title)}</text>',
                f'<text x="{x + 12}" y="127" font-size="11" fill="#afc6d8">'
                f'{escape(subtitle)}</text>',
                f'<g transform="translate({x + 144},266) scale(.91)">', art[i],
                '</g></g>']
    svg += ['<text x="24" y="434" font-size="11" fill="#afc6d8">'
            'Authored geometric effects. No historical provenance, physical '
            'shadow accuracy, or production renderer support is implied.</text>',
            '</g></svg>']
    target = Path(__file__).with_name('experimental.svg')
    target.write_text('\n'.join(svg) + '\n', encoding='utf-8')
    print(f'Wrote {target.name}: 5 experimental panels.')


def main():
    width, height = 1492, 1298
    pieces = [
        f'<svg xmlns="http://www.w3.org/2000/svg" width="{width}" height="{height}" '
        f'viewBox="0 0 {width} {height}" role="img" aria-labelledby="title desc">',
        '<title id="title">Triangle radial patterns: twelve construction studies</title>',
        '<desc id="desc">Original schematic geometric proposals accompanying the '
        '2026-10-25 survey. Historical inspirations are adaptations, not reproductions.</desc>',
        f'<rect width="{width}" height="{height}" fill="{PAPER}"/>',
        '<g font-family="DejaVu Sans, sans-serif" fill="#183e53">',
        '<text x="28" y="43" font-size="29">Triangle radial patterns</text>',
        '<text x="28" y="72" font-size="14">12 original construction sketches '
        '/ candidate numbers match the 20-candidate survey / 2026-10-25</text>',
    ]
    records = []
    for panel, (number, title, subtitle, build) in enumerate(STUDIES):
        CELLS.clear()
        expected_area = build()
        total_area = sum(area(p) for p, _, _ in CELLS)
        if expected_area is not None:
            assert isclose(total_area, expected_area, rel_tol=1e-10), number
        assert all(abs(x) < 150 and abs(y) < 150
                   for vertices, _, _ in CELLS for x, y in vertices), number
        record = {"candidate": number, "triangle_count": len(CELLS),
                  "area_before_gutters": round(total_area, 6),
                  "area_identity_checked": expected_area is not None}
        records.append(record)
        x, y = 28 + (panel % 4) * 362, 100 + (panel // 4) * 382
        pieces += [
            f'<g id="candidate-{number:02d}">',
            f'<rect x="{x}" y="{y}" width="350" height="370" rx="5" '
            'fill="#fffefb" stroke="#d6d9d1"/>',
            f'<text x="{x + 16}" y="{y + 28}" font-size="17" font-weight="bold">'
            f'{number:02d}  {escape(title)}</text>',
            f'<text x="{x + 16}" y="{y + 51}" font-size="12" fill="#596d72">'
            f'{escape(subtitle)}</text>',
            f'<g transform="translate({x + 175},{y + 200})">',
        ]
        for vertices, color, inset in CELLS:
            center = centroid(vertices)
            points = [mix(center, p, inset) for p in vertices]
            text = " ".join(f"{px:.4f},{py:.4f}" for px, py in points)
            pieces.append(f'<polygon points="{text}" fill="{INKS[color]}"/>')
        pieces += ['</g>',
                   f'<text x="{x + 16}" y="{y + 351}" font-size="11" fill="#596d72">'
                   f'{len(CELLS)} drawn triangles / schematic study</text>', '</g>']
    pieces += [
        '<text x="28" y="1276" font-size="12" fill="#596d72">'
        'Geometry and color are authored for comparison. Gutters reveal cells. '
        'These sketches are not new Izzi API outputs.</text>',
        '</g>',
        '<metadata>' + escape(json.dumps(records, sort_keys=True)) + '</metadata>',
        '</svg>',
    ]
    target = Path(__file__).with_name("constructions.svg")
    target.write_text("\n".join(pieces) + "\n", encoding="utf-8")
    print(f"Wrote {target.name}: {len(records)} panels, "
          f"{sum(r['triangle_count'] for r in records)} triangles; "
          f"{sum(r['area_identity_checked'] for r in records)} area identities checked.")
    experimental_sheet()


if __name__ == "__main__":
    main()
