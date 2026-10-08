"""Read-only gate recurrence evidence; never alters the Ninja decision."""
import hashlib
import json
import subprocess
import time
from pathlib import Path


def capture(root, cache, phase):
    root, cache = Path(root), Path(cache)
    paths = ('build/tools/dtk', 'build.ninja', '.ninja_log',
             'build/GFZE01/config.json', 'config/GFZE01/config.yml')
    row = {'timestamp': time.time(), 'phase': phase, 'inputs': {}}
    for name in paths:
        p = root / name
        try:
            s = p.stat()
            item: dict = {'mtime_ns': s.st_mtime_ns, 'size': s.st_size}
            if name == 'build/tools/dtk':
                item['sha256'] = hashlib.sha256(p.read_bytes()).hexdigest()
            row['inputs'][name] = item
        except OSError:
            row['inputs'][name] = {'missing': True}
    cache.mkdir(parents=True, exist_ok=True)
    log = cache / ('gate-explain-' + phase + '.log')
    with log.open('w') as out:
        try:
            cp = subprocess.run(['ninja', '-d', 'explain', '-n', 'build/GFZE01/ok'],
                                cwd=root, stdout=out, stderr=subprocess.STDOUT, timeout=15)
            row['explain_rc'] = cp.returncode
        except subprocess.TimeoutExpired:
            row['explain_timeout'] = True
    row['explain_log'] = str(log)
    path = cache / ('gate-inputs-' + phase + '.json')
    stage = path.with_suffix('.json.new')
    stage.write_text(json.dumps(row, indent=2) + '\n')
    stage.replace(path)
    return row


def safe_capture(root, cache, phase):
    try:
        return capture(root, cache, phase)
    except Exception:
        # Diagnostics are not permission to skip or fail a real verification gate.
        return None
