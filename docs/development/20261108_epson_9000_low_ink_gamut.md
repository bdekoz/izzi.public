# Epson P9000 Commercial Edition: low-ink colors and gamut notes

Date: 2026-11-08. Target: **44-inch Epson SureColor P9000 Commercial Edition
(SCP9000CE), with violet ink, on Epson Metallic Photo Paper Glossy**.

The goal is to find attractive, legible color combinations that conserve the
two low cartridges, **Cyan (C)** and **Vivid Magenta (VM)**. This reference
consolidates the Cartofreako MSP proof discussion, the supplied preliminary
color-chart PDF, the anniversary bill work, and the user's subsequent printer,
paper and ink-inventory clarifications. Here, “low-ink gamut” names a practical
palette investigation; no measured reduced-gamut boundary has been established.

The most useful next file is the **ICC output profile for this exact printer,
violet ink set, paper and print mode**, accompanied by the active driver/media/
quality settings. That can help assess color appearance. Estimating which
colors conserve C and VM additionally requires the actual ink separation or
per-channel usage data from the driver/RIP, or controlled printing evidence.

## 1. Confirmed target and available ink

The user's final model confirmation and [Epson product link](https://epson.com/For-Work/Printers/Large-Format/Epson-SureColor-P9000-Commercial-Edition-Printer/p/SCP9000CE)
establish the P9000 Commercial Edition as the target. Epson specifies its
UltraChrome HDX configuration with violet in place of the Standard Edition's
Light Light Black. The earlier “Stylus Pro 9000” wording and the briefly linked
17-inch P900 are not the model to use when selecting this job's profile.

| Cartridge | User-reported state | Consequence for the investigation |
| --- | --- | --- |
| Cyan, C | Low | Minimize its consumption; no percentage or remaining volume supplied. |
| Vivid Magenta, VM | Low | Minimize its consumption independently of VLM. |
| Photo Black, PK | Among the most available | Relevant black for the confirmed glossy stock. |
| Matte Black, MK | Among the most available | Record it, but keep the black mode appropriate to the stock. |
| Vivid Light Magenta, VLM | Among the most available | Keep pale warm/lilac candidates in the proof; actual VM/VLM separation remains unknown. |
| Light Black, LK | Among the most available | Include neutral linework and gray ramps as controls. |
| Light Cyan, LC | Among the most available | Keep pale azure/blue candidates in the proof; actual C/LC separation remains unknown. |
| Yellow, Orange, Green | Not specified | Check levels before treating warm or green colors as cheap. |
| Violet | Present; amount not specified | Include light cobalt/violet comparisons, without assuming abundant violet. |

C and LC are separate cartridges, as are VM and VLM. “Low cyan/magenta” must
not erase the reported availability of their light counterparts. The fact
that a light ink is plentiful does not establish that the installed driver
will substitute it for a low full-strength ink.

The paper is specifically **Epson Metallic Photo Paper Glossy**, not Metallic
Photo Paper Luster or an unidentified metallic stock. Epson lists it among
the P9000 family's **Photo Black** media. Use its matching media configuration
and PK profile; the available MK cartridge is not a reason to choose a matte
paper setting. [Epson P-series User's Guide, p. 156](https://files.support.epson.com/docid/cpd5/cpd50227.pdf#page=156)

In these designs, an RGB `#FFFFFF` region is intended to leave the **metallic
paper base unprinted**. Its physical appearance is not the white of an ordinary screen
preview. Compare color and contrast under consistent illumination and viewing
angles. Epson's [paper description and profile instructions](https://epson.com/For-Work/Paper/Pro-Imaging/Metallic-Photo-Paper-Glossy/m/S045590)
identify the metallic glossy finish and direct users to printer-specific ICC
downloads. The linked product page represents one sheet size; the user's
loaded sheet/roll size has not been specified.

## 2. What printer data would help?

| Data to retain | What it establishes | What it does not establish |
| --- | --- | --- |
| Matching `.icc` / `.icm` output profile | Characterized color response for its printer/media/mode; useful for soft proofing and comparing colors. | Current cartridge levels or, for an ordinary RGB printer profile, physical C/LC/VM/VLM/V channel quantities. |
| Queue PPD, current options and available options | Configured model, filter/driver, media, resolution, color options and possible profile selectors. | That an application's per-job settings used those defaults, or that a selected driver matches the physical printer. |
| IPP supply/status attributes, or a dated control-panel record | Reported cartridge names/levels, warnings and printer identity where supplied. | Ink required by each proposed RGB color; level bars are not a separation model. |
| RIP media preset, ink limits, separation or per-channel job estimate, if available | Which physical channels the actual production path uses for a swatch/job. | Measured appearance unless paired with the corresponding profile/proof. |
| Measured test-chart data, such as CGATS/`.ti3`, with its settings | Observed color response and a basis for characterization. | A reliable current characterization after changing media, ink mode or separation settings. |

ICC files normally come from the driver/media installation or a profiling
workflow; do not assume the printer can export its own complete ICC profile.
An IPP `printer-icc-profiles` attribute, when provided, may advertise profile
URIs. A colord entry named `RGB` or `Gray` with no actual profile filename is
not by itself a paper-specific characterization file.

For this job, request **P9000 Commercial Edition / violet + Epson Metallic
Photo Paper Glossy + Photo Black + the actual quality/resolution and driver**.
Epson's [P9000 Commercial Edition support page](https://epson.com/Support/Printers/Single-Function-Inkjet-Printers/SureColor-Series/Epson-SureColor-P9000-Commercial-Edition/s/SPT_SCP9000CE)
has an ICC Profiles section. A specific matching downloadable file was not
verified during this consolidation. Prefer the profile actually used by the
working print setup; verify the media and ink-set metadata before substituting
a download. A P9000 Standard Edition/LLK profile or an unrelated “metallic”
paper profile is not interchangeable with the requested combination.

## 3. Retrieve the evidence on Fedora 44

### What was checked on this host

Read-only inspection found Fedora Linux **44 MATE-Compiz**, CUPS **2.4.19**,
colord **1.4.8**, system-config-printer **1.5.18**, and Gutenprint **5.3.5**.
`lpstat`, `lpoptions`, `ipptool`, `colormgr`, `system-config-printer`,
`gnome-control-center` and `gcm-viewer` are installed.

CUPS is active. Its only configured queue is
`EPSON-Epson-Stylus-Pro-3880`, and colord identifies that queue as a 3880.
**No queue identified as a P9000 or matching P9000 profile was retrieved here.**
This describes the configuration, not an independent identification of the
physical device behind that queue. Run the following steps on the computer
or print server that manages the P9000.

### A. Fedora MATE / graphical inspection

Open **Print Settings** from the desktop, or run:

```sh
system-config-printer
```

Select the P9000 queue and open **Properties**. Record **Settings → Make and
Model**, its connection, **Printer Options** for paper/quality/ink configuration,
and **Job Options**. Inspect **Ink/Toner Levels** if the backend supplies it.
These tab names were checked in the installed Fedora 44 application. Also
record the actual application's print-dialog settings, since they can override
queue defaults. Inspect without applying changes.

The CUPS browser interface at [local printers](http://localhost:631/printers/)
can also show queue/model/status information when the web interface is enabled.
If it is disabled, use the commands below; enabling it is unnecessary for
collecting this evidence.

In a GNOME session, **Settings → Color** exposes assigned printer profiles;
`gnome-control-center color` opens that panel. GNOME distinguishes the default
profile from other profiles that may match particular media. The CLI below
works independently of the desktop and gives the actual profile filename.
[GNOME profile-assignment help](https://help.gnome.org/gnome-help/color-assignprofiles.html)

### B. CUPS queue options and PPD

Run from the Izzi repository, replacing the queue placeholder with an existing
P9000 queue name. Set `epson_server` to the CUPS server hostname if it is remote
(the examples use the standard port). These commands read configuration and
write local evidence files; they do not submit a print job or change defaults.

```sh
epson_server='localhost'
lpstat -h "$epson_server" -p -d
lpstat -h "$epson_server" -v

epson_queue='REPLACE_WITH_ACTUAL_P9000_QUEUE'
epson_capture="$PWD/build/epson-low-ink-20261108"
mkdir -p "$epson_capture"

lpoptions -h "$epson_server" -p "$epson_queue" > "$epson_capture/options-current.txt"
lpoptions -h "$epson_server" -p "$epson_queue" -l > "$epson_capture/options-supported.txt"

curl --fail --location \
  "http://${epson_server}:631/printers/${epson_queue}.ppd" \
  --output "$epson_capture/queue.ppd"
```

`lpoptions` without `-o`, `-r` or `-d` reports options; `-l` includes available
choices and selected defaults. Keep both outputs. [OpenPrinting lpoptions reference](https://openprinting.github.io/cups/doc/man-lpoptions.html)

On the local CUPS server, `/etc/cups/ppd/QUEUE.ppd` is another source for a
PPD-based queue if permissions allow reading it. Some driverless or remote
setups will not expose a PPD; keep their IPP/options data instead. Inspect the
retrieved PPD, if present:

```sh
rg -n '^\*(NickName|ModelName|Product|cupsFilter|cupsICCProfile|cupsICCQualifier|Default)' \
  "$epson_capture/queue.ppd"
```

`cupsICCProfile` may name an ICC file selected by color model, media and
resolution; `cupsICCQualifier` can redefine those selectors. Relative profile
filenames in this PPD convention are relative to `/usr/share/cups/profiles`.
The PPD is a description of the queue's printing path, not a current ink-cost
table. [CUPS PPD color-profile extensions](https://www.cups.org/doc/spec-ppd.html#cupsICCProfile)

### C. Read cartridge and profile attributes through IPP

```sh
ipptool -tv -T 15 \
  "ipp://${epson_server}:631/printers/${epson_queue}" \
  /usr/share/cups/ipptool/get-printer-attributes.test \
  > "$epson_capture/printer-attributes.txt" 2>&1

rg -n -i 'printer-make-and-model|printer-state|marker-|printer-supply|printer-icc|media-|resolution|print-color' \
  "$epson_capture/printer-attributes.txt"
```

This uses `Get-Printer-Attributes`, not a print operation. Keep the full
response and any test failure message: the bundled test also checks for
attributes that an older device may omit. An unavailable attribute is not
proof that a cartridge is full or that no profile exists. If the printer
advertises a direct IPP endpoint, it can be queried separately; do not assume
a `socket://` connection is an IPP URL. [CUPS ipptool reference](https://www.cups.org/doc/man-ipptool.html)

Keep `marker-names`, `marker-colors`, `marker-levels`, `marker-types` and
`marker-message` together, preserving their order. CUPS defines 0–100 as
percentages; -1 means unavailable, -2 unknown, and -3 unknown/not at capacity.
Those negative values are not ink percentages. Supply reporting depends on
the backend/device and may be absent or stale; compare it with a dated printer
panel record. [OpenPrinting supply reporting](https://openprinting.github.io/cups/doc/api-filter.html)

### D. Locate and copy the actual ICC file

These commands query **local colord**, even if `epson_server` above is remote.
Run them on the host managing the production profiles:

```sh
colormgr get-devices-by-kind printer > "$epson_capture/colord-printers.txt"
colormgr get-profiles > "$epson_capture/colord-profiles.txt"

epson_color_device='REPLACE_WITH_P9000_DEVICE_ID'
colormgr device-get-default-profile "$epson_color_device" \
  > "$epson_capture/colord-default-profile.txt"

epson_profile_id='REPLACE_WITH_MATCHING_PROFILE_ID'
colormgr find-profile "$epson_profile_id" > "$epson_capture/colord-selected-profile.txt"
```

Read the selected profile's **Filename**, title, device kind and qualifiers.
Choose the profile for the actual media/mode; the default alone may not be
the one selected for this job. Copy the file at that reported path:

```sh
epson_icc='/absolute/path/from/the/profile/Filename.icc'
cp -- "$epson_icc" "$epson_capture/"
sha256sum "$epson_icc" > "$epson_capture/icc-sha256.txt"
gcm-viewer --file "$epson_icc"
```

`gcm-viewer` is an optional inspection step. Colord IDs and object paths are
metadata references, not file paths. If there is no filename, record that
result instead of creating an empty `.icc` file. Additional accessible profile
locations can be inspected with:

```sh
rg --files "$HOME/.local/share/icc" "$HOME/.color/icc" \
  /usr/share/color/icc /usr/share/cups/profiles /var/lib/colord/icc \
  2>/dev/null | rg -i '\.(icc|icm)$'
```

Keep the profile together with the PPD/options and application settings.
Merely installing or assigning an ICC profile does not establish that the
actual print application/filter used it. [Colord command-line reference](https://www.freedesktop.org/software/colord/using.html)

## 4. Reconstructing the web discussion from its PDF

The user identified [this web chat](https://chatgpt.com/g/g-p-6ac9e2516fa08191b8e414e56efcb3ac-art/c/6aed20a3-4634-83e8-b1ce-9f5fff7f9ffc)
as the source of epson_p9000_low_ink_color_chart.pdf.
The web fetch did not expose the conversation text. The user authorized
reconstruction from the supplied PDF, so the account below describes that
artifact, not an invented transcript or an inspection of the chat's original
printer photograph.

The PDF is one landscape Letter page, **792 × 612 pt**, with five starter
swatches and a twelve-hue, three-tone grid. Its working condition is P9000
with C and VM low. It favors small pale tests, thin marks and exposed paper,
and labels hue families by qualitative expected demand. It supplies neither
an ICC characterization nor measured channel-usage data. Its statement that
orange/green inks may help is an estimate, not evidence of a C/VM-free recipe.

The user's abundant LC/VLM/LK/black inventory and exact metallic glossy stock
are now part of this investigation; the PDF does not quantify those conditions.
Preserve its colors and labels as a starting record, then test them with the
actual production path.

### Exact starter colors

Values were read from PDF DeviceRGB `rg` fill operators, not sampled from a
screenshot. Convert each component with `floor(255 × component + 0.5)`; retain
the source decimal value when reproducing the chart.

| PDF starter swatch | Decimal RGB | 8-bit RGB | Hex |
| --- | --- | --- | --- |
| Paper white | `(1, 1, 1)` | `(255, 255, 255)` | `#FFFFFF` |
| Cream | `(1, .98, .87)` | `(255, 250, 222)` | `#FFFADE` |
| Pale yellow | `(1, 1, .72)` | `(255, 255, 184)` | `#FFFFB8` |
| Soft apricot | `(1, .9, .75)` | `(255, 230, 191)` | `#FFE6BF` |
| Pale green | `(.91, .98, .8)` | `(232, 250, 204)` | `#E8FACC` |

### Complete hue grid

The PDF's labels mean **F: prioritize pale trials**, **T: test with the profile**,
and **L: restrict large colored areas, especially saturated/dark ones**.
They are qualitative labels from the preliminary chart, not measured
printable/unprintable boundaries or guaranteed channel savings. An F does
not make a full saturated or dark fill inexpensive.

| Hue | Pale | Saturated | Dark | PDF label |
| --- | --- | --- | --- | --- |
| Red | `#FFCCCC` | `#FF0000` | `#660000` | L |
| Orange | `#FFE6CC` | `#FF8000` | `#663300` | T |
| Yellow | `#FFFFCC` | `#FFFF00` | `#666600` | F |
| Lime | `#E6FFCC` | `#80FF00` | `#336600` | T |
| Green | `#CCFFCC` | `#00FF00` | `#006600` | T |
| Teal | `#CCFFE6` | `#00FF80` | `#006633` | L |
| Cyan | `#CCFFFF` | `#00FFFF` | `#006666` | L |
| Azure | `#CCE6FF` | `#0080FF` | `#003366` | L |
| Blue | `#CCCCFF` | `#0000FF` | `#000066` | L |
| Violet | `#E6CCFF` | `#8000FF` | `#330066` | L |
| Magenta | `#FFCCFF` | `#FF00FF` | `#660066` | L |
| Rose | `#FFCCE6` | `#FF0080` | `#660033` | L |

The source grid uses saturated RGB triples in hue order
`(1,0,0)`, `(1,.5,0)`, `(1,1,0)`, `(.5,1,0)`, `(0,1,0)`, `(0,1,.5)`,
`(0,1,1)`, `(0,.5,1)`, `(0,0,1)`, `(.5,0,1)`, `(1,0,1)`, `(1,0,.5)`.
Its pale row is `.8 + .2 × saturated`; its dark row is `.4 × saturated`,
component by component. Those formulas reproduce the inspected operands;
they are chart construction, not an ink-mixing formula.

The anniversary project's **light peach** is Pale/Orange `#FFE6CC`, **lime**
is Pale/Lime `#E6FFCC`, **brown** is Dark/Orange `#663300`, and structural
**blue** is Dark/Azure `#003366`. **Light cobalt** is the project's name for
Pale/Blue `#CCCCFF`; it is not a separately labeled swatch in the PDF.
Compare it with Pale/Azure `#CCE6FF` and Pale/Violet `#E6CCFF` at equal areas.
Starter Pale Green differs from both Pale/Green and Pale/Lime; starter Pale
Yellow also differs from the hue-grid Pale/Yellow.

## 5. What the Cartofreako discussion contributes

The 4 November MSP formulation note
records metallic glossy paper, grays reportedly appearing light violet, a
preference for dark grounds, rejected earlier greens, and four approved
design pairs. Its historical printer/ink wording is “Stylus Pro 9000” and
low cyan/light magenta. For the current job, the later user confirmation in
section 1 establishes **P9000 Commercial Edition, low C/VM, plentiful VLM**.
Retain the earlier observations without converting them into a measured
color correction or a current claim that VLM is low.

| MSP pair | Ground RGB / hex | Foreground RGB / hex | Saved palette |
| --- | --- | --- | --- |
| Wine / amber | `(36,4,19)` / `#240413` | `(252,197,83)` / `#FCC553` | [Randoma11y](https://randoma11y.com/%23fcc553/%23240413) |
| Ultramarine / porcelain | `(25,63,184)` / `#193FB8` | `(245,234,231)` / `#F5EAE7` | [Randoma11y](https://randoma11y.com/%23193fb8/%23f5eae7) |
| Plum / ice blue | `(42,12,65)` / `#2A0C41` | `(115,222,249)` / `#73DEF9` | [Randoma11y](https://randoma11y.com/%2373def9/%232a0c41) |
| Midnight blue / champagne | `(1,4,52)` / `#010434` | `(234,215,132)` / `#EAD784` | [Randoma11y](https://randoma11y.com/%23ead784/%23010434) |

These are approved artwork palettes, not verified low-C/VM separations. Their
large dark grounds serve the earlier visual preference; compare a small
actual-size sample before adopting that coverage for the conservation goal.
The useful shared method is **the same artwork crop, at the same physical
scale, with explicit RGB labels and fixed print settings**.

Cartofreako's existing proof is `msp-4-ink-profile.20261104.svg/.png/.pdf`
under `output/msp-speculations-20261103/msp-speculation-5/`. It is a 44-inch ×
200 mm strip with four 10.5-inch panels, matching solid swatches, 12 pt
astronomy labels and 6 pt satellite labels. Its generator is
`scripts/build-msp-ink-profile.py` in that repository (`make msp-ink-profile`).
The name “ink-profile” denotes a comparison strip; the note reports completed
digital checks and no measured physical proof.

## 6. Color combinations to test first

Keep the desired peach/lime/brown/blue vocabulary and the requested light
cobalt. The additional neutral, pale blue and pale violet trials below respond to
the current inventory. All are **candidates**, pending the matching profile,
channel evidence and print comparison.

| Trial | Ground / pale field | Main marks | Purpose |
| --- | --- | --- | --- |
| N — metallic neutral control | Exposed stock `#FFFFFF` | `#000000`, with `#333333` / `#777777` ramps | New controls for available PK/LK and the reported violet-gray cast; RGB gray/black is not proof of black-only ink. |
| P — peach / brown | `#FFE6CC`, small lime `#E6FFCC` accents | Brown `#663300`, fine blue `#003366` | Existing warm anniversary direction; keep the blue area small. |
| L — lime / brown | `#E6FFCC`, small peach accents | Brown `#663300` | Existing complementary trial; verify Yellow/Green levels and output. |
| C — light cobalt / brown | `#CCCCFF`, small peach accents | Brown `#663300` | User-preferred blue candidate; test the actual violet/LC/VLM/C/VM separation. |
| A — pale azure / charcoal | `#CCE6FF`, exposed metallic stock | `#333333` | Compare a lighter blue field with C at equal area and mark density. |
| V — pale violet or rose / charcoal | Separate `#E6CCFF` and `#FFCCE6` patches | `#333333` | Explore the available light magenta and installed violet without assuming zero VM. |
| Y — cream / brown | `#FFFADE`, then `#FFFFB8` comparison | `#663300` | PDF starter direction, conditional on actual Yellow supply and print appearance. |

The neutral hex values are new proof controls, not extracted PDF swatches.
P, L and C correspond to the existing anniversary recipes; A, V and N are
comparisons to add to a subsequent physical proof, not changes already made
to the delivered cards.

Pale colors and sparse marks are useful design levers, but a large pale field
can still consume substantial ink. Abundant LC/VLM motivates these trials;
the driver decides where to transition to C/VM. Do not estimate tank demand
from `1-R`, `1-G`, `1-B`, a generic RGB-to-CMYK conversion, or the visible
amount of blue in a preview. Brown, gray and black can also be composite
colors in the active printing path.

For the anniversary cards, current layout targets are at least **40% exposed
paper** and roughly **8–12% dark painted area**. Delivered digital coverage
is 61.2–80.4% exposed paper, with four front-face advisories at 12.2–13.2%
dark coverage. These are rendered-pixel proxies, not cartridge measurements.
The [anniversary session](sessions/20261108.guilloche_anniversary_24_stage_1.md) and
palette recipes
retain the exact project parameters.

## 7. Proof sequence and decision record

1. Capture the P9000 CE identity, available ink levels, matching profile and
   actual media/quality settings. Confirm PK for this paper and keep the
   color-management path consistent.
2. Check nozzle condition before judging the violet cast or banding. Epson
   distinguishes low ink from an expended cartridge; printing cannot continue
   with an expended cartridge until it is replaced. The palette work does not
   bypass that condition. [P-series Quick Reference, pp. 29 and 35–37](https://files.support.epson.com/docid/cpd5/cpd50157.pdf)
3. Print a compact, labeled proof at **100% / actual size** on the actual
   Metallic Photo Paper Glossy. Reuse one portrait/ornament crop for every
   comparison; add the neutral and cobalt/azure/violet/rose patches at equal
   area. Keep a blank paper patch in the same sheet.
4. Use the existing ink/type/engraving proof
   for D/G portraits at three pitches, native guilloche, 7.5–9 pt Atkinson
   text, script lettering and the line/gap ladder. It contains cobalt and
   azure; the new N/V trials above are not already present in that file.
5. Compare recognition, text contrast, fine-line survival, neutral cast and
   metallic appearance. Record viewing light/angle and elapsed drying time.
   Compare ink estimates only for identical area/settings and distinguish
   image-ink estimates from cleaning or black-switch consumption.
6. Select combinations based on the physical result. If the RIP provides
   physical-channel estimates, rank acceptable results by C and VM use.
   Otherwise retain a qualitative conservation hypothesis rather than
   inventing percentages or claiming a measured low-ink gamut.

Record: date, queue/driver, printer/ink set, all reported cartridge levels,
paper product and loaded size, PK mode, resolution/quality, ICC filename/hash,
rendering intent and black-point compensation if exposed, which component
manages color, proof file/hash, viewing conditions, observed result, and any
per-channel estimate or measurement. Current missing evidence is the actual
P9000 queue/profile/settings, remaining amounts for Y/O/G/V, and a controlled
printed comparison. Printer model, paper family and the relative low/plentiful
ink inventory are now supplied by the user.

## 8. Provenance and scope

| Source | Use and access status |
| --- | --- |
| Cartofreako formulation note | Read directly; four original pairs, metallic-paper observations and actual-size proof method. |
| [User-linked web chat](https://chatgpt.com/g/g-p-6ac9e2516fa08191b8e414e56efcb3ac-art/c/6aed20a3-4634-83e8-b1ce-9f5fff7f9ffc) | Origin supplied by the user; conversation text inaccessible through web fetch. |
| Preliminary PDF | Visually inspected and text/vector operands extracted; 41 swatches reconstructed. |
| User follow-ups on 2026-11-08 | Confirm 44-inch P9000 CE/violet, low C/VM, plentiful PK/MK/VLM/LK/LC, and Epson Metallic Photo Paper Glossy. |
| Anniversary assets | Implemented palettes, source-derived portraits, actual-size proof and digital checks. |
| Epson, OpenPrinting/CUPS, colord and GNOME references linked above | Primary references for model/ink/media behavior and data retrieval; installed Fedora tools supplied local version/UI evidence. |

Source fingerprints:

```text
epson_p9000_low_ink_color_chart.pdf
fb6d3a21a1be127527c48e847eb5d5754bd1a71b32451a1166693dc5601b5ccc

20261104_msp_duotone_low_ink_formulation.md
cd8e3c0c1b0f647ad656dcebc3caf78dc2715492d41b4ad614b469be95ac1590

examples.data/guilloche-anniversary-24/palettes.json
35452995fcd314aafc465f3364f6c5d65c63e5464f119692ba82237180e2e196
```

The earlier documents and original PDF retain their historical wording.
Use this consolidation for the current model, ink and paper interpretation.
