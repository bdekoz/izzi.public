#!/usr/bin/env python3
"""Generate a short synthetic speech WAV with local eSpeak NG and FFmpeg."""

from pathlib import Path
import shutil
import subprocess
import tempfile


def main():
    root = Path(__file__).resolve().parent
    binary = shutil.which('espeak-ng') or shutil.which('espeak')
    if not binary:
        raise SystemExit('Install espeak-ng (or espeak) to regenerate the speech fixture')
    with tempfile.TemporaryDirectory(prefix='izzi-speech-fixture-') as temporary:
        source = Path(temporary) / 'speech.wav'
        subprocess.run([binary, '-v', 'en-us', '-s', '145', '-w', str(source),
                        '-f', str(root / 'fixture-text.txt')], check=True)
        subprocess.run(['ffmpeg', '-nostdin', '-y', '-v', 'error', '-i', str(source),
                        '-ac', '1', '-ar', '16000', '-c:a', 'pcm_s16le',
                        str(root / 'fixture.wav')], check=True)
    print(root / 'fixture.wav')


if __name__ == '__main__':
    main()
