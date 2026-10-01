#!/usr/bin/env python3
"""Four real providers, one ledger, explicit model policy, independent backoff."""
from __future__ import annotations
import fcntl
import hashlib
import json
import os
import re
from pathlib import Path
import signal
import subprocess
import sys
import time
from fleet import (ROOT, CACHE, META, atomic, load, pid_alive, activity_state,
                   cooldown, context_id, db_rows, telemetry, verified_progress,
                   record_gate, run_gate, stop_runner, tail_bytes, active_runner_pids, MAX_INPUT_TOKENS, MAX_OUTPUT_TOKENS)

POLICY = {
    'claude': {'harness':'claude','model':'claude-opus-5-5','effort':'high','display':'Opus 5.5 High'},
    'gpt': {'harness':'codex','model':'gpt-6.1-sol','effort':'medium','display':'6.1-Sol Medium'},
    'agy': {'harness':'agy','model':'gemini-3.8-flash-high','effort':'high','display':'Gemini 3.8 High'},
    'cline': {'harness':'cline','model':'stealth/space-bunny-alpha','effort':'high','display':'Space Bunny Alpha High'},
}
CONTROL = CACHE / 'control-v3.json'
STATE = CACHE / 'status-v3.json'
RUNTIME = CACHE / 'runtime-v3.json'
HISTORY = CACHE / 'scheduled-v3.json'
STOP = False

def defaults():
    return {family:{'enabled':True,'parallel':1} for family in POLICY}

def configuration():
    return load(CONTROL, defaults())

def budget_limits(family, parallel, count):
    if family == 'cline':
        return dict(input=1000000,output=128000,batch_input=1000000*count,
                    batch_output=128000*count,log_bytes=12*1024*1024*parallel,
                    # Usage arrives only when a response ends; a first high-effort
                    # response of 15-33k tokens takes 150-300+ s, so wait past the
                    # 600 s session timeout rather than stop the whole batch.
                    guard_each=False,usage_wait=660)
    return dict(input=MAX_INPUT_TOKENS,output=MAX_OUTPUT_TOKENS,
                batch_input=1000000,batch_output=80000,log_bytes=12*1024*1024,
                guard_each=True,usage_wait=120)

def command(family, batch, symbols, parallel):
    p = POLICY[family]
    return [str(ROOT / '.venv/bin/python'), str(ROOT / 'tools/orchestrate.py'),
        '--harness',p['harness'],'--model',p['model'],'--effort',p['effort'],
        '--parallel',str(parallel),'--tool-parallel','1','--timeout','600',
        '--max-checks','8','--max-stale','3','--max-attempts','3',
        '--verify-interval','60','--no-trivial','--batch',batch,'--symbols',*symbols]

def choose(rows, seen, context, count, reserved=()):
    claimed_units={(r['module'],r['unit']) for r in rows if r['status']=='claimed' and r.get('unit')}
    eligible=[r for r in rows if r['status']=='unmatched' and r['attempts']<3 and r['size']<=1024
              and seen.get(r['symbol'])!=context and r['symbol'] not in reserved
              and not (r.get('unit') and (r['module'],r['unit']) in claimed_units)]
    eligible.sort(key=lambda r:(-(r.get('best',r.get('best_percent',0)) or 0),r['attempts'],r['size'],r['symbol']))
    used=set(claimed_units)
    selected=[]
    for row in eligible:
        unit=(row['module'],row.get('unit') or row['symbol'])
        if unit in used:
            continue
        selected.append(row['symbol']);used.add(unit)
        if len(selected)>=count:
            break
    return selected

def policy_context():
    digest=hashlib.sha256((context_id()+json.dumps(POLICY,sort_keys=True)).encode())
    for name in ('fleet_clients.py','fleet_cline.mjs','fleet_mcp.py','fleet_agy_guard.py','fleet_provider.py'):
        digest.update((ROOT/'tools'/name).read_bytes())
    return digest.hexdigest()

def retry_delay(text, failures):
    reset=re.search(r'Resets in\s+(?:(\d+)h)?\s*(?:(\d+)m)?\s*(?:(\d+)s)?',text,re.I)
    if reset and any(reset.groups()):
        hours,minutes,seconds=(int(x or 0) for x in reset.groups())
        return max(60,hours*3600+minutes*60+seconds+5)
    return cooldown(text,failures)

def gate_identity(context):
    digest=hashlib.sha256()
    for path in [ROOT / 'config/GFZE01/units.json',*sorted((ROOT/'config/GFZE01').glob('*/splits.txt'))]:
        if path.exists():
            digest.update(path.read_bytes())
    head=subprocess.check_output(['git','rev-parse','HEAD'],cwd=ROOT,text=True).strip()
    return [head,context,digest.hexdigest()]

def publish(families):
    total,last=verified_progress()
    atomic(STATE,dict(families=families,heartbeat=time.time(),supervisor_pid=os.getpid(),
                       total_verified=total,last_landing=last))

def status():
    config=configuration();state=load(STATE,{})
    fresh=time.time()-state.get('heartbeat',0)<30 and pid_alive(state.get('supervisor_pid'))
    result={}
    for family,(name,icon) in META.items():
        on=config[family]['enabled']
        data=state.get('families',{}).get(family,{})
        current=(data.get('status','starting') if fresh else 'error') if on else 'off'
        reason=data.get('reason','Starting constrained provider transport.') if fresh else 'Supervisor stopped/stale; no active work inferred from a PID.'
        if not on: reason='Stopped by operator.'
        active=data.get('active',0) if fresh and on else 0
        suffix={'working':'●','starting':'◌','idle':'·','blocked':'!','error':'!','stalled':'!','rate-limited':'⏳','off':''}.get(current,'!')
        tip=f"{name}: {current.upper()} — {POLICY[family]['display']}\n{reason}\n{active} claimed / {config[family]['parallel']} session limit"
        tip+=f"\nBatch: {data.get('checks',0)} checks, {data.get('completed',0)} finished, {data.get('verified',0)} link-verified"
        tip+=f"\nFleet total: {state.get('total_verified',0)} unique link-verified; last commit {state.get('last_landing','none')}"
        tip+='\n'+'\n'.join(f"{c['module']}: {c['symbol']} ({c.get('checks') or 0} checks)" for c in data.get('claims',[]))
        if data.get('retry_at',0)>time.time():tip+=f"\nAutomatic retry in {int(data['retry_at']-time.time())}s; no model fallback."
        tip+='\nLeft: toggle | Right/up: +session | Down: -session | Middle: logs'
        result[family]=dict(on=on,status=current,count=active,icon=icon,name=name,
                           label=icon+suffix+(str(active) if active else ''),tooltip=tip)
    return result

def control(action,family):
    if family not in POLICY:raise ValueError('Unknown provider')
    CACHE.mkdir(parents=True,exist_ok=True)
    with (CACHE/'control-v3.lock').open('w') as lock:
        fcntl.flock(lock,fcntl.LOCK_EX)
        config=configuration();value=config[family]
        if action=='toggle':value['enabled']=not value['enabled']
        elif action=='scale-up':
            desired=min(8 if family=='cline' else 4,value['parallel']+1)
            others=sum(c['parallel'] for k,c in config.items() if k!=family and c['enabled'])
            if others+desired>12:
                print('Fleet concurrency cap is 12; no extra process launched.');return
            value['parallel']=desired;value['enabled']=True
        elif action=='scale-down':value['parallel']=max(1,value['parallel']-1)
        atomic(CONTROL,config);print(json.dumps(config))

class Job:
    def __init__(self,family,symbols,parallel,gate_passed):
        self.family,self.symbols,self.parallel=family,symbols,parallel
        self.limits=budget_limits(family,parallel,len(symbols))
        self.batch=f'fleet-v2-{family}-{time.time_ns()}'
        self.started=time.time()
        self.directory=ROOT/'.fzgx/runs'/self.batch
        record_gate(self.directory,gate_passed,self.started)
        (CACHE/'logs').mkdir(parents=True,exist_ok=True)
        self.logpath=CACHE/'logs'/f'{family}.log'
        self.log=self.logpath.open('a');self.offset=self.log.tell()
        self.log.write(f'\n--- {self.batch} {POLICY[family]["display"]}: {symbols} ---\n');self.log.flush()
        self.proc=subprocess.Popen(command(family,self.batch,symbols,parallel),cwd=ROOT,
            stdout=self.log,stderr=subprocess.STDOUT,stdin=subprocess.DEVNULL,
            env={**os.environ,'PYTHONUNBUFFERED':'1','FZGX_BOUND_TRANSPORT':'1',
                'FZGX_MAX_MODEL_INPUT_TOKENS':str(self.limits['input']),'FZGX_MAX_MODEL_OUTPUT_TOKENS':str(self.limits['output'])})
    def close(self):
        stop_runner(self.proc);self.log.close()
    def text(self):
        return tail_bytes(self.logpath,start=self.offset)+'\n'+'\n'.join(tail_bytes(p,4000) for p in self.directory.glob('*.log'))

def run():
    global STOP
    CACHE.mkdir(parents=True,exist_ok=True)
    lock=(CACHE/'supervisor-v2.lock').open('w');fcntl.flock(lock,fcntl.LOCK_EX|fcntl.LOCK_NB)
    signal.signal(signal.SIGTERM,lambda *_:set_stop());signal.signal(signal.SIGINT,lambda *_:set_stop())
    if not CONTROL.exists():atomic(CONTROL,defaults())
    from fzgx import api,oracle
    from fzgx.project import Project
    jobs={};seen=load(HISTORY,{})
    runtime=load(RUNTIME,{'families':{},'failed_gate':None})
    statuses={family:runtime.get('families',{}).get(family,{'status':'starting','reason':'Preparing exact-model transport.'}) for family in POLICY}
    failures={family:statuses[family].get('failures',0) for family in POLICY}
    retries={family:statuses[family].get('retry_at',0) for family in POLICY}
    def persist():
        atomic(RUNTIME,dict(families=statuses,failed_gate=runtime.get('failed_gate')))
    try:
        while not STOP and active_runner_pids():
            publish({k:dict(status='blocked',reason='Waiting for previous host to drain.',active=0) for k in POLICY});time.sleep(3)
        if not STOP:
            p=Project()
            for claim in db_rows("SELECT symbol,claimed_by FROM functions WHERE status='claimed' AND substr(claimed_by,1,9)='fleet-v2-'"):
                r=api.release(p,claim['symbol'],'Four-provider recovery: preserve abandoned candidate',agent=claim['claimed_by'],save_only=True)
                if not r.get('ok'):raise RuntimeError('Claim recovery failed: '+json.dumps(r))
        while not STOP:
            config=configuration();now=time.time()
            for family,job in list(jobs.items()):
                t=telemetry(job.batch,job.limits)
                data=dict(t,batch=job.batch,active=len(t['claims']),status=activity_state(job.proc.poll() is None,len(t['claims']),t['last_event'],job.started,now),
                          reason='Live bounded provider; claims/checks from shared ledger, not inferred success.')
                stop_reason=None
                if not config[family]['enabled'] or config[family]['parallel']!=job.parallel:stop_reason='Operator control changed; draining current work.'
                elif t['verify_ok'] is False:stop_reason='Hash verification failed; saving and holding producers.'
                elif t['over_budget'] or t['usage_missing']:stop_reason='Token/output guard reached or provider usage telemetry missing; saving best candidate.'
                elif now-max(t['last_event'],job.started)>300 or now-job.started>1500:stop_reason='Activity/batch deadline; saving best candidate.'
                if stop_reason:
                    data['reason']=stop_reason;publish(dict(statuses,**{family:data}));job.close()
                if job.proc.poll() is not None:
                    job.close();t=telemetry(job.batch,job.limits)
                    text=job.text();rc=job.proc.returncode
                    rows=[]
                    path=job.directory/'results.json'
                    if path.exists():
                        value=load(path,[]);rows=value if isinstance(value,list) else value.get('results',[]) if isinstance(value,dict) else []
                    broken=rc!=0 or t['verify_ok'] is not True or any(str(r.get('outcome','')).startswith(('crash','timeout','incomplete','WRONG-MODEL')) for r in rows)
                    rate=cooldown(text,0)==1800
                    if rate or broken:
                        failures[family]+=1;delay=retry_delay(text,failures[family]);state='rate-limited' if rate else 'error'
                        reason=(text.strip().splitlines()[-1] if text.strip() else 'Provider request failed.')[:300]
                    elif t['verified']:
                        failures[family]=0;delay=3;state='idle';reason='Batch complete with link-verified matches; selecting fresh work.'
                    else:
                        failures[family]+=1;delay=min(300,30*failures[family]);state='idle';reason='Batch saved best candidates without a match; bounded automatic backoff.'
                    retries[family]=time.time()+delay
                    statuses[family]=dict(t,status=state,reason=stop_reason or reason,active=0,claims=[],failures=failures[family],retry_at=retries[family],batch=job.batch)
                    del jobs[family];persist()
                else:statuses[family]=data
            publish(statuses)
            for family in POLICY:
                if STOP:break
                cfg=config[family]
                if family in jobs:continue
                if not cfg['enabled']:
                    statuses[family]=dict(status='off',reason='Stopped by operator.',active=0);continue
                if time.time()<retries[family]:continue
                context=policy_context();identity=gate_identity(context)
                if runtime.get('failed_gate')==identity:
                    statuses[family]=dict(status='blocked',reason='Unchanged failing 16-target gate. No repeated builds or model requests.',active=0);continue
                statuses[family]=dict(status='starting',reason='Checking real 16-target hash gate before assignment.',active=0);publish(statuses)
                with oracle.build_lock('submit.lock',timeout_s=1800),oracle.build_lock():
                    passed=run_gate()
                if not passed:
                    runtime['failed_gate']=identity;persist();continue
                runtime['failed_gate']=None
                rows=api.inventory(Project())
                reserved={s for job in jobs.values() for s in job.symbols}
                # Reserve whole existing units too, including assignments whose
                # claim subprocess has not yet committed its SQLite transaction.
                index={r['symbol']:r for r in rows}
                reserved_units={(index[s]['module'],index[s]['unit']) for s in reserved if s in index and index[s].get('unit')}
                for row in rows:
                    if row.get('unit') and (row['module'],row['unit']) in reserved_units:row['status']='claimed'
                symbols=choose(rows,seen,context,cfg['parallel']*2,reserved)
                if not symbols:
                    statuses[family]=dict(status='idle',reason='No fresh eligible target below attempt cap; no blind retries.',active=0);retries[family]=time.time()+30;continue
                for symbol in symbols:seen[symbol]=context
                atomic(HISTORY,seen)
                job=Job(family,symbols,cfg['parallel'],passed);jobs[family]=job
                statuses[family]=dict(status='starting',reason=f'Preparing {len(symbols)} distinct assigned functions with {POLICY[family]["display"]}.',batch=job.batch,active=0,runner_pid=job.proc.pid)
                print(f'{family}: {job.batch}, {symbols}, {POLICY[family]["display"]}',flush=True)
                persist();publish(statuses)
            time.sleep(3)
    finally:
        for family,job in list(jobs.items()):job.close()
        publish({k:dict(status='off',active=0,reason='Supervisor stopped; bound tools drained.') for k in POLICY})

def set_stop():
    global STOP
    STOP=True

def main():
    args=sys.argv[1:];action=args[0] if args else 'status'
    if action=='status':print(json.dumps(status()))
    elif action=='daemon':run()
    elif action in ('toggle','scale-up','scale-down'):control(action,args[1])
    elif action=='scroll':control('scale-up' if args[2]=='up' else 'scale-down',args[1])
    elif action=='watch':
        family=args[1]
        if family not in POLICY:raise ValueError('Unknown provider')
        subprocess.run([str(Path.home()/'.config/eww/scripts/foot-popup.sh'),f'fzgx-{family}','950','650','tail','-F',str(CACHE/'logs'/f'{family}.log')],check=False)
    elif action=='reset-limits':print('All providers use their pinned models and automatic cooldown; no unsafe quota reset.')
    else:raise SystemExit('Unknown fleet command')

if __name__=='__main__':main()
