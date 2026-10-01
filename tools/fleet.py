#!/usr/bin/env python3
"""New fzgx fleet: one owner, bound matcher tools, truthful Eww telemetry.

No worker worktree merges or automatic pushes. orchestrate.py owns private work
copies, atomic SQLite claims, tool limits and serialized 16-target verification.
"""
from __future__ import annotations
import fcntl
import hashlib
import json
import os
import re
from pathlib import Path
import signal
import sqlite3
import subprocess
import sys
import time

ROOT = Path(__file__).resolve().parents[1]
CACHE = Path.home() / '.cache/fzgx-agents'
CONTROL = CACHE / 'control-v2.json'
STATE = CACHE / 'status-v2.json'
HISTORY = CACHE / 'scheduled-v2.json'
META = {'claude': ('Claude', '󰛄'), 'gpt': ('GPT', '󰭹'), 'cline': ('Cline', '󰊠'), 'agy': ('AGY', '󰆧')}
DISABLED = 'Disabled: this CLI has no verified function-bound, five-tool transport. GPT runs the constrained fleet.'
STOP = False
MAX_INPUT_TOKENS = 256000
MAX_OUTPUT_TOKENS = 32000

def load(path, fallback):
    try:
        return json.loads(path.read_text())
    except (OSError, ValueError):
        return fallback

def atomic(path, data):
    path.parent.mkdir(parents=True, exist_ok=True)
    tmp = path.with_name(path.name + f'.{os.getpid()}.tmp')
    tmp.write_text(json.dumps(data, indent=2) + '\n')
    tmp.replace(path)

def pid_alive(pid):
    try:
        if not pid:
            return False
        # Do not treat a zombie or reused PID as an active matcher host.
        return Path(f'/proc/{int(pid)}/stat').read_text().split(') ', 1)[1][0] != 'Z'
    except (OSError, ValueError, IndexError):
        return False

def activity_state(alive, claims, last_event, started, now):
    if not alive:
        return 'error'
    if claims and last_event and now - last_event < 180:
        return 'working'
    if claims and now - max(last_event, started) >= 180:
        return 'stalled'
    return 'starting'

def pick(rows, seen, context, count):
    eligible = [r for r in rows if r['status'] == 'unmatched' and r['attempts'] < 3
                and seen.get(r['symbol']) != context and r['size'] <= 1024]
    # Saved near-matches first, then small untouched targets. Never raise caps.
    eligible.sort(key=lambda r: (-(r.get('best', r.get('best_percent', 0)) or 0), r['attempts'], r['size'], r['symbol']))
    return [r['symbol'] for r in eligible[:count]]

def runner_command(batch, symbols, parallel):
    return [str(ROOT / '.venv/bin/python'), str(ROOT / 'tools/orchestrate.py'), '--harness', 'codex',
            '--model', 'gpt-6.1-sol', '--effort', 'medium', '--parallel', str(parallel),
            '--tool-parallel', '2', '--timeout', '600', '--max-checks', '8',
            '--max-stale', '3', '--max-attempts', '3', '--verify-interval', '60',
            '--no-trivial', '--batch', batch, '--symbols', *symbols]

def provider_error_text(text):
    """Classify provider errors, never event names or model/source content."""
    errors = []
    for line in text.splitlines():
        try:
            row = json.loads(line)
        except ValueError:
            errors.append(line)
            continue
        if not isinstance(row, dict):
            continue
        if row.get('type') == 'rate_limit_event':
            info = row.get('rate_limit_info') or {}
            if info.get('status') == 'rejected':
                errors.append('rate_limit_error: provider rejected request')
            # allowed/allowed_warning are informative, not request failures.
            continue
        if row.get('type') == 'error' or row.get('is_error'):
            errors.append(json.dumps({k: row[k] for k in ('error', 'errors', 'message', 'result') if k in row}))
        result = row.get('result')
        if isinstance(result, dict) and result.get('error'):
            errors.append(str(result['error']))
    return '\n'.join(errors)

def cooldown(text, failures):
    text = provider_error_text(text).lower()
    if (re.search(r'(?:http|error|status|code).{0,20}\b429\b', text)
            or re.search(r'\b429\b.{0,80}(?:quota|resource_exhausted|too.?many)', text)
            or any(s in text for s in ('resource_exhausted', 'rate_limit', 'rate limit', 'insufficient_balance', 'individual quota'))):
        return 1800
    if re.search(r'(?:http|error|status|code).{0,20}\b529\b', text) or 'overloaded' in text:
        return 300
    return min(1800, 60 * 2 ** min(failures, 5))

def context_id():
    # Ledger snapshots and acceptance commits are NOT new evidence. Only changes
    # to compiler/context tooling/config permit another bounded attempt.
    digest = hashlib.sha256()
    for directory, pattern in ((ROOT / 'tools/fzgx', '*.py'), (ROOT / 'include', '*.h'), (ROOT / 'config/GFZE01', '*.yml')):
        for p in sorted(directory.rglob(pattern)):
            digest.update(str(p.relative_to(ROOT)).encode()); digest.update(p.read_bytes())
    return digest.hexdigest()

def db_rows(sql, args=()):
    path = ROOT / '.fzgx/ledger.db'
    with sqlite3.connect(f'file:{path}?mode=ro', uri=True, timeout=5) as db:
        db.row_factory = sqlite3.Row
        return [dict(r) for r in db.execute(sql, args)]

def tail_bytes(path, size=16384, start=0):
    try:
        with path.open('rb') as f:
            f.seek(0, 2); f.seek(max(start, f.tell() - size))
            return f.read(size).decode(errors='replace')
    except OSError:
        return ''

def telemetry(batch, limits=None):
    limits = limits or dict(input=MAX_INPUT_TOKENS, output=MAX_OUTPUT_TOKENS,
                            batch_input=1000000, batch_output=80000,
                            log_bytes=12*1024*1024, guard_each=True, usage_wait=120)
    directory = ROOT / '.fzgx/runs' / batch
    claims = db_rows("SELECT f.symbol, f.module, a.checks, f.claimed_by FROM functions f LEFT JOIN attempts a ON a.id=(SELECT MAX(id) FROM attempts WHERE symbol=f.symbol AND ended IS NULL) WHERE f.status='claimed' AND substr(f.claimed_by,1,?)=?", (len(batch) + 1, batch + '-'))
    attempts = db_rows('SELECT symbol,checks,outcome,tokens_in,tokens_out FROM attempts WHERE substr(agent,1,?)=?', (len(batch) + 1, batch + '-'))
    files = list(directory.glob('*.jsonl')) + list(directory.glob('*.assignment.json')) + list(directory.glob('*.terminal.json')) + list(directory.glob('*.log'))
    last = max((p.stat().st_mtime for p in files), default=0)
    usages = [load(path, {}) for path in directory.glob('*.usage.json')]
    tokens_out = sum(u.get('outputTokens', 0) for u in usages)
    tokens_in = sum(u.get('inputTokens', 0) for u in usages)
    usage_missing = any(not (directory / f"{c['symbol']}.usage.json").exists()
                        and (directory / f"{c['symbol']}.assignment.json").exists()
                        and time.time() - (directory / f"{c['symbol']}.assignment.json").stat().st_mtime > limits['usage_wait']
                        for c in claims)
    over_budget = limits['guard_each'] and any(u.get('outputTokens', 0) >= limits['output'] or u.get('inputTokens', 0) >= limits['input'] for u in usages)
    over_budget = over_budget or tokens_in >= limits['batch_input'] or tokens_out >= limits['batch_output'] or sum(p.stat().st_size for p in files) >= limits['log_bytes']
    verified = set()
    verify_ok = None
    for line in tail_bytes(directory / 'verify.jsonl', 131072).splitlines():
        try:
            row = json.loads(line)
        except ValueError:
            continue
        if not isinstance(row, dict) or not isinstance(row.get('ok'), bool) or not isinstance(row.get('verified'), list):
            continue
        verify_ok = row['ok']; verified.update(row['verified'])
    accepted_symbols = {a['symbol'] for a in attempts if a['outcome'] == 'matched'}
    # The watcher also reports cached/global backlog; never credit other agents
    # or old matches to this batch's productivity.
    verified.intersection_update(accepted_symbols)
    return {'claims': claims, 'checks': sum(a['checks'] or 0 for a in attempts),
            'completed': sum(a['outcome'] is not None for a in attempts), 'verified': len(verified),
            'verify_ok': verify_ok, 'last_event': last, 'directory': str(directory),
            'tokens_in': tokens_in, 'tokens_out': tokens_out, 'over_budget': over_budget, 'usage_missing': usage_missing}

def verified_progress():
    rows = db_rows("SELECT DISTINCT a.symbol, f.matched_commit, a.ended FROM attempts a JOIN functions f ON f.symbol=a.symbol WHERE substr(a.agent,1,9)='fleet-v2-' AND a.outcome='matched' AND f.status='matched' AND f.link_state='verified' ORDER BY a.ended DESC")
    return len({r['symbol'] for r in rows}), (rows[0]['matched_commit'] if rows else 'none')

def publish(data):
    total, last = verified_progress()
    atomic(STATE, dict(data, total_verified=total, last_landing=last,
                       heartbeat=time.time(), supervisor_pid=os.getpid()))

def status():
    config = load(CONTROL, {'enabled': True, 'parallel': 4})
    data = load(STATE, {})
    now = time.time()
    fresh = now - data.get('heartbeat', 0) < 30 and pid_alive(data.get('supervisor_pid'))
    out = {}
    for family, (name, icon) in META.items():
        on = family == 'gpt' and config['enabled']
        state = 'off'
        details = DISABLED if family != 'gpt' else 'Stopped by operator.'
        if on:
            state = data.get('status', 'starting') if fresh else 'error'
            details = data.get('reason', '') if fresh else 'Supervisor is stopped/stale; no work claimed as running.'
            details += f"\n{data.get('active', 0)} claimed functions / {config['parallel']} session limit"
            details += f"\nBatch: {data.get('checks', 0)} checks, {data.get('completed', 0)} finished, {data.get('verified', 0)} link-verified"
            details += f"\nSince repair: {data.get('total_verified', 0)} link-verified; last landing: {data.get('last_landing', 'none')}"
            details += '\n' + '\n'.join(f"{c['module']}: {c['symbol']} ({c.get('checks') or 0} checks)" for c in data.get('claims', []))
            if data.get('retry_at', 0) > now:
                details += f"\nAutomatic retry in {int(data['retry_at'] - now)}s"
        count = data.get('active', 0) if on and fresh else 0
        suffix = {'working':'●', 'starting':'◌', 'stalled':'!', 'error':'!', 'blocked':'!', 'rate-limited':'⏳', 'idle':'·', 'off':''}.get(state, '!')
        out[family] = dict(on=on, status=state, count=count, icon=icon, name=name,
                           label=icon + suffix + (str(count) if count else ''),
                           tooltip=f'{name}: {state.upper()}\n{details}\nLeft: toggle | Right/up: +session | Down: -session | Middle: logs')
    return out

def change_control(command, family):
    if family not in META:
        raise ValueError('unknown family')
    if family != 'gpt':
        print(DISABLED)
        subprocess.run(['notify-send', 'F-Zero GX Fleet', DISABLED], check=False)
        return
    CACHE.mkdir(parents=True, exist_ok=True)
    with (CACHE / 'control.lock').open('w') as lock:
        fcntl.flock(lock, fcntl.LOCK_EX)
        data = load(CONTROL, {'enabled': True, 'parallel': 4})
        if command == 'toggle':
            data['enabled'] = not data['enabled']
        elif command == 'scale-up':
            data['parallel'] = min(8, data['parallel'] + 1); data['enabled'] = True
        elif command == 'scale-down':
            data['parallel'] = max(1, data['parallel'] - 1)
        atomic(CONTROL, data)
        print(json.dumps(data))

def stop_runner(proc):
    if proc and proc.poll() is None:
        # Runner traps SIGTERM: drain mutations, save best bodies, release claims.
        proc.send_signal(signal.SIGTERM)
        try:
            proc.wait(timeout=120)
        except subprocess.TimeoutExpired:
            raise RuntimeError('Runner cleanup timed out; refusing to launch another owner')

def run_gate():
    log = CACHE / 'gate.log'
    with log.open('w') as output:
        cp = subprocess.run(['ninja', '-j', '4'], cwd=ROOT, stdout=output, stderr=subprocess.STDOUT, timeout=180)
    # Cached Ninja still must execute the hash gate, not merely be up-to-date.
    with log.open('a') as output:
        hashes = subprocess.run([str(ROOT / 'build/tools/dtk'), 'shasum', '-c', 'config/GFZE01/build.sha1'], cwd=ROOT, stdout=output, stderr=subprocess.STDOUT, timeout=30) if cp.returncode == 0 else None
    return cp.returncode == 0 and hashes is not None and hashes.returncode == 0

def record_gate(directory, passed, timestamp):
    if passed is not True:
        raise ValueError('Cannot publish initial health without a passed hash gate')
    directory.mkdir(parents=True, exist_ok=False)
    # No-op verification watches are silent. Initial health is the actual
    # pre-batch 16-target hash gate, not a watcher result or accepted match.
    (directory / 'verify.jsonl').write_text(json.dumps(dict(timestamp=timestamp, ok=True,
        verified=[], rejected=[], bootstrap_gate=True)) + '\n')

def active_runner_pids():
    active = []
    for path in Path('/proc').glob('[0-9]*/cmdline'):
        try:
            args = path.read_bytes().split(b'\0')
        except OSError:
            continue
        if (str(ROOT / 'tools/orchestrate.py').encode() in args
                and any(a.startswith(b'fleet-v2-') for a in args)):
            active.append(int(path.parent.name))
    return active

def daemon():
    global STOP
    CACHE.mkdir(parents=True, exist_ok=True)
    lock = (CACHE / 'supervisor-v2.lock').open('w')
    fcntl.flock(lock, fcntl.LOCK_EX | fcntl.LOCK_NB)
    signal.signal(signal.SIGTERM, lambda *_: set_stop())
    signal.signal(signal.SIGINT, lambda *_: set_stop())
    if not CONTROL.exists():
        atomic(CONTROL, {'enabled': True, 'parallel': 4})
    from fzgx import api
    from fzgx.project import Project
    proc = None
    output = None
    launched_parallel = 0
    batch_log_start = 0
    seen = load(HISTORY, {})
    runtime_path = CACHE / 'runtime-v2.json'
    runtime = load(runtime_path, {})
    failures, retry_at = runtime.get('failures', 0), runtime.get('retry_at', 0)
    backoff_status = runtime.get('backoff_status', 'idle')
    batch, started = '', 0
    gate_ok = False
    gate_context = runtime.get('failed_gate')
    def save_runtime():
        atomic(runtime_path, dict(failures=failures, retry_at=retry_at, failed_gate=gate_context, backoff_status=backoff_status))
    try:
        # On service restart, never start a second host alongside an orphan.
        while not STOP and active_runner_pids():
            publish(dict(status='blocked', active=0, reason='Waiting for previous bound runner to drain; no overlapping host launched.'))
            time.sleep(3)
        if not STOP:
            p = Project()
            for claim in db_rows("SELECT symbol, claimed_by FROM functions WHERE status='claimed' AND substr(claimed_by,1,9)='fleet-v2-'"):
                result = api.release(p, claim['symbol'], 'Supervisor recovery: prior bound host exited; preserving best work',
                                     agent=claim['claimed_by'], save_only=True)
                if not result.get('ok'):
                    raise RuntimeError('Abandoned-claim recovery failed: ' + json.dumps(result))
        while not STOP:
            config = load(CONTROL, {'enabled': True, 'parallel': 4})
            now = time.time()
            data: dict = dict(status='off', reason='Stopped by operator.', active=0, claims=[])
            if proc is not None:
                t = telemetry(batch)
                data.update(t, active=len(t['claims']), batch=batch)
                rc = proc.poll()
                if not config['enabled'] or config['parallel'] != launched_parallel:
                    stop_runner(proc); rc = proc.returncode
                if rc is None:
                    data['status'] = activity_state(True, len(t['claims']), t['last_event'], started, now)
                    data['reason'] = 'Bound matcher tools only; claims and checks are live SQLite evidence.'
                    if t['verify_ok'] is False or (t['verify_ok'] is None and now - started > 60):
                        data['status'] = 'blocked'; data['reason'] = 'Hash verification failed; stopping producers.'
                        stop_runner(proc); gate_ok = False
                    elif t['over_budget'] or t['usage_missing'] or now - max(t['last_event'], started) > 300 or now - started > 1500:
                        data['reason'] = 'Token/output budget, inactivity or batch deadline: saving/releasing and backing off.'
                        stop_runner(proc)
                    else:
                        if t['verify_ok'] is None:
                            data.update(status='starting', reason='Waiting for first valid verifier record; not claiming healthy work yet.')
                        publish(data); time.sleep(3); continue
                t = telemetry(batch)
                if output:
                    output.close()
                output = None
                result_text = tail_bytes(CACHE / 'logs/gpt.log', start=batch_log_start)
                proc = None
                if rc != 0 or not t['verify_ok']:
                    failures += 1; retry_at = now + cooldown(result_text, failures)
                    backoff_status = 'rate-limited' if cooldown(result_text, 0) == 1800 else 'error'
                elif not t['verified']:
                    failures += 1; retry_at = now + min(600, 30 * failures)
                    backoff_status = 'idle'
                else:
                    failures = 0; retry_at = now + 3
                    backoff_status = 'idle'
                save_runtime()
                gate_ok = False  # Check gate between every batch; never spend on a red tree.
            if not config['enabled']:
                publish(data); time.sleep(3); continue
            if now < retry_at:
                data.update(status=backoff_status, reason='Bounded automatic backoff; no model requests.', retry_at=retry_at)
                publish(data); time.sleep(3); continue
            context = context_id()
            if not gate_ok:
                # A red gate is retried only after source HEAD/context changes.
                head = subprocess.check_output(['git', 'rev-parse', 'HEAD'], cwd=ROOT, text=True).strip()
                identity = [head, context]
                if gate_context == identity:
                    data.update(status='blocked', reason='16-target gate failed. No unchanged rebuild loop. See gate.log.')
                    publish(data); time.sleep(10); continue
                data.update(status='starting', reason='Checking 16-target build/hash gate before assigning work.')
                publish(data)
                gate_ok = run_gate()
                gate_context = None if gate_ok else identity
                save_runtime()
                if not gate_ok:
                    continue
            rows = api.inventory(Project(), status='unmatched')
            symbols = pick(rows, seen, context, config['parallel'] * 2)
            if not symbols:
                data.update(status='idle', reason='No fresh under-1KiB targets below attempt cap. No blind retries or model polling.')
                publish(data); time.sleep(15); continue
            batch = f'fleet-v2-{time.time_ns()}'
            directory = ROOT / '.fzgx/runs' / batch
            record_gate(directory, gate_ok, now)
            for symbol in symbols:
                seen[symbol] = context
            atomic(HISTORY, seen)
            (CACHE / 'logs').mkdir(exist_ok=True)
            output = (CACHE / 'logs/gpt.log').open('a')
            batch_log_start = output.tell()
            output.write(f'\n--- {batch}: {symbols} ---\n'); output.flush()
            launched_parallel = config['parallel']
            proc = subprocess.Popen(runner_command(batch, symbols, launched_parallel), cwd=ROOT,
                                    stdout=output, stderr=subprocess.STDOUT, env={**os.environ, 'PYTHONUNBUFFERED':'1', 'FZGX_MAX_MODEL_INPUT_TOKENS':'128000', 'FZGX_MAX_MODEL_OUTPUT_TOKENS':'16000'}, stdin=subprocess.DEVNULL)
            started = time.time()
            print(f'{batch}: {len(symbols)} unique targets; {launched_parallel} bound sessions', flush=True)
            publish(dict(data, status='starting', reason='Preparing unique function assignments.', batch=batch, runner_pid=proc.pid))
            time.sleep(3)
    finally:
        stop_runner(proc)
        if output:
            output.close()
        publish(dict(status='off', active=0, reason='Supervisor stopped; matcher cleanup completed.'))

def set_stop():
    global STOP
    STOP = True

def main():
    args = sys.argv[1:]
    command = args[0] if args else 'status'
    if command == 'status':
        print(json.dumps(status()))
    elif command == 'daemon':
        daemon()
    elif command in ('toggle', 'scale-up', 'scale-down'):
        change_control(command, args[1])
    elif command == 'scroll':
        change_control('scale-up' if args[2] == 'up' else 'scale-down', args[1])
    elif command == 'watch':
        family = args[1]
        if family not in META:
            raise ValueError('unknown family')
        logfile = CACHE / 'logs' / ('gpt.log' if family == 'gpt' else f'{family}-1.log')
        subprocess.run([str(Path.home() / '.config/eww/scripts/foot-popup.sh'), f'fzgx-{family}', '950', '650', 'tail', '-f', str(logfile)], check=False)
    elif command == 'reset-limits':
        print('Backoff is automatic; no unsafe quota reset or duplicate retry.')
    else:
        raise SystemExit('Unknown fleet command')

if __name__ == '__main__':
    # Four-provider coordinator. V2 helpers above remain for recovery and the
    # existing acceptance harness, but the GPT-only daemon is not launched.
    from fleet_multi import main as multi_main
    multi_main()
