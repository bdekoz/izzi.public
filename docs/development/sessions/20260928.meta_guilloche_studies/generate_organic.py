#!/usr/bin/env python3
"""Original botanical and coral construction studies, not native Izzi presets.

Standard library only. These bounded drawings compare morphology; they are
stylizations, not taxonomic illustrations or simulations of biological growth.
"""
import argparse
from html import escape
import math
from pathlib import Path
import random
import xml.etree.ElementTree as ET

INK = '#23464d'
parts = []
paths = 0
vertices = 0


def path(points, width=0.8, closed=False, fill='none', color=INK):
    global paths, vertices
    assert len(points) >= 2
    assert all(math.isfinite(v) for p in points for v in p)
    assert all(0 <= x <= 320 and 0 <= y <= 320 for x, y in points)
    d = 'M' + ' L'.join(f'{x:.3f},{y:.3f}' for x, y in points)
    parts.append(f'<path d="{d}{" Z" if closed else ""}" fill="{fill}" '
                 f'stroke="{color}" stroke-width="{width}" '
                 'stroke-linejoin="round" stroke-linecap="round"/>')
    paths += 1
    vertices += len(points)


def cubic(c, t):
    weights = ((1-t)**3, 3*t*(1-t)**2, 3*t*t*(1-t), t**3)
    return tuple(sum(w*p[j] for w, p in zip(weights, c)) for j in (0, 1))


def normal(c, t):
    weights = (3*(1-t)**2, 6*t*(1-t), 3*t*t)
    dx, dy = (sum(w*(b[j]-a[j]) for w, a, b in zip(weights, c, c[1:]))
              for j in (0, 1))
    length = math.hypot(dx, dy)
    assert length > 0
    return -dy/length, dx/length


def blade(base, tip, width, serrated=False):
    dx, dy = tip[0]-base[0], tip[1]-base[1]
    length = math.hypot(dx, dy)
    nx, ny = -dy/length, dx/length
    def point(u, side, scale=1):
        teeth = (1.0 if round(u*48) % 2 == 0 else 0.76) if serrated else 1
        w = width * math.sin(math.pi*u)**0.85 * teeth * side * scale
        return base[0]+u*dx+nx*w, base[1]+u*dy+ny*w
    for scale in ([1] if serrated else [1, 0.48]):
        edges = [point(i/48, 1, scale) for i in range(49)]
        edges += [point(i/48, -1, scale) for i in reversed(range(49))]
        path(edges, closed=True, width=0.8)
    path([base, tip], width=0.7)
    if serrated:
        for i in range(5, 43, 4):
            u = i/48
            for side in (-1, 1):
                path([point(u-0.07, 0), point(u, side, 0.88)], width=0.55)


def pinnate():
    controls = [(118, 302), (187, 208), (116, 105), (206, 18)]
    path([cubic(controls, i/180) for i in range(181)], width=1.6)
    for i in range(15):
        t = 0.12 + i*0.052
        for side in (-1, 1):
            u = t + (0.014 if side == 1 else 0)
            base = cubic(controls, u)
            nx, ny = normal(controls, u)
            length = 67*math.sin(math.pi*u)**0.7 + 5
            tip = (base[0]+nx*side*length+ny*29,
                   base[1]+ny*side*length-nx*29)
            blade(base, tip, 3.5+2*math.sin(math.pi*u))


def fan_palm():
    base = (160, 246)
    path([(152, 305), base], width=2)
    outline = [base]
    for i in range(35):
        angle = -2.86 + i*2.58/34
        radius = 143 if i % 2 == 0 else 98
        outline.append((base[0]+radius*math.cos(angle), base[1]+radius*math.sin(angle)))
    path(outline, closed=True, width=1.3)
    for i in range(18):
        a = -2.86 + i*2.58/17
        for delta in (-0.012, 0, 0.012):
            path([(base[0]+r*math.cos(a+delta), base[1]+r*math.sin(a+delta))
                  for r in range(13, 143, 2)], width=0.6)


def ginkgo():
    base = (160, 245)
    def radius(a):
        notch = 27*math.exp(-((a+math.pi/2)/0.17)**2)
        return 148 - notch + 3.5*math.sin(11*a+0.4) + 1.8*math.sin(19*a)
    def polar(a, r):
        return base[0]+r*math.cos(a), base[1]+r*math.sin(a)
    path([(153, 302), (157, 273), base], width=1.5)
    angles = [-2.78 + i*2.42/240 for i in range(241)]
    path([base] + [polar(a, radius(a)) for a in angles], closed=True, width=1.2)
    for i in range(9):
        a = -2.66+i*2.18/8
        middle = polar(a, 62)
        path([base, middle], width=0.7)
        for side in (-1, 1):
            b = a+side*0.065
            fork = polar(b, 103)
            path([middle, fork], width=0.65)
            for second in (-1, 1):
                c = b+second*0.025
                path([fork, polar(c, radius(c)-1)], width=0.6)


def cannabis():
    base = (159, 252)
    path([(152, 307), base], width=1.7)
    for degrees, length, width in [(-85, 100, 9), (-57, 143, 13),
                                   (-29, 179, 16), (1, 211, 18),
                                   (30, 169, 16), (59, 132, 13), (85, 93, 9)]:
        a = math.radians(degrees)-math.pi/2
        blade(base, (base[0]+length*math.cos(a), base[1]+length*math.sin(a)),
              width, serrated=True)


def branching_coral():
    rng = random.Random(20260928)
    def branch(base, angle, length, depth, width):
        end = (base[0]+length*math.cos(angle), base[1]+length*math.sin(angle))
        bend = rng.uniform(-6, 6)
        c = [base, (base[0]+length*0.32*math.cos(angle)+bend,
                    base[1]+length*0.32*math.sin(angle)),
             (base[0]+length*0.78*math.cos(angle)-bend,
              base[1]+length*0.78*math.sin(angle)), end]
        points = [cubic(c, i/40) for i in range(41)]
        path(points, width=width)
        if depth:
            branch(end, angle-rng.uniform(0.34, 0.67), length*0.74, depth-1, width*0.65)
            branch(end, angle+rng.uniform(0.30, 0.58), length*0.69, depth-1, width*0.65)
    branch((150, 302), -1.53, 84, 4, 5.2)
    branch((155, 283), -0.96, 61, 3, 3.2)
    branch((148, 279), -2.18, 54, 3, 3.1)


def sea_fan():
    base = (160, 297)
    def p(a, u):
        r = (205+12*math.cos(3*a+0.4))*u
        return base[0]+0.73*r*math.cos(a)+5*u*u, base[1]+r*math.sin(a)
    angles = [-2.82+i*2.49/32 for i in range(33)]
    for a in angles:
        path([p(a, i/90) for i in range(3, 91)], width=0.85)
    for row in range(3, 22):
        for i, (a, b) in enumerate(zip(angles, angles[1:])):
            if (i+row*3) % 11 == 0:
                continue
            u = row/22
            path([p(a+(b-a)*j/8, u+0.013*math.sin(math.pi*j/8)*(1 if i%2 else -1))
                  for j in range(9)], width=0.55)
    path([(160, 310), (160, 285)], width=2.7)


def foliose_coral():
    for cx, cy, rx, ry, phase in [(158, 112, 127, 68, 0.3),
                                 (146, 173, 114, 67, 1.1),
                                 (171, 237, 108, 59, 2.2)]:
        def ring(scale):
            pts = []
            for i in range(361):
                a = 2*math.pi*i/360
                r = 1+0.055*math.cos(7*a+phase)+0.04*math.cos(3*a+0.6)
                pts.append((cx+rx*scale*r*math.cos(a),
                            cy+ry*scale*r*math.sin(a)+9*scale*math.cos(a+phase)))
            return pts
        path(ring(1), width=1.2, fill='white', closed=True)
        for k in range(1, 10):
            path(ring(0.16+k*0.075), closed=True, width=0.65)


def labyrinth_coral():
    # A bounded Hilbert-order meander supplies a graphic analogue of grooves.
    def hilbert(n, d):
        x = y = 0
        s, t = 1, d
        while s < n:
            rx, ry = 1 & (t//2), 1 & (t ^ (1 & (t//2)))
            if ry == 0:
                if rx == 1:
                    x, y = s-1-x, s-1-y
                x, y = y, x
            x += s*rx
            y += s*ry
            t //= 4
            s *= 2
        return x, y
    control = [(48+32*x, 48+32*y) for x, y in (hilbert(8, d) for d in range(64))]
    assert len(set(control)) == 64
    assert all(math.dist(a, b) == 32 for a, b in zip(control, control[1:]))
    rounded = [control[0]]
    for a, b, c in zip(control, control[1:], control[2:]):
        start = (b[0]+(a[0]-b[0])*0.28, b[1]+(a[1]-b[1])*0.28)
        end = (b[0]+(c[0]-b[0])*0.28, b[1]+(c[1]-b[1])*0.28)
        rounded.append(start)
        for i in range(1, 13):
            t = i/12
            rounded.append(((1-t)**2*start[0]+2*t*(1-t)*b[0]+t*t*end[0],
                            (1-t)**2*start[1]+2*t*(1-t)*b[1]+t*t*end[1]))
    rounded.append(control[-1])
    # Smooth bounded shear makes the otherwise regular meander asymmetric.
    rounded = [(x+6*math.sin(y/43), y+4*math.sin(x/51)) for x, y in rounded]
    parts.append('<defs><clipPath id="brain"><ellipse cx="160" cy="160" rx="133" ry="131"/></clipPath></defs>')
    parts.append('<g clip-path="url(#brain)">')
    path(rounded, width=12)
    path(rounded, width=9, color='white')
    parts.append('</g>')
    parts.append('<ellipse cx="160" cy="160" rx="133" ry="131" fill="none" stroke="#23464d" stroke-width="1.2"/>')


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output', type=Path, default=Path(__file__).with_name('organic.svg'))
    args = parser.parse_args()
    parts.extend(['<svg xmlns="http://www.w3.org/2000/svg" width="1440" height="1110" '
                  'viewBox="0 0 1440 1110" role="img" aria-labelledby="title desc">',
                  '<title id="title">Four leaf forms and four coral studies</title>',
                  '<desc id="desc">Original construction studies: feather palm, fan palm, '
                  'ginkgo, cannabis, branching coral, sea fan, foliose coral and a '
                  'labyrinth analogue of grooved coral. Botanical silhouettes, branching '
                  'topology and reticulation are distinct controls.</desc>',
                  '<style>text{font-family:DejaVu Sans,sans-serif;fill:#23464d}'
                  '.label{font-size:19px;font-weight:600}.note{font-size:12px;fill:#516e71}</style>',
                  '<rect width="1440" height="1110" fill="#f5f6f2"/>',
                  '<text x="30" y="49" font-size="29" font-weight="600">Leaves and coral: distinct structures</text>',
                  '<text x="30" y="81" font-size="16">Original geometry studies · morphology presets proposed for Izzi · 28 September 2026</text>'])
    studies = [
        ('Feather palm', 'Leaflets along a bent rachis', pinnate, 'Unequal lengths and staggered attachments'),
        ('Fan palm', 'Joined blade with separated outer segments', fan_palm, 'Radiating folds; a common attachment region'),
        ('Ginkgo', 'Fan blade, notch and forked veins', ginkgo, 'Simple leaf; dichotomous vein construction'),
        ('Cannabis', 'Seven separate, serrated leaflets', cannabis, 'Palmate attachment; unequal lengths'),
        ('Branching coral', 'Bounded unequal branch hierarchy', branching_coral, 'Stylized colony; fixed depth and deterministic seed'),
        ('Sea fan', 'Planar branches with cross-connections', sea_fan, 'Reticulated network; not an ordinary tree'),
        ('Foliose coral', 'Overlapping ruffled plates', foliose_coral, 'Nested contours and explicit front-to-back order'),
        ('Grooved coral study', 'Rounded labyrinth inside a massive outline', labyrinth_coral, 'Graphic analogue; no biological growth claim'),
    ]
    for i, (title, subtitle, draw, note) in enumerate(studies):
        x, y = 26+(i%4)*350, 116+(i//4)*477
        parts.append(f'<g transform="translate({x},{y})"><rect width="338" height="452" '
                     'rx="8" fill="white" stroke="#d7dedb"/>')
        parts.append(f'<text x="12" y="30" class="label">{escape(title)}</text>')
        parts.append(f'<text x="12" y="53" class="note">{escape(subtitle)}</text><g transform="translate(9,79)">')
        draw()
        parts.append(f'</g><text x="12" y="430" class="note">{escape(note)}</text></g>')
    parts.append('<text x="30" y="1085" class="note">Stylized structural comparisons, independently generated. These are proposals, not production Izzi output.</text></svg>')
    svg = '\n'.join(parts)+'\n'
    ET.fromstring(svg)
    args.output.write_text(svg)
    print(f'{args.output}: {paths} paths; {vertices} sampled vertices; finite coordinates, panel bounds and XML checked')


if __name__ == '__main__':
    main()
