"""`fzgx` command line: the only surface matcher agents are allowed to use."""

from __future__ import annotations

import argparse
import json
import shutil
import subprocess
import sys
import time
from pathlib import Path
from typing import List, Optional

from . import oracle
from .carve import carve
from .context import build_context
from .ledger import Ledger
from .lint import lint_paths
from .project import ROOT, Project

DEFAULT_TTL = 1800
MAX_ATTEMPTS = 3


def _print(obj, as_json: bool) -> None:
    if as_json:
        print(json.dumps(obj, indent=None, default=str))
    elif isinstance(obj, str):
        print(obj)
    else:
        print(json.dumps(obj, indent=2, default=str))


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


# ----------------------------------------------------------------------- cmds
def cmd_sync(a, p: Project, l: Ledger) -> int:
    rows = []
    for module in p.modules:
        for s in p.functions(module):
            rows.append({"symbol": s.name, "module": module, "unit": p.unit_of(s), "addr": s.addr, "size": s.size})
    n = l.sync_functions(rows)
    matched = 0
    for u in p.load_units():
        if u["status"] == "matching":
            for s in u.get("symbols", []):
                row = l.get(s)
                if row and row["status"] != "matched":
                    l.db.execute("UPDATE functions SET status='matched', best_percent=100 WHERE symbol=?", (s,))
                    matched += 1
    _print({"functions": len(rows), "inserted": n, "marked_matched": matched}, a.json)
    return 0


def cmd_inventory(a, p: Project, l: Ledger) -> int:
    l.expire_claims()
    rows = l.list(module=a.module, status=a.status, limit=a.limit)
    if a.max_size:
        rows = [r for r in rows if r["size"] <= a.max_size]
    out = [{"symbol": r["symbol"], "module": r["module"], "size": r["size"], "status": r["status"],
            "attempts": r["attempts"], "best": r["best_percent"], "unit": r["unit"]} for r in rows]
    if a.json:
        _print(out, True)
    else:
        for r in out:
            print(f"{r['status']:9s} {r['module']:13s} {r['symbol']:28s} {r['size']:6d}B  att={r['attempts']} best={r['best']:.0f}%")
        print(f"({len(out)} functions)")
    return 0


def cmd_claim(a, p: Project, l: Ledger) -> int:
    try:
        row = l.claim(a.symbol, a.agent, a.ttl, a.max_attempts)
    except (LookupError, PermissionError) as e:
        _print({"ok": False, "error": str(e)}, a.json)
        return 2
    res = None
    if not a.no_carve:
        try:
            res = carve(p, a.symbol)
            if res.created:
                _reconfigure_and_split(p)
        except Exception as e:  # release the claim so nobody is stuck
            l.finish(a.symbol, "carve-failed", "unmatched", notes=str(e))
            _print({"ok": False, "error": f"carve failed: {e}"}, a.json)
            return 3
    _print({"ok": True, "symbol": a.symbol, "unit": res.source if res else row["unit"],
            "ranges": res.ranges if res else [], "notes": res.notes if res else [],
            "attempt": row["attempts"] + 1, "ttl": a.ttl}, a.json)
    return 0


def cmd_carve(a, p: Project, l: Ledger) -> int:
    results = []
    created = False
    for s in a.symbols:
        try:
            r = carve(p, s, dry_run=a.dry_run)
            created |= r.created
            results.append(r.__dict__)
        except LookupError as e:
            results.append({"symbol": s, "error": str(e)})
    if created and not a.dry_run:
        _reconfigure_and_split(p)
    _print(results, a.json)
    return 0


def cmd_context(a, p: Project, l: Ledger) -> int:
    print(build_context(p, l, a.symbol, a.budget_tokens))
    return 0


def cmd_check(a, p: Project, l: Ledger) -> int:
    res = oracle.check(p, a.symbol, a.max_diff_lines)
    if res.ok:
        l.bump_checks(a.symbol, res.percent)
    if a.json:
        _print(res.to_json(), True)
    elif not res.ok:
        print(f"CHECK FAILED: {res.error}")
    else:
        print(f"{a.symbol}: {res.percent:.1f}%  unit={res.unit}  " + ("MATCH" if res.matched else "no match"))
        others = {k: v for k, v in res.symbols.items() if k != a.symbol}
        if others:
            print("other functions in unit: " + ", ".join(f"{k}={v:.0f}%" for k, v in others.items()))
        if res.data_sections:
            print("data: " + ", ".join(f"{k}={v:.0f}%" for k, v in res.data_sections.items()))
        if res.missing_in_base:
            print("missing in our object: " + ", ".join(res.missing_in_base))
        if res.extra_in_base:
            print("extra in our object: " + ", ".join(res.extra_in_base))
        if res.diff:
            print("diff (target | ours):")
            print("\n".join(res.diff))
    return 0 if res.ok and res.matched else 1


def cmd_submit(a, p: Project, l: Ledger) -> int:
    sym = p.find_symbol(a.symbol)
    if sym is None:
        _print({"ok": False, "error": "unknown symbol"}, a.json); return 2
    unit_src = p.unit_of(sym)
    if not unit_src:
        _print({"ok": False, "error": "not carved"}, a.json); return 2
    src_path = ROOT / "src" / unit_src
    findings = lint_paths([src_path])
    if findings:
        _print({"ok": False, "error": "lint", "findings": findings}, a.json); return 4
    res = oracle.check(p, a.symbol, a.max_diff_lines)
    reason = oracle.unit_fully_matches(res)
    if reason:
        _print({"ok": False, "error": reason, "percent": res.percent, "diff": res.diff}, a.json); return 1

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
            _print({"ok": False, "error": "relink or hash check failed; unit reverted to nonmatching", "output": tail}, a.json)
            return 5
        module_cfg = p.module_config_dir(sym.module)
        files = [str(src_path), str(p.units_path), str(module_cfg / "splits.txt"), str(module_cfg / "symbols.txt")]
        _git("add", *files)
        msg = f"match: {sym.module}/{a.symbol}" + (f" ({a.message})" if a.message else "")
        cp = _git("commit", "-q", "-m", msg, "--", *files)
        commit = _git("rev-parse", "--short", "HEAD").stdout.strip() if cp.returncode == 0 else None
    if a.names:
        try:
            l.propose_names(a.symbol, a.agent, json.loads(Path(a.names).read_text()))
        except Exception as e:
            print(f"warning: names sidecar ignored: {e}", file=sys.stderr)
    l.finish(a.symbol, "matched", "matched", commit=commit, notes=a.message or "",
             model=a.model, harness=a.harness, tokens_in=a.tokens_in, tokens_out=a.tokens_out,
             cost_usd=a.cost_usd)
    _print({"ok": True, "symbol": a.symbol, "commit": commit, "unit": unit_src}, a.json)
    return 0


def cmd_release(a, p: Project, l: Ledger) -> int:
    row = l.get(a.symbol)
    if row is None or row["status"] != "claimed":
        _print({"ok": False, "error": "not claimed"}, a.json); return 2
    body_path = None
    sym = p.find_symbol(a.symbol)
    unit_src = p.unit_of(sym) if sym else None
    if unit_src and (ROOT / "src" / unit_src).exists():
        dest = ROOT / "build" / "fzgx" / "attempts" / f"{a.symbol}.{int(time.time())}.c"
        dest.parent.mkdir(parents=True, exist_ok=True)
        shutil.copy(ROOT / "src" / unit_src, dest)
        body_path = str(dest)
        # keep the tree clean: restore the carved stub so the unit compiles to nothing
        (ROOT / "src" / unit_src).write_text(
            f'#include "types.h"\n\n// {a.symbol}: carved by fzgx; best attempt saved to {dest.name}\n')
    l.finish(a.symbol, "released", "unmatched", notes=a.reason, body_path=body_path,
             model=a.model, harness=a.harness, tokens_in=a.tokens_in, tokens_out=a.tokens_out,
             cost_usd=a.cost_usd)
    _print({"ok": True, "symbol": a.symbol, "saved": body_path, "attempts": row["attempts"] + 1}, a.json)
    return 0


def cmd_block(a, p: Project, l: Ledger) -> int:
    issue = None
    if a.issue:
        row = l.get(a.symbol)
        body = f"Function `{a.symbol}` exhausted {row['attempts'] if row else '?'} cheap-tier attempts.\n\n"
        body += f"Best: {row['best_percent']:.1f}%\n\nReason: {a.reason}\n" if row else a.reason
        cp = subprocess.run(["gh", "issue", "create", "--title", f"blocked: {a.symbol}", "--body", body,
                             "--label", "blocked"], cwd=ROOT, text=True, capture_output=True)
        tail = cp.stdout.strip().rsplit("/", 1)[-1]
        if cp.returncode == 0 and tail.isdigit():
            issue = int(tail)
    l.block(a.symbol, a.reason, issue)
    _print({"ok": True, "symbol": a.symbol, "issue": issue}, a.json)
    return 0


def cmd_unblock(a, p: Project, l: Ledger) -> int:
    l.unblock(a.symbol)
    _print({"ok": True}, a.json)
    return 0


def cmd_report(a, p: Project, l: Ledger) -> int:
    summary = l.summary()
    costs = dict(l.costs())
    report = p.build_dir / "report.json"
    objdiff = None
    if report.exists():
        m = json.loads(report.read_text()).get("measures", {})
        objdiff = {k: m.get(k) for k in ("matched_code_percent", "matched_functions", "total_functions",
                                          "complete_units", "total_units")}
    out = {"ledger": summary, "costs": costs, "objdiff": objdiff}
    if a.json:
        _print(out, True)
    else:
        tot = {}
        for st in summary.values():
            for k, v in st.items():
                if not k.endswith("_bytes"):
                    tot[k] = tot.get(k, 0) + v
        print("status totals:", ", ".join(f"{k}={v}" for k, v in sorted(tot.items())))
        print("costs:", costs)
        if objdiff:
            print("objdiff:", objdiff)
    return 0


def cmd_snapshot(a, p: Project, l: Ledger) -> int:
    path = l.snapshot()
    _print({"ok": True, "path": p.rel(path)}, a.json)
    return 0


def cmd_restore(a, p: Project, l: Ledger) -> int:
    _print({"ok": True, "rows": l.restore()}, a.json)
    return 0


def cmd_lint(a, p: Project, l: Ledger) -> int:
    paths = [Path(x) if Path(x).is_absolute() else ROOT / x for x in (a.paths or ["src"])]
    findings = lint_paths(paths)
    if a.json:
        _print(findings, True)
    else:
        for f, rule, line, msg in findings:
            print(f"{f}:{line}: {rule}: {msg}")
        print(f"{len(findings)} finding(s)")
    return 1 if findings else 0


def cmd_names(a, p: Project, l: Ledger) -> int:
    _print([dict(r) for r in l.pending_names()], a.json)
    return 0


# ---------------------------------------------------------------------- main
def build_parser() -> argparse.ArgumentParser:
    ap = argparse.ArgumentParser(prog="fzgx", description="F-Zero GX decomp agent CLI")
    ap.add_argument("--version", default="GFZE01")
    ap.add_argument("--json", action="store_true", help="machine-readable output")
    sub = ap.add_subparsers(dest="cmd", required=True)

    s = sub.add_parser("sync", help="load functions from dtk config into the ledger"); s.set_defaults(fn=cmd_sync)
    s = sub.add_parser("inventory", help="list functions"); s.set_defaults(fn=cmd_inventory)
    s.add_argument("--module"); s.add_argument("--status"); s.add_argument("--limit", type=int)
    s.add_argument("--max-size", type=int, help="only functions up to N bytes")
    s = sub.add_parser("claim", help="claim a function and carve its unit"); s.set_defaults(fn=cmd_claim)
    s.add_argument("symbol"); s.add_argument("--agent", required=True)
    s.add_argument("--ttl", type=int, default=DEFAULT_TTL); s.add_argument("--max-attempts", type=int, default=MAX_ATTEMPTS)
    s.add_argument("--no-carve", action="store_true")
    s = sub.add_parser("carve", help="carve functions into units without claiming"); s.set_defaults(fn=cmd_carve)
    s.add_argument("symbols", nargs="+"); s.add_argument("--dry-run", action="store_true")
    s = sub.add_parser("context", help="print the context bundle"); s.set_defaults(fn=cmd_context)
    s.add_argument("symbol"); s.add_argument("--budget-tokens", type=int, default=6000)
    s = sub.add_parser("check", help="compile + objdiff one function"); s.set_defaults(fn=cmd_check)
    s.add_argument("symbol"); s.add_argument("--max-diff-lines", type=int, default=80)
    s = sub.add_parser("submit", help="accept a 100% match: relink, hash, lint, commit"); s.set_defaults(fn=cmd_submit)
    s.add_argument("symbol"); s.add_argument("--agent", default="unknown"); s.add_argument("--names", help="JSON sidecar of name proposals")
    s.add_argument("--message"); s.add_argument("--max-diff-lines", type=int, default=40)
    s = sub.add_parser("release", help="give up on a claim, keeping the best attempt"); s.set_defaults(fn=cmd_release)
    s.add_argument("symbol"); s.add_argument("--reason", required=True)
    for name in ("submit", "release"):
        sp = sub.choices[name]
        sp.add_argument("--model"); sp.add_argument("--harness")
        sp.add_argument("--tokens-in", type=int, default=0); sp.add_argument("--tokens-out", type=int, default=0)
        sp.add_argument("--cost-usd", type=float, default=0.0)
    s = sub.add_parser("block", help="mark blocked (optionally open a GitHub issue)"); s.set_defaults(fn=cmd_block)
    s.add_argument("symbol"); s.add_argument("--reason", required=True); s.add_argument("--issue", action="store_true")
    s = sub.add_parser("unblock"); s.set_defaults(fn=cmd_unblock); s.add_argument("symbol")
    s = sub.add_parser("report", help="progress and cost summary"); s.set_defaults(fn=cmd_report)
    s = sub.add_parser("snapshot", help="write state/ledger.json"); s.set_defaults(fn=cmd_snapshot)
    s = sub.add_parser("restore", help="load state/ledger.json into the local ledger"); s.set_defaults(fn=cmd_restore)
    s = sub.add_parser("lint", help="shiftability/style lint"); s.set_defaults(fn=cmd_lint); s.add_argument("paths", nargs="*")
    s = sub.add_parser("names", help="pending name proposals for the librarian"); s.set_defaults(fn=cmd_names)
    return ap


def main(argv: Optional[List[str]] = None) -> int:
    a = build_parser().parse_args(argv)
    return a.fn(a, Project(a.version), Ledger())
