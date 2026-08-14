#!/usr/bin/env python3
"""Build deterministic, transcript-free acoustic candidates for the pilot audio."""

from __future__ import annotations

import argparse
import hashlib
import json
import math
import os
import platform
import shutil
import subprocess
import sys
import tempfile
from importlib.metadata import version
from pathlib import Path
from typing import Any

os.environ.setdefault("OMP_NUM_THREADS", "1")
os.environ.setdefault("OPENBLAS_NUM_THREADS", "1")
os.environ.setdefault("MKL_NUM_THREADS", "1")

import numpy as np
from PIL import Image, ImageDraw, ImageFont


HERE = Path(__file__).resolve().parent
REPOSITORY = HERE.parents[1]
SOURCE_RELATIVE = Path(
    "resources/here-lies-trouble/seed-audio-corpus/here-lies-trouble-1.20260415.mp3"
)
SOURCE = REPOSITORY / SOURCE_RELATIVE
EXPECTED_SOURCE_SHA256 = (
    "32e984630b27abbe1acb505002e36059047a3a671de5526f872cffd24cb30d1e"
)
AUTHOR_INPUT_RELATIVE = Path(
    "examples/seedance2/audio-vertical-authoring.minimal.json"
)
AUTHOR_INPUT = REPOSITORY / AUTHOR_INPUT_RELATIVE
PROFILE_RELATIVE = Path(
    "examples/seedance2/audio-candidates-energy-1.profile.json"
)
PROFILE_PATH = REPOSITORY / PROFILE_RELATIVE
DEFAULT_OUTPUT_ROOT = (
    REPOSITORY / "tests/baselines/audio/here-lies-trouble-1"
)


class CandidateError(Exception):
    """The deterministic candidate run cannot continue safely."""


def canonical_json(value: Any) -> bytes:
    return json.dumps(
        value, ensure_ascii=True, separators=(",", ":"), sort_keys=True
    ).encode("utf-8")


def sha256_bytes(value: bytes) -> str:
    return hashlib.sha256(value).hexdigest()


def file_sha256(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as source:
        for block in iter(lambda: source.read(1024 * 1024), b""):
            digest.update(block)
    return digest.hexdigest()


def write_json(path: Path, value: Any) -> None:
    path.write_text(
        json.dumps(value, ensure_ascii=True, indent=2, sort_keys=True) + "\n",
        encoding="utf-8",
    )


def load_json(path: Path) -> Any:
    try:
        return json.loads(path.read_text(encoding="utf-8"))
    except (OSError, UnicodeError, json.JSONDecodeError) as error:
        raise CandidateError(f"cannot read JSON {path}: {error}") from error


def rounded(value: float, places: int) -> float:
    result = round(float(value), places)
    return 0.0 if result == 0.0 else result


def clip01(value: float) -> float:
    return min(1.0, max(0.0, float(value)))


def validate_profile(profile: dict[str, Any]) -> None:
    if profile.get("profile_name") != "izzi-audio-candidates-energy-1":
        raise CandidateError("unexpected analysis profile name")
    if profile.get("sample_rate_hz") != 16000:
        raise CandidateError("energy-1 requires a 16 kHz analysis master")
    if profile.get("vad", {}).get("state") != "UNAVAILABLE":
        raise CandidateError("energy-1 must report VAD as UNAVAILABLE")
    if profile.get("vad", {}).get("weight") != 0.0:
        raise CandidateError("unavailable VAD must retain zero weight")
    weights = profile.get("weights", {})
    if set(weights) != {
        "activity",
        "energy_change",
        "spectral_flux",
        "silence_boundary",
        "timeline_coverage",
        "feature_diversity",
    }:
        raise CandidateError("unexpected feature-weight set")
    if not math.isclose(sum(weights.values()), 1.0, abs_tol=1e-12):
        raise CandidateError("feature weights must sum to exactly one")
    windows = profile.get("candidate_windows", {})
    if windows.get("window_samples") != 160000:
        raise CandidateError("candidate window must contain 160,000 samples")
    if windows.get("coverage_bands") != 10:
        raise CandidateError("candidate profile must contain ten coverage bands")
    if windows.get("alternatives_per_band") != 3:
        raise CandidateError("candidate profile must retain three alternatives")
    if windows.get("alternative_minimum_start_separation_samples") != 160000:
        raise CandidateError("ranked alternatives must be separated by ten seconds")


def ffmpeg_evidence() -> tuple[str, str]:
    environment = os.environ.copy()
    environment["LC_ALL"] = "C"
    result = subprocess.run(
        ["ffmpeg", "-version"],
        check=True,
        capture_output=True,
        text=True,
        env=environment,
    )
    version_text = result.stdout
    version_line = version_text.splitlines()[0]
    if not version_line.startswith("ffmpeg version 7.1.5 "):
        raise CandidateError(
            f"ffmpeg 7.1.5 is required; observed {version_line!r}"
        )
    return version_line, sha256_bytes(version_text.encode("utf-8"))


def decode_source(
    source: Path, scratch_pcm: Path, profile: dict[str, Any]
) -> list[str]:
    arguments = []
    for argument in profile["decoder"]["arguments"]:
        arguments.append(
            argument.format(source=str(source), scratch_pcm=str(scratch_pcm))
        )
    command = ["ffmpeg", *arguments]
    environment = os.environ.copy()
    environment["LC_ALL"] = "C"
    result = subprocess.run(
        command,
        check=False,
        capture_output=True,
        text=True,
        env=environment,
    )
    if result.returncode != 0:
        raise CandidateError(
            f"ffmpeg decode failed with status {result.returncode}: "
            f"{result.stderr.strip()}"
        )
    if not scratch_pcm.is_file() or scratch_pcm.stat().st_size % 2:
        raise CandidateError("decoded PCM is missing or has an invalid byte count")
    return command


def extract_frame_features(
    pcm: np.ndarray, profile: dict[str, Any]
) -> dict[str, np.ndarray]:
    frame = profile["frame_analysis"]
    frame_samples = frame["frame_samples"]
    hop_samples = frame["hop_samples"]
    if len(pcm) < frame_samples:
        raise CandidateError("analysis master is shorter than one feature frame")
    frame_count = 1 + (len(pcm) - frame_samples) // hop_samples
    amplitude_scale = frame["amplitude_scale"]
    rms_floor = frame["rms_floor"]
    clipping_threshold = frame["clipping_threshold_pcm"]
    hann = np.hanning(frame_samples).astype(np.float64)
    frequencies = np.fft.rfftfreq(
        frame_samples, d=1.0 / profile["sample_rate_hz"]
    )

    rms = np.empty(frame_count, dtype=np.float64)
    rms_dbfs = np.empty(frame_count, dtype=np.float64)
    clipping_rate = np.empty(frame_count, dtype=np.float64)
    centroid_hz = np.empty(frame_count, dtype=np.float64)
    spectral_flux = np.empty(frame_count, dtype=np.float64)
    previous_spectrum: np.ndarray | None = None

    batch_frames = 2048
    for first in range(0, frame_count, batch_frames):
        last = min(frame_count, first + batch_frames)
        sample_first = first * hop_samples
        sample_last = (last - 1) * hop_samples + frame_samples
        block = np.asarray(pcm[sample_first:sample_last], dtype=np.int16)
        integer_frames = np.lib.stride_tricks.sliding_window_view(
            block, frame_samples
        )[::hop_samples][: last - first]
        floating_frames = integer_frames.astype(np.float64) / amplitude_scale

        block_rms = np.sqrt(np.mean(floating_frames * floating_frames, axis=1))
        rms[first:last] = block_rms
        rms_dbfs[first:last] = 20.0 * np.log10(
            np.maximum(block_rms, rms_floor)
        )
        clipping_rate[first:last] = np.mean(
            np.abs(integer_frames.astype(np.int32)) >= clipping_threshold,
            axis=1,
        )

        magnitudes = np.abs(
            np.fft.rfft(floating_frames * hann[np.newaxis, :], axis=1)
        )
        magnitude_sums = np.sum(magnitudes, axis=1, keepdims=True)
        normalized_spectra = np.divide(
            magnitudes,
            magnitude_sums,
            out=np.zeros_like(magnitudes),
            where=magnitude_sums > 0.0,
        )
        centroid_hz[first:last] = normalized_spectra @ frequencies

        block_flux = np.empty(last - first, dtype=np.float64)
        if previous_spectrum is None:
            block_flux[0] = 0.0
        else:
            block_flux[0] = np.linalg.norm(
                normalized_spectra[0] - previous_spectrum
            )
        if last - first > 1:
            block_flux[1:] = np.linalg.norm(
                normalized_spectra[1:] - normalized_spectra[:-1], axis=1
            )
        spectral_flux[first:last] = block_flux
        previous_spectrum = normalized_spectra[-1].copy()

    silent = rms_dbfs <= frame["silence_threshold_dbfs"]
    return {
        "rms": rms,
        "rms_dbfs": rms_dbfs,
        "clipping_rate": clipping_rate,
        "centroid_hz": centroid_hz,
        "spectral_flux": spectral_flux,
        "silent": silent,
    }


def prefix_sum(values: np.ndarray) -> np.ndarray:
    return np.concatenate(
        (np.array([0.0], dtype=np.float64), np.cumsum(values, dtype=np.float64))
    )


def range_mean(prefix: np.ndarray, first: int, last: int) -> float:
    if last <= first:
        return 0.0
    return float((prefix[last] - prefix[first]) / (last - first))


def candidate_identity(
    source_sha256: str,
    profile_sha256: str,
    start_sample: int,
    end_sample: int,
    raw_features: dict[str, float],
    normalized_features: dict[str, float],
) -> tuple[str, str]:
    feature_hash = sha256_bytes(
        canonical_json(
            {
                "normalized_features": normalized_features,
                "raw_features": raw_features,
            }
        )
    )
    identity = {
        "end_sample": end_sample,
        "feature_vector_sha256": feature_hash,
        "profile_sha256": profile_sha256,
        "source_sha256": source_sha256,
        "start_sample": start_sample,
    }
    return f"ec1-{sha256_bytes(canonical_json(identity))[:20]}", feature_hash


def enumerate_candidates(
    frame_features: dict[str, np.ndarray],
    decoded_samples: int,
    source_sha256: str,
    profile_sha256: str,
    profile: dict[str, Any],
) -> list[list[dict[str, Any]]]:
    frame = profile["frame_analysis"]
    windows = profile["candidate_windows"]
    parameters = profile["normalization_parameters"]
    weights = profile["weights"]
    frame_samples = frame["frame_samples"]
    frame_hop = frame["hop_samples"]
    window_samples = windows["window_samples"]
    grid_hop = windows["grid_hop_samples"]
    band_count = windows["coverage_bands"]
    if window_samples % frame_hop or grid_hop % frame_hop:
        raise CandidateError("window and grid must align with feature-frame hops")

    frames_per_window = 1 + (window_samples - frame_samples) // frame_hop
    energy_changes = np.abs(np.diff(frame_features["rms_dbfs"]))
    prefixes = {
        "rms_dbfs": prefix_sum(frame_features["rms_dbfs"]),
        "clipping_rate": prefix_sum(frame_features["clipping_rate"]),
        "centroid_hz": prefix_sum(frame_features["centroid_hz"]),
        "spectral_flux": prefix_sum(frame_features["spectral_flux"]),
        "silent": prefix_sum(frame_features["silent"].astype(np.float64)),
        "active": prefix_sum((~frame_features["silent"]).astype(np.float64)),
        "energy_changes": prefix_sum(energy_changes),
    }
    feature_places = parameters["feature_rounding_decimal_places"]
    score_places = parameters["score_rounding_decimal_places"]
    context_frames = parameters["boundary_context_frames"]
    all_bands: list[list[dict[str, Any]]] = []

    for band_index in range(band_count):
        band_start = (band_index * decoded_samples) // band_count
        band_end = ((band_index + 1) * decoded_samples) // band_count
        first_start = ((band_start + grid_hop - 1) // grid_hop) * grid_hop
        final_start = ((band_end - window_samples) // grid_hop) * grid_hop
        if final_start < first_start:
            raise CandidateError(
                f"coverage band {band_index + 1} cannot contain a full window"
            )
        feasible_center = (first_start + final_start) / 2.0
        feasible_half_span = (final_start - first_start) / 2.0
        band_candidates: list[dict[str, Any]] = []

        for start_sample in range(first_start, final_start + 1, grid_hop):
            end_sample = start_sample + window_samples
            first_frame = start_sample // frame_hop
            final_frame = first_frame + frames_per_window
            active_fraction = range_mean(
                prefixes["active"], first_frame, final_frame
            )
            clipping_fraction = range_mean(
                prefixes["clipping_rate"], first_frame, final_frame
            )
            energy_change = range_mean(
                prefixes["energy_changes"], first_frame, final_frame - 1
            )
            internal_flux = range_mean(
                prefixes["spectral_flux"], first_frame + 1, final_frame
            )
            mean_rms_dbfs = range_mean(
                prefixes["rms_dbfs"], first_frame, final_frame
            )
            mean_centroid_hz = range_mean(
                prefixes["centroid_hz"], first_frame, final_frame
            )

            pre_first = max(0, first_frame - context_frames)
            pre_silence = (
                prefixes["silent"][first_frame] - prefixes["silent"][pre_first]
            ) / context_frames
            boundary_end_frame = end_sample // frame_hop
            if boundary_end_frame >= len(frame_features["silent"]):
                post_silence = 0.0
            else:
                post_last = min(
                    len(frame_features["silent"]),
                    boundary_end_frame + context_frames,
                )
                post_silence = (
                    prefixes["silent"][post_last]
                    - prefixes["silent"][boundary_end_frame]
                ) / context_frames
            boundary_silence = (pre_silence + post_silence) / 2.0

            activity_score = clip01(
                active_fraction
                * (
                    1.0
                    - clip01(
                        parameters["activity_clipping_penalty_multiplier"]
                        * clipping_fraction
                    )
                )
            )
            energy_score = clip01(
                energy_change / parameters["energy_change_full_scale_db"]
            )
            flux_score = clip01(
                internal_flux / parameters["spectral_flux_full_scale"]
            )
            if feasible_half_span == 0.0:
                coverage_score = 1.0
            else:
                coverage_score = clip01(
                    1.0
                    - abs(start_sample - feasible_center) / feasible_half_span
                )

            raw_features = {
                "active_frame_fraction": rounded(active_fraction, feature_places),
                "clipping_frame_fraction": rounded(
                    clipping_fraction, feature_places
                ),
                "mean_frame_rms_dbfs": rounded(mean_rms_dbfs, feature_places),
                "mean_internal_energy_change_db": rounded(
                    energy_change, feature_places
                ),
                "mean_internal_spectral_flux": rounded(
                    internal_flux, feature_places
                ),
                "mean_spectral_centroid_hz": rounded(
                    mean_centroid_hz, feature_places
                ),
                "post_boundary_silence_fraction": rounded(
                    post_silence, feature_places
                ),
                "pre_boundary_silence_fraction": rounded(
                    pre_silence, feature_places
                ),
            }
            normalized_features = {
                "activity": rounded(activity_score, feature_places),
                "energy_change": rounded(energy_score, feature_places),
                "silence_boundary": rounded(boundary_silence, feature_places),
                "spectral_flux": rounded(flux_score, feature_places),
                "timeline_coverage": rounded(coverage_score, feature_places),
            }
            candidate_id, feature_hash = candidate_identity(
                source_sha256,
                profile_sha256,
                start_sample,
                end_sample,
                raw_features,
                normalized_features,
            )
            base_score = sum(
                weights[name] * normalized_features[name]
                for name in (
                    "activity",
                    "energy_change",
                    "spectral_flux",
                    "silence_boundary",
                    "timeline_coverage",
                )
            )
            band_candidates.append(
                {
                    "base_score_without_diversity": rounded(
                        base_score, score_places
                    ),
                    "candidate_id": candidate_id,
                    "coverage_band": band_index + 1,
                    "end_sample": end_sample,
                    "feature_vector_sha256": feature_hash,
                    "normalized_features": normalized_features,
                    "raw_features": raw_features,
                    "start_sample": start_sample,
                }
            )
        all_bands.append(band_candidates)
    return all_bands


def diversity_score(
    candidate: dict[str, Any], references: list[tuple[float, ...]], profile: dict[str, Any]
) -> float:
    if not references:
        return 1.0
    normalized = candidate["normalized_features"]
    vector = np.array(
        [
            normalized["activity"],
            normalized["energy_change"],
            normalized["spectral_flux"],
            normalized["silence_boundary"],
        ],
        dtype=np.float64,
    )
    distance = min(
        float(np.linalg.norm(vector - np.asarray(reference)))
        for reference in references
    )
    maximum = profile["normalization_parameters"][
        "diversity_max_euclidean_distance"
    ]
    return clip01(distance / maximum)


def diversity_vector(candidate: dict[str, Any]) -> tuple[float, ...]:
    normalized = candidate["normalized_features"]
    return (
        normalized["activity"],
        normalized["energy_change"],
        normalized["spectral_flux"],
        normalized["silence_boundary"],
    )


def rank_alternatives(
    candidates_by_band: list[list[dict[str, Any]]], profile: dict[str, Any]
) -> tuple[list[list[dict[str, Any]]], list[dict[str, Any]]]:
    alternatives_per_band = profile["candidate_windows"][
        "alternatives_per_band"
    ]
    diversity_weight = profile["weights"]["feature_diversity"]
    feature_places = profile["normalization_parameters"][
        "feature_rounding_decimal_places"
    ]
    score_places = profile["normalization_parameters"][
        "score_rounding_decimal_places"
    ]
    previous_system_vectors: list[tuple[float, ...]] = []
    ranked_bands: list[list[dict[str, Any]]] = []
    system_proposal: list[dict[str, Any]] = []

    for band_candidates in candidates_by_band:
        references = list(previous_system_vectors)
        used_ids: set[str] = set()
        used_ranges: list[tuple[int, int]] = []
        ranked: list[dict[str, Any]] = []
        for rank in range(1, alternatives_per_band + 1):
            scored: list[tuple[float, int, str, dict[str, Any], float]] = []
            for candidate in band_candidates:
                if candidate["candidate_id"] in used_ids:
                    continue
                if any(
                    candidate["start_sample"] < used_end
                    and used_start < candidate["end_sample"]
                    for used_start, used_end in used_ranges
                ):
                    continue
                diversity = diversity_score(candidate, references, profile)
                score = (
                    candidate["base_score_without_diversity"]
                    + diversity_weight * diversity
                )
                scored.append(
                    (
                        -rounded(score, score_places),
                        candidate["start_sample"],
                        candidate["candidate_id"],
                        candidate,
                        diversity,
                    )
                )
            if not scored:
                raise CandidateError("coverage band has too few distinct candidates")
            _, _, _, source_candidate, diversity = min(scored)
            winner = dict(source_candidate)
            winner["alternative_rank"] = rank
            winner["decision_status"] = (
                "PROPOSED-NOT-LOCKED"
                if rank == 1
                else "ALTERNATIVE-NOT-LOCKED"
            )
            winner["feature_diversity"] = rounded(diversity, feature_places)
            winner["score"] = rounded(
                winner["base_score_without_diversity"]
                + diversity_weight * diversity,
                score_places,
            )
            winner["system_proposal"] = rank == 1
            winner["user_decision"] = "UNAVAILABLE"
            ranked.append(winner)
            used_ids.add(winner["candidate_id"])
            used_ranges.append((winner["start_sample"], winner["end_sample"]))
            references.append(diversity_vector(winner))
            if rank == 1:
                system_proposal.append(winner)
        previous_system_vectors.append(diversity_vector(ranked[0]))
        ranked_bands.append(ranked)

    windows = profile["candidate_windows"]
    expected_samples = windows["window_samples"]
    if len(system_proposal) != windows["coverage_bands"]:
        raise CandidateError("system proposal does not contain ten candidates")
    ordered = sorted(system_proposal, key=lambda item: item["start_sample"])
    for candidate in ordered:
        if candidate["end_sample"] - candidate["start_sample"] != expected_samples:
            raise CandidateError("system proposal contains a non-ten-second window")
    for left, right in zip(ordered, ordered[1:]):
        if left["end_sample"] > right["start_sample"]:
            raise CandidateError("system proposal contains overlapping windows")
    return ranked_bands, system_proposal


def exact_segment_metrics(segment: np.ndarray, profile: dict[str, Any]) -> dict[str, float]:
    frame = profile["frame_analysis"]
    places = profile["normalization_parameters"][
        "feature_rounding_decimal_places"
    ]
    floating = segment.astype(np.float64) / frame["amplitude_scale"]
    rms = math.sqrt(float(np.mean(floating * floating)))
    peak = float(np.max(np.abs(segment.astype(np.int32)))) / frame[
        "amplitude_scale"
    ]
    clipping = float(
        np.mean(
            np.abs(segment.astype(np.int32)) >= frame["clipping_threshold_pcm"]
        )
    )
    return {
        "exact_peak_dbfs": rounded(
            20.0 * math.log10(max(peak, frame["rms_floor"])), places
        ),
        "exact_rms_dbfs": rounded(
            20.0 * math.log10(max(rms, frame["rms_floor"])), places
        ),
        "exact_sample_clipping_fraction": rounded(clipping, places),
    }


def save_png(image: Image.Image, path: Path) -> None:
    image.save(path, format="PNG", optimize=False, compress_level=9)


def draw_candidate_snapshot(
    segment: np.ndarray, candidate: dict[str, Any], path: Path
) -> None:
    width, height = 960, 360
    image = Image.new("RGB", (width, height), (0, 0, 0))
    drawing = ImageDraw.Draw(image)
    font = ImageFont.load_default()
    cyan = (64, 220, 235)
    white = (245, 245, 245)
    gray = (115, 125, 130)
    drawing.text((24, 14), "ACOUSTIC EVIDENCE ONLY - NO TRANSCRIPT", fill=white, font=font)
    drawing.text(
        (24, 34),
        f"{candidate['candidate_id']}  band {candidate['coverage_band']:02d}  "
        f"rank {candidate['alternative_rank']}  {candidate['decision_status']}",
        fill=cyan,
        font=font,
    )
    drawing.text(
        (24, 54),
        f"samples [{candidate['start_sample']}, {candidate['end_sample']})  "
        f"seconds [{candidate['start_seconds']:.3f}, {candidate['end_seconds']:.3f})  "
        f"score {candidate['score']:.6f}",
        fill=white,
        font=font,
    )

    waveform_left, waveform_top = 24, 88
    waveform_width, waveform_height = 912, 104
    drawing.rectangle(
        (
            waveform_left,
            waveform_top,
            waveform_left + waveform_width - 1,
            waveform_top + waveform_height - 1,
        ),
        outline=gray,
    )
    drawing.line(
        (
            waveform_left,
            waveform_top + waveform_height // 2,
            waveform_left + waveform_width - 1,
            waveform_top + waveform_height // 2,
        ),
        fill=(35, 45, 48),
    )
    edges = np.linspace(0, len(segment), waveform_width + 1, dtype=np.int64)
    center = waveform_top + waveform_height // 2
    scale = (waveform_height // 2 - 3) / 32768.0
    for column in range(waveform_width):
        values = segment[edges[column] : edges[column + 1]]
        low = int(round(center - float(np.max(values)) * scale))
        high = int(round(center - float(np.min(values)) * scale))
        drawing.line(
            (waveform_left + column, low, waveform_left + column, high), fill=cyan
        )

    floating = segment.astype(np.float64) / 32768.0
    spectrum_frame = 512
    spectrum_hop = 256
    spectrum_frames = np.lib.stride_tricks.sliding_window_view(
        floating, spectrum_frame
    )[::spectrum_hop]
    magnitudes = np.abs(
        np.fft.rfft(
            spectrum_frames * np.hanning(spectrum_frame)[np.newaxis, :], axis=1
        )
    )
    db = 20.0 * np.log10(np.maximum(magnitudes, 1e-12))
    db -= float(np.max(db))
    intensity = np.clip((db + 80.0) / 80.0, 0.0, 1.0)
    intensity = np.flipud((intensity.T * 255.0).astype(np.uint8))
    spectrogram_rgb = np.stack(
        (
            intensity // 5,
            (intensity.astype(np.uint16) * 4 // 5).astype(np.uint8),
            intensity,
        ),
        axis=2,
    )
    spectrogram = Image.fromarray(spectrogram_rgb).resize(
        (waveform_width, 116), resample=Image.Resampling.NEAREST
    )
    image.paste(spectrogram, (waveform_left, 212))
    drawing.rectangle((24, 212, 935, 327), outline=gray)
    raw = candidate["raw_features"]
    drawing.text(
        (24, 336),
        f"active {raw['active_frame_fraction']:.3f}  "
        f"RMS {candidate['exact_metrics']['exact_rms_dbfs']:.2f} dBFS  "
        f"flux {raw['mean_internal_spectral_flux']:.5f}  "
        f"centroid {raw['mean_spectral_centroid_hz']:.1f} Hz  "
        "user choice UNAVAILABLE",
        fill=white,
        font=font,
    )
    save_png(image, path)


def draw_contact_sheet(
    ranked_bands: list[list[dict[str, Any]]], package: Path, output: Path
) -> None:
    thumb_width, thumb_height = 400, 150
    heading_height = 40
    sheet = Image.new(
        "RGB", (thumb_width * 3, heading_height + thumb_height * 10), (0, 0, 0)
    )
    drawing = ImageDraw.Draw(sheet)
    font = ImageFont.load_default()
    drawing.text(
        (12, 10),
        "HERE LIES TROUBLE - 10 BANDS x 3 ACOUSTIC ALTERNATIVES - CHOICES UNAVAILABLE",
        fill=(245, 245, 245),
        font=font,
    )
    for band_index, alternatives in enumerate(ranked_bands):
        for column, candidate in enumerate(alternatives):
            source = package / candidate["snapshot"]["path"]
            with Image.open(source) as snapshot:
                thumbnail = snapshot.convert("RGB").resize(
                    (thumb_width, thumb_height), resample=Image.Resampling.NEAREST
                )
            sheet.paste(
                thumbnail,
                (column * thumb_width, heading_height + band_index * thumb_height),
            )
    save_png(sheet, output)


def add_snapshot_evidence(
    pcm: np.ndarray,
    ranked_bands: list[list[dict[str, Any]]],
    package: Path,
    profile: dict[str, Any],
) -> None:
    snapshots = package / "snapshots"
    snapshots.mkdir()
    sample_rate = profile["sample_rate_hz"]
    for alternatives in ranked_bands:
        for candidate in alternatives:
            start = candidate["start_sample"]
            end = candidate["end_sample"]
            segment = np.asarray(pcm[start:end], dtype=np.int16)
            if len(segment) != profile["candidate_windows"]["window_samples"]:
                raise CandidateError("snapshot segment has an invalid sample count")
            candidate["start_seconds"] = rounded(start / sample_rate, 6)
            candidate["end_seconds"] = rounded(end / sample_rate, 6)
            candidate["duration_seconds"] = 10.0
            candidate["sample_count"] = len(segment)
            candidate["exact_metrics"] = exact_segment_metrics(segment, profile)
            relative = Path("snapshots") / (
                f"band-{candidate['coverage_band']:02d}-"
                f"rank-{candidate['alternative_rank']}-"
                f"{candidate['candidate_id']}.png"
            )
            absolute = package / relative
            draw_candidate_snapshot(segment, candidate, absolute)
            candidate["snapshot"] = {
                "path": relative.as_posix(),
                "sha256": file_sha256(absolute),
            }


def negotiation_markdown(
    package_id: str,
    source_sha256: str,
    profile_sha256: str,
    decoded_samples: int,
    ranked_bands: list[list[dict[str, Any]]],
    system_proposal: list[dict[str, Any]],
) -> str:
    lines = [
        "# Here Lies Trouble acoustic-candidate negotiation sheet",
        "",
        f"Package: `{package_id}`  ",
        "Workflow state: `CANDIDATES-READY`  ",
        "System proposal: `PROPOSED-NOT-LOCKED`  ",
        "Selected choices: `UNAVAILABLE`  ",
        "User acceptance: `UNAVAILABLE`",
        "",
        "This sheet exposes local acoustic evidence only. It contains no transcript,",
        "dialogue label, speaker or identity inference, emotion claim, or semantic",
        "summary. The ten rank-one entries are a system proposal for negotiation, not",
        "chosen scenes and not a summary of the complete recording. Silence never",
        "constitutes acceptance.",
        "",
        f"- Source SHA-256: `{source_sha256}`",
        f"- Profile SHA-256: `{profile_sha256}`",
        f"- Decoded samples: `{decoded_samples}` at 16,000 Hz",
        "- Every candidate: exactly 160,000 samples (10 seconds)",
        "- VAD: `UNAVAILABLE`; profile: `izzi-audio-candidates-energy-1`",
        "",
        "## Ten-band system proposal",
        "",
        "| Band | Candidate | Time [start, end) s | Score | Decision |",
        "|---:|---|---:|---:|---|",
    ]
    for candidate in system_proposal:
        lines.append(
            f"| {candidate['coverage_band']:02d} | `{candidate['candidate_id']}` | "
            f"[{candidate['start_seconds']:.3f}, {candidate['end_seconds']:.3f}) | "
            f"{candidate['score']:.6f} | PROPOSED-NOT-LOCKED |"
        )
    lines.extend(
        [
            "",
            "## Ranked alternatives",
            "",
            "`Activity` is the non-silent frame fraction after the fixed clipping",
            "penalty. `Energy delta`, `flux`, and `boundary silence` are measured",
            "acoustic features, not claims about the recording's meaning.",
            "",
        ]
    )
    for band_index, alternatives in enumerate(ranked_bands, 1):
        band_start = (band_index - 1) * decoded_samples // 10 / 16000
        band_end = band_index * decoded_samples // 10 / 16000
        lines.extend(
            [
                f"### Coverage band {band_index:02d}",
                "",
                f"Band bounds: [{band_start:.6f}, {band_end:.6f}) seconds.",
                "",
                "| Rank | Candidate | Time [start, end) s | Activity | RMS dBFS | "
                "Energy delta dB | Flux | Boundary silence | Score | User choice |",
                "|---:|---|---:|---:|---:|---:|---:|---:|---:|---|",
            ]
        )
        for candidate in alternatives:
            raw = candidate["raw_features"]
            normalized = candidate["normalized_features"]
            lines.append(
                f"| {candidate['alternative_rank']} | `{candidate['candidate_id']}` | "
                f"[{candidate['start_seconds']:.3f}, {candidate['end_seconds']:.3f}) | "
                f"{normalized['activity']:.4f} | "
                f"{candidate['exact_metrics']['exact_rms_dbfs']:.3f} | "
                f"{raw['mean_internal_energy_change_db']:.5f} | "
                f"{raw['mean_internal_spectral_flux']:.6f} | "
                f"{normalized['silence_boundary']:.4f} | "
                f"{candidate['score']:.6f} | UNAVAILABLE |"
            )
        lines.extend(
            [
                "",
                f"Snapshot row: [contact sheet](contact-sheet.png); individual PNGs "
                f"are under `snapshots/` for band {band_index:02d}.",
                "",
            ]
        )
    lines.extend(
        [
            "## Human gate",
            "",
            "The next state is `NEGOTIATING`, but it has not begun. A human may later",
            "accept, reject, request an alternative, or adjust a boundary. No entry can",
            "become `TEN-LOCKED` without ten explicit decisions and a new preserved",
            "decision record.",
            "",
        ]
    )
    return "\n".join(lines)


def logical_decoder_arguments(profile: dict[str, Any]) -> list[str]:
    return [
        argument.format(
            source=SOURCE_RELATIVE.as_posix(),
            scratch_pcm="SCRATCH/analysis-master.s16le",
        )
        for argument in profile["decoder"]["arguments"]
    ]


def build_manifest(
    package_id: str,
    run_identity_sha256: str,
    source_sha256: str,
    author_input_sha256: str,
    profile_sha256: str,
    profile: dict[str, Any],
    decoded_samples: int,
    pcm_sha256: str,
    ffmpeg_version_line: str,
    ffmpeg_build_sha256: str,
    ranked_bands: list[list[dict[str, Any]]],
    system_proposal: list[dict[str, Any]],
    contact_sheet_sha256: str,
    negotiation_sheet_sha256: str,
) -> dict[str, Any]:
    sample_rate = profile["sample_rate_hz"]
    band_records = []
    for band_index, alternatives in enumerate(ranked_bands):
        band_start = band_index * decoded_samples // 10
        band_end = (band_index + 1) * decoded_samples // 10
        band_records.append(
            {
                "alternatives": alternatives,
                "band": band_index + 1,
                "end_sample": band_end,
                "end_seconds": rounded(band_end / sample_rate, 6),
                "start_sample": band_start,
                "start_seconds": rounded(band_start / sample_rate, 6),
            }
        )
    proposal_records = [
        {
            "candidate_id": candidate["candidate_id"],
            "coverage_band": candidate["coverage_band"],
            "decision_status": "PROPOSED-NOT-LOCKED",
            "end_sample": candidate["end_sample"],
            "end_seconds": candidate["end_seconds"],
            "sample_count": candidate["sample_count"],
            "start_sample": candidate["start_sample"],
            "start_seconds": candidate["start_seconds"],
            "user_decision": "UNAVAILABLE",
        }
        for candidate in system_proposal
    ]
    return {
        "analysis_master": {
            "channels": 1,
            "decoded_sample_count": decoded_samples,
            "duration_seconds": rounded(decoded_samples / sample_rate, 6),
            "format": "signed-16-bit-little-endian",
            "path": "SCRATCH/analysis-master.s16le",
            "retained": False,
            "sample_rate_hz": sample_rate,
            "sha256": pcm_sha256,
        },
        "artifacts": {
            "contact_sheet": {
                "path": "contact-sheet.png",
                "sha256": contact_sheet_sha256,
            },
            "negotiation_sheet": {
                "path": "NEGOTIATION.md",
                "sha256": negotiation_sheet_sha256,
            },
            "snapshots": {
                "count": 30,
                "hashes_recorded_per_candidate": True,
            },
        },
        "author_input": {
            "path": AUTHOR_INPUT_RELATIVE.as_posix(),
            "sha256": author_input_sha256,
            "visual_style_used_for_candidate_ranking": False,
        },
        "coverage_bands": band_records,
        "decoder": {
            "arguments": logical_decoder_arguments(profile),
            "ffmpeg_build_evidence_sha256": ffmpeg_build_sha256,
            "ffmpeg_version": ffmpeg_version_line,
            "scratch_pcm_retained": False,
        },
        "evidence_status": "MEASURED-LOCAL",
        "manifest_version": 1,
        "package_id": package_id,
        "privacy": {
            "audio_excerpt_files_retained": False,
            "network_used": False,
            "provider_called": False,
            "semantic_dialogue_labels": False,
            "transcript_state": "NOT-READ",
        },
        "profile": {
            "contract": profile,
            "name": "izzi-audio-candidates-energy-1",
            "path": PROFILE_RELATIVE.as_posix(),
            "sha256": profile_sha256,
            "vad_state": "UNAVAILABLE",
        },
        "run_identity_sha256": run_identity_sha256,
        "selected_choices": "UNAVAILABLE",
        "source": {
            "channels": 1,
            "codec": "mp3",
            "duration_seconds": rounded(decoded_samples / sample_rate, 6),
            "path": SOURCE_RELATIVE.as_posix(),
            "sample_rate_hz": 16000,
            "sha256": source_sha256,
        },
        "state": "CANDIDATES-READY",
        "system_proposal": {
            "candidate_count": 10,
            "items": proposal_records,
            "status": "PROPOSED-NOT-LOCKED",
            "user_acceptance": "UNAVAILABLE",
        },
        "toolchain": {
            "analyzer": {
                "path": "examples/seedance2/analyze-audio-candidates-energy.py",
                "sha256": file_sha256(Path(__file__).resolve()),
            },
            "numpy": np.__version__,
            "pillow": version("Pillow"),
            "python": platform.python_version(),
        },
        "transfer_authorized": False,
        "transfer_state": "NOT-SHARED",
    }


def write_sha256sums(package: Path) -> None:
    paths = sorted(
        path
        for path in package.rglob("*")
        if path.is_file() and path.name != "SHA256SUMS"
    )
    text = "".join(
        f"{file_sha256(path)}  {path.relative_to(package).as_posix()}\n"
        for path in paths
    )
    (package / "SHA256SUMS").write_text(text, encoding="utf-8")


def tree_hashes(root: Path) -> dict[str, str]:
    return {
        path.relative_to(root).as_posix(): file_sha256(path)
        for path in sorted(root.rglob("*"))
        if path.is_file()
    }


def preserve_or_verify(stage: Path, final: Path) -> str:
    if final.exists():
        if not final.is_dir():
            raise CandidateError(f"candidate package path is not a directory: {final}")
        staged_hashes = tree_hashes(stage)
        preserved_hashes = tree_hashes(final)
        if staged_hashes != preserved_hashes:
            differing = sorted(
                set(staged_hashes) ^ set(preserved_hashes)
                | {
                    path
                    for path in set(staged_hashes) & set(preserved_hashes)
                    if staged_hashes[path] != preserved_hashes[path]
                }
            )
            raise CandidateError(
                "resumed output differs from the preserved package: "
                + ", ".join(differing)
            )
        return "RESUME-VERIFIED-BYTE-IDENTICAL"
    stage.rename(final)
    return "PRESERVED-NEW-PACKAGE"


def build_package(
    output_root: Path,
    pcm: np.ndarray,
    source_sha256: str,
    author_input_sha256: str,
    profile_sha256: str,
    profile: dict[str, Any],
    pcm_sha256: str,
    ffmpeg_version_line: str,
    ffmpeg_build_sha256: str,
) -> tuple[Path, str]:
    frame_features = extract_frame_features(pcm, profile)
    candidates = enumerate_candidates(
        frame_features,
        len(pcm),
        source_sha256,
        profile_sha256,
        profile,
    )
    ranked_bands, system_proposal = rank_alternatives(candidates, profile)
    run_identity = {
        "analysis_master_sha256": pcm_sha256,
        "analyzer_sha256": file_sha256(Path(__file__).resolve()),
        "ffmpeg_build_evidence_sha256": ffmpeg_build_sha256,
        "numpy_version": np.__version__,
        "pillow_version": version("Pillow"),
        "profile_sha256": profile_sha256,
        "python_version": platform.python_version(),
        "source_sha256": source_sha256,
    }
    run_identity_sha256 = sha256_bytes(canonical_json(run_identity))
    package_id = (
        f"izzi-audio-candidates-energy-1-{run_identity_sha256[:16]}"
    )

    output_root.mkdir(parents=True, exist_ok=True)
    temporary_name = tempfile.mkdtemp(prefix=".candidate-stage-", dir=output_root)
    stage = Path(temporary_name)
    try:
        add_snapshot_evidence(pcm, ranked_bands, stage, profile)
        contact_sheet = stage / "contact-sheet.png"
        draw_contact_sheet(ranked_bands, stage, contact_sheet)
        negotiation = negotiation_markdown(
            package_id,
            source_sha256,
            profile_sha256,
            len(pcm),
            ranked_bands,
            system_proposal,
        )
        negotiation_path = stage / "NEGOTIATION.md"
        negotiation_path.write_text(negotiation, encoding="utf-8")
        manifest = build_manifest(
            package_id,
            run_identity_sha256,
            source_sha256,
            author_input_sha256,
            profile_sha256,
            profile,
            len(pcm),
            pcm_sha256,
            ffmpeg_version_line,
            ffmpeg_build_sha256,
            ranked_bands,
            system_proposal,
            file_sha256(contact_sheet),
            file_sha256(negotiation_path),
        )
        write_json(stage / "manifest.json", manifest)
        write_sha256sums(stage)
        final = output_root / package_id
        disposition = preserve_or_verify(stage, final)
        if disposition == "PRESERVED-NEW-PACKAGE":
            stage = final
        return final, disposition
    finally:
        if stage.exists() and stage.name.startswith(".candidate-stage-"):
            shutil.rmtree(stage)


def validate_author_link() -> str:
    author = load_json(AUTHOR_INPUT)
    if author.get("audio_path") != SOURCE_RELATIVE.as_posix():
        raise CandidateError("compact author input does not name the pinned pilot")
    return file_sha256(AUTHOR_INPUT)


def analyze_pilot(output_root: Path) -> tuple[Path, str]:
    source_sha256 = file_sha256(SOURCE)
    if source_sha256 != EXPECTED_SOURCE_SHA256:
        raise CandidateError(
            f"source hash mismatch: expected {EXPECTED_SOURCE_SHA256}, "
            f"observed {source_sha256}"
        )
    profile = load_json(PROFILE_PATH)
    validate_profile(profile)
    profile_sha256 = file_sha256(PROFILE_PATH)
    author_input_sha256 = validate_author_link()
    ffmpeg_version_line, ffmpeg_build_sha256 = ffmpeg_evidence()

    with tempfile.TemporaryDirectory(
        prefix="izzi-audio-analysis-master-", dir="/tmp"
    ) as scratch_directory:
        scratch_pcm = Path(scratch_directory) / "analysis-master.s16le"
        decode_source(SOURCE, scratch_pcm, profile)
        pcm_sha256 = file_sha256(scratch_pcm)
        decoded_samples = scratch_pcm.stat().st_size // 2
        pcm = np.memmap(scratch_pcm, mode="r", dtype="<i2", shape=(decoded_samples,))
        package, disposition = build_package(
            output_root,
            pcm,
            source_sha256,
            author_input_sha256,
            profile_sha256,
            profile,
            pcm_sha256,
            ffmpeg_version_line,
            ffmpeg_build_sha256,
        )
        del pcm
    return package, disposition


def synthetic_check() -> None:
    profile = load_json(PROFILE_PATH)
    validate_profile(profile)
    profile_sha256 = file_sha256(PROFILE_PATH)
    sample_rate = profile["sample_rate_hz"]
    synthetic_band_seconds = 36
    decoded_samples = 10 * synthetic_band_seconds * sample_rate
    samples = np.arange(decoded_samples, dtype=np.int64)
    phase = samples.astype(np.float64) / sample_rate
    band_samples = synthetic_band_seconds * sample_rate
    carrier = np.sin(
        2.0
        * np.pi
        * (180.0 + (samples // band_samples) * 35.0)
        * phase
    )
    gate = ((samples % band_samples) >= sample_rate // 2) & (
        (samples % band_samples) < band_samples - sample_rate // 2
    )
    pcm = np.where(gate, carrier * 12000.0, 0.0).astype(np.int16)
    source_sha256 = sha256_bytes(pcm.astype("<i2", copy=False).tobytes())
    frame_features = extract_frame_features(pcm, profile)
    candidates = enumerate_candidates(
        frame_features,
        decoded_samples,
        source_sha256,
        profile_sha256,
        profile,
    )
    first_ranked, first_proposal = rank_alternatives(candidates, profile)
    second_ranked, second_proposal = rank_alternatives(candidates, profile)
    if canonical_json(first_ranked) != canonical_json(second_ranked):
        raise CandidateError("synthetic ranking is not deterministic")
    if canonical_json(first_proposal) != canonical_json(second_proposal):
        raise CandidateError("synthetic proposal is not deterministic")
    if len(first_ranked) != 10 or any(len(band) != 3 for band in first_ranked):
        raise CandidateError("synthetic run did not retain 10 x 3 alternatives")
    for candidate in first_proposal:
        if candidate["end_sample"] - candidate["start_sample"] != 160000:
            raise CandidateError("synthetic proposal contains an invalid duration")
    ordered = sorted(first_proposal, key=lambda item: item["start_sample"])
    if any(
        left["end_sample"] > right["start_sample"]
        for left, right in zip(ordered, ordered[1:])
    ):
        raise CandidateError("synthetic proposal overlaps")
    print(
        "PASS synthetic deterministic profile "
        f"bands=10 alternatives=30 proposal=10 profile_sha256={profile_sha256}"
    )


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--output-root",
        type=Path,
        default=DEFAULT_OUTPUT_ROOT,
        help="parent directory for the content-addressed preserved package",
    )
    parser.add_argument(
        "--self-test",
        action="store_true",
        help="run deterministic synthetic feature and ranking checks only",
    )
    return parser.parse_args()


def main() -> int:
    arguments = parse_args()
    try:
        if arguments.self_test:
            synthetic_check()
        else:
            package, disposition = analyze_pilot(arguments.output_root)
            hashes = tree_hashes(package)
            manifest = load_json(package / "manifest.json")
            print(f"PASS source_sha256={manifest['source']['sha256']}")
            print(
                "PASS analysis_master "
                f"samples={manifest['analysis_master']['decoded_sample_count']} "
                f"sha256={manifest['analysis_master']['sha256']} retained=false"
            )
            print(
                "PASS candidates state=CANDIDATES-READY bands=10 alternatives=30 "
                "proposal=10 status=PROPOSED-NOT-LOCKED selected=UNAVAILABLE"
            )
            print(f"PASS package={package} disposition={disposition}")
            print(
                f"PASS manifest_sha256={hashes['manifest.json']} "
                f"contact_sheet_sha256={hashes['contact-sheet.png']} "
                f"files={len(hashes)}"
            )
    except (CandidateError, OSError, subprocess.SubprocessError) as error:
        print(f"FAIL {error}", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
