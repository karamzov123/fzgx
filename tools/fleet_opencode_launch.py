#!/usr/bin/env python3
"""Prime the isolated native server and attest the guard before any model request."""
import base64
import json
import os
from pathlib import Path
import secrets
import signal
import socket
import subprocess
import sys
import time
import urllib.request


def main():
    binary, *args = sys.argv[1:]
    receipt = Path(os.environ['FZGX_OPENCODE_GUARD_LOG'])
    nonce = secrets.token_hex(32)
    env = dict(os.environ, FZGX_OPENCODE_GUARD_NONCE=nonce,
               OPENCODE_SERVER_PASSWORD=secrets.token_urlsafe(32))
    with socket.socket() as sock:
        sock.bind(('127.0.0.1', 0))
        port = sock.getsockname()[1]
    url = f'http://127.0.0.1:{port}'
    auth = base64.b64encode(('opencode:' + env['OPENCODE_SERVER_PASSWORD']).encode()).decode()
    server_log = receipt.with_suffix('.server.log')
    server = None
    try:
        with server_log.open('w') as log:
            server = subprocess.Popen([binary, 'serve', '--hostname', '127.0.0.1', '--port', str(port)],
                                      env=env, stdout=log, stderr=log)
            deadline = time.monotonic() + 45
            ready = False
            while time.monotonic() < deadline and server.poll() is None:
                try:
                    request = urllib.request.Request(url + '/agent', headers={'Authorization': 'Basic ' + auth})
                    with urllib.request.urlopen(request, timeout=3) as response:
                        response.read()
                    rows = [json.loads(line) for line in receipt.read_text().splitlines()]
                    ready = any(row.get('event') == 'ready' and row.get('nonce') == nonce
                                and row.get('pid') == server.pid for row in rows)
                    if ready:
                        break
                except (OSError, ValueError):
                    pass
                time.sleep(.2)
            if not ready:
                print('FZGX_GUARD_NOT_READY: refusing model request', file=sys.stderr)
                return 70
            run = [binary, *args, '--attach', url, '--dir', str(Path.cwd())]
            return subprocess.call(run, env=env)
    finally:
        if server is not None and server.poll() is None:
            server.terminate()
            try:
                server.wait(timeout=5)
            except subprocess.TimeoutExpired:
                server.kill()
                server.wait()


if __name__ == '__main__':
    signal.signal(signal.SIGTERM, lambda *_: sys.exit(143))
    raise SystemExit(main())
