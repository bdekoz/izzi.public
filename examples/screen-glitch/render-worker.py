"""Render one native worker frame from its length-prefixed stdout protocol."""
import argparse
from pathlib import Path
import subprocess

parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument("worker")
parser.add_argument("recipe")
parser.add_argument("output")
parser.add_argument("--progress", type=float, default=0.175)
args = parser.parse_args()
if not 0 <= args.progress <= 1:
    parser.error("progress must be between 0 and 1")
result = subprocess.run(
    [args.worker, args.recipe], input=f"{args.progress}\n".encode(),
    stdout=subprocess.PIPE, check=True,
)
length, svg = result.stdout.split(b"\n", 1)
if len(svg) != int(length):
    raise RuntimeError("Worker output length mismatch")
Path(args.output).write_bytes(svg)
