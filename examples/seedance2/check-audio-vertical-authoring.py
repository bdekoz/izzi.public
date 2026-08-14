#!/usr/bin/env python3
"""Validate the compact audio-first author input without reading audio data."""

from __future__ import annotations

import argparse
import hashlib
import json
import sys
import tempfile
from importlib.metadata import version
from pathlib import Path, PurePosixPath, PureWindowsPath
from typing import Any

from jsonschema import Draft202012Validator


HERE = Path(__file__).resolve().parent
REPOSITORY = HERE.parents[1]
DEFAULT_SCHEMA = HERE / "audio-vertical-authoring.proposed.schema.json"
DEFAULT_INSTANCE = HERE / "audio-vertical-authoring.minimal.json"
DEFAULT_RESOURCE_ROOT = REPOSITORY / "resources"
INVALID_FIXTURES = HERE / "audio-vertical-authoring.invalid"
EXPECTED_INVALID_FIXTURES = {
    "absolute-path.invalid.json",
    "short-style.invalid.json",
    "title-card-nonaligned.invalid.json",
    "title-card-too-long.invalid.json",
    "title-card-too-short.invalid.json",
    "traversal.invalid.json",
    "unknown-field.invalid.json",
}


class InputRejected(Exception):
    """The author input or its resource path violates the local contract."""


def file_sha256(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as source:
        for block in iter(lambda: source.read(65536), b""):
            digest.update(block)
    return digest.hexdigest()


def load_json(path: Path) -> Any:
    try:
        return json.loads(path.read_text(encoding="utf-8"))
    except (OSError, json.JSONDecodeError) as error:
        raise InputRejected(f"cannot read JSON {path}: {error}") from error


def load_validator(schema_path: Path) -> Draft202012Validator:
    schema = load_json(schema_path)
    try:
        Draft202012Validator.check_schema(schema)
    except Exception as error:
        raise InputRejected(f"invalid Draft 2020-12 schema: {error}") from error
    return Draft202012Validator(schema)


def reject_schema_errors(
    validator: Draft202012Validator, instance: Any
) -> None:
    errors = sorted(validator.iter_errors(instance), key=lambda error: error.json_path)
    if errors:
        error = errors[0]
        raise InputRejected(f"schema {error.json_path}: {error.message}")


def canonical_file_beneath_resource_root(
    instance: dict[str, Any], repository: Path, resource_root: Path
) -> Path:
    """Resolve symlinks and require the target beneath an authorized root.

    This function only resolves and stats the path. It does not open, decode, hash,
    transcribe, or otherwise inspect the audio file.
    """

    raw_path = instance["audio_path"]
    posix_path = PurePosixPath(raw_path)
    windows_path = PureWindowsPath(raw_path)
    if posix_path.is_absolute() or windows_path.is_absolute():
        raise InputRejected("audio_path must be repository-relative")
    if ".." in posix_path.parts or ".." in windows_path.parts:
        raise InputRejected("audio_path must not contain a '..' segment")

    try:
        canonical_repository = repository.resolve(strict=True)
        canonical_root = resource_root.resolve(strict=True)
    except OSError as error:
        raise InputRejected(f"cannot resolve repository or resource root: {error}") from error

    if not canonical_repository.is_dir() or not canonical_root.is_dir():
        raise InputRejected("repository and authorized resource root must be directories")
    try:
        canonical_root.relative_to(canonical_repository)
    except ValueError as error:
        raise InputRejected(
            "authorized resource root resolves outside the repository"
        ) from error

    try:
        canonical_target = (canonical_repository / raw_path).resolve(strict=True)
    except OSError as error:
        raise InputRejected(f"audio_path does not resolve to an existing file: {error}") from error
    try:
        canonical_target.relative_to(canonical_root)
    except ValueError as error:
        raise InputRejected(
            "audio_path escapes the authorized resource root after symlink resolution"
        ) from error
    if not canonical_target.is_file():
        raise InputRejected("audio_path must resolve to a regular file")
    return canonical_target


def validate_instance(
    path: Path,
    validator: Draft202012Validator,
    repository: Path,
    resource_root: Path,
) -> Path:
    instance = load_json(path)
    reject_schema_errors(validator, instance)
    if not isinstance(instance, dict):
        raise InputRejected("author input must be an object")
    return canonical_file_beneath_resource_root(instance, repository, resource_root)


def check_symlink_escape(validator: Draft202012Validator) -> None:
    """Regression-check that canonical confinement rejects an escaping symlink."""

    with tempfile.TemporaryDirectory(prefix="izzi-audio-path-check-") as temporary:
        base = Path(temporary)
        repository = base / "repository"
        resource_root = repository / "resources"
        outside = base / "outside"
        resource_root.mkdir(parents=True)
        outside.mkdir()
        outside_audio = outside / "placeholder.mp3"
        outside_audio.touch()
        (resource_root / "escape.mp3").symlink_to(outside_audio)
        instance = {
            "schema_version": 1,
            "vertical_name": "Confinement regression",
            "visual_style": "Calm white geometric lines on a deep black field.",
            "audio_path": "resources/escape.mp3",
        }
        reject_schema_errors(validator, instance)
        try:
            canonical_file_beneath_resource_root(
                instance, repository, resource_root
            )
        except InputRejected:
            return
        raise InputRejected("symlink-escape regression was unexpectedly accepted")


def check_fixtures(
    schema_path: Path, repository: Path, resource_root: Path
) -> None:
    validator = load_validator(schema_path)
    validate_instance(DEFAULT_INSTANCE, validator, repository, resource_root)
    print(
        f"PASS valid {DEFAULT_INSTANCE.relative_to(REPOSITORY)} "
        f"sha256={file_sha256(DEFAULT_INSTANCE)}"
    )

    fixture_paths = sorted(INVALID_FIXTURES.glob("*.invalid.json"))
    observed_names = {path.name for path in fixture_paths}
    if observed_names != EXPECTED_INVALID_FIXTURES:
        raise InputRejected(
            "invalid fixture set mismatch: "
            f"expected={sorted(EXPECTED_INVALID_FIXTURES)} "
            f"observed={sorted(observed_names)}"
        )
    for fixture in fixture_paths:
        try:
            validate_instance(fixture, validator, repository, resource_root)
        except InputRejected as error:
            print(
                f"PASS rejected {fixture.relative_to(REPOSITORY)} "
                f"sha256={file_sha256(fixture)} reason={error}"
            )
        else:
            raise InputRejected(f"invalid fixture was accepted: {fixture}")

    check_symlink_escape(validator)
    print("PASS rejected synthetic symlink escape after canonical resolution")
    print(f"PASS schema sha256={file_sha256(schema_path)}")
    print(
        f"PASS fixture suite valid=1 invalid={len(fixture_paths)} "
        f"jsonschema={version('jsonschema')} network=unused audio_content=unread"
    )


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "instance",
        nargs="?",
        type=Path,
        default=DEFAULT_INSTANCE,
        help="author JSON to validate (defaults to the minimal example)",
    )
    parser.add_argument("--schema", type=Path, default=DEFAULT_SCHEMA)
    parser.add_argument("--repo-root", type=Path, default=REPOSITORY)
    parser.add_argument(
        "--resource-root",
        type=Path,
        default=DEFAULT_RESOURCE_ROOT,
        help="authorized resource directory; canonical target must remain beneath it",
    )
    parser.add_argument(
        "--check-fixtures",
        action="store_true",
        help="check the minimal example, all invalid fixtures, and symlink confinement",
    )
    return parser.parse_args()


def main() -> int:
    arguments = parse_args()
    try:
        if arguments.check_fixtures:
            check_fixtures(
                arguments.schema, arguments.repo_root, arguments.resource_root
            )
        else:
            validator = load_validator(arguments.schema)
            canonical_target = validate_instance(
                arguments.instance,
                validator,
                arguments.repo_root,
                arguments.resource_root,
            )
            print(
                f"PASS {arguments.instance} sha256={file_sha256(arguments.instance)} "
                f"canonical_audio_path={canonical_target} "
                f"jsonschema={version('jsonschema')} audio_content=unread"
            )
    except InputRejected as error:
        print(f"FAIL {error}", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
