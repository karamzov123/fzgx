"""Bound CLI execution with live logs and durable provider usage counters."""
from __future__ import annotations
import json
import os
from pathlib import Path
import signal
import subprocess
import threading
import time
from fleet_clients import client_root, command
from fleet_activity import ToolProgress

STOP = threading.Event()

class ProcessDrainError(RuntimeError):
    """Do not save/release a claim whose owned descendants have not drained."""


def owned_processes(owner, known, agent=None):
    """Linux process-session plus captured ancestry, with PID-reuse cookies."""
    if not Path('/proc').is_dir():
        raise ProcessDrainError('Cannot prove bound process drain without /proc')
    processes={}
    bound=set()
    for path in Path('/proc').glob('[0-9]*/stat'):
        try:
            fields=path.read_text().rsplit(')',1)[1].split()
            pid=int(path.parent.name)
            if fields[0] not in ('Z','X'):
                processes[pid]=(int(fields[1]),int(fields[3]),int(fields[19]))
                if agent and path.stat().st_uid==os.getuid():
                    entries=path.with_name('environ').read_bytes().split(b'\0')
                    if ('FZGX_AGENT_ID='+agent).encode() in entries:
                        bound.add(pid)
        except (OSError,ValueError,IndexError):
            continue
    if owner in processes and owner in known and processes[owner][2]!=known[owner]:
        raise ProcessDrainError('Client PID was reused; refusing descendant signals')
    selected={pid for pid,(_,session,cookie) in processes.items()
              if session==owner or known.get(pid)==cookie or pid in bound}
    while True:
        children={pid for pid,(parent,_,_) in processes.items() if parent in selected}
        if children<=selected:
            break
        selected.update(children)
    for pid in selected:
        known[pid]=processes[pid][2]
    return {pid:processes[pid][2] for pid in selected}


def drain_processes(owner, progress, known, timeout_s=600, agent=None):
    """Wait for active tools before terminating idle descendants; verify exit."""
    deadline=time.monotonic()+timeout_s
    while True:
        remaining=owned_processes(owner,known,agent)
        if not remaining:
            return
        progress.refresh()
        if not progress.active and not progress.uncertain:
            for pid,cookie in remaining.items():
                try:
                    fields=Path(f'/proc/{pid}/stat').read_text().rsplit(')',1)[1].split()
                    if int(fields[19])==cookie:
                        os.kill(pid,signal.SIGTERM)
                except (ProcessLookupError,FileNotFoundError):
                    pass
        if time.monotonic()>deadline:
            STOP.set()
            raise ProcessDrainError(f'Bound descendants did not drain: {sorted(remaining)}; claim retained')
        time.sleep(.1)

def atomic_usage(path, data):
    tmp = path.with_suffix('.usage.tmp')
    tmp.write_text(json.dumps(data) + '\n'); tmp.replace(path)

class Usage:
    def __init__(self):
        self.messages = {}
        self.total = None
    def update(self, row):
        # Cline publishes cumulative counters; Claude publishes per-message
        # counters and a cumulative final result. Never sum repeated deltas.
        kind = row.get('type')
        if kind == 'step_finish':
            # opencode reports per-step counters on the part, never a cumulative
            # result object, so accumulate steps exactly like per-message usage.
            step = row.get('step-finish') or row.get('part') or {}
            tokens = step.get('tokens') if isinstance(step, dict) else None
            if isinstance(tokens, dict):
                cache = tokens.get('cache') or {}
                key = 'oc-' + str(row.get('messageID') or step.get('messageID') or len(self.messages))
                self.messages[key] = dict(
                    inputTokens=(tokens.get('input', 0) or 0) + (cache.get('read', 0) or 0) + (cache.get('write', 0) or 0),
                    outputTokens=tokens.get('output', 0) or 0)
                self.total = {k: sum(v[k] for v in self.messages.values()) for k in ('inputTokens', 'outputTokens')}
        if row.get('event') == 'step_update':
            step = row.get('step_update') or {}
            usage = step.get('usage')
            if step.get('state') == 'DONE' and isinstance(usage, dict):
                self.messages['agy-' + str(step.get('step_index'))] = dict(inputTokens=(usage.get('input_tokens', 0) or 0)+(usage.get('cache_read_tokens', 0) or 0), outputTokens=usage.get('output_tokens', 0) or 0)
                self.total = {k:sum(v[k] for v in self.messages.values()) for k in ('inputTokens','outputTokens')}
        if row.get('event') == 'result' and isinstance(row.get('result'), dict):
            usage = row['result'].get('usage') or {}
            self.total = dict(inputTokens=(usage.get('input_tokens', 0) or 0)+(usage.get('cache_read_tokens', 0) or 0), outputTokens=usage.get('output_tokens', 0) or 0)
        if kind == 'usage-updated' and isinstance(row.get('usage'), dict):
            # inputTokens is cache-inclusive; carry the cache split through so cost
            # and cross-family comparisons are measured rather than assumed
            # (docs/findings/276). Never add cacheRead on top of inputTokens.
            self.total = dict(inputTokens=row['usage'].get('inputTokens', 0), outputTokens=row['usage'].get('outputTokens', 0),
                              cacheReadTokens=row['usage'].get('cacheReadTokens', 0) or 0,
                              cacheWriteTokens=row['usage'].get('cacheWriteTokens', 0) or 0,
                              totalCost=row['usage'].get('totalCost', 0) or 0)
        msg = row.get('message') or {}
        if isinstance(msg, dict) and isinstance(msg.get('usage'), dict):
            usage = msg['usage']
            self.messages[msg.get('id', str(len(self.messages)))] = dict(
                inputTokens=sum(usage.get(k, 0) or 0 for k in ('input_tokens','cache_creation_input_tokens','cache_read_input_tokens')),
                outputTokens=usage.get('output_tokens', 0) or 0)
            self.total = {k:sum(v[k] for v in self.messages.values()) for k in ('inputTokens','outputTokens')}
        if kind == 'result' and isinstance(row.get('usage'), dict):
            usage=row['usage']
            self.total=dict(inputTokens=sum(usage.get(k,0) or 0 for k in ('input_tokens','cache_creation_input_tokens','cache_read_input_tokens')) or usage.get('inputTokens',0),
                            outputTokens=usage.get('output_tokens',0) or usage.get('outputTokens',0))
        return self.total

def run(family, prompt, model, directory, env, timeout_s):
    idle_timeout=float(env.get('FZGX_TOOL_IDLE_TIMEOUT_S', '600'))
    if not 0 <= idle_timeout < float('inf'):
        raise ValueError('FZGX_TOOL_IDLE_TIMEOUT_S must be finite and nonnegative')
    client = client_root(family, env['FZGX_AGENT_ID'])
    client.mkdir(parents=True, exist_ok=True)
    cmd=command(family,prompt,model,client,env)
    symbol=env['FZGX_SYMBOL']
    terminal=Path(env['FZGX_RESULT_FILE'])
    log_path=directory / f'{symbol}.log'
    (directory / f'{symbol}.assignment.json').write_text(json.dumps(dict(symbol=symbol,agent=env['FZGX_AGENT_ID'],harness=family,model=model,started=time.time())) + '\n')
    proc=subprocess.Popen(cmd,cwd=client,env=env,stdin=subprocess.DEVNULL,stdout=subprocess.PIPE,stderr=subprocess.STDOUT,
                          text=True,start_new_session=True,bufsize=1)
    usage=Usage()
    budget=threading.Event()
    violation=threading.Event()
    def read():
        with log_path.open('w',buffering=1) as log:
            for line in proc.stdout:
                log.write(line)
                try:
                    row=json.loads(line)
                except ValueError:
                    continue
                if not isinstance(row,dict):
                    continue
                # The toolset is pinned by the session-local agent config, which
                # is the only thing standing between a matcher and a shell. If a
                # call ever escapes it, stop the session immediately instead of
                # letting it wander through the tree burning the assignment.
                if row.get('type') == 'tool_use':
                    name=(row.get('part') or {}).get('tool') or ''
                    if not name.startswith('fzgx_'):
                        violation.set()
                        with (directory / 'violations.jsonl').open('a') as out:
                            out.write(json.dumps(dict(timestamp=time.time(),
                                                       tool=name, agent=env['FZGX_AGENT_ID']))+'\n')
                counters=usage.update(row)
                if counters is not None:
                    atomic_usage(directory / f'{symbol}.usage.json', counters)
                    # 0 disables a guard, as in codex_server.
                    max_in=int(env.get('FZGX_MAX_MODEL_INPUT_TOKENS','0'));max_out=int(env.get('FZGX_MAX_MODEL_OUTPUT_TOKENS','0'))
                    if (max_in and counters['inputTokens'] >= max_in) or (max_out and counters['outputTokens'] >= max_out):
                        budget.set()
    reader=threading.Thread(target=read,daemon=True);reader.start()
    started=time.monotonic()
    progress=ToolProgress(directory / f'{symbol}.tools.jsonl', time.time(),
                          idle_timeout)
    guard_reason=None
    known={}
    drain_failed=False
    last_scan=time.monotonic()
    try:
        owned_processes(proc.pid,known,env['FZGX_AGENT_ID'])
        while proc.poll() is None:
            if time.monotonic()-last_scan>1:
                owned_processes(proc.pid,known,env['FZGX_AGENT_ID']);last_scan=time.monotonic()
            tool_idle=progress.expired(time.time())
            deadline=time.monotonic()-started > timeout_s
            if terminal.exists() or STOP.is_set() or budget.is_set() or violation.is_set() or tool_idle or deadline:
                if not terminal.exists() and not STOP.is_set():
                    guard_reason=('tool-policy' if violation.is_set() else 'token-budget'
                                  if budget.is_set() else 'deadline' if deadline else 'tool-idle')
                # Interrupt the client, not all of its compiler/MCP children.
                # Function-bound tool operations drain before claim cleanup.
                owned_processes(proc.pid,known,env['FZGX_AGENT_ID'])
                proc.send_signal(signal.SIGTERM)
                try:
                    proc.wait(timeout=120)
                except subprocess.TimeoutExpired as error:
                    STOP.set()
                    raise ProcessDrainError('Client did not stop; claim retained') from error
                break
            time.sleep(0.25)
        drain_processes(proc.pid,progress,known,agent=env['FZGX_AGENT_ID'])
        reader.join(timeout=10)
        if guard_reason:
            receipt=dict(timestamp=time.time(), reason=guard_reason, rc=proc.returncode,
                         symbol=symbol, agent=env['FZGX_AGENT_ID'],
                         last_compiler_progress=progress.last_progress,
                         idle_timeout_s=progress.timeout_s)
            (directory / f'{symbol}.guard.json').write_text(json.dumps(receipt)+'\n')
            with log_path.open('a') as log:
                log.write(json.dumps(dict(event='fleet_guard', **receipt))+'\n')
        output=log_path.read_text() if log_path.exists() else ''
        return output,proc.returncode
    except ProcessDrainError as error:
        drain_failed=True
        STOP.set()
        receipt=dict(event='fleet_guard',timestamp=time.time(),reason='drain-failed',
                     symbol=symbol,agent=env['FZGX_AGENT_ID'],error=str(error))
        try:
            (directory/f'{symbol}.guard.json').write_text(json.dumps(receipt)+'\n')
            with log_path.open('a') as log:
                log.write(json.dumps(receipt)+'\n')
        except OSError:
            # A full/unwritable log must not downgrade a claim-retaining error.
            pass
        raise
    finally:
        if proc.poll() is None:
            STOP.set()
            proc.send_signal(signal.SIGTERM)
            try:
                proc.wait(timeout=120)
            except subprocess.TimeoutExpired as error:
                raise ProcessDrainError('Client still alive; claim retained') from error
        if not drain_failed:
            try:
                drain_processes(proc.pid,progress,known,agent=env['FZGX_AGENT_ID'])
            except Exception as error:
                STOP.set()
                raise ProcessDrainError('Cannot prove descendant drain; claim retained') from error
        reader.join(timeout=10)
        proc.stdout.close()
