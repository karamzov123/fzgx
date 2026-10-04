"""Fleet-wide probe: does a different optimisation level beat as-is, per function?

Follows the measurement in docs/REGISTER_REPAIR.md. Taking the best of -O2/-O3/-O4 against
as-is never regressed (0 of 25) and won on 28% of the sample, because a level that hurts a
function simply loses to as-is -- unlike committing to one level, which lost 9 of 21.

Reports only. It never writes unit options and never reconfigures the build: applying a level
means `_set_unit_opts` plus `_reconfigure_and_split`, which mutates the build and must not
race the live fleet, so that decision stays with whoever submits.

The population is "functions with a local body", not a ledger score band. For migrated
functions the recorded percentage came from another machine and does not describe the source
on this tree -- fn_10_8DFC is recorded at 90%+ and compiles at 10.14%.
"""

import json
import os
import tempfile
from pathlib import Path
from typing import Dict, Optional

from . import api, oracle

# All six optimisation levels the compiler accepts (-O5 and -Ot are rejected as unknown).
# The first three were the original set; -O0/-O1/-Os are included because the optimum is
# per-function and a level that looks catastrophic on one function is not evidence about
# another. -lmw and -use_lmw_stwm are deliberately excluded: they change prologue and
# epilogue register-save style, which is an ABI concern rather than a tuning knob.
LEVELS = ('-O0', '-O1', '-O2', '-O3', '-O4', '-Os')
_OUT = (Path(os.environ.get('FZGX_STATE', Path(__file__).resolve().parents[2] / '.fzgx'))
         / 'levelscan.json')


def _score(p, sym, words, body: str, flag: Optional[str]):
    """Compile one body under one setting and score it against retail `words`, or None."""
    with tempfile.TemporaryDirectory() as td:
        src, obj = Path(td) / 'b.c', Path(td) / 'b.o'
        src.write_text(body)
        cp = oracle.compile_source(p, sym.module, src, obj, None, flag)
        if cp.returncode:
            return None
        ours = oracle.words(obj, sym.name)
    if not ours:
        return None
    return oracle.word_score(words, ours)[0]


def _save(out: Path, checked: int, wins: Dict[str, dict]) -> None:
    out.parent.mkdir(parents=True, exist_ok=True)
    out.write_text(json.dumps({'checked': checked, 'wins': list(wins.values())}, indent=1))


def run(p, limit: int = 0, out: Path = _OUT, resume: bool = True,
        log=lambda *_: None) -> Dict[str, object]:
    """Probe every function with a local body; record strict wins. Resumable."""
    from .ledger import Ledger
    wins: Dict[str, dict] = {}
    if resume and out.exists():
        try:
            wins = {r['symbol']: r for r in json.loads(out.read_text()).get('wins', [])}
        except Exception:
            wins = {}
    checked = 0
    db = Ledger().db
    names = [s for (s,) in db.execute(
        "SELECT symbol FROM functions WHERE status='unmatched' AND attempts>0 ORDER BY symbol")]
    todo = [s for s in names if s not in wins]
    if limit:
        todo = todo[:limit]
    for i, symbol in enumerate(todo, 1):
        try:
            body = api._attempt_text(p, symbol)
            if not body:
                continue
            sym = p.resolve(symbol)
            words = oracle.words(Path(p.target_object_for(sym)), sym.name)
            if not words:
                continue
            base = _score(p, sym, words, body, None)
            if base is None:
                continue
            best, best_flag, scores = base, None, {}
            for flag in LEVELS:
                v = _score(p, sym, words, body, flag)
                scores[flag] = v
                if v is not None and v > best + 1e-9:
                    best, best_flag = v, flag
            checked += 1
            if best_flag:
                wins[symbol] = {'symbol': symbol, 'module': sym.module, 'flag': best_flag,
                                'as_is': round(base, 3), 'best': round(best, 3),
                                'gain': round(best - base, 3), 'scores': scores}
                log('WIN  %-16s %6.2f%% -> %6.2f%% via %s (+%.1f)'
                    % (symbol, base, best, best_flag, best - base))
        except Exception as e:  # one bad function must not end the scan
            log('skip %-16s %s' % (symbol, repr(e)[:60]))
        if i % 5 == 0:
            _save(out, checked, wins)
    _save(out, checked, wins)
    return {'ok': True, 'checked': checked, 'wins': len(wins), 'out': str(out)}


def format_report(r: Dict[str, object]) -> str:
    return 'LEVELSCAN checked=%s wins=%s -> %s' % (r.get('checked'), r.get('wins'), r.get('out'))
