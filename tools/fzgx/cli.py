"""`fzgx` command line: thin wrapper over tools/fzgx/api.py."""

from __future__ import annotations

import argparse
import json
from pathlib import Path
from typing import List, Optional

from . import api, trivial
from .project import Project


def _print(obj, as_json: bool) -> None:
    if as_json:
        print(json.dumps(obj, indent=None, default=str))
    elif isinstance(obj, str):
        print(obj)
    else:
        print(json.dumps(obj, indent=2, default=str))


def cmd_sync(a, p):
    _print(api.sync(p), a.json); return 0


def cmd_inventory(a, p):
    rows = api.inventory(p, a.module, a.status, a.limit, a.max_size)
    if a.json:
        _print(rows, True)
    else:
        for r in rows:
            print(f"{r['status']:9s} {r['module']:13s} {r['symbol']:28s} {r['size']:6d}B  att={r['attempts']} best={r['best']:.0f}%")
        print(f"({len(rows)} functions)")
    return 0


def cmd_claim(a, p):
    r = api.claim(p, a.symbol, a.agent, a.ttl, a.max_attempts, a.no_carve)
    _print(r, a.json); return 0 if r["ok"] else 2


def cmd_carve(a, p):
    _print(api.carve_many(p, a.symbols, a.dry_run), a.json); return 0


def cmd_context(a, p):
    print(api.context(p, a.symbol, a.budget_tokens)); return 0


def cmd_check(a, p):
    r = api.check(p, a.symbol, a.max_diff_lines, a.versions)
    _print(r if a.json else api.format_check(r), a.json)
    if "versions" in r:
        return 0 if any(v >= 100.0 for v in r["versions"].values()) else 1
    return 0 if r["ok"] and r["matched"] else 1


def cmd_submit(a, p):
    names = json.loads(Path(a.names).read_text()) if a.names else None
    r = api.submit(p, a.symbol, a.agent, a.message or "", a.harness, a.model, a.mw_version, a.extra_cflags,
                   names, a.tokens_in, a.tokens_out, a.cost_usd, a.max_diff_lines)
    _print(r, a.json); return 0 if r["ok"] else 1


def cmd_release(a, p):
    r = api.release(p, a.symbol, a.reason, a.harness, a.model, a.tokens_in, a.tokens_out, a.cost_usd)
    _print(r, a.json); return 0 if r["ok"] else 2


def cmd_block(a, p):
    _print(api.block(p, a.symbol, a.reason, a.issue), a.json); return 0


def cmd_unblock(a, p):
    _print(api.unblock(p, a.symbol), a.json); return 0


def cmd_report(a, p):
    r = api.report(p)
    if a.json:
        _print(r, True)
    else:
        tot = {}
        for st in r["ledger"].values():
            for k, v in st.items():
                if not k.endswith("_bytes"):
                    tot[k] = tot.get(k, 0) + v
        print("status totals:", ", ".join(f"{k}={v}" for k, v in sorted(tot.items())))
        print("costs:", r["costs"])
        if r["objdiff"]:
            print("objdiff:", r["objdiff"])
    return 0


def cmd_snapshot(a, p):
    _print(api.snapshot(p), a.json); return 0


def cmd_restore(a, p):
    _print(api.restore(p), a.json); return 0


def cmd_lint(a, p):
    findings = api.lint(p, a.paths)
    if a.json:
        _print(findings, True)
    else:
        for f, rule, line, msg in findings:
            print(f"{f}:{line}: {rule}: {msg}")
        print(f"{len(findings)} finding(s)")
    return 1 if findings else 0


def cmd_trivial(a, p):
    _print(trivial.apply(p, a.module.split(",") if a.module else None, a.limit, a.dry_run), a.json); return 0


def cmd_compare(a, p):
    """A/B: per-function outcome, checks, tokens and cost for two agent-id prefixes."""
    from .ledger import Ledger
    l = Ledger()
    def rows(prefix):
        out = {}
        for r in l.db.execute("SELECT symbol, outcome, checks, final_percent, tokens_in, tokens_out, cost_usd, model "
                              "FROM attempts WHERE agent LIKE ? ORDER BY id", (prefix + "%",)):
            out[r["symbol"]] = dict(r)
        return out
    A, B = rows(a.a), rows(a.b)
    syms = sorted(set(A) | set(B))
    def fmt(r):
        if not r: return "-"
        o = (r["outcome"] or "?").replace("shadow-", "")
        return f"{o[:8]:8s} {r['final_percent'] or 0:5.1f}% c={r['checks'] or 0} in={r['tokens_in'] or 0:>7} out={r['tokens_out'] or 0:>5} ${r['cost_usd'] or 0:.3f}"
    print(f"{'symbol':14s} | A: {a.a:20s} | B: {a.b}")
    for s in syms:
        print(f"{s:14s} | {fmt(A.get(s))} | {fmt(B.get(s))}")
    def tot(R):
        m = sum(1 for r in R.values() if (r["outcome"] or "").endswith("matched"))
        return f"matched {m}/{len(R)}, checks {sum(r['checks'] or 0 for r in R.values())}, tokens in {sum(r['tokens_in'] or 0 for r in R.values())} out {sum(r['tokens_out'] or 0 for r in R.values())}, ${sum(r['cost_usd'] or 0 for r in R.values()):.2f}"
    print(f"A total: {tot(A)}"); print(f"B total: {tot(B)}")
    return 0


def cmd_names(a, p):
    _print(api.names(p), a.json); return 0


def cmd_read_unit(a, p):
    r = api.read_unit(p, a.symbol)
    _print(r if a.json else (r.get("source") or r.get("error")), a.json); return 0 if r["ok"] else 2


def cmd_write_unit(a, p):
    r = api.write_unit(p, a.symbol, a.agent, Path(a.file).read_text())
    _print(r, a.json); return 0 if r["ok"] else 2


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
    s.add_argument("--ttl", type=int, default=api.DEFAULT_TTL); s.add_argument("--max-attempts", type=int, default=api.MAX_ATTEMPTS)
    s.add_argument("--no-carve", action="store_true")
    s = sub.add_parser("carve", help="carve functions into units without claiming"); s.set_defaults(fn=cmd_carve)
    s.add_argument("symbols", nargs="+"); s.add_argument("--dry-run", action="store_true")
    s = sub.add_parser("context", help="print the context bundle"); s.set_defaults(fn=cmd_context)
    s.add_argument("symbol"); s.add_argument("--budget-tokens", type=int, default=6000)
    s = sub.add_parser("read-unit", help="print a carved unit's source"); s.set_defaults(fn=cmd_read_unit); s.add_argument("symbol")
    s = sub.add_parser("write-unit", help="replace a claimed unit's source from a file"); s.set_defaults(fn=cmd_write_unit)
    s.add_argument("symbol"); s.add_argument("--agent", required=True); s.add_argument("--file", required=True)
    s = sub.add_parser("check", help="compile + objdiff one function"); s.set_defaults(fn=cmd_check)
    s.add_argument("symbol"); s.add_argument("--max-diff-lines", type=int, default=80)
    s.add_argument("--versions", help="'all' or comma list, e.g. GC/1.2.5n,GC/1.3.2: compile under each compiler and report %")
    s = sub.add_parser("submit", help="accept a 100% match: relink, hash, lint, commit"); s.set_defaults(fn=cmd_submit)
    s.add_argument("symbol"); s.add_argument("--agent", default="unknown"); s.add_argument("--names", help="JSON sidecar of name proposals")
    s.add_argument("--message"); s.add_argument("--max-diff-lines", type=int, default=40)
    s.add_argument("--mw-version", help="record a compiler version for this unit (e.g. GC/1.3) before relinking")
    s.add_argument("--extra-cflags", help="extra compiler flags for this unit, space separated")
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
    s = sub.add_parser("compare", help="A/B table for two agent-id prefixes (e.g. b3c-claude vs shadow-b3c-codex)"); s.set_defaults(fn=cmd_compare)
    s.add_argument("--a", required=True); s.add_argument("--b", required=True)
    s = sub.add_parser("trivial", help="mechanically match single-blr and `li r3,N; blr` functions"); s.set_defaults(fn=cmd_trivial)
    s.add_argument("--module", help="comma list; default all"); s.add_argument("--limit", type=int); s.add_argument("--dry-run", action="store_true")
    return ap


def main(argv: Optional[List[str]] = None) -> int:
    a = build_parser().parse_args(argv)
    return a.fn(a, Project(a.version))
