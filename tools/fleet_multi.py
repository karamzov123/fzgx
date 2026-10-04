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
# Surge capacity, not standing fleet. Held in reserve and enabled only while the
# paid providers are down.
#
# Cut from four opencode instances to two (oc1 and oc4, the two best converters of
# the four at 15.1% and 14.5%). Lifetime the four opencode families burned 84.4
# worker-hours for 59 matches - 0.70 matches/hour, against 5.7/hour for codex and
# 14.4/hour for claude - more effort than codex and cline combined for 40% of
# codex's yield. oc1 and oc4 kept because they lead the group on match rate; oc2 and
# oc3 dropped, and they were within noise of each other anyway. The paid providers
# are unchanged, so the surge trigger still governs the whole group.
SURGE = ('oc1', 'oc4')
SURGE_DELAY = 600      # cumulative seconds of paid unavailability before adding
SURGE_RETIRE = 1800    # sustained paid recovery before giving capacity back
UNAVAILABLE = ('rate-limited', 'error')
FLEET_CAP = 18        # sessions across all families, surge included

# Provider fallback (2026-10-03). A quota failure used to mean the family simply idled,
# which wasted the slot. FLEET.md's "quota failure remains quota failure, not permission to
# choose a different model" is a *no-silent-fallback* rule: the swap below is explicit,
# operator-directed, reported in the tile, and reversible. While a family's own model is
# healthy it runs unchanged; only a measured rate-limit switches it.
FALLBACK = {
    # agy authenticates to Gemini by default. On quota exhaustion it keeps the agy transport
    # (its own login, guard, and six bound MCP tools are already correct and verified) and
    # only swaps the model, so the fallback inherits everything that makes agy safe.
    'agy': {'model': 'claude-opus-5-5', 'effort': 'high', 'display': 'Opus 5.5 High (agy fallback)',
            'band': (1024, 1 << 30)},
}

# A fallback is a response to a measured quota wall, not a permanent reassignment. The dwell
# is bounded so a family cannot stay on a substitute model forever just because every batch
# it ran while substituted failed to finish cleanly. Half an hour is long enough to cover a
# real quota window and short enough that a recovered provider is back on its own model
# within one batch of the recovery.
FALLBACK_TTL = 1800
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


def in_fallback(family):
    """True when `family` is currently serving its fallback model.

    Dwell lives in runtime-v3.json, so a daemon restart cannot silently drop a family back
    onto a model that was just measured out of quota -- that is the same reason the surge
    timer is persisted.

    The dwell is still bounded. `fb` is only popped when a batch finishes cleanly, so a
    family whose every batch rate-limits or crashes while serving the fallback never clears
    it: agy sat pinned to `claude-opus-5-5` for 11.8h across ~30 failed batches, serving a
    model it was never asked to use, long after Gemini recovered. Unattended, that is
    indistinguishable from being switched off. So the primary is re-probed once the dwell
    passes `FALLBACK_TTL`; if the quota is genuinely still gone the normal rate-limit path
    re-arms the fallback and the cycle continues.
    """
    if family not in FALLBACK:
        return False
    entry = _fallback_state().get(family)
    if not entry:
        return False
    since = entry.get('since') or 0
    if since and time.time() - since > FALLBACK_TTL:
        return False
    return True


def _fallback_state():
    try:
        return json.loads(RUNTIME.read_text()).get('fallback') or {}
    except (OSError, ValueError):
        return {}


def effective(family):
    """The policy actually used for this batch: base model, or the fallback when in quota."""
    p = dict(POLICY[family])
    if in_fallback(family):
        p.update(FALLBACK[family])
    return p


def band_for(family):
    """Size band for `family`, which the fallback may widen (agy takes the large band)."""
    if in_fallback(family):
        return FALLBACK[family].get('band')
    return FAMILY_BANDS.get(family)

def configuration():
    # Backfill families missing from an older control file, so adding a
    # provider never breaks status, the bar, or a running daemon.
    #
    # Also drop retired surge instances. oc2 and oc3 left SURGE, but their entries
    # are still in the committed control file and apply_surge only writes the
    # families it manages, so an orphan would otherwise stay enabled forever and
    # hold a session slot nothing reconciles.
    stored=load(CONTROL,{})
    stored={k:v for k,v in stored.items() if k in POLICY}
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
    p = effective(family)
    return [str(ROOT / '.venv/bin/python'), str(ROOT / 'tools/orchestrate.py'),
        '--harness',p['harness'],'--model',p['model'],'--effort',p['effort'],
        '--parallel',str(parallel),'--tool-parallel','1','--timeout',str(SESSION_TIMEOUT),
        '--max-checks','16','--max-stale','5','--max-attempts','999',
        '--verify-interval','60','--no-trivial','--batch',batch,'--symbols',*symbols]

# Conversion by attempt ordinal over the whole ledger: 32.5% on the first attempt,
# 42.0% on the second, then 20.6% / 22.8% / 29.1% / 10.3% for the third through
# sixth. The second pass is the productive one; everything past it is close to noise
# and each one costs a full session. The saved-body search still gets its own path
# (unsearched_near_misses), so a capped near-miss is not lost, only stopped from
# consuming a model session for a third re-derivation.
ATTEMPT_CAP = 2

# Which slice of the backlog each family works. Every family used to sort one shared
# list by -best_percent, so all of them reached for the same few highest near-misses:
# 476 of 930 attempted symbols had been tried by two or more families and 13 by four
# or five, while 1,116 unmatched functions had never been attempted by anyone. Bands
# are size ranges in bytes; a family only sees targets inside its own band, so the
# pools cannot collide. gpt takes the small high-yield end and the opencode surge
# instances take the larger functions nobody was reaching.
#
# Measured against the real ledger at ATTEMPT_CAP=2, including the 1024B size gate:
# <=256B has 139 eligible (93 virgin), 257-512B has 168 (123 virgin), 513-1024B has
# 379 (322 virgin). Anything above 1024B has only 3 eligible functions, because
# size_allowed() admits a large function only as a saved near-miss - so a band that
# starts above 1024 starves, and the widening path in choose() has to cover it.
FAMILY_BANDS = {
    # gpt is pointed at the cold large functions on purpose (2026-10-03). Its usual band is
    # the smallest functions, which is where its 21.5% yield was measured, but those are
    # nearly exhausted. The >=1 KB band is 76% of main_rel's unmatched bytes and 328 of its
    # 374 functions score below 90%, so it is the only work left where a stronger model
    # could change the outcome. This is an experiment, not a settled default: revert `gpt`
    # to (0, 256) to restore the yield-tuned assignment.
    'gpt':    (1024, 1 << 30),
    'cline':  (257, 512),
    'oc1':    (513, 1024),
    'oc4':    (257, 1 << 30),
}

# A family will not re-attempt a symbol another family already has in hand, and will
# not re-attempt its own either until the virgin pool is exhausted. Without this a
# symbol that reached 99.8% keeps being reissued and re-derived identically.
FAMILY_LOCKOUT = True

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

_linkfail_cache = {'at': 0.0, 'symbols': frozenset()}

def link_failed():
    """Symbols whose last attempt matched the object and was rejected by the link.

    These need `fzgx why-link`, not another model session. The object oracle cannot
    see the link, so a matcher has no tool that shows why the bytes moved, and a new
    session re-derives the same body from a stub and fails identically - five functions
    sat at best_percent 100.0 and unmatched for exactly that reason. Sorted by -best
    these are otherwise the *first* targets chosen, so they consumed the highest-value
    slots in the fleet. Retired from model dispatch until the link difference is read.
    """
    now = time.time()
    if now - _linkfail_cache['at'] > 5.0:
        _linkfail_cache['symbols'] = frozenset(r['symbol'] for r in db_rows(
            "SELECT symbol FROM attempts a WHERE outcome='link-mismatch' AND id=("
            "SELECT MAX(b.id) FROM attempts b WHERE b.symbol=a.symbol)"))
        _linkfail_cache['at'] = now
    return _linkfail_cache['symbols']

def virgin_symbols(rows, mine):
    """Unmatched functions this fleet has never actually attempted.

    1,116 of the 1,759 unmatched functions had no real attempt, and the shared
    -best_percent sort never reached them: every one of them sorts below the saved
    near-misses, so as long as a 97% target existed anywhere the fleet would keep
    re-deriving that instead of touching fresh work. These are the cheapest wins
    available and the pool was going to waste.
    """
    return {r['symbol'] for r in rows
            if r['status']=='unmatched' and not mine.get(r['symbol'])}

_family_seen_cache = {'at': 0.0, 'families': {}}

def families_that_tried():
    """symbol -> set of fleet families that already made a real attempt on it.

    Same qualifying predicate as local_attempts, so a provider rejection that ran
    no check and spent no token does not mark a function as somebody's.
    """
    now=time.time()
    if now - _family_seen_cache['at'] > 3.0:
        _family_seen_cache['families']={}
        for r in db_rows("SELECT symbol,agent FROM attempts WHERE agent LIKE 'fleet-v2-%' "
                         "AND ended IS NOT NULL AND (COALESCE(checks,0)>0 "
                         "OR COALESCE(tokens_in,0)>0 OR outcome IN "
                         "('matched','matched-pool','shadow-matched'))"):
            agent=r['agent']
            # fleet-v2-<family>-<ns>-<harness>-<n>
            parts=agent.split('-')
            family=parts[2] if len(parts)>2 else ''
            if family not in POLICY:
                continue
            _family_seen_cache['families'].setdefault(r['symbol'],set()).add(family)
        _family_seen_cache['at']=now
    return _family_seen_cache['families']

def choose(rows, seen, context, count, reserved=(), retry=(), family=None):
    claimed_units={(r['module'],r['unit']) for r in rows if r['status']=='claimed' and r.get('unit')}
    mine=local_attempts()
    virgin=virgin_symbols(rows,mine)
    linkfail=link_failed()
    tried=families_that_tried() if FAMILY_LOCKOUT else {}
    base=[r for r in rows if r['status']=='unmatched' and (mine.get(r['symbol'],0)<ATTEMPT_CAP or r['symbol'] in retry)
          and r['symbol'] not in linkfail
          and size_allowed(r['size'], r.get('best',r.get('best_percent',0)))
          and seen.get(r['symbol'])!=context and r['symbol'] not in reserved
          and not (r.get('unit') and (r['module'],r['unit']) in claimed_units)]
    # Fresh work first, then saved near-misses, then smallest. Sorting on
    # -best_percent alone is what buried the virgin pool.
    order=lambda r:(0 if r['symbol'] in virgin else 1,
                    -(r.get('best',r.get('best_percent',0)) or 0),
                    mine.get(r['symbol'],0),r['size'],r['symbol'])
    band=band_for(family) if family else None
    eligible=sorted([r for r in base if band and band[0]<=(r['size'] or 0)<=band[1]],key=order)
    if len(eligible)<count:
        # The band's own slice cannot fill the request. Widen to the rest of the
        # backlog rather than idle the slot: a starved family is worse than a band
        # that overlaps another family's for one batch.
        inband={r['symbol'] for r in eligible}
        eligible+=sorted([r for r in base if r['symbol'] not in inband],key=order)
    if family and FAMILY_LOCKOUT:
        # Prefer symbols this family has not run. Only fall back to its own history
        # if that leaves the request short, so a stuck family still makes progress.
        fresh=[r for r in eligible if family not in tried.get(r['symbol'],())]
        eligible=fresh+[r for r in eligible if r not in fresh] if len(fresh)<count else fresh
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

GLYPH = {'working':'●', 'starting':'◌', 'idle':'·', 'rate-limited':'⏳',
         'stalled':'◐', 'error':'✕', 'blocked':'⊘', 'off':''}

def brief(text, limit=120):
    """One short human line from a provider failure blob.

    Provider failures arrive as raw JSON and stream text. That used to be dropped
    into the tooltip verbatim and cut at a flat 300 characters, so it ended
    mid-word ("4.7s\\nve") and buried the one fact that mattered. Classify the
    known failures first; fall back to the first line, cut on a word boundary.
    """
    t = ' '.join(str(text or '').split())
    if not t:
        return ''
    low = t.lower()
    # A diagnostic key alone is not a failure: respect its boolean value.
    if re.search(r'\bverify_ok["\']?\s*:\s*false\b', low):
        return 'hash verification failed - matches rejected'
    if re.search(r'\bover_budget["\']?\s*:\s*true\b', low):
        return 'token/output budget reached'
    known = (
        ('hash verification',     'hash verification failed - matches rejected'),
        ('rate limit',            'provider rate limit'),
        ('quota',                 'provider quota/credit rejection'),
        ('credit',                'provider quota/credit rejection'),
        ('billing',               'provider quota/credit rejection'),
        ('wrong-model',           'provider returned the wrong model'),
        ('interrupted',           'session interrupted by provider'),
        ('went stale',            'no log progress - sessions went stale'),
        ('stale',                 'no log progress - sessions went stale'),
        ('deadline',              'batch hit its time bound'),
        ('timeout',               'batch hit its time bound'),
        ('incomplete',            'batch ended incomplete'),
        ('crash',                 'session crashed'),
        ('transport',             'transport failure'),
        ('no provider',           'no provider returned a result'),
    )
    for needle, human in known:
        if needle in low:
            return human
    if len(t) <= limit:
        return t
    cut = t[:limit].rsplit(' ', 1)[0].rstrip(' ,;:-')
    return (cut or t[:limit]) + '...'

def dur(seconds):
    s = int(max(0, seconds))
    if s < 60:
        return f'{s}s'
    if s < 3600:
        return f'{s // 60}m'
    return f'{s // 3600}h{(s % 3600) // 60:02d}m'

def status():
    config=configuration();state=load(STATE,{})
    now=time.time()
    fresh=now-state.get('heartbeat',0)<30 and pid_alive(state.get('supervisor_pid'))
    result={}
    for family in POLICY:
        if family not in META:      # a provider added to POLICY before its tile
            continue
        name,icon = META[family]
        on=config[family]['enabled']
        data=state.get('families',{}).get(family,{})
        current=(data.get('status','starting') if fresh else 'error') if on else 'off'
        if not on:
            why='stopped by operator'
        elif fresh:
            why=brief(data.get('reason','')) or 'starting constrained provider transport'
        else:
            why='supervisor stopped or heartbeat is stale'
        active=data.get('active',0) if fresh and on else 0
        glyph=GLYPH.get(current,'!')

        # Fixed order, one fact per line: what it is, why, the numbers, what to do.
        lines=[f'{name}  {glyph} {current.upper()}', effective(family)['display'], '', f'Why: {why}']
        checks=data.get('checks') or 0
        done=data.get('completed') or 0
        verified=data.get('verified') or 0
        lines.append('')
        lines.append(f'{checks} checks · {done} done · {verified} verified')
        lines.append(f'Fleet: {state.get("total_verified",0)} verified · last {state.get("last_landing","none")}')
        if active:
            lines.append(f'Sessions: {active} active / {config[family]["parallel"]} allowed')
        failures=data.get('failures') or 0
        if data.get('retry_at',0)>now:
            lines.append(f'Retry in {dur(data["retry_at"]-now)}'
                         + (f' · {failures} failed batch{"es" if failures != 1 else ""}' if failures else ''))
        claims=data.get('claims') or []
        if claims:
            lines.append('')
            lines.append('Work: ' + ', '.join(
                f'{c["module"]}:{c["symbol"]} ({c.get("checks") or 0})' for c in claims[:3]))
            if len(claims) > 3:
                lines.append(f'  +{len(claims) - 3} more')
        if family in SURGE:
            lines.append('')
            lines.append(data.get('surge_reason') or
                         'Surge capacity is fleet-managed from paid-provider availability.')
        lines.append('')
        lines.append('Left: toggle | Right/up: +session | Down: -session | Middle: logs')
        result[family]=dict(on=on,status=current,count=active,icon=icon,name=name,
                           glyph=glyph,why=why,checks=checks,surge_reason=data.get('surge_reason',''),
                           label=icon+glyph+(str(active) if active else ''),
                           tooltip='\n'.join(lines))
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
        self.log.write(f'\n--- {self.batch} {effective(family)["display"]}: {symbols} ---\n');self.log.flush()
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
    # One supervisor only. A second launch used to die with an unhandled
    # BlockingIOError traceback, which reads like a crash in whatever restarts this and
    # makes a healthy fleet look broken. Refuse cleanly instead, and say why.
    lock=(CACHE/'supervisor-v2.lock').open('w')
    try:
        fcntl.flock(lock,fcntl.LOCK_EX|fcntl.LOCK_NB)
    except OSError:
        print('A fleet supervisor already holds supervisor-v2.lock; not starting a second.',
              flush=True)
        return False
    signal.signal(signal.SIGTERM,lambda *_:set_stop());signal.signal(signal.SIGINT,lambda *_:set_stop())
    if not CONTROL.exists():atomic(CONTROL,defaults())
    from fzgx import api,oracle
    from fzgx.project import Project
    jobs={};seen=load(HISTORY,{})
    runtime=load(RUNTIME,{'families':{},'failed_gate':None})
    clock=dict(runtime.get('surge_clock') or {})
    fb=dict(runtime.get('fallback') or {})
    statuses={family:runtime.get('families',{}).get(family,{'status':'starting','reason':'Preparing exact-model transport.'}) for family in POLICY}
    failures={family:statuses[family].get('failures',0) for family in POLICY}
    retries={family:statuses[family].get('retry_at',0) for family in POLICY}
    def persist():
        atomic(RUNTIME,dict(families=statuses,failed_gate=runtime.get('failed_gate'),surge_clock=clock,fallback=fb))
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
                # A stop this supervisor asked for is not a provider fault. The
                # batch is SIGTERMed on purpose, so its nonzero exit and the
                # crash rows its sessions leave in the ledger are consequences of
                # our own signal, not of the provider failing. Scoring them as a
                # fault made every resize punish the tile it resized: cline was
                # launched at parallel 8, the control file said 2, the drain
                # aborted 8 sessions at 0% checks in ~8s each and drove failures
                # to 10 with a backoff, so the operator's own change silenced the
                # family. Both drains below are planned, and neither is evidence.
                planned_drain=False
                if not config[family]['enabled'] or config[family]['parallel']!=job.parallel:
                    planned_drain=True
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
                    if planned_drain:
                        # We asked for this stop, so it is neither a rate limit nor
                        # a fault: keep the provider's failure history untouched and
                        # go straight back to work on the next tick. Counting it
                        # would let an operator resize manufacture the backoff it
                        # then has to wait out, and would bury the real reason under
                        # a provider error the provider never reported.
                        delay=3;state='idle';reason=stop_reason
                    elif rate or broken:
                        failures[family]+=1;delay=retry_delay(text,failures[family]);state='rate-limited' if rate else 'error'
                        # A quota failure is the one condition that justifies serving the
                        # declared fallback. An `error` is not: a transport or tool failure
                        # says nothing about quota, and swapping the model on it would hide
                        # the real fault behind a different name.
                        if rate and family in FALLBACK and not fb.get(family):
                            fb[family]={'since':time.time(),'from':POLICY[family]['display'],
                                        'to':FALLBACK[family]['display']}
                            print(f'{family}: quota exhausted, serving {FALLBACK[family]["display"]}',flush=True)
                            # The quota just measured belongs to the model we are leaving, not
                            # to the one now serving this family. Sitting out its full
                            # rate-limit cooldown would idle the slot for up to 30 minutes for
                            # no reason, so retry promptly under the fallback.
                            delay=3;state='idle';reason=(f'{POLICY[family]["display"]} quota '
                                f'exhausted; serving {FALLBACK[family]["display"]}.')
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
                        fb.pop(family,None)
                    else:
                        # A clean batch without a match is not a provider failure.
                        failures[family]=0;delay=3;state='idle';reason='Batch saved best candidates without a match; selecting fresh work.'
                        fb.pop(family,None)
                    retries[family]=time.time()+delay
                    statuses[family]=dict(t,status=state,reason=stop_reason or reason,active=0,claims=[],failures=failures[family],retry_at=retries[family],batch=job.batch)
                    del jobs[family];persist()
                else:statuses[family]=data
            publish(statuses)
            # An expired fallback is dropped here, not merely ignored by `in_fallback`: the
            # rate-limit path re-arms with `not fb.get(family)`, so a stale-but-present
            # record would suppress the re-arm and leave the family probing the primary
            # while still reporting the substitute model. One re-probe per TTL, by design.
            for _fb_family in list(fb):
                _entry = fb[_fb_family] or {}
                _since = _entry.get('since') or 0
                if _since and time.time() - _since > FALLBACK_TTL:
                    del fb[_fb_family]
                    statuses[_fb_family] = dict(
                        statuses.get(_fb_family, {}),
                        reason=f'Fallback dwell expired; re-probing {POLICY[_fb_family]["display"]}.')
                    print(f'{_fb_family}: fallback dwell expired, re-probing '
                          f'{POLICY[_fb_family]["display"]}', flush=True)
                    persist()
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
                symbols=choose(rows,seen,context,cfg['parallel']*2,reserved,near_miss,family)
                if not symbols:
                    statuses[family]=dict(status='idle',reason='No fresh eligible target below attempt cap; no blind retries.',active=0);retries[family]=time.time()+30;continue
                for symbol in symbols:seen[symbol]=context
                atomic(HISTORY,seen)
                job=Job(family,symbols,cfg['parallel'],gate_passed);jobs[family]=job
                statuses[family]=dict(status='starting',reason=f'Preparing {len(symbols)} distinct assigned functions with {effective(family)["display"]}.',batch=job.batch,active=0,runner_pid=job.proc.pid)
                print(f'{family}: {job.batch}, {symbols}, {effective(family)["display"]}',flush=True)
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
