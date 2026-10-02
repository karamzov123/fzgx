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

STOP = threading.Event()

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
    try:
        while proc.poll() is None:
            if terminal.exists() or STOP.is_set() or budget.is_set() or violation.is_set() or time.monotonic()-started > timeout_s:
                # Interrupt the client, not all of its compiler/MCP children.
                # Function-bound tool operations drain before claim cleanup.
                proc.send_signal(signal.SIGTERM)
                proc.wait(timeout=120)
                break
            time.sleep(0.25)
        reader.join(timeout=10)
        output=log_path.read_text() if log_path.exists() else ''
        return output,proc.returncode
    finally:
        if proc.poll() is None:
            # Refuse to release/reassign ownership while a client is alive.
            proc.send_signal(signal.SIGTERM);proc.wait(timeout=120)
        try:
            os.killpg(proc.pid,signal.SIGTERM)
        except ProcessLookupError:
            pass
        reader.join(timeout=10)
        proc.stdout.close()
