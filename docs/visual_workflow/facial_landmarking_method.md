# Facial landmarking method (ai-time-to-die T1 masks)

Date: 2026-08-14

Status: working method for the ai-time-to-die draft-4 attempted render; not a
baseline, accessibility-conformance, publication, or release approval

## Purpose

The T1 face-mask channel places pure-primary guilloche/moire/surface-tension
line masks at detected eye and mouth points of faces in the source video
(Blade Runner cut-4). Mask placement quality depends entirely on the landmark
points, so draft-4 replaces the OpenCV Haar cascade primary detector with
MediaPipe FaceLandmarker while keeping the Haar path as a per-second
zero-dependency fallback.

## Detector

- **MediaPipe FaceLandmarker** (Tasks API, `mediapipe` 1.0.0, image running
  mode). Model: `face_landmarker.task` (float16), downloaded from
  `storage.googleapis.com/mediapipe-models/face_landmarker/face_landmarker/float16/1/`
  into `build/private/mediapipe/face_landmarker.task` (gitignored; sha256
  `64184e229b263107bc2b804c6625db1341ff2bb731874b0bcc2fe6544e0bc9ff`,
  Apache-2.0). The pip package does not bundle the `.task` asset.
- Options: `num_faces=2`, `min_face_detection_confidence=0.5`,
  `min_face_presence_confidence=0.5`; first face's landmarks are used.
- Fallback: OpenCV Haar (`haarcascade_frontalface_alt2` + eye + smile,
  `/usr/share/opencv4/haarcascades`) per second when MediaPipe finds no face.

## Landmark mapping

MediaPipe returns 478 normalized landmarks. The renderer maps three points to
pixel coordinates (normalized x/y times frame width/height):

| Point | MediaPipe indices | Derivation |
|---|---|---|
| Left eye (subject's left) | 33, 133 | midpoint of outer/inner corner |
| Right eye | 362, 263 | midpoint of outer/inner corner |
| Mouth | 13, 14 | midpoint of upper/lower inner lip center |

Eyes are sorted left-to-right by x so the mask triangle (eye1–eye2–mouth)
stays stable, matching the draft-2/3 triangle semantics. Iris landmarks
(468/473) are present in the model but not required; the corner midpoints are
used for stability.

## Cadence and output contract

- Detection runs once per source second (`int(duration)` frames at
  `start + second`), the same cadence as the draft-3 Haar pass, because the
  mask color/pattern assignment rotates once per second.
- Output shape (unchanged from draft-3 `face_triangles`):

```json
{
  "<second>": {
    "eyes": [[x1, y1], [x2, y2]],
    "mouth": [x, y]
  }
}
```

- Coordinates are in source-video pixels; `mask_frames` applies the existing
  crop/scale mapping (`crop_x = 656.5`, `scale = W/607`) unchanged.
- Missing eye or mouth points are skipped (a face with one visible eye
  produces one mask point), per review issue #19.

## Determinism and verification

- Detection frames are extracted with the same ffmpeg call and cached under
  `WORK/frames/detect-mp-*`.
- A second with no detected face is recorded and resolved through the Haar
  fallback; both detectors' results are written into the draft-4
  `render-notes.json` review record (counts of mediapipe-vs-fallback seconds
  are not yet surfaced as separate fields and can be added on request).
- The mask frame gate remains visual: reviewers inspect frames at the T1
  timecodes (0:06–0:26, 0:36–0:46) for mask placement on the face.

## Notes and caveats

- MediaPipe logs TFLite/XNNPACK setup warnings to stderr once per process;
  these are informational.
- Detection is CPU-only and per-second, so it is not the render bottleneck.
- The TFLite model runs through the Tasks API; the legacy `solutions` API was
  removed in mediapipe 1.0.0 and must not be used.
