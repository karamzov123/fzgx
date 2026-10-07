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
    # DISABLED 2026-10-05, and the pin is deliberately left on the withdrawn model.
    # OpenRouter withdrew `stealth/space-bunny-alpha` from its catalogue, so every session
    # failed in about a second with a 404 "No endpoints found" -- 271 sessions in a day and
    # zero matches, against the best conversion rate of any family (4.0% on the day it was
    # healthy). Repinning to a free model that answers would have been worse than useless:
    # the substitutes that still respond (nemotron-120b, inkling, laguna) are not
    # decompilation-capable, and a matcher that cannot hold a register allocation in its head
    # burns checks without moving a function toward a match. The lane stays configured and
    # disabled until the real model returns or a stronger one is found; `permanent_fault`
    # parks it at PERMANENT_RETRY if it is ever switched back on while still withdrawn.
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
UNAVAILABLE = ('rate-limited', 'error', 'unavailable')
FLEET_CAP = 8         # laptop ceiling across all families, surge included

def resource_snapshot():
    """Linux host headroom; no model credentials or unrelated process data."""
    memory=dict(line.split(':',1) for line in Path('/proc/meminfo').read_text().splitlines())
    disk=os.statvfs(ROOT)
    temperatures=[]
    for sensor in Path('/sys/class/thermal').glob('thermal_zone*/temp'):
        try:
            temperatures.append(int(sensor.read_text()) / 1000)
        except (OSError, ValueError):
            pass
    batteries=[];ac=[]
    for supply in Path('/sys/class/power_supply').glob('*'):
        try:
            kind=(supply/'type').read_text().strip()
            if kind=='Battery':batteries.append(int((supply/'capacity').read_text()))
            elif kind in ('Mains','USB','USB_C'):ac.append((supply/'online').read_text().strip()=='1')
        except (OSError, ValueError):
            pass
    return dict(available_bytes=int(memory['MemAvailable'].split()[0])*1024,
                free_bytes=disk.f_bavail*disk.f_frsize,free_inodes=disk.f_favail,
                temperature_c=max(temperatures,default=0),
                battery_percent=min(batteries,default=100),on_ac=any(ac) if ac else not batteries)

def launch_hold(active, parallel, stats=None):
    """Hold admission under pressure; running bound tools drain normally."""
    if parallel < 1 or active + parallel > FLEET_CAP:
        return f'Global session cap {FLEET_CAP}: {active} active, {parallel} requested.'
    try:
        stats=resource_snapshot() if stats is None else stats
        if stats['available_bytes'] < 2 << 30:return 'Host memory reserve below 2 GiB; holding new batches.'
        if stats['free_bytes'] < 8 << 30:return 'Host disk reserve below 8 GiB; holding new batches.'
        if stats['free_inodes'] < 100000:return 'Host inode reserve below 100000; holding new batches.'
        if stats['temperature_c'] >= 90:return 'Host temperature at least 90 C; holding new batches.'
        if not stats['on_ac'] and stats['battery_percent'] <= 25:return 'Host battery at most 25% off AC; holding new batches.'
    except (OSError, KeyError, ValueError) as error:
        return f'Cannot verify host resource headroom: {error}'
    return ''

# Faults that no amount of retrying can clear. OpenRouter withdrew
# `stealth/space-bunny-alpha` from its catalogue, and every cline session then failed
# with a 404 "No endpoints found" inside one second -- 271 sessions a day, 0 matches,
# against the family's best conversion rate. Classified as a plain `error` it retried
# every RETRY_DELAY for the life of the daemon, so the fleet reported "cline: error,
# retrying" forever instead of naming the cause. Retiring the family until an operator
# repins a model is the only honest response: the model id is gone from the provider,
# not throttled, and no backoff schedule can bring it back.
PERMANENT_FAULT = ('no endpoints found', 'model not found', 'unknown model',
                   'model is not available', 'no providers found', 'freetiererror',
                   'free tier can only be used from within opencode')
PERMANENT_RETRY = 24 * 3600  # do not relaunch known-doomed sessions overnight

# Waves of work per batch. One batch at a time per family (see the `family in jobs` check at
# the launch site), so batch size -- not `parallel` -- is what sets a single-batch family's
# throughput. 2 waves matches the previous behaviour; 4 keeps a family's slots busy through a
# long session instead of idling between short batches.
BATCH_WAVES = 4

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

# Upper bound on any provider-reported retry delay. The longest genuine message observed is
# "Resets in 2h44m22s"; three hours covers it with margin while making it impossible for a
# single mis-parse to park a lane for days.
RETRY_MAX = 3 * 3600
for _n in SURGE:
    POLICY[_n] = {'harness':'opencode','model':'opencode/space-bunny-free','effort':'xhigh',
                  'display':f'Space Bunny Free xHigh #{_n[-1]}','managed':True}
POLICY['oc1'].update(model='opencode/fledge-alpha-free',
                      display='Fledge Alpha Free xHigh (experimental)')
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
    retry_at = entry.get('retry_at') or 0
    # A provider that names its own reset time decides when we come back; the TTL is only
    # the bound for a provider that gave none.
    horizon = retry_at or (since + FALLBACK_TTL)
    if horizon and time.time() > horizon:
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

# One-word tier: functions this fleet has already spent attempts on and whose only remaining
# defect is a single instruction word.
#
# `ATTEMPT_CAP` retires a function after two real attempts, which is right for functions whose
# bodies are wrong -- more attempts re-derive the same wrong source. It is wrong for a function
# that is one instruction away, because the next attempt is not a re-derivation, it is one edit.
# Measured 2026-10-04 over the >=99% band: 24 functions sit at <=2 differing words (seven at
# exactly 1), and **all 24 are past ATTEMPT_CAP and therefore unreachable** by choose(). The
# fleet was spending its whole budget on virgin 1-2 KB work at a measured 12.1% conversion while
# 24 functions sat one word from a match.
#
# `sweep` cannot reach them either: the deterministic engine measured 0 matches from 40,208
# candidates across this band (docs/findings/282), and finding 285 shows why -- the residue is
# one operand, and which operand is a per-function question. A model session with the word
# diff is the right owner, which is exactly what ATTEMPT_CAP was preventing.
#
# The list is data, not a guess: `fzgx shapecensus --words` regenerates it, and it is kept in
# state/ so a reviewer can see the measurement rather than trust the constant. `retry=`
# already bypasses the cap, so these ride the existing mechanism -- no new dispatch path.
#
# Re-measure before trusting this: the tier is empty once the tier is closed, and the file is
# regenerated by `fzgx shapecensus --words --json > state/one_word_tier.json`.
ONE_WORD_TIER = ROOT / "state" / "one_word_tier.json"
ONE_WORD_RESERVE = 4          # slots per batch held for this tier while it is non-empty

# Share of a batch given to the 80-95% mid band. Bounded rather than a strict precedence
# because both extremes of that choice were measured and both are wrong: ranking the mid band
# above virgin took virgin to 0 of 12 picks, and ranking virgin above it (the previous rule)
# took the mid band to 0. 25% keeps the largest proven source -- virgin, 872 conversions --
# holding the majority of every batch while giving the band that had no owner at all a
# guaranteed share.
MID_BAND_SHARE = 0.25

# Share of a batch for the one-word tier. Bounded rather than "all of them": the tier is 18
# functions and letting it lead unbounded took 7 of every 12 slots and left none for virgin
# work, which is the largest proven source of matches (872 conversions). It still leads, because
# each entry is one edit away from done, but it drains instead of crowding the queue out.
TIER_SHARE = 0.33

# Families that share the tier, in a fixed order. The rotation below needs a stable index so
# every family lands on a different subset.
TIER_FAMILIES = ("agy", "cline", "gpt", "oc1", "oc4")


def one_word_tier(family=None):
    """Symbols whose next attempt is a single edit, from the measured snapshot.

    Read fresh each call: the tier drains as functions match, and a cached copy would keep
    handing out work that is already done. A missing or stale file yields an empty set, which
    is the safe direction -- the fleet then behaves exactly as it did before this existed.

    `family` rotates the result so the five families cover disjoint slices instead of all
    proposing the same head of the list. Measured before the rotation existed: five families
    picking six each proposed `fn_10_A90C` five times over -- 30 slots, about 8 distinct
    functions, and the rest of the tier untouched. The claim is atomic so the losers move on,
    but a refused claim is a wasted slot, and the tier is the scarcest work in the project.
    Ordering is by differing words so the rotation is stable and spreads the cheapest first.
    """
    try:
        data = json.loads(ONE_WORD_TIER.read_text())
    except (OSError, ValueError):
        return set()
    rows = data if isinstance(data, list) else data.get("functions", [])
    out = {row["symbol"] for row in rows
           # differing <= 2 AND a body that still diverges: a relocation-only defect reads as
           # 0 words, so a 0/0 entry means "no defect measured", not "one edit away".
           if row.get("symbol") and 0 < row.get("differing", 99) <= 2}
    if not family or len(out) < 2:
        return out
    try:
        offset = TIER_FAMILIES.index(family) * ONE_WORD_RESERVE
    except ValueError:
        return out
    ranked = sorted(out, key=lambda s: (next((r.get("differing", 99) for r in rows
                                             if r.get("symbol") == s), 99), s))
    offset %= len(ranked)
    return set(ranked[offset:] + ranked[:offset])

# The bands below are the only thing that decides where a family looks, and they were sized when
# the small-function pools were fresh. Measured 2026-10-04 against the real backlog:
#
#   band        backlog   virgin   attempted
#   <=256          198        4        194      <- exhausted
#   257-512        283        0        283      <- exhausted
#   513-1K         514        0        514      <- exhausted
#   1-2K           399      339         60      <- where the untouched work is
#   >2K            214      206          8      <- ditto
#
# cline (257,512) and oc1 (513,1024) therefore had **zero** virgin functions available and were
# structurally forced into re-deriving saved near-misses, while 545 virgin functions sat in the
# bands only oc4 and gpt could see. That is where the yield went: over the preceding 6 hours,
# 56 of 98 attempts (57%) landed on functions at >=99% -- the band finding 282 measured at
# **0 matches from 40,208 candidates** -- and 3 (3%) on virgin work that converts at 12.1%.
#
# So the bands now cover the supply that actually exists. cline takes the 1-2K virgin band
# (339 functions, the largest pool, and 12.1% measured conversion); oc1 takes >2K (206, gated
# by SIZE_GATE so only the >=95% subset is reachable -- see the note below); oc4 and gpt keep
# wide bands. Overlapping is deliberate: the widening path in choose() means a family whose band
# is empty falls back to the whole backlog rather than idling, and that fallback is what lets
# cline and oc1 keep working while their own bands drain.
#
# A band is a supply statement, not a yield statement: it says where the untouched work is, and
# the widening path in choose() covers a band that runs dry. Re-measure with the size-band table
# above before changing one.
FAMILY_BANDS = {
    # The whole virgin supply sits in two bands: 330 functions at 1-2 KB and 206 above 2 KB.
    # Nothing below 1 KB is untouched (0 virgin at <=256, 257-512 and 513-1K), so any family
    # banded there is structurally starved and will fall through to re-deriving near-misses.
    # Bands below therefore partition the two live pools rather than repeat the old mistake.
    #
    # cline: the 1-2 KB pool, the largest and the one with measured conversion (12.1%, 55/454).
    # Its old band was (257,512), which held 283 functions and 0 virgin.
    'cline':  (1024, 2048),
    # oc1: above 2 KB, the second pool. size_allowed() admits a function past SIZE_GATE=2048
    # only as a saved near-miss at >=95%, so this band is reached mainly through that path.
    'oc1':    (2048, 1 << 30),
    # oc4 and gpt keep wide bands deliberately: they are the families that should absorb
    # whatever the narrow bands leave, and the widening path in choose() means a narrow band
    # never idles a slot.
    'gpt':    (1024, 1 << 30),
    'oc4':    (257, 1 << 30),
    # agy is a single-slot family, so contention costs it proportionally more than a
    # parallel=6 family, and it had no band at all (band_for returned None, so no size filter
    # applied and it drew from the entire backlog -- 4 of 6 targets overlapping cline/oc4/gpt,
    # and the four phantom symbols at the head of its queue). It takes 512-2048, the widest
    # slice in which it can lead on supply instead of trailing into a pool cline is draining.
    #
    # Overlapping bands are expected and not a bug. Measured: with these bands agy, cline, oc4
    # and gpt propose the *same* four symbols, because FAMILY_LOCKOUT only demotes a symbol
    # among functions this fleet has already run and all four are reaching virgin work for the
    # first time. FAMILY_LOCKOUT does NOT separate them, and an earlier draft of this comment
    # claimed it did -- corrected after measuring it. What actually prevents two families
    # working one symbol is the atomic claim in api.claim: the loser is refused, and the
    # backlog is wide enough that the next tick fills the slot. Verified live: 10 concurrent
    # claims, no symbol held twice.
    'agy':    (512, 2048),
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

SIZE_GATE = 2048
NEAR_MISS_PCT = 95.0
# Fraction of the batch that may go to work past SIZE_GATE, to measure whether the gate is
# still earning its keep rather than assuming it either way. 0 disables the probe.
#
# The gate was raised from 1024 to 2048 on the evidence that "2-4KB is 0/10 and >4KB is
# 0/14". Re-measured over the whole fleet history that evidence is **8 functions, not 24**:
# only 8 above-2KB functions were ever first-touched (4 virgin, 4 near-miss), and none
# converted. That is too thin a sample to close 206 functions and 674,472 B -- **38% of all
# outstanding bytes** -- and it was self-sealing: the gate blocks exactly the functions needed to
# grow the sample.
#
# The real cost was never measured and is concrete. oc1's band is (2048, inf), so:
#
#     oc1 band inventory      214 functions
#     of those servable         8
#
# oc1 could not reach 206 virgin functions in its only band and had 0 virgin reachable. The
# fleet was not being protected from bad work above 2KB; one family was idle.
#
# Rather than move the gate on an 8-sample argument in either direction, the gate stands and a
# quarter of each batch is allowed past it as a **measured probe**. If those conversions land,
# the gate was wrong and should be raised. If they do not, the probe is the evidence the
# original claim never had. Either way the question gets answered with data instead of inherited
# numbers, and the probe is one constant to revert.
OVERSIZE_PROBE = 0.25
# A saved body that already diffs at this closeness is worth more per compile than
# a cold small function, so the size gate widens for it instead of excluding it
# outright.
#
# The gate was 1024B and was starving the fleet. Of the 610 functions this fleet had
# never actually attempted, 598 were blocked by it and only 12 were servable, so the
# "fresh work first" ordering in choose() had almost nothing fresh to order and the
# fleet fell through to whatever was left in base -- which is why the backlog looked
# exhausted. Measured over three days of fleet attempts, the 1-2KB band converts at
# 10.3% (6/58), which is indistinguishable from the 11.4% (370/3232) of the band the
# gate was written to protect. Beyond 2KB the evidence does not support opening it:
# 2-4KB is 0/10 and >4KB is 0/14. So the gate moves to 2048, where the data says yes,
# and stops there.
#
# The 1024B gate is a throughput heuristic, not a correctness rule: 51 near-misses
# above it (fn_1_FD3A8 at 99.49%, fn_10_107D4 at 98.19%) had a local body ready and
# were never scheduled.
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

def choose(rows, seen, context, count, reserved=(), retry=(), family=None, protected_modules=()):
    # Do not let autonomous commits absorb pre-existing operator source edits.
    rows=[r for r in rows if r['module'] not in protected_modules]
    claimed_units={(r['module'],r['unit']) for r in rows if r['status']=='claimed' and r.get('unit')}
    mine=local_attempts()
    virgin=virgin_symbols(rows,mine)
    linkfail=link_failed()
    tried=families_that_tried() if FAMILY_LOCKOUT else {}
    tier=one_word_tier(family)
    # The one-word tier joins `retry`, the existing mechanism for "past the cap, but one more
    # attempt is worth it". It is deliberately NOT filtered by `tried`: family lockout exists to
    # stop five families re-deriving the same wrong body, but a one-word function is not a wrong
    # body -- it is one edit, and every family that runs it produces the same single answer. The
    # atomic claim in api.claim still prevents two families working one symbol at once, and
    # filtering by `tried` emptied the tier for every family when measured, which is how this
    # was found.
    eligible_retry=set(retry)|tier
    base=[r for r in rows if r['status']=='unmatched' and (mine.get(r['symbol'],0)<ATTEMPT_CAP or r['symbol'] in eligible_retry)
          and r['symbol'] not in linkfail
          and size_allowed(r['size'], r.get('best',r.get('best_percent',0)))
          and seen.get(r['symbol'])!=context and r['symbol'] not in reserved
          and not (r.get('unit') and (r['module'],r['unit']) in claimed_units)]
    # Fresh work first, then the one-word tier, then saved near-misses, then smallest. Sorting
    # on -best_percent alone is what buried the virgin pool, and sorting on virgin alone is what
    # buried the tier: a one-word function is worth more than a cold 1-2 KB function regardless
    # of which pool it came from, and 12.1% measured conversion on virgin work does not outrank
    # an edit that closes the function.
    # The 80-95% band: unfinished work that no ordering rule could reach.
    #
    # Measured 2026-10-05: 384 functions sit between 80% and 95%, 149 of them admitted by
    # ATTEMPT_CAP, and **choose() selected 0 of them for every family**. Not because they were
    # ineligible -- `virgin` holds 523 unmatched functions and this band sorts behind all of
    # them, so with virgin-first ordering the queue never reaches it. Simulating the one-word
    # tier drained still gave 0/8, which rules out the tier as the cause and confirms the
    # virgin-first rule is. "Whatever is open" is therefore not true of this band: nothing in
    # the policy would ever pick it up.
    #
    # And it is not a dead shelf. Grouping every function the fleet has ever matched by its
    # best percent before the fleet first touched it:
    #
    #     <80%  872      95-99%  194
    #     80-95%  378     >=99%   64
    #
    # 378 functions have converted *from* this band, against 194 from the near-miss bands the
    # policy does prioritise. It is the second-largest proven source of matches and it had no
    # owner at all. The mid band gets slots ahead of virgin work without displacing the tier.
    mid_band={r['symbol'] for r in rows if r['status']=='unmatched'
              and 80 <= (r.get('best',r.get('best_percent',0)) or 0) < 95
              and mine.get(r['symbol'],0) < ATTEMPT_CAP}
    order=lambda r:(0 if r['symbol'] in tier else 1 if r['symbol'] in mid_band else 2
                    if r['symbol'] in virgin else 3,
                    -(r.get('best',r.get('best_percent',0)) or 0),
                    mine.get(r['symbol'],0),r['size'],r['symbol'])
    band=band_for(family) if family else None
    eligible=sorted([r for r in base if band and band[0]<=(r['size'] or 0)<=band[1]],key=order)
    # The tier is lifted above the band. A family band exists so that families do not all grind
    # the same size shelf; it is a division of labour, not a quality gate, and it cannot be
    # allowed to hide a function that is one instruction from a match. Measured: all 18 tier
    # symbols pass size_allowed, but the band excluded 5 of them outright and demoted the rest,
    # which is what left cline and gpt at 0/6. Everything here is still gated by base -- attempt
    # cap via eligible_retry, link failures, in-flight units and the current-context guard.
    if tier or mid_band:
        # Both pools are lifted above the band for the same reason, and the mid band's case is
        # the sharper one: 126 of its 149 eligible functions are under 1 KB, and every family
        # band starts at 1 KB or higher (cline 1024, agy 512, oc1 2048, oc4 257, gpt 1024), so
        # oc1's band excluded **all** of them and the others saw at most 23. A band is a
        # division of labour, not a quality gate -- it exists so families do not all grind the
        # same size shelf, which is a reason to share, not a reason to hide a pool with no
        # other route to a session. Everything is still gated by base: attempt cap, link
        # failures, in-flight units and the current-context guard.
        lifted=[r for r in base if r['symbol'] in tier or r['symbol'] in mid_band]
        # Dedup by symbol, not by id(). `id()` identifies the dict object, and `lifted` and
        # `eligible` are built by two separate comprehensions over `base`, so the same row is a
        # different object in each and `id(r) not in seen_ids` is always true. That duplicated
        # every lifted row and, worse, left the later `inband` guard reading a list that no
        # longer matched what was being prepended.
        lifted_symbols={r['symbol'] for r in lifted}
        eligible=sorted(lifted,key=order)+[r for r in eligible if r['symbol'] not in lifted_symbols]
    if len(eligible)<count:
        # The band's own slice cannot fill the request. Widen to the rest of the
        # backlog rather than idle the slot: a starved family is worse than a band
        # that overlaps another family's for one batch.
        inband={r['symbol'] for r in eligible}
        eligible+=sorted([r for r in base if r['symbol'] not in inband],key=order)
    if family and FAMILY_LOCKOUT:
        # Prefer symbols this family has not run. Only fall back to its own history
        # if that leaves the request short, so a stuck family still makes progress.
        # Lockout is a division of labour against wasted re-derivation; it is not applied to the
        # tier, because a one-word function is one edit that every family answers the same way,
        # and the atomic claim in api.claim already stops two families holding one symbol. This
        # matters in practice: with lockout applied, cline/gpt/oc4/agy each scored 0/6 tier picks
        # because they had already run most tier symbols, while oc1 scored 3/6. The tier has to
        # be leading the list, not trailing it -- as `fresh + tierrows` it sat behind six virgin
        # functions and was never reached at all.
        keep=[r for r in eligible if r['symbol'] in tier or r['symbol'] in mid_band]
        rest=[r for r in eligible if family not in tried.get(r['symbol'],())
              and r['symbol'] not in tier and r['symbol'] not in mid_band]
        rest+=[r for r in eligible if r not in keep and r not in rest]
        eligible=keep+rest
    # Three pools, each with a bounded share of the batch, then virgin takes the remainder.
    #
    # This replaced a strict precedence, and both extremes were measured and are wrong. Ranking
    # the mid band above virgin took virgin to **0 of 12** picks, and virgin is the largest
    # proven source of matches (872 conversions). Ranking virgin above the mid band is what
    # starved it in the first place, at 0 picks.
    #
    # The caps are enforced by `_pick` below, at the point rows are actually taken, not by
    # reordering the list here. Slicing the list did not bind: the lockout block above has
    # already partitioned rows, so tier rows sit *after* the head and a sliced head left 9 of 12
    # picks in the tier with the cap nominally at 4.
    def _pick(pool, cap):
        out=[]
        for r in eligible:
            if len(out) >= cap: break
            if r['symbol'] in pool and r['symbol'] not in _used:
                out.append(r['symbol']); _used.add(r['symbol'])
        return out
    _used=set()
    n_tier=min(ONE_WORD_RESERVE, int(count * TIER_SHARE))
    n_mid=int(count * MID_BAND_SHARE)
    # context ids already run in this batch; the guard `base` applies to gated rows too
    seen_g=set(seen.values()) if seen else set()
    chosen=_pick(tier, n_tier)
    chosen+=_pick(mid_band, n_mid)
    # Oversize probe: a bounded slice of work past SIZE_GATE, which size_allowed otherwise blocks
    # (see OVERSIZE_PROBE). Drawn from the gated rows explicitly, so it can only ever be this
    # share -- it does not widen the gate for anything else, and a batch with no oversize work
    # simply does not fill this share from it.
    if OVERSIZE_PROBE:
        # `base` only holds size_allowed rows, so the gated ones are re-read from `rows`. They
        # are otherwise-ineligible (that is what the gate means) and are admitted here and
        # nowhere else, which is what makes this a probe rather than a gate change.
        gated=[r for r in rows if r['status']=='unmatched'
               and not size_allowed(r['size'], r.get('best',r.get('best_percent',0)))
               and r['symbol'] not in linkfail
               and mine.get(r['symbol'],0) < ATTEMPT_CAP
               and r['symbol'] not in seen_g
               and r['symbol'] not in reserved
               and not (r.get('unit') and (r['module'],r['unit']) in claimed_units)]
        probe=int(count * OVERSIZE_PROBE)
        n_probe=0
        for r in gated:
            if len(chosen) >= probe + n_tier + n_mid: break
            if r['symbol'] not in _used:
                chosen.append(r['symbol']); _used.add(r['symbol']); n_probe+=1
    # Only pools that were *filled to their cap* have their remaining rows withheld. A pool that
    # could not fill its share has nothing to withhold, so its leftover rows stay in the fallback
    # or the batch idles -- measured at 9 of 12 when every capped pool was withheld regardless of
    # how many rows it actually held.
    full=set()
    if len([s for s in _used if s in tier])>=n_tier: full|=tier
    if len([s for s in _used if s in mid_band])>=n_mid: full|=mid_band
    if OVERSIZE_PROBE and n_probe>=probe: full|={r['symbol'] for r in gated}
    eligible=[r for r in eligible if r['symbol'] not in set(chosen) and r['symbol'] not in full]
    used=set(claimed_units)
    selected=list(chosen)
    for row in eligible:
        if len(selected)>=count:
            break
        unit=(row['module'],row.get('unit') or row['symbol'])
        if unit in used:
            continue
        selected.append(row['symbol']);used.add(unit)
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

def permanent_fault(text):
    """True when the provider says the pinned model itself is gone.

    Deliberately narrow: it matches only the catalogue-withdrawal wording, never a
    generic 404 or transport error, because retiring a family on a transient fault
    would take a working provider offline. The check reads the provider's error text
    rather than the harness exit code, since a withdrawn model exits exactly like any
    other failed session.
    """
    haystack=provider_error_text(text).lower()
    return any(marker in haystack for marker in PERMANENT_FAULT)

def retry_delay(text, failures):
    """Backoff that trusts the provider's own reset time when it gives one.

    Codex reports an absolute reset ("try again at Oct 2nd, 2026 1:08 AM"), Claude
    a wall clock ("resets 1am") or a relative one ("Resets in 1h 5m"). Guessing a
    flat half hour for the absolute forms meant retrying into a wall that had not
    lifted yet, which is how a usage limit turned into a crash loop.

    Whatever the provider says, the result is bounded by `RETRY_MAX`. A single lane
    parked for days is worse than one that retries early and fails: agy was observed
    holding a 147.3h backoff while every message in its own logs said the quota reset
    within 2h44m, so the parse was not the thing producing the number. A cap makes the
    damage bounded whatever the cause, and it is what a six-hour unattended run needs.
    """
    return min(RETRY_MAX, _retry_delay_uncapped(text, failures))


def _retry_delay_uncapped(text, failures):
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
         'stalled':'◐', 'error':'✕', 'blocked':'⊘', 'off':'',
         # A retired family is not retrying and will not recover on its own, so it reads
         # as a stop, not a wait. Leaving it to fall through to '!' put a withdrawn model
         # in the same bucket as a crash.
         'unavailable':'⊗'}

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
                slots = committed[family]['parallel']
                if room < slots:
                    continue
                committed[family]['enabled'] = True
                room -= slots
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
    dirty=subprocess.run(['git','diff','HEAD','--name-only','--','src'],cwd=ROOT,
                         text=True,capture_output=True,check=True).stdout.splitlines()
    protected_modules=set()
    for path in dirty:
        parts=Path(path).parts
        if len(parts)>3 and parts[:2]==('src','rel'):protected_modules.add(parts[2])
        elif len(parts)>2 and parts[:2]==('src','dol'):protected_modules.add('main')
    if protected_modules:
        print('Preserving operator source edits; no dispatch in: '+', '.join(sorted(protected_modules)),flush=True)
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
                    elif permanent_fault(text):
                        # The provider no longer offers the pinned model. Retrying cannot
                        # fix that, and a backoff that grows to RETRY_MAX still leaves the
                        # family launching doomed batches, so park it and say why. The
                        # reason carries the provider's own words so the operator can see
                        # which model id to repin without reading a run directory.
                        delay=PERMANENT_RETRY;state='unavailable'
                        reason=f'{POLICY[family]["model"]} is not served by {POLICY[family]["harness"]}: {provider_error_text(text).strip()[:240]}'
                        print(f'{family}: retired, {reason}',flush=True)
                        failures[family]=0
                    elif rate or broken:
                        failures[family]+=1;delay=retry_delay(text,failures[family]);state='rate-limited' if rate else 'error'
                        # A quota failure is the one condition that justifies serving the
                        # declared fallback. An `error` is not: a transport or tool failure
                        # says nothing about quota, and swapping the model on it would hide
                        # the real fault behind a different name.
                        if rate and family in FALLBACK and not fb.get(family):
                            fb[family]={'since':time.time(),'from':POLICY[family]['display'],
                                        'to':FALLBACK[family]['display'],
                                        # When the provider names its own reset time, that is the
                                        # honest expiry for this fallback, not FALLBACK_TTL. A fixed
                                        # short TTL re-probes into a quota wall that is still up:
                                        # agy said "Resets in 2h37m38s", was re-probed after 30
                                        # minutes, hit the same wall, and the second rate limit
                                        # extended the backoff again. Re-probe when the provider
                                        # says the quota is back.
                                        'retry_at':time.time()+retry_delay(text,failures[family])}
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
                _horizon = _entry.get('retry_at') or (_since + FALLBACK_TTL)
                if _since and time.time() > _horizon:
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
                    statuses[family]=dict(status='off',reason='Stopped by operator.',active=0,
                                          retry_at=retries[family]);continue
                if time.time()<retries[family]:continue
                hold=launch_hold(sum(j.parallel for j in jobs.values()),cfg['parallel'])
                if hold:
                    statuses[family]=dict(status='blocked',reason=hold,active=0);continue
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
                # Batch size is the throughput lever for a family that runs one batch at a time.
                # `if family in jobs: continue` means a family can never have two batches in
                # flight, so `parallel` alone does not set its rate: cline at parallel 6 was
                # assigned 12 symbols per batch and delivered 11 sessions/hour against a
                # 6.5% conversion -- the best rate of any family -- because 12 symbols is all
                # it could ever be working on. Measured inter-batch idle is 1% of wall time
                # (median gap 8 min), so the batches are not idling between runs; they are
                # simply small.
                #
                # A bigger batch does not raise concurrency, so it does not risk more parallel
                # sessions against one account than `parallel` already allows -- it just keeps
                # the same number of slots busy for longer. The deadline scales with it
                # (`SESSION_TIMEOUT * ceil(len(symbols)/parallel) + 600`), and the unit-reserve
                # logic below already handles a larger reservation.
                batch_size=max(cfg['parallel']*2, cfg['parallel']*BATCH_WAVES)
                symbols=choose(rows,seen,context,batch_size,reserved,near_miss,family,
                               protected_modules=protected_modules)
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
