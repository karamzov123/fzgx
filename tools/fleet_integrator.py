"""Publish verified local main to the user's fork; never integrate worker branches.

The existing verifier is the sole candidate integrator. This component has no
model, claims, candidate edits, commits, merges, rebases or force pushes.
"""
from __future__ import annotations
import argparse
from contextlib import contextmanager, ExitStack
import fcntl
import json
import os
from pathlib import Path
import sqlite3
import subprocess
import sys
import time

ROOT = Path(__file__).resolve().parents[1]
FORK = 'https://github.com/karamzov123/fzgx.git'
STATE = Path.home()/'.cache/fzgx-agents/integrator-v1.json'

@contextmanager
def exclusive(path):
    path.parent.mkdir(parents=True, exist_ok=True)
    with path.open('a') as file:
        fcntl.flock(file, fcntl.LOCK_EX | fcntl.LOCK_NB)
        try:
            yield
        finally:
            fcntl.flock(file, fcntl.LOCK_UN)

class Publisher:
    def __init__(self, root=ROOT, state=STATE, fork=FORK, *, gate=None, pending=None):
        self.root, self.state, self.fork = Path(root), Path(state), fork
        self.gate = gate or self.real_gate
        self.pending = pending or self.real_pending
        self.data = json.loads(self.state.read_text()) if self.state.exists() else {}

    def git(self, *args, check=True):
        result = subprocess.run(['git', '-c', 'core.hooksPath=/dev/null', *args],
            cwd=self.root, text=True, capture_output=True, timeout=90,
            env={**os.environ, 'GIT_TERMINAL_PROMPT':'0', 'GCM_INTERACTIVE':'never'})
        if check and result.returncode:
            raise RuntimeError(result.stderr.strip() or result.stdout.strip() or 'Git operation failed')
        return result

    def record(self, status, reason, **extra):
        self.data.update(status=status, reason=reason, timestamp=time.time(), **extra)
        self.state.parent.mkdir(parents=True, exist_ok=True)
        tmp=self.state.with_suffix('.tmp')
        tmp.write_text(json.dumps(self.data, indent=2)+'\n');tmp.replace(self.state)
        return dict(self.data)

    def real_pending(self):
        path=self.root/'.fzgx/ledger.db'
        if not path.exists():
            raise RuntimeError('Authoritative ledger is missing; refusing publication')
        with sqlite3.connect(path.as_uri()+'?mode=ro', uri=True) as db:
            return bool(db.execute("SELECT 1 FROM functions WHERE link_state='pending' LIMIT 1").fetchone()) or (self.root/'.fzgx/verify_dependencies.jsonl').exists()

    def real_gate(self):
        # Called only under the same submit/build locks used by the verifier.
        log=self.state.with_suffix('.gate.log')
        with log.open('w') as output:
            for cmd in (['ninja','-j','4','build/GFZE01/ok'],
                        ['build/tools/dtk','shasum','-q','-c','config/GFZE01/build.sha1'],
                        [sys.executable,'tools/fzgx.py','lint']):
                output.write('COMMAND '+repr(cmd)+'\n');output.flush()
                result=subprocess.run(cmd,cwd=self.root,stdout=output,stderr=subprocess.STDOUT,timeout=1800)
                if result.returncode:
                    return False
        return True

    def remote_head(self):
        rows=self.git('ls-remote',self.fork,'refs/heads/main').stdout.splitlines()
        if len(rows)!=1 or rows[0].split()[1]!='refs/heads/main':
            raise RuntimeError('Fork main is missing or ambiguous; refusing branch creation')
        return rows[0].split()[0]

    def once(self):
        try:
            with exclusive(self.state.with_suffix('.lock')):
                # Read again inside singleton ownership (multiple manual invocations).
                if self.state.exists():self.data=json.loads(self.state.read_text())
                try:
                    return self._once()
                except BlockingIOError:
                    return self.record('waiting','Project submit/build lock is busy; yielding to the existing verifier')
                except Exception as error:
                    return self.record('error',str(error)[:1000])
        except BlockingIOError:
            # Do not race the active publisher's status file.
            return dict(status='busy', reason='Another publisher owns the singleton lock')

    def _once(self):
        # Reject personal remote retargeting/multiple push destinations. Actual
        # network commands use the pinned URL, never remote defaults/refspecs.
        for flag in ([], ['--push']):
            urls=self.git('remote','get-url',*flag,'--all','origin').stdout.splitlines()
            if urls != [self.fork]:
                return self.record('blocked','origin does not exclusively identify the approved fork')
        if self.git('symbolic-ref','--short','HEAD').stdout.strip()!='main':
            return self.record('blocked','Only local main may be published; no checkout performed')
        head=self.git('rev-parse','HEAD').stdout.strip()
        remote=self.remote_head()
        if remote==head:
            return self.record('up-to-date','Fork already contains current local main', published=head,remote_head=remote)
        if self.git('merge-base','--is-ancestor',remote,head,check=False).returncode:
            return self.record('blocked','Remote main is divergent or unknown locally; no fetch/merge/rebase/force push',head=head,remote_head=remote)
        paths=self.git('log','--format=','--name-only','--diff-filter=AM',f'{remote}..{head}').stdout.splitlines()
        forbidden=[p for p in paths if Path(p).suffix.lower() in {'.iso','.gcm','.dol','.rel'}
                   or Path(p).name in {'.env','credentials.json','auth.json','jetski_state.pbtxt'}]
        if forbidden:
            return self.record('blocked','Outgoing history includes prohibited retail/auth artifacts: '+', '.join(sorted(set(forbidden)))[:600],head=head)
        if self.data.get('failed_gate')==head:
            return self.record('blocked','Unchanged failing commit; no repeated builds',head=head)
        with ExitStack() as stack:
            # Nonblocking: the publisher yields to candidate submit/verification.
            for name in ('submit.lock','build.lock'):
                stack.enter_context(exclusive(self.root/'.fzgx'/name))
            if self.pending():
                return self.record('waiting','Existing verifier has pending work; no competing integration',head=head)
            if self.git('diff','--quiet','HEAD','--',check=False).returncode:
                return self.record('waiting','Tracked/index changes are present; never stage, reset or commit them',head=head)
            if self.git('rev-parse','HEAD').stdout.strip()!=head:
                return self.record('waiting','HEAD changed before verification; retry next timer tick')
            if self.data.get('verified')!=head:
                if not self.gate():
                    return self.record('blocked','Real hash/lint gate failed; see integrator-v1.gate.log',failed_gate=head,head=head)
                if self.git('rev-parse','HEAD').stdout.strip()!=head or self.git('diff','--quiet','HEAD','--',check=False).returncode:
                    return self.record('waiting','Tree changed during gate; no verification receipt issued')
                self.record('verified','Pinned clean commit passed real hash/lint gate',verified=head,verified_at=time.time(),failed_gate=None)
        # Release compiler locks BEFORE network I/O. Push only the proven SHA;
        # newly accepted local commits are left for the next timer tick.
        self.git('push','--porcelain',self.fork,f'{head}:refs/heads/main')
        observed=self.remote_head()
        if observed!=head:
            return self.record('error','Push readback does not equal the verified target',head=head,remote_head=observed)
        return self.record('published','Fast-forward push verified by exact remote readback',published=head,remote_head=observed)

def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('action',choices=['once','status'],nargs='?',default='status')
    args=parser.parse_args()
    if args.action=='status':
        print(STATE.read_text() if STATE.exists() else json.dumps({'status':'not-run'}))
    else:
        result=Publisher().once();print(json.dumps(result),flush=True)
        if result['status']=='error':raise SystemExit(1)

if __name__=='__main__':main()
