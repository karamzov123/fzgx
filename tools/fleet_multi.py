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
from datetime import datetime, timedelta
from fleet import (ROOT, CACHE, META, atomic, load, pid_alive, activity_state, provider_error_text,
                   cooldown, context_id, db_rows, telemetry, verified_progress,
                   record_gate, run_gate, stop_runner, tail_bytes, active_runner_pids)

POLICY = {
    'claude': {'harness':'claude','model':'claude-opus-5-5','effort':'high','display':'Opus 5.5 High'},
    'gpt': {'harness':'codex','model':'gpt-6.1-sol','effort':'medium','display':'6.1-Sol Medium'},
    'agy': {'harness':'agy','model':'gemini-3.8-flash-high','effort':'high','display':'Gemini 3.8 High'},
    'cline': {'harness':'cline','model':'stealth/space-bunny-alpha','effort':'high','display':'Space Bunny Alpha High'},
}
# Surge capacity, not standing fleet. Four opencode instances on the free model
# at xhigh, held in reserve and enabled only while the paid providers are down.
# They are the same work with the same bounds as every other matcher: one
# function per session, six bound tools, no shell, no filesystem.
PAID = ('claude', 'gpt', 'agy')
SURGE = tuple(f'oc{n}' for n in range(1, 5))
SURGE_DELAY = 600      # cumulative seconds of paid unavailability before adding
SURGE_RETIRE = 1800    # sustained paid recovery before giving capacity back
UNAVAILABLE = ('rate-limited', 'error')
FLEET_CAP = 18        # sessions across all families, surge included
for _n in SURGE:
    POLICY[_n] = {'harness':'opencode','model':'opencode/space-bunny-free','effort':'xhigh',
                  'display':f'Space Bunny Free xHigh #{_n[-1]}','managed':True}
del _n
CONTROL = CACHE / 'control-v3.json'
STATE = CACHE / 'status-v3.json'
RUNTIME = CACHE / 'runtime-v3.json'
HISTORY = CACHE / 'scheduled-v3.json'
STOP = False

def defaults():
    # Surge families start disabled: capacity is added by measured paid-provider
    # unavailability, never by being switched on and left running.
    return {family:{'enabled':family not in SURGE,'parallel':1} for family in POLICY}

def configuration():
    # Backfill families missing from an older control file, so adding a
    # provider never breaks status, the bar, or a running daemon.
    stored=load(CONTROL,{})
    return {family:dict(stored.get(family) or defaults()[family]) for family in POLICY}

SESSION_TIMEOUT = 1800   # hang bound only; 16 checks / 5 stale checks end real work
IDLE_TIMEOUT = 900       # no log event from any session of the batch

def budget_limits(family, parallel, count):
    # No token guards: a session is already bounded by its check, stale-check
    # and turn limits, and provider quota rejections have their own cooldown.
    # Cumulative input counts re-read (cached) context every turn, so the old
    # 256k guard killed productive Claude sessions as "crashes" at 7 checks.
    # Only runaway log growth still stops a batch.
    unlimited=float('inf')
    return dict(input=unlimited,output=unlimited,batch_input=unlimited,batch_output=unlimited,
                log_bytes=64*1024*1024*parallel,guard_each=False,usage_wait=unlimited)

def command(family, batch, symbols, parallel):
    p = POLICY[family]
    return [str(ROOT / '.venv/bin/python'), str(ROOT / 'tools/orchestrate.py'),
        '--harness',p['harness'],'--model',p['model'],'--effort',p['effort'],
        '--parallel',str(parallel),'--tool-parallel','1','--timeout',str(SESSION_TIMEOUT),
        '--max-checks','16','--max-stale','5','--max-attempts','999',
        '--verify-interval','60','--no-trivial','--batch',batch,'--symbols',*symbols]

ATTEMPT_CAP = 3

_attempts_cache = {'at': 0.0, 'mine': {}}

def local_attempts():
    """Attempts this fleet made that actually reached a model.

    The restored upstream history (DeepSeek, effort-none and Luna batches of September)
    left 464 small functions at 90%+ past the cap with their saved C on another
    machine; those attempts say nothing about this fleet's models and tools, so
    they do not count toward its cap.

    A provider refused for quota, credits or transport reasons also produces an
    ended attempt, but one with no checks and no tokens: no evidence and no work,
    while still consuming a slot against ATTEMPT_CAP and retiring the function
    from selection. 139 such rows existed when this was measured (58 of them
    codex; the rest agy/claude/cline/opencode), and about 27 functions were
    sitting at ATTEMPT_CAP on nothing but those rows. Both numbers move while the
    fleet runs. An attempt only counts if it ran a check, spent tokens, or matched.

    Memoised for one tick: the daemon calls this twice per family per tick (once
    in choose, once in unsearched_near_misses) and always got the same 300-entry
    dict back, so eight families rebuilt it sixteen times to answer one question.
    Callers only read the result.
    """
    now = time.time()
    if now - _attempts_cache['at'] > 3.0:
        _attempts_cache['mine'] = {r['symbol']:r['n'] for r in db_rows(
            "SELECT symbol,count(*) n FROM attempts WHERE agent LIKE 'fleet-v2-%' AND ended IS NOT NULL "
            "AND (COALESCE(checks,0)>0 OR COALESCE(tokens_in,0)>0 "
            "OR outcome IN ('matched','matched-pool','shadow-matched')) GROUP BY symbol")}
        _attempts_cache['at'] = now
    return _attempts_cache['mine']

def unsearched_near_misses(rows):
    """Functions past the attempt cap whose best saved body the current search engine
    has never run on. Each gets one more pass: the claim searches that body first (a
    match costs no model request), otherwise one model attempt continues from the
    searched body. Its release marks the body, which ends the eligibility."""
    from fzgx import api
    mine=local_attempts()
    wanted={r['symbol'] for r in rows if r['status']=='unmatched' and mine.get(r['symbol'],0)>=ATTEMPT_CAP
            and (r.get('best',r.get('best_percent',0)) or 0)>=90
            and size_allowed(r['size'], r.get('best',r.get('best_percent',0)))}
    if not wanted:
        return set()
    engine=api._engine_id();best={}
    # Filter on symbol in SQL. The predicate was applied in Python, after the query
    # had already fetched all 12k attempt rows, and then stat()ed every saved body
    # and read a marker JSON per candidate - about 1000 filesystem syscalls, every
    # tick, for 60 answers.
    names=sorted(wanted)
    found=[]
    for i in range(0,len(names),400):   # stay well under SQLITE_MAX_VARIABLE_NUMBER
        chunk=names[i:i+400]
        found += db_rows(
            'SELECT symbol,best_body_path,'
            'MAX(COALESCE(final_percent,0),COALESCE(best_in_attempt,0)) score,id '
            'FROM attempts WHERE ended IS NOT NULL AND best_body_path IS NOT NULL '
            f"AND symbol IN ({','.join('?' * len(chunk))}) "
            'ORDER BY symbol, score DESC, id DESC', chunk)
    # Highest score then highest id, exactly as the Python chase used to pick.
    # A bare GROUP BY would not do: the two-argument MAX is scalar, not an
    # aggregate, so best_body_path came from an arbitrary row and usually
    # resolved to a restored upstream body that does not exist on this machine.
    for a in found:
        if a['symbol'] in best:
            continue
        # Restored upstream history names bodies that are not on this machine.
        if Path(a['best_body_path']).exists():
            best[a['symbol']]=(a['score'],a['id'],a['best_body_path'])
    out=set()
    for symbol,(_,_,path) in best.items():
        marker=load(Path(path+'.searched.json'),{})
        if marker.get('engine')!=engine or marker.get('budget_s',0)<api.SEARCH_S:
            out.add(symbol)
    return out

SIZE_GATE = 1024
NEAR_MISS_PCT = 95.0
# A saved body that already diffs at this closeness is worth more per compile than
# a cold small function, so the size gate widens for it instead of excluding it
# outright. The 1024B gate is a throughput heuristic, not a correctness rule: 51
# near-misses above it (fn_1_FD3A8 at 99.49%, fn_10_107D4 at 98.19%) had a local
# body ready and were never scheduled.
def size_allowed(size, best):
    return size<=SIZE_GATE or (best or 0)>=NEAR_MISS_PCT

def choose(rows, seen, context, count, reserved=(), retry=()):
    claimed_units={(r['module'],r['unit']) for r in rows if r['status']=='claimed' and r.get('unit')}
    mine=local_attempts()
    eligible=[r for r in rows if r['status']=='unmatched' and (mine.get(r['symbol'],0)<ATTEMPT_CAP or r['symbol'] in retry)
              and size_allowed(r['size'], r.get('best',r.get('best_percent',0)))
              and seen.get(r['symbol'])!=context and r['symbol'] not in reserved
              and not (r.get('unit') and (r['module'],r['unit']) in claimed_units)]
    eligible.sort(key=lambda r:(-(r.get('best',r.get('best_percent',0)) or 0),mine.get(r['symbol'],0),r['size'],r['symbol']))
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

RESET_IN = re.compile(r'Resets in\s+(?:(\d+)h)?\s*(?:(\d+)m)?\s*(?:(\d+)s)?', re.I)
# Codex states the reset as "try again at Oct 2nd, 2026 1:08 AM" on one turn and
# as "try again at 1:08 AM" on the next; Claude as "resets 1am". The date is
# optional, so both forms resolve to the same wall clock.
RESET_AT = re.compile(r'try again at\s+(?:([A-Z][a-z]{2})\s+(\d{1,2})(?:st|nd|rd|th)?,?\s*(\d{4})?\s*)?'
                      r'(?:at\s*)?(\d{1,2}):(\d{2})\s*(am|pm)', re.I)
RESET_CLOCK = re.compile(r'resets?\s+(\d{1,2})(?::(\d{2}))?\s*(am|pm)', re.I)

def _seconds_until(hour, minute, meridiem):
    """Seconds until the next occurrence of a wall-clock time, in local time.

    Built from now() rather than strptime: a '%Y %I:%M %p' pattern has no day or
    month, so strptime silently defaults to January 1 and every reset lands in
    the distant past.
    """
    try:
        hour12 = int(hour) % 12 or 12
        when = datetime.now().replace(hour=hour12, minute=int(minute or 0),
                                      second=0, microsecond=0)
    except (TypeError, ValueError):
        return None
    if meridiem:
        if meridiem.lower() == 'pm' and hour12 != 12:
            when += timedelta(hours=12)
        elif meridiem.lower() == 'am' and hour12 == 12:
            when -= timedelta(hours=12)
    seconds = (when - datetime.now()).total_seconds()
    if seconds < -60:       # that time already passed today: mean tomorrow
        seconds += 86400
    return max(60, min(86400, seconds + 30))

def retry_delay(text, failures):
    """Backoff that trusts the provider's own reset time when it gives one.

    Codex reports an absolute reset ("try again at Oct 2nd, 2026 1:08 AM"), Claude
    a wall clock ("resets 1am") or a relative one ("Resets in 1h 5m"). Guessing a
    flat half hour for the absolute forms meant retrying into a wall that had not
    lifted yet, which is how a usage limit turned into a crash loop.
    """
    match = RESET_IN.search(text)
    if match and any(match.groups()):
        hours, minutes, seconds = (int(x or 0) for x in match.groups())
        return max(60, hours * 3600 + minutes * 60 + seconds + 5)
    match = RESET_AT.search(text)
    if match:
        month, day, year, hour, minute, meridiem = match.groups()
        if month:
            try:
                when = datetime.strptime(
                    f'{month} {day} {year or datetime.now().year} {int(hour) % 12 or 12}:{minute} {meridiem.upper()}',
                    '%b %d %Y %I:%M %p')
            except ValueError:
                pass
            else:
                # A reset already in the past means the window has passed; retry
                # soon rather than sitting out the full half hour.
                return max(60, min(86400, (when - datetime.now()).total_seconds() + 30))
        else:
            seconds = _seconds_until(hour, minute, meridiem)
            if seconds is not None:
                return seconds
    match = RESET_CLOCK.search(text)
    if match:
        seconds = _seconds_until(*match.groups())
        if seconds is not None:
            return seconds
    return cooldown(text, failures)

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
        if family in SURGE:
            tip+='\n'+data.get('surge_reason','Surge capacity is fleet-managed from paid-provider availability.')
        if data.get('retry_at',0)>time.time():tip+=f"\nAutomatic retry in {int(data['retry_at']-time.time())}s; no model fallback."
        tip+='\nLeft: toggle | Right/up: +session | Down: -session | Middle: logs'
        result[family]=dict(on=on,status=current,count=active,icon=icon,name=name,
                           checks=data.get('checks',0),surge_reason=data.get('surge_reason',''),
                           label=icon+suffix+(str(active) if active else ''),tooltip=tip)
    return result

MANAGED = 'Surge capacity is fleet-managed: it comes and goes with paid-provider availability, so it is not manually switchable. Use the paid tiles to change fleet size.'

def control(action,family):
    if family not in POLICY:raise ValueError('Unknown provider')
    if family in SURGE:
        print(MANAGED);return
    CACHE.mkdir(parents=True,exist_ok=True)
    with (CACHE/'control-v3.lock').open('w') as lock:
        fcntl.flock(lock,fcntl.LOCK_EX)
        config=configuration();value=config[family]
        if action=='toggle':value['enabled']=not value['enabled']
        elif action=='scale-up':
            desired=min(8 if family=='cline' else 4,value['parallel']+1)
            others=sum(c['parallel'] for k,c in config.items() if k!=family and c['enabled'])
            if others+desired>FLEET_CAP:
                print(f'Fleet concurrency cap is {FLEET_CAP}; no extra process launched.');return
            value['parallel']=desired;value['enabled']=True
        elif action=='scale-down':value['parallel']=max(1,value['parallel']-1)
        atomic(CONTROL,config);print(json.dumps(config))

def batch_outcomes(batch):
    """Terminal outcome of every attempt in a batch, straight from the ledger.

    This used to read ``<run dir>/results.json``. Nothing has ever written that
    name: the codex app-server writes ``results.jsonl`` and the bound harnesses
    write no results file at all, so the list was always empty and the
    crash/timeout/WRONG-MODEL check could never fire. A batch whose every session
    died was therefore treated as a clean batch that simply matched nothing, and
    relaunched three seconds later, forever. The ledger records the terminal
    outcome of every attempt regardless of transport.
    """
    return db_rows("SELECT symbol,outcome,checks FROM attempts "
                   "WHERE agent>=? AND agent<? AND ended IS NOT NULL",
                   (batch + '-', batch + '0'))

def paid_unavailable(statuses, config):
    """Paid providers that are not producing work.

    An operator-disabled provider counts. Parking a paid model is a statement
    about capacity, not evidence that it works, and free capacity is still
    wanted in that case. A provider that is merely idle, starting or blocked is
    not down: it is either finishing work or held by a real gate.
    """
    return [f for f in PAID
            if not config[f]['enabled'] or statuses.get(f, {}).get('status') in UNAVAILABLE]

def surge_want(statuses, config, clock):
    """Dwell-timed surge decision, returned with the observed down count.

    Unavailability is accumulated, not wall-clocked. A single five-minute
    outage followed by full health is one five-minute outage, not a ten-minute
    one, so it must not buy capacity; but a provider that keeps lapsing and
    re-limiting does starve the fleet, so its minutes count. Capacity is
    surrendered only after a long healthy stretch, and re-arming then requires
    fresh evidence.
    """
    now = time.time()
    down = paid_unavailable(statuses, config)
    live = len(down) >= 2
    if clock.get('down_since'):          # close out the open down interval
        clock['down_accum'] = clock.get('down_accum', 0) + (now - clock['down_since'])
        clock['down_since'] = 0
    if live:
        clock['down_since'] = now
    if clock.get('surge'):
        if live:
            clock['healthy_since'] = 0
            return down, True
        if not clock.get('healthy_since'):
            clock['healthy_since'] = now
        return down, now - clock['healthy_since'] < SURGE_RETIRE
    if live and clock.get('down_accum', 0) >= SURGE_DELAY:
        return down, True
    return down, False

def apply_surge(statuses, config, clock):
    """Reconcile surge families with the decision. Returns a reason, or None.

    The transition is detected against the committed config, not against the
    dwell bookkeeping, so the file and the decision cannot disagree. Never
    raises the fleet above FLEET_CAP, and never quietly enables fewer than
    requested: a shortfall is reported, once, so the tile can state it.
    """
    down, want = surge_want(statuses, config, clock)
    want = bool(want)
    if want == any(config[f]['enabled'] for f in SURGE):
        return None
    # Deep enough copy: the nested per-family dicts are shared by a bare
    # dict(config), and mutating them in place would make the equality guard
    # below compare the object with itself and silently skip the write.
    committed = {f: dict(c) for f, c in config.items()}
    active = 0
    if want:
        base = sum(c['parallel'] for f, c in config.items() if f not in SURGE and c['enabled'])
        room = max(0, FLEET_CAP - base)
        for family in SURGE:
            if not committed[family]['enabled']:
                if room < 1:
                    break
                committed[family]['enabled'] = True
                room -= 1
        active = sum(1 for f in SURGE if committed[f]['enabled'])
        if not active:
            # Nothing may start under the cap. Say so once instead of failing
            # silently on every tick; re-checked as the paid fleet changes size.
            if clock.get('cap_note'):
                return None
            clock['cap_note'] = True
            return (f'{len(down)}/{len(PAID)} paid providers down; '
                    f'no surge room under fleet cap {FLEET_CAP}.')
    else:
        for family in SURGE:
            committed[family]['enabled'] = False
    if committed == config:
        return None
    clock['surge'] = want
    clock['cap_note'] = False
    atomic(CONTROL, committed)
    up = len(PAID) - len(down)
    if want:
        held = int(clock.get('down_accum', 0))
        note = '' if active == len(SURGE) else f' (fleet cap {FLEET_CAP} reached)'
        return (f'{len(down)}/{len(PAID)} paid providers down for {held}s cumulative; '
                f'surge capacity {active}/{len(SURGE)}{note}.')
    held = int(time.time() - clock.get('healthy_since', time.time()))
    clock['healthy_since'] = 0
    clock['down_accum'] = 0
    return (f'{up}/{len(PAID)} paid providers available for {held}s; surge capacity retired.')

class Job:
    def __init__(self,family,symbols,parallel,gate_passed):
        self.family,self.symbols,self.parallel=family,symbols,parallel
        self.limits=budget_limits(family,parallel,len(symbols))
        self.batch=f'fleet-v2-{family}-{time.time_ns()}'
        self.started=time.time()
        self.deadline=SESSION_TIMEOUT*(-(-len(symbols)//parallel))+600
        self.directory=ROOT/'.fzgx/runs'/self.batch
        record_gate(self.directory,gate_passed,self.started)
        (CACHE/'logs').mkdir(parents=True,exist_ok=True)
        self.logpath=CACHE/'logs'/f'{family}.log'
        self.log=self.logpath.open('a');self.offset=self.log.tell()
        self.log.write(f'\n--- {self.batch} {POLICY[family]["display"]}: {symbols} ---\n');self.log.flush()
        self.proc=subprocess.Popen(command(family,self.batch,symbols,parallel),cwd=ROOT,
            stdout=self.log,stderr=subprocess.STDOUT,stdin=subprocess.DEVNULL,
            env={**os.environ,'PYTHONUNBUFFERED':'1','FZGX_BOUND_TRANSPORT':'1',
                'FZGX_MAX_MODEL_INPUT_TOKENS':'0','FZGX_MAX_MODEL_OUTPUT_TOKENS':'0'})
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
    clock=dict(runtime.get('surge_clock') or {})
    statuses={family:runtime.get('families',{}).get(family,{'status':'starting','reason':'Preparing exact-model transport.'}) for family in POLICY}
    failures={family:statuses[family].get('failures',0) for family in POLICY}
    retries={family:statuses[family].get('retry_at',0) for family in POLICY}
    def persist():
        atomic(RUNTIME,dict(families=statuses,failed_gate=runtime.get('failed_gate'),surge_clock=clock))
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
                previous=statuses.get(family,{})
                data=dict(t,batch=job.batch,active=len(t['claims']),status=activity_state(job.proc.poll() is None,len(t['claims']),t['last_event'],job.started,now),
                          surge_reason=previous.get('surge_reason',''),
                          reason='Live bounded provider; claims/checks from shared ledger, not inferred success.')
                stop_reason=None
                if not config[family]['enabled'] or config[family]['parallel']!=job.parallel:
                    stop_reason=('Surge capacity retired; draining current work.' if family in SURGE
                                 else 'Operator control changed; draining current work.')
                elif t['verify_ok'] is False:stop_reason='Hash verification failed; saving and holding producers.'
                elif t['over_budget']:stop_reason='Batch log size guard reached; saving best candidate.'
                elif now-max(t['last_event'],job.started)>IDLE_TIMEOUT or now-job.started>job.deadline:stop_reason='Activity/batch deadline; saving best candidate.'
                if stop_reason:
                    data['reason']=stop_reason;publish(dict(statuses,**{family:data}));job.close()
                if job.proc.poll() is not None:
                    job.close();t=telemetry(job.batch,job.limits)
                    text=job.text();rc=job.proc.returncode
                    rows=batch_outcomes(job.batch)
                    broken=rc!=0 or t['verify_ok'] is not True or any(str(r['outcome']).startswith(('crash','timeout','incomplete','WRONG-MODEL')) for r in rows)
                    rate=cooldown(text,0)==1800
                    if rate or broken:
                        failures[family]+=1;delay=retry_delay(text,failures[family]);state='rate-limited' if rate else 'error'
                        # Report the provider's own words, not the last line of
                        # the stream: that is usually a tool-timing event, which
                        # told the operator nothing about why the batch stopped.
                        detail = provider_error_text(text).strip()
                        if not detail:
                            tail = text.strip().splitlines()
                            detail = tail[-1] if tail else 'Provider request failed.'
                        reason = detail[:300]
                    elif t['verified']:
                        failures[family]=0;delay=3;state='idle';reason='Batch complete with link-verified matches; selecting fresh work.'
                    else:
                        # A clean batch without a match is not a provider failure.
                        failures[family]=0;delay=3;state='idle';reason='Batch saved best candidates without a match; selecting fresh work.'
                    retries[family]=time.time()+delay
                    statuses[family]=dict(t,status=state,reason=stop_reason or reason,active=0,claims=[],failures=failures[family],retry_at=retries[family],batch=job.batch)
                    del jobs[family];persist()
                else:statuses[family]=data
            publish(statuses)
            # Surge capacity is decided from measured paid-provider state, after
            # this tick's telemetry and before anything is launched.
            note = apply_surge(statuses, config, clock)
            if note:
                for family in SURGE:
                    statuses[family]=dict(statuses[family], surge_reason=note)
                print('surge: ' + note, flush=True)
                persist()
                config = configuration()
            # The 16-target gate is a property of the tree, not of a family. Run it
            # at most once per tick: with eight families, per-family gating held the
            # build lock and rebuilt eight times to learn the same answer. The
            # scheduling context is a function of the same tree, so it is computed
            # once here too rather than per family.
            context=policy_context()
            identity=gate_identity(context)
            gate_passed=None
            # The corpus and the near-miss pool are properties of the tree, not of a
            # family. Both were rebuilt per family: inventory() re-parses 1.7MB of
            # symbols.txt into 20k objects, and the near-miss sweep stat()s every
            # saved body in the ledger. Once per tick, before any rows are marked
            # reserved, also makes the pool independent of what is in flight.
            rows=api.inventory(Project())
            near_miss=unsearched_near_misses(rows)
            for family in POLICY:
                if STOP:break
                cfg=config[family]
                if family in jobs:continue
                if not cfg['enabled']:
                    statuses[family]=dict(status='off',reason='Stopped by operator.',active=0);continue
                if time.time()<retries[family]:continue
                if gate_passed is None:
                    if runtime.get('failed_gate')==identity:
                        statuses[family]=dict(status='blocked',reason='Unchanged failing 16-target gate. No repeated builds or model requests.',active=0);continue
                    statuses[family]=dict(status='starting',reason='Checking real 16-target hash gate before assignment.',active=0);publish(statuses)
                    with oracle.build_lock('submit.lock',timeout_s=1800),oracle.build_lock():
                        gate_passed=run_gate()
                    if not gate_passed:
                        runtime['failed_gate']=identity;persist();continue
                    runtime['failed_gate']=None
                reserved={s for job in jobs.values() for s in job.symbols}
                # Reserve whole existing units too, including assignments whose
                # claim subprocess has not yet committed its SQLite transaction.
                index={r['symbol']:r for r in rows}
                reserved_units={(index[s]['module'],index[s]['unit']) for s in reserved if s in index and index[s].get('unit')}
                for row in rows:
                    if row.get('unit') and (row['module'],row['unit']) in reserved_units:row['status']='claimed'
                symbols=choose(rows,seen,context,cfg['parallel']*2,reserved,near_miss)
                if not symbols:
                    statuses[family]=dict(status='idle',reason='No fresh eligible target below attempt cap; no blind retries.',active=0);retries[family]=time.time()+30;continue
                for symbol in symbols:seen[symbol]=context
                atomic(HISTORY,seen)
                job=Job(family,symbols,cfg['parallel'],gate_passed);jobs[family]=job
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
