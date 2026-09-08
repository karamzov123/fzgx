"""Library layer shared by the CLI (tools/fzgx.py) and the MCP server (tools/fzgx_mcp.py).

Every function takes plain arguments and returns JSON-serialisable data; no
printing, no sys.exit. Each call opens its own Ledger (SQLite connections are
per-thread) so the MCP server can run calls concurrently.
"""

from __future__ import annotations

import json
import shutil
import subprocess
import time
from pathlib import Path
from typing import Any, Dict, List, Optional

from . import oracle
from .carve import carve
from .context import build_context
from .ledger import Ledger
from .lint import lint_paths
from .project import ROOT, STATE_DIR, Project

DEFAULT_TTL = 1800
MAX_ATTEMPTS = 3
MAX_CHECKS = 8       # per attempt
MAX_STALE = 2        # consecutive checks without improving the attempt's best %
STUB = '#include "types.h"\n\n// {symbol}: carved by fzgx; {note}\n'


def _git(*args: str) -> subprocess.CompletedProcess:
    return subprocess.run(["git", *args], cwd=ROOT, text=True, capture_output=True)


def _reconfigure_and_split(p: Project) -> None:
    with oracle.build_lock():
        cp = oracle.configure(p)
        if cp.returncode != 0:
            raise RuntimeError(cp.stderr[-2000:])
        cp = oracle.run(["ninja", p.rel(p.build_dir / "config.json")])
        if cp.returncode != 0:
            raise RuntimeError((cp.stdout + cp.stderr)[-2000:])


def _unit_source(p: Project, symbol: str) -> Optional[str]:
    sym = p.resolve(symbol)
    return p.unit_of(sym) if sym else None


def _key(p: Project, symbol: str) -> str:
    sym = p.resolve(symbol)
    return p.key(sym) if sym else symbol


# ------------------------------------------------------------------ inventory
def sync(p: Project) -> Dict[str, int]:
    l = Ledger()
    rows = []
    for module in p.modules:
        for s in p.functions(module):
            rows.append({"symbol": p.key(s), "module": module, "unit": p.unit_of(s), "addr": s.addr, "size": s.size})
    n = l.sync_functions(rows)
    matched = 0
    for u in p.load_units():
        if u["status"] == "matching":
            for s in u.get("symbols", []):
                row = l.get(s)
                if row and row["status"] != "matched":
                    l.db.execute("UPDATE functions SET status='matched', best_percent=100 WHERE symbol=?", (s,))
                    matched += 1
    return {"functions": len(rows), "inserted": n, "marked_matched": matched}


def inventory(p: Project, module: Optional[str] = None, status: Optional[str] = None,
              limit: Optional[int] = None, max_size: Optional[int] = None) -> List[Dict[str, Any]]:
    l = Ledger()
    l.expire_claims()
    rows = l.list(module=module, status=status, limit=limit)
    if max_size:
        rows = [r for r in rows if r["size"] <= max_size]
    return [{"symbol": r["symbol"], "module": r["module"], "size": r["size"], "status": r["status"],
             "attempts": r["attempts"], "best": r["best_percent"], "unit": r["unit"],
             "claimed_by": r["claimed_by"]} for r in rows]


# ---------------------------------------------------------------------- claim
def claim(p: Project, symbol: str, agent: str, ttl: int = DEFAULT_TTL,
          max_attempts: int = MAX_ATTEMPTS, no_carve: bool = False) -> Dict[str, Any]:
    l = Ledger()
    if p.resolve(symbol) is None:
        return {"ok": False, "error": f"unknown or ambiguous symbol {symbol!r} (use module:name for _prolog/_epilog)"}
    key = _key(p, symbol)
    try:
        row = l.claim(key, agent, ttl, max_attempts)
    except (LookupError, PermissionError) as e:
        return {"ok": False, "error": str(e)}
    res = None
    if not no_carve:
        try:
            res = carve(p, symbol)
            if res.created:
                _reconfigure_and_split(p)
        except Exception as e:  # release the claim so nobody is stuck
            l.finish(key, "carve-failed", "unmatched", notes=str(e))
            return {"ok": False, "error": f"carve failed: {e}"}
    unit = res.source if res else row["unit"]
    out = {"ok": True, "symbol": symbol, "unit": unit, "path": f"src/{unit}" if unit else None,
           "ranges": res.ranges if res else [], "notes": res.notes if res else [],
           "attempt": row["attempts"] + 1, "max_attempts": max_attempts, "ttl": ttl,
           "budget": f"{MAX_CHECKS} checks per attempt; stop after {MAX_STALE} checks without improvement"}
    try:
        out["context"] = build_context(p, l, symbol)
    except LookupError as e:
        out["context"] = f"(no context: {e})"
    return out


def carve_many(p: Project, symbols: List[str], dry_run: bool = False) -> List[Dict[str, Any]]:
    results, created = [], False
    for s in symbols:
        try:
            r = carve(p, s, dry_run=dry_run)
            created |= r.created
            results.append(r.__dict__)
        except LookupError as e:
            results.append({"symbol": s, "error": str(e)})
    if created and not dry_run:
        _reconfigure_and_split(p)
    return results


# -------------------------------------------------------------------- context
def context(p: Project, symbol: str, budget_tokens: int = 6000) -> str:
    return build_context(p, Ledger(), symbol, budget_tokens)


def read_unit(p: Project, symbol: str) -> Dict[str, Any]:
    unit = _unit_source(p, symbol)
    if not unit:
        return {"ok": False, "error": "not carved"}
    path = ROOT / "src" / unit
    return {"ok": True, "path": f"src/{unit}", "source": path.read_text() if path.exists() else ""}


def write_unit(p: Project, symbol: str, agent: str, source: str) -> Dict[str, Any]:
    """Replace the claimed unit's source. The only write path a matcher has."""
    l = Ledger()
    row = l.get(_key(p, symbol))
    if row is None:
        return {"ok": False, "error": "unknown symbol"}
    if row["status"] != "claimed" or row["claimed_by"] != agent:
        return {"ok": False, "error": f"{symbol} is not claimed by {agent} (status {row['status']}, by {row['claimed_by']})"}
    unit = _unit_source(p, symbol)
    if not unit:
        return {"ok": False, "error": "not carved"}
    if "asm" in source and ("asm {" in source or "asm(" in source or "asm void" in source):
        return {"ok": False, "error": "inline asm is not allowed"}
    att = l.current_attempt(_key(p, symbol))
    stop = _budget_stop(att)
    if stop:
        return {"ok": False, "error": stop + "; call release(symbol, agent, reason) now"}
    path = ROOT / "src" / unit
    path.write_text(source if source.endswith("\n") else source + "\n")
    findings = lint_paths([path])
    result = check(p, symbol)
    return {"ok": True, "path": f"src/{unit}", "bytes": len(source),
            "lint": [{"rule": r, "line": ln, "msg": m} for _, r, ln, m in findings],
            "check": format_check(result)}


def _budget_stop(att) -> Optional[str]:
    if att is None:
        return None
    if (att["checks"] or 0) >= MAX_CHECKS:
        return f"budget exhausted: {MAX_CHECKS} checks used"
    if (att["stale_checks"] or 0) >= MAX_STALE and (att["best_in_attempt"] or 0) < 100.0:
        return f"plateau: {MAX_STALE} consecutive checks without improvement (best {att['best_in_attempt']:.1f}%)"
    return None


# --------------------------------------------------------------------- oracle
def check(p: Project, symbol: str, max_diff_lines: int = 80, versions: Optional[str] = None) -> Dict[str, Any]:
    if versions:
        vers = oracle.CANDIDATE_VERSIONS if versions == "all" else versions.split(",")
        out = oracle.check_versions(p, symbol, vers)
        return {"ok": True, "symbol": symbol, "versions": out,
                "note": "-1 compiler missing, -2 compile error, -3 diff error"}
    res = oracle.check(p, symbol, max_diff_lines)
    out = res.to_json()
    if res.ok:
        key = _key(p, symbol)
        stats = Ledger().bump_checks(key, res.percent)
        out["budget"] = stats
        if stats.get("improved"):
            unit = _unit_source(p, symbol)
            if unit:
                best = STATE_DIR / "attempts" / f"{key}.best.c"
                best.parent.mkdir(parents=True, exist_ok=True)
                shutil.copy(ROOT / "src" / unit, best)
        att = Ledger().current_attempt(key)
        stop = _budget_stop(att)
        if stop and not res.matched:
            out["stop"] = stop
    return out


def format_check(res: Dict[str, Any]) -> str:
    if "versions" in res:
        return "\n".join(f"{v:10s} {'n/a' if pct < 0 else f'{pct:.1f}%'}" for v, pct in res["versions"].items())
    if not res["ok"]:
        return f"CHECK FAILED: {res['error']}"
    lines = [f"{res['symbol']}: {res['percent']:.1f}%  unit={res['unit']}  " + ("MATCH" if res["matched"] else "no match")]
    others = {k: v for k, v in res["symbols"].items() if k != res["symbol"]}
    if others:
        lines.append("other functions in unit: " + ", ".join(f"{k}={v:.0f}%" for k, v in others.items()))
    if res["data_sections"]:
        lines.append("data: " + ", ".join(f"{k}={v:.0f}%" for k, v in res["data_sections"].items()))
    if res["missing_in_base"]:
        lines.append("missing in our object: " + ", ".join(res["missing_in_base"]))
    if res["extra_in_base"]:
        lines.append("extra in our object: " + ", ".join(res["extra_in_base"]))
    if res["diff"]:
        lines.append("diff (target | ours):")
        lines.extend(res["diff"])
    b = res.get("budget")
    if b:
        lines.append(f"budget: check {b['checks']}/{MAX_CHECKS}, {b['stale']}/{MAX_STALE} without improvement, best this attempt {b['best_in_attempt']:.1f}%")
    if res.get("stop"):
        lines.append(f"STOP: {res['stop']}. Do not write again; call release(symbol, agent, reason).")
    return "\n".join(lines)


def submit(p: Project, symbol: str, agent: str = "unknown", message: str = "",
           harness: Optional[str] = None, model: Optional[str] = None,
           mw_version: Optional[str] = None, extra_cflags: Optional[str] = None,
           names: Optional[List[Dict[str, str]]] = None, tokens_in: int = 0,
           tokens_out: int = 0, cost_usd: float = 0.0, max_diff_lines: int = 40) -> Dict[str, Any]:
    l = Ledger()
    sym = p.resolve(symbol)
    if sym is None:
        return {"ok": False, "error": "unknown or ambiguous symbol"}
    key = p.key(sym)
    unit_src = p.unit_of(sym)
    if not unit_src:
        return {"ok": False, "error": "not carved"}
    row = l.get(key)
    if row and row["status"] == "claimed" and row["claimed_by"] not in (agent, None):
        return {"ok": False, "error": f"claimed by {row['claimed_by']}, not {agent}"}
    src_path = ROOT / "src" / unit_src
    findings = lint_paths([src_path])
    if findings:
        return {"ok": False, "error": "lint", "findings": findings}
    if mw_version or extra_cflags:
        _set_unit_opts(p, unit_src, mw_version, extra_cflags)
        _reconfigure_and_split(p)
    res = oracle.check(p, symbol, max_diff_lines)
    reason = oracle.unit_fully_matches(res)
    if reason:
        return {"ok": False, "error": reason, "percent": res.percent, "diff": res.diff}

    with oracle.build_lock():
        units = p.load_units()
        for u in units:
            if u["source"] == unit_src:
                u["status"] = "matching"
        p.save_units(units)
        cp = oracle.configure(p)
        if cp.returncode == 0:
            cp = oracle.relink(p)
        if cp.returncode != 0:
            for u in units:
                if u["source"] == unit_src:
                    u["status"] = "nonmatching"
            p.save_units(units)
            oracle.configure(p)
            tail = "\n".join((cp.stdout + cp.stderr).splitlines()[-25:])
            return {"ok": False, "error": "relink or hash check failed; unit reverted to nonmatching", "output": tail}
        module_cfg = p.module_config_dir(sym.module)
        files = [str(src_path), str(p.units_path), str(module_cfg / "splits.txt"), str(module_cfg / "symbols.txt")]
        _git("add", *files)
        msg = f"match: {sym.module}/{sym.name}" + (f" ({message})" if message else "")
        cp = _git("commit", "-q", "-m", msg, "--", *files)
        commit = _git("rev-parse", "--short", "HEAD").stdout.strip() if cp.returncode == 0 else None
    if names:
        l.propose_names(key, agent, names)
    if row and row["status"] != "claimed":
        # submitted without a live claim (e.g. after a ledger reset): open an attempt so accounting is complete
        l.db.execute("INSERT INTO attempts(symbol, agent, started) VALUES(?,?,?)", (key, agent, int(time.time())))
    l.finish(key, "matched", "matched", commit=commit, notes=message or "", model=model, harness=harness,
             tokens_in=tokens_in, tokens_out=tokens_out, cost_usd=cost_usd)
    return {"ok": True, "symbol": symbol, "commit": commit, "unit": unit_src}


def _set_unit_opts(p: Project, unit_src: str, mw_version: Optional[str], extra_cflags: Optional[str]) -> None:
    units = p.load_units()
    for u in units:
        if u["source"] == unit_src:
            if mw_version:
                u["mw_version"] = mw_version
            if extra_cflags:
                u["extra_cflags"] = extra_cflags.split()
    p.save_units(units)


def release(p: Project, symbol: str, reason: str, harness: Optional[str] = None,
            model: Optional[str] = None, tokens_in: int = 0, tokens_out: int = 0,
            cost_usd: float = 0.0, agent: Optional[str] = None) -> Dict[str, Any]:
    l = Ledger()
    key = _key(p, symbol)
    row = l.get(key)
    if row is None or row["status"] != "claimed":
        return {"ok": False, "error": "not claimed"}
    if agent and row["claimed_by"] != agent:
        return {"ok": False, "error": f"claimed by {row['claimed_by']}, not {agent}"}
    body_path = None
    unit_src = _unit_source(p, symbol)
    if unit_src and (ROOT / "src" / unit_src).exists():
        dest = STATE_DIR / "attempts" / f"{key}.{int(time.time())}.c"
        dest.parent.mkdir(parents=True, exist_ok=True)
        best = STATE_DIR / "attempts" / f"{key}.best.c"
        # keep the best-scoring body, not necessarily the last one written
        shutil.copy(best if best.exists() else ROOT / "src" / unit_src, dest)
        best.unlink(missing_ok=True)
        body_path = str(dest)
        (ROOT / "src" / unit_src).write_text(STUB.format(symbol=symbol, note=f"best attempt saved to {dest.name}"))
    l.finish(key, "released", "unmatched", notes=reason, body_path=body_path, model=model,
             harness=harness, tokens_in=tokens_in, tokens_out=tokens_out, cost_usd=cost_usd)
    return {"ok": True, "symbol": symbol, "saved": body_path, "attempts": row["attempts"] + 1}


# ---------------------------------------------------------------- bookkeeping
def block(p: Project, symbol: str, reason: str, open_issue: bool = False) -> Dict[str, Any]:
    l = Ledger()
    issue = None
    symbol = _key(p, symbol)
    if open_issue:
        row = l.get(symbol)
        body = f"Function `{symbol}` exhausted {row['attempts'] if row else '?'} cheap-tier attempts.\n\n"
        body += f"Best: {row['best_percent']:.1f}%\n\nReason: {reason}\n" if row else reason
        cp = subprocess.run(["gh", "issue", "create", "--title", f"blocked: {symbol}", "--body", body,
                             "--label", "blocked"], cwd=ROOT, text=True, capture_output=True)
        tail = cp.stdout.strip().rsplit("/", 1)[-1]
        if cp.returncode == 0 and tail.isdigit():
            issue = int(tail)
    l.block(symbol, reason, issue)
    return {"ok": True, "symbol": symbol, "issue": issue}


def unblock(p: Project, symbol: str) -> Dict[str, Any]:
    Ledger().unblock(_key(p, symbol))
    return {"ok": True}


def report(p: Project) -> Dict[str, Any]:
    l = Ledger()
    objdiff = None
    rp = p.build_dir / "report.json"
    if rp.exists():
        m = json.loads(rp.read_text()).get("measures", {})
        objdiff = {k: m.get(k) for k in ("matched_code_percent", "matched_functions", "total_functions",
                                          "complete_units", "total_units")}
    return {"ledger": l.summary(), "costs": dict(l.costs()), "objdiff": objdiff}


def snapshot(p: Project) -> Dict[str, Any]:
    return {"ok": True, "path": p.rel(Ledger().snapshot())}


def restore(p: Project) -> Dict[str, Any]:
    return {"ok": True, "rows": Ledger().restore()}


def lint(p: Project, paths: Optional[List[str]] = None) -> List[Any]:
    ps = [Path(x) if Path(x).is_absolute() else ROOT / x for x in (paths or ["src"])]
    return [list(f) for f in lint_paths(ps)]


def names(p: Project) -> List[Dict[str, Any]]:
    return [dict(r) for r in Ledger().pending_names()]
