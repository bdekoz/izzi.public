#!/usr/bin/env python3
"""Validate and resolve the hash-bound audio negotiation contract offline."""

from __future__ import annotations

import argparse
import copy
import hashlib
import json
import sys
import tempfile
from importlib.metadata import version
from itertools import combinations
from pathlib import Path, PurePosixPath, PureWindowsPath
from typing import Any

from jsonschema import Draft202012Validator
from jsonschema.exceptions import SchemaError


HERE = Path(__file__).resolve().parent
REPOSITORY = HERE.parents[1]
SCHEMA = HERE / "audio-negotiation-1.schema.json"
REAL_SKELETON = HERE / "audio-negotiation.unavailable.json"
FIXTURES = HERE / "audio-negotiation.fixtures"
AUTHORIZED_MANIFEST_ROOT = REPOSITORY / "tests/baselines/audio"
EXPECTED_MANIFEST_RELATIVE = Path(
    "tests/baselines/audio/here-lies-trouble-1/"
    "izzi-audio-candidates-energy-1-4ebd1fc24c7fb66d/manifest.json"
)
EXPECTED_MANIFEST_SHA256 = (
    "a3cd27642c777f62415a8fd562b152f08a0735f001c4bd920ba9c41b2c4fa141"
)
EXPECTED_VALID_FIXTURES = {
    "synthetic-all-selections.json": "TEN-LOCKED",
    "synthetic-nonlocking-actions.json": "NEGOTIATING",
}
EXPECTED_INVALID_FIXTURES = {
    "absolute-path.json",
    "duplicate-bands.json",
    "false-lock-flag.json",
    "false-lock-claim.json",
    "hash-mismatch.json",
    "manifest-path-mismatch.json",
    "missing-band.json",
    "system-sourced-selection.json",
    "traversal-path.json",
    "unknown-candidate.json",
    "unsupported-action.json",
}


class NegotiationRejected(Exception):
    """The input, authority evidence, or candidate invariant is invalid."""


def canonical_json(value: Any) -> bytes:
    return json.dumps(
        value, ensure_ascii=True, separators=(",", ":"), sort_keys=True
    ).encode("utf-8")


def file_sha256(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as source:
        for block in iter(lambda: source.read(65536), b""):
            digest.update(block)
    return digest.hexdigest()


def load_json(path: Path) -> Any:
    try:
        return json.loads(path.read_text(encoding="utf-8"))
    except (OSError, UnicodeError, json.JSONDecodeError) as error:
        raise NegotiationRejected(f"cannot read JSON {path}: {error}") from error


def load_validator() -> Draft202012Validator:
    schema = load_json(SCHEMA)
    try:
        Draft202012Validator.check_schema(schema)
    except SchemaError as error:
        raise NegotiationRejected(f"invalid Draft 2020-12 schema: {error}") from error
    return Draft202012Validator(schema)


def reject_schema_errors(
    validator: Draft202012Validator, instance: Any
) -> None:
    errors = sorted(validator.iter_errors(instance), key=lambda error: error.json_path)
    if errors:
        first = errors[0]
        raise NegotiationRejected(f"schema {first.json_path}: {first.message}")


def resolve_manifest_reference(
    reference: dict[str, str],
    repository: Path = REPOSITORY,
    authorized_root: Path = AUTHORIZED_MANIFEST_ROOT,
    expected_relative: Path = EXPECTED_MANIFEST_RELATIVE,
    expected_sha256: str = EXPECTED_MANIFEST_SHA256,
) -> Path:
    raw_path = reference["path"]
    posix_path = PurePosixPath(raw_path)
    windows_path = PureWindowsPath(raw_path)
    if posix_path.is_absolute() or windows_path.is_absolute():
        raise NegotiationRejected("candidate manifest path must be repository-relative")
    if ".." in posix_path.parts or ".." in windows_path.parts:
        raise NegotiationRejected("candidate manifest path contains traversal")
    if raw_path != expected_relative.as_posix():
        raise NegotiationRejected("candidate manifest path does not match the binding")
    if reference["sha256"] != expected_sha256:
        raise NegotiationRejected("candidate manifest claim does not match the bound hash")

    try:
        canonical_repository = repository.resolve(strict=True)
        canonical_root = authorized_root.resolve(strict=True)
    except OSError as error:
        raise NegotiationRejected(
            f"cannot resolve repository or authorized root: {error}"
        ) from error
    if not canonical_repository.is_dir() or not canonical_root.is_dir():
        raise NegotiationRejected("repository and authorized root must be directories")
    try:
        canonical_root.relative_to(canonical_repository)
    except ValueError as error:
        raise NegotiationRejected(
            "authorized manifest root resolves outside the repository"
        ) from error

    try:
        manifest_path = (canonical_repository / raw_path).resolve(strict=True)
    except OSError as error:
        raise NegotiationRejected(f"candidate manifest cannot be resolved: {error}") from error
    try:
        manifest_path.relative_to(canonical_root)
    except ValueError as error:
        raise NegotiationRejected(
            "candidate manifest escapes the authorized root after symlink resolution"
        ) from error
    if not manifest_path.is_file():
        raise NegotiationRejected("candidate manifest must resolve to a regular file")
    observed_sha256 = file_sha256(manifest_path)
    if observed_sha256 != expected_sha256:
        raise NegotiationRejected(
            "candidate manifest bytes do not match the bound SHA-256"
        )
    return manifest_path


def ranges_overlap(left: dict[str, Any], right: dict[str, Any]) -> bool:
    return (
        left["start_sample"] < right["end_sample"]
        and right["start_sample"] < left["end_sample"]
    )


def validate_candidate_manifest(
    manifest: dict[str, Any]
) -> tuple[dict[int, dict[str, dict[str, Any]]], int]:
    try:
        if manifest["state"] != "CANDIDATES-READY":
            raise NegotiationRejected("bound manifest is not CANDIDATES-READY")
        if manifest["selected_choices"] != "UNAVAILABLE":
            raise NegotiationRejected("bound manifest already claims selected choices")
        if manifest["system_proposal"]["status"] != "PROPOSED-NOT-LOCKED":
            raise NegotiationRejected("bound system proposal has an invalid status")
        if manifest["system_proposal"]["user_acceptance"] != "UNAVAILABLE":
            raise NegotiationRejected("bound manifest already claims user acceptance")
        decoded_samples = manifest["analysis_master"]["decoded_sample_count"]
        bands = manifest["coverage_bands"]
    except (KeyError, TypeError) as error:
        raise NegotiationRejected(
            f"bound candidate manifest is missing required evidence: {error}"
        ) from error
    if len(bands) != 10 or [band.get("band") for band in bands] != list(range(1, 11)):
        raise NegotiationRejected("bound manifest must contain ordered bands 1 through 10")

    candidates_by_band: dict[int, dict[str, dict[str, Any]]] = {}
    global_ids: set[str] = set()
    for band in bands:
        band_number = band["band"]
        alternatives = band.get("alternatives", [])
        if len(alternatives) != 3:
            raise NegotiationRejected(
                f"bound manifest band {band_number} must contain three alternatives"
            )
        band_candidates: dict[str, dict[str, Any]] = {}
        for candidate in alternatives:
            candidate_id = candidate.get("candidate_id")
            if not isinstance(candidate_id, str) or candidate_id in global_ids:
                raise NegotiationRejected("bound candidate IDs must be globally unique")
            if candidate.get("coverage_band") != band_number:
                raise NegotiationRejected("candidate coverage-band evidence is inconsistent")
            start = candidate.get("start_sample")
            end = candidate.get("end_sample")
            if (
                not isinstance(start, int)
                or not isinstance(end, int)
                or end - start != 160000
                or candidate.get("sample_count") != 160000
            ):
                raise NegotiationRejected(
                    "every bound candidate must contain exactly 160,000 samples"
                )
            if start < band["start_sample"] or end > band["end_sample"]:
                raise NegotiationRejected("candidate range escapes its coverage band")
            if start < 0 or end > decoded_samples:
                raise NegotiationRejected("candidate range escapes the analysis master")
            if candidate.get("user_decision") != "UNAVAILABLE":
                raise NegotiationRejected("bound candidate already claims a user decision")
            global_ids.add(candidate_id)
            band_candidates[candidate_id] = candidate
        if any(
            ranges_overlap(left, right)
            for left, right in combinations(alternatives, 2)
        ):
            raise NegotiationRejected(
                f"bound alternatives overlap in coverage band {band_number}"
            )
        candidates_by_band[band_number] = band_candidates

    proposal = manifest["system_proposal"].get("items", [])
    if len(proposal) != 10:
        raise NegotiationRejected("bound system proposal must contain ten items")
    proposal_ranges = []
    for expected_band, item in enumerate(proposal, 1):
        if item.get("coverage_band") != expected_band:
            raise NegotiationRejected("system proposal bands must be ordered and unique")
        candidate = candidates_by_band[expected_band].get(item.get("candidate_id"))
        if candidate is None:
            raise NegotiationRejected("system proposal references an unknown candidate")
        for field in ("start_sample", "end_sample", "sample_count"):
            if item.get(field) != candidate.get(field):
                raise NegotiationRejected("system proposal range evidence is inconsistent")
        if item.get("decision_status") != "PROPOSED-NOT-LOCKED":
            raise NegotiationRejected("system proposal item falsely claims a decision")
        proposal_ranges.append(item)
    if any(
        ranges_overlap(left, right)
        for left, right in combinations(proposal_ranges, 2)
    ):
        raise NegotiationRejected("bound system proposal contains overlapping ranges")
    return candidates_by_band, decoded_samples


def verify_decision_sources(instance: dict[str, Any]) -> None:
    evidence_status = instance["evidence_status"]
    decisions = instance["decisions"]
    active = [decision for decision in decisions if decision["action"] != "UNAVAILABLE"]
    if evidence_status == "UNAVAILABLE":
        if active:
            raise NegotiationRejected(
                "unavailable evidence cannot contain an active decision"
            )
    elif evidence_status == "VISIBLE-USER-DECISIONS":
        if any(decision["decision_source"] != "USER" for decision in active):
            raise NegotiationRejected(
                "visible user decisions require an explicit USER source"
            )
    elif evidence_status == "SYNTHETIC-TEST-EVIDENCE":
        if any(
            decision["decision_source"] != "SYNTHETIC-TEST"
            for decision in active
        ):
            raise NegotiationRejected(
                "synthetic decisions must retain a SYNTHETIC-TEST source"
            )


def resolve_instance(
    instance: dict[str, Any], validator: Draft202012Validator
) -> dict[str, Any]:
    reject_schema_errors(validator, instance)
    manifest_path = resolve_manifest_reference(instance["candidate_manifest"])
    manifest = load_json(manifest_path)
    candidates_by_band, _ = validate_candidate_manifest(manifest)

    decisions = instance["decisions"]
    bands = [decision["band"] for decision in decisions]
    if len(set(bands)) != 10 or sorted(bands) != list(range(1, 11)):
        raise NegotiationRejected("decisions must contain each band exactly once")
    decisions = sorted(decisions, key=lambda decision: decision["band"])
    verify_decision_sources(instance)

    selections = []
    for decision in decisions:
        candidate_id = decision["candidate_id"]
        if candidate_id is not None:
            candidate = candidates_by_band[decision["band"]].get(candidate_id)
            if candidate is None:
                raise NegotiationRejected(
                    f"band {decision['band']} references an unknown candidate"
                )
            if decision["action"] == "SELECT":
                selections.append(candidate)

    if all(decision["action"] == "UNAVAILABLE" for decision in decisions):
        derived_state = "CANDIDATES-READY"
    elif all(decision["action"] == "SELECT" for decision in decisions):
        derived_state = "TEN-LOCKED"
    else:
        derived_state = "NEGOTIATING"

    if derived_state == "TEN-LOCKED":
        if len(selections) != 10 or len({item["candidate_id"] for item in selections}) != 10:
            raise NegotiationRejected("TEN-LOCKED requires ten unique candidates")
        if any(
            item["end_sample"] - item["start_sample"] != 160000
            or item["sample_count"] != 160000
            for item in selections
        ):
            raise NegotiationRejected(
                "TEN-LOCKED requires exact 160,000-sample selections"
            )
        if any(
            ranges_overlap(left, right)
            for left, right in combinations(selections, 2)
        ):
            raise NegotiationRejected("TEN-LOCKED selections must not overlap")
        evidence = instance["evidence_status"]
        sources = {decision["decision_source"] for decision in decisions}
        if evidence == "VISIBLE-USER-DECISIONS" and sources != {"USER"}:
            raise NegotiationRejected("real TEN-LOCKED requires USER-sourced selections")
        if evidence == "SYNTHETIC-TEST-EVIDENCE" and sources != {"SYNTHETIC-TEST"}:
            raise NegotiationRejected(
                "synthetic TEN-LOCKED test requires synthetic sources"
            )
        if evidence == "UNAVAILABLE":
            raise NegotiationRejected("unavailable evidence cannot reach TEN-LOCKED")

    derived_lock_claim = derived_state == "TEN-LOCKED"
    if instance["claimed_state"] != derived_state:
        raise NegotiationRejected(
            f"claimed state {instance['claimed_state']} does not match {derived_state}"
        )
    if instance["lock_claim"] != derived_lock_claim:
        raise NegotiationRejected("lock claim does not match the derived state")

    evidence_status = instance["evidence_status"]
    synthetic_lock_test = (
        derived_state == "TEN-LOCKED"
        and evidence_status == "SYNTHETIC-TEST-EVIDENCE"
    )
    authoritative_lock = (
        derived_state == "TEN-LOCKED"
        and evidence_status == "VISIBLE-USER-DECISIONS"
    )
    if evidence_status == "UNAVAILABLE":
        selection_status = "UNAVAILABLE"
    elif evidence_status == "SYNTHETIC-TEST-EVIDENCE":
        selection_status = "SYNTHETIC-TEST-ONLY-NOT-USER-DECISIONS"
    else:
        selection_status = "VISIBLE-USER-DECISIONS"
    unresolved_bands = [
        decision["band"]
        for decision in decisions
        if decision["action"] != "SELECT"
    ]
    return {
        "actual_user_acceptance": (
            "VISIBLE" if authoritative_lock else "UNAVAILABLE"
        ),
        "authoritative_lock": authoritative_lock,
        "candidate_manifest": {
            "path": EXPECTED_MANIFEST_RELATIVE.as_posix(),
            "sha256": EXPECTED_MANIFEST_SHA256,
        },
        "derived_state": derived_state,
        "input_sha256": hashlib.sha256(canonical_json(instance)).hexdigest(),
        "locked_real_manifest_written": False,
        "nonlocking_action_count": sum(
            decision["action"] in {"REJECT", "REQUEST-REANALYSIS", "DEFER"}
            for decision in decisions
        ),
        "resolution_version": 1,
        "revision": instance["revision"],
        "selection_evidence": {
            "candidate_ids": [item["candidate_id"] for item in selections],
            "selection_count": len(selections),
            "status": selection_status,
        },
        "synthetic_lock_test": synthetic_lock_test,
        "unresolved_bands": unresolved_bands,
    }


def resolve_file(path: Path, validator: Draft202012Validator) -> dict[str, Any]:
    instance = load_json(path)
    if not isinstance(instance, dict):
        raise NegotiationRejected("negotiation input must be an object")
    return resolve_instance(instance, validator)


def expect_manifest_invariant_failure(
    manifest: dict[str, Any], label: str
) -> None:
    try:
        validate_candidate_manifest(manifest)
    except NegotiationRejected:
        print(f"PASS rejected synthetic manifest invariant={label}")
        return
    raise NegotiationRejected(f"synthetic manifest invariant was accepted: {label}")


def check_manifest_invariants() -> None:
    manifest = load_json(REPOSITORY / EXPECTED_MANIFEST_RELATIVE)
    invalid_sample_count = copy.deepcopy(manifest)
    invalid_sample_count["coverage_bands"][0]["alternatives"][0][
        "sample_count"
    ] = 159999
    expect_manifest_invariant_failure(invalid_sample_count, "exact-sample-count")

    overlapping = copy.deepcopy(manifest)
    first = overlapping["coverage_bands"][0]["alternatives"][0]
    second = overlapping["coverage_bands"][0]["alternatives"][1]
    second["start_sample"] = first["start_sample"]
    second["end_sample"] = first["end_sample"]
    second["sample_count"] = 160000
    expect_manifest_invariant_failure(overlapping, "non-overlap")


def check_symlink_escape() -> None:
    with tempfile.TemporaryDirectory(
        prefix="izzi-negotiation-path-check-", dir="/tmp"
    ) as temporary:
        base = Path(temporary)
        repository = base / "repository"
        authorized_root = repository / "tests/baselines/audio"
        authorized_root.mkdir(parents=True)
        outside = base / "outside"
        outside.mkdir()
        outside_manifest = outside / "manifest.json"
        outside_manifest.write_text("{}\n", encoding="utf-8")
        escaping_link = authorized_root / "escape.json"
        escaping_link.symlink_to(outside_manifest)
        relative = Path("tests/baselines/audio/escape.json")
        reference = {
            "path": relative.as_posix(),
            "sha256": file_sha256(outside_manifest),
        }
        try:
            resolve_manifest_reference(
                reference,
                repository=repository,
                authorized_root=authorized_root,
                expected_relative=relative,
                expected_sha256=reference["sha256"],
            )
        except NegotiationRejected:
            print("PASS rejected synthetic manifest symlink escape")
            return
        raise NegotiationRejected("synthetic manifest symlink escape was accepted")


def check_fixtures(validator: Draft202012Validator) -> None:
    manifest_before = file_sha256(REPOSITORY / EXPECTED_MANIFEST_RELATIVE)
    if manifest_before != EXPECTED_MANIFEST_SHA256:
        raise NegotiationRejected("bound candidate manifest changed before checks")

    real_result = resolve_file(REAL_SKELETON, validator)
    if (
        real_result["derived_state"] != "CANDIDATES-READY"
        or real_result["selection_evidence"]["selection_count"] != 0
        or real_result["selection_evidence"]["status"] != "UNAVAILABLE"
        or real_result["authoritative_lock"]
        or real_result["synthetic_lock_test"]
    ):
        raise NegotiationRejected("real unavailable skeleton inferred a decision")
    print(
        f"PASS real {REAL_SKELETON.relative_to(REPOSITORY)} "
        f"sha256={file_sha256(REAL_SKELETON)} state=CANDIDATES-READY "
        "selections=0 acceptance=UNAVAILABLE"
    )

    valid_paths = sorted((FIXTURES / "valid").glob("*.json"))
    observed_valid = {path.name for path in valid_paths}
    if observed_valid != set(EXPECTED_VALID_FIXTURES):
        raise NegotiationRejected("valid fixture set does not match the checker contract")
    for path in valid_paths:
        result = resolve_file(path, validator)
        expected_state = EXPECTED_VALID_FIXTURES[path.name]
        if result["derived_state"] != expected_state:
            raise NegotiationRejected(f"fixture {path.name} derived the wrong state")
        if path.name == "synthetic-all-selections.json":
            if (
                not result["synthetic_lock_test"]
                or result["authoritative_lock"]
                or result["actual_user_acceptance"] != "UNAVAILABLE"
                or result["selection_evidence"]["status"]
                != "SYNTHETIC-TEST-ONLY-NOT-USER-DECISIONS"
            ):
                raise NegotiationRejected("synthetic lock test gained false authority")
        if path.name == "synthetic-nonlocking-actions.json":
            if result["nonlocking_action_count"] != 3 or result["authoritative_lock"]:
                raise NegotiationRejected("non-locking decisions incorrectly locked")
        print(
            f"PASS valid {path.relative_to(REPOSITORY)} "
            f"sha256={file_sha256(path)} state={expected_state} "
            f"result_sha256={hashlib.sha256(canonical_json(result)).hexdigest()}"
        )

    invalid_paths = sorted((FIXTURES / "invalid").glob("*.json"))
    observed_invalid = {path.name for path in invalid_paths}
    if observed_invalid != EXPECTED_INVALID_FIXTURES:
        raise NegotiationRejected("invalid fixture set does not match the checker contract")
    for path in invalid_paths:
        try:
            resolve_file(path, validator)
        except NegotiationRejected as error:
            print(
                f"PASS rejected {path.relative_to(REPOSITORY)} "
                f"sha256={file_sha256(path)} reason={error}"
            )
        else:
            raise NegotiationRejected(f"invalid fixture was accepted: {path.name}")

    check_symlink_escape()
    check_manifest_invariants()
    manifest_after = file_sha256(REPOSITORY / EXPECTED_MANIFEST_RELATIVE)
    if manifest_after != manifest_before:
        raise NegotiationRejected("bound candidate manifest changed during checks")
    print(
        "PASS fixture suite real=1 synthetic-valid=2 invalid=11 "
        "symlink=1 manifest-invariants=2 "
        f"jsonschema={version('jsonschema')} network=unused "
        "audio=unread transcript=unread locked-real-manifest=unwritten"
    )


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "input",
        nargs="?",
        type=Path,
        default=REAL_SKELETON,
        help="negotiation JSON to resolve; defaults to the all-unavailable skeleton",
    )
    parser.add_argument(
        "--check-fixtures",
        action="store_true",
        help="run valid, invalid, confinement, and manifest-invariant checks",
    )
    return parser.parse_args()


def main() -> int:
    arguments = parse_args()
    try:
        validator = load_validator()
        if arguments.check_fixtures:
            check_fixtures(validator)
        else:
            result = resolve_file(arguments.input, validator)
            print(json.dumps(result, ensure_ascii=True, indent=2, sort_keys=True))
            print(
                "result_sha256="
                + hashlib.sha256(canonical_json(result)).hexdigest()
            )
    except NegotiationRejected as error:
        print(f"FAIL {error}", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
