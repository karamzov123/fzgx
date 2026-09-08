#!/usr/bin/env python3
"""Headless batch runner: one harness process per function, N in parallel, one summary.

    uv run tools/orchestrate.py --harness claude --parallel 16 --select main_rel:8:96:48
    uv run tools/orchestrate.py --harness codex  --parallel 16 --symbols fn_1_A fn_1_B ...

Each worker spawns `claude -p --agent matcher` (or `codex exec`) with only the
project MCP server, no skills, no other MCPs, and the matcher's tool list. The
RESULT line is parsed from the harness output, real token usage and cost are
written to the ledger, crashed or timed-out workers have their claim released,
and a batch report lands in docs/batches/. The orchestrating model calls this
once per batch and acts on the summary.
"""

from __future__ import annotations

import argparse
import json
import os
import re
import subprocess
import sys
import time
from concurrent.futures import ThreadPoolExecutor, as_completed
from pathlib import Path
from typing import Dict, List, Optional

from fzgx import api, trivial
from fzgx.ledger import Ledger
from fzgx.project import ROOT, STATE_DIR, Project

RESULT_RE = re.compile(r"RESULT:\s*(matched|released)\s+(\S+)\s+([\d.]+)%(?:\s+checks=(\d+))?")
MATCHER_TOOLS = ["Read", "mcp__fzgx__claim", "mcp__fzgx__write_unit", "mcp__fzgx__check",
                 "mcp__fzgx__submit", "mcp__fzgx__release"]
# The user's defaults are Fable 5.1 (claude) and GPT-6 Astra (codex); matchers must never run on those.
EXPECTED_MODEL = {"claude": "claude-haiku-4-5", "codex": "gpt-5.6-luna"}
# $/M tokens from platform.openai.com/docs/pricing (2026-09-08): input, cached input, cache write, output.
# Codex reports usage but no cost; Claude Code reports total_cost_usd itself.
CODEX_PRICES = {"gpt-5.6-luna": (0.20, 0.02, 0.25, 1.20), "gpt-5.6-terra": (2.00, 0.20, 2.50, 12.00),
                "gpt-5.6-sol": (4.00, 0.40, 5.00, 20.00), "gpt-6-astra": (10.00, 1.00, 12.50, 50.00)}
CODEX_DISABLE = ["plugins", "recommended_plugins", "plugin_sharing", "remote_plugin", "apps", "browser_use",
                 "browser_use_external", "in_app_browser", "computer_use", "skill_search", "skill_mcp_dependency_install"]


def select(p: Project, spec: str) -> List[str]:
    """module:min:max:count — untouched, uncarved functions spread across sizes."""
    module, lo, hi, n = spec.split(":")
    rows = [r for r in api.inventory(p, module=module, status="unmatched")
            if r["attempts"] == 0 and not r["unit"] and int(lo) <= r["size"] <= int(hi)]
    rows.sort(key=lambda r: (r["size"], r["symbol"]))
    step = max(1, len(rows) // int(n))
    return [r["symbol"] for r in rows[::step][: int(n)]]


def claude_cmd(symbol: str, agent_id: str, model: str) -> List[str]:
    prompt = f"SYMBOL={symbol}  AGENT_ID={agent_id}. Match this function following your loop."
    return ["claude", "-p", prompt, "--agent", "matcher", "--model", model,
            "--output-format", "json", "--no-session-persistence",
            "--mcp-config", ".mcp.json", "--strict-mcp-config", "--disable-slash-commands",
            "--allowedTools", ",".join(MATCHER_TOOLS)]


def codex_cmd(symbol: str, agent_id: str, model: str) -> List[str]:
    prompt = (f"SYMBOL={symbol}  AGENT_ID={agent_id}. You are a Matcher: follow the Matcher "
              f"section of AGENTS.md exactly, using only the fzgx MCP tools. Finish with the RESULT line.")
    # --ignore-user-config: no user MCP servers/skills (480k -> 125k input tokens on a smoke test)
    cmd = ["codex", "exec", "--json", "--skip-git-repo-check", "--ignore-user-config", "-s", "read-only",
           "-m", model]
    # none of these belong in a matcher's context (each adds tool schemas or injected text every call)
    for feat in CODEX_DISABLE:
        cmd += ["--disable", feat]
    return cmd + [
            "-c", 'mcp_servers.fzgx.command="uv"',
            "-c", 'mcp_servers.fzgx.args=["run","tools/fzgx_mcp.py"]',
            "-c", f'mcp_servers.fzgx.cwd="{ROOT}"',
            # codex exec runs with approval_policy=never; without this every mutating MCP call is refused
            "-c", 'mcp_servers.fzgx.default_tools_approval_mode="approve"',
            prompt]


def parse_claude(out: str) -> Dict:
    for line in out.splitlines():
        if line.startswith("{"):
            try:
                d = json.loads(line)
            except json.JSONDecodeError:
                continue
            u = d.get("usage", {})
            return {"text": d.get("result", ""), "cost": d.get("total_cost_usd", 0.0) or 0.0,
                    "turns": d.get("num_turns"), "tokens_in": u.get("input_tokens", 0) + u.get("cache_read_input_tokens", 0)
                    + u.get("cache_creation_input_tokens", 0), "tokens_out": u.get("output_tokens", 0),
                    "model": next(iter(d.get("modelUsage", {}) or {"": None}), "")}
    return {"text": out, "cost": 0.0, "turns": None, "tokens_in": 0, "tokens_out": 0, "model": ""}


def codex_session_model(thread_id: str) -> str:
    """Codex's --json events omit the model; its session rollout records it."""
    if not thread_id:
        return ""
    root = Path.home() / ".codex" / "sessions"
    for f in sorted(root.rglob(f"*{thread_id}*"), key=lambda f: f.stat().st_mtime, reverse=True):
        m = re.search(r'"model":"([^"]+)"', f.read_text(errors="replace"))
        if m:
            return m.group(1)
    return ""


def parse_codex(out: str) -> Dict:
    text, tin, tout, model, thread = "", 0, 0, "", ""
    cached = cache_w = 0
    for line in out.splitlines():
        if not line.startswith("{"):
            continue
        try:
            ev = json.loads(line)
        except json.JSONDecodeError:
            continue
        t = ev.get("type", "")
        if t == "thread.started":
            thread = ev.get("thread_id", "")
        if t == "item.completed" and ev.get("item", {}).get("type") == "agent_message":
            text = ev["item"].get("text", text)
        if t == "turn.completed":
            u = ev.get("usage", {})
            # OpenAI usage: input_tokens already includes cached_input_tokens; output includes reasoning
            tin += u.get("input_tokens", 0)
            cached += u.get("cached_input_tokens", 0)
            cache_w += u.get("cache_write_input_tokens", 0)
            tout += u.get("output_tokens", 0)
        model = ev.get("model", model) or model
    model = model or codex_session_model(thread)
    pi, pc, pw, po = CODEX_PRICES.get(model, (0.0, 0.0, 0.0, 0.0))
    cost = ((tin - cached) * pi + cached * pc + cache_w * pw + tout * po) / 1e6
    return {"text": text or out[-2000:], "cost": round(cost, 6), "turns": None, "tokens_in": tin,
            "tokens_out": tout, "model": model}


def run_one(p: Project, harness: str, model: str, symbol: str, idx: int, timeout: int, batch: str,
            shadow: bool = False) -> Dict:
    agent_id = f"{'shadow-' if shadow else ''}{batch}-{harness}-{idx}"
    cmd = claude_cmd(symbol, agent_id, model) if harness == "claude" else codex_cmd(symbol, agent_id, model)
    t0 = time.time()
    try:
        cp = subprocess.run(cmd, cwd=ROOT, text=True, capture_output=True, timeout=timeout,
                            stdin=subprocess.DEVNULL, env={**os.environ, "CLAUDE_CODE_DISABLE_TERMINAL_TITLE": "1"})
        out, rc = cp.stdout + "\n" + cp.stderr, cp.returncode
    except subprocess.TimeoutExpired as e:
        out, rc = (e.stdout or "") + "\n" + (e.stderr or ""), -9
    info = parse_claude(out) if harness == "claude" else parse_codex(out)
    m = RESULT_RE.search(info["text"] or "") or RESULT_RE.search(out)
    outcome = m.group(1) if m else ("timeout" if rc == -9 else "crash")
    if info["model"] and not info["model"].startswith(EXPECTED_MODEL[harness]):
        outcome = f"WRONG-MODEL({info['model']})"
    pct = float(m.group(3)) if m else None
    checks = int(m.group(4)) if m and m.group(4) else None
    key = api._key(p, symbol)
    l = Ledger()
    # cost accounting onto the attempt this agent opened
    l.db.execute("UPDATE attempts SET tokens_in=?, tokens_out=?, cost_usd=?, harness=?, model=COALESCE(NULLIF(?, ''), model) "
                 "WHERE id=(SELECT id FROM attempts WHERE symbol=? AND agent=? ORDER BY id DESC LIMIT 1)",
                 (info["tokens_in"], info["tokens_out"], info["cost"], harness, info["model"], key, agent_id))
    row = l.get(key)
    if row and row["status"] == "claimed" and row["claimed_by"] == agent_id:
        api.release(p, symbol, f"harness {outcome} (rc={rc}); no RESULT line", harness=harness, model=info["model"], agent=agent_id)
        outcome = f"{outcome}+released"
    log = STATE_DIR / "runs" / batch / f"{symbol}.log"
    log.parent.mkdir(parents=True, exist_ok=True)
    log.write_text(out)
    return {"symbol": symbol, "outcome": outcome, "percent": pct, "checks": checks, "cost": info["cost"], "model": info["model"],
            "tokens_in": info["tokens_in"], "tokens_out": info["tokens_out"], "turns": info["turns"],
            "secs": round(time.time() - t0, 1), "rc": rc}


def main(argv: Optional[List[str]] = None) -> int:
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--harness", choices=["claude", "codex"], default="claude")
    ap.add_argument("--model", help="claude: haiku|sonnet|opus (default haiku); codex: model name (default gpt-5.6-luna)")
    ap.add_argument("--parallel", type=int, default=16)
    ap.add_argument("--timeout", type=int, default=900, help="seconds per agent")
    ap.add_argument("--symbols", nargs="*", default=[])
    ap.add_argument("--select", help="module:min_size:max_size:count, e.g. main_rel:8:96:48")
    ap.add_argument("--budget-usd", type=float, help="stop launching new agents past this spend (claude only reports cost)")
    ap.add_argument("--batch", default=time.strftime("b%Y%m%d-%H%M"))
    ap.add_argument("--no-trivial", action="store_true", help="skip the mechanical blr/li pass first")
    ap.add_argument("--dry-run", action="store_true")
    ap.add_argument("--shadow", action="store_true",
                    help="A/B trial: run on already-matched functions without relinking or committing")
    a = ap.parse_args(argv)
    model = a.model or ("haiku" if a.harness == "claude" else "gpt-5.6-luna")
    p = Project()

    symbols = list(a.symbols)
    if a.select:
        symbols += select(p, a.select)
    if not symbols:
        print("nothing selected", file=sys.stderr)
        return 2
    print(f"batch {a.batch}: {len(symbols)} functions, harness={a.harness} model={model} parallel={a.parallel}")
    if a.dry_run:
        print(" ".join(symbols))
        return 0
    if not a.no_trivial and not a.shadow:
        triv = trivial.apply(p)
        print(f"trivial pass: {triv.get('applied', 0)} matched mechanically")
    if not a.shadow:
        carved = api.carve_many(p, symbols)
        n_new = sum(1 for c in carved if c.get('created'))
        print(f"carved {n_new} new units")
        if n_new:
            subprocess.run(["git", "add", "src", "config"], cwd=ROOT, capture_output=True)
            subprocess.run(["git", "commit", "-q", "-m", f"carve: {n_new} units for batch {a.batch}"], cwd=ROOT, capture_output=True)

    results: List[Dict] = []
    spent = 0.0
    t0 = time.time()
    with ThreadPoolExecutor(max_workers=a.parallel) as ex:
        futs = {}
        queue = list(enumerate(symbols, 1))
        while queue or futs:
            while queue and len(futs) < a.parallel and (a.budget_usd is None or spent < a.budget_usd):
                i, s = queue.pop(0)
                futs[ex.submit(run_one, p, a.harness, model, s, i, a.timeout, a.batch, a.shadow)] = s
            if not futs:
                break
            done = next(as_completed(list(futs)))
            futs.pop(done)
            r = done.result()
            results.append(r)
            spent += r["cost"] or 0.0
            if r["outcome"].startswith("WRONG-MODEL"):
                print(f"ABORT: {r['symbol']} ran on {r['outcome']}; expected {EXPECTED_MODEL[a.harness]}", flush=True)
                queue.clear()
            print(f"  {r['outcome']:16s} {r['symbol']:14s} {'' if r['percent'] is None else f'{r['percent']:.1f}%':7s} "
                  f"checks={r['checks'] if r['checks'] is not None else '-'} turns={r['turns'] or '-'} "
                  f"${r['cost']:.3f} {r['secs']}s", flush=True)
    ver = {"verified": [], "rejected": []}
    if not a.shadow:
        ver = api.verify_links(p, f"batch {a.batch}: link-verified matches")
        print(f"verify: {len(ver.get('verified', []))} verified, {len(ver.get('rejected', []))} rejected"
              + (f" ({ver.get('error')})" if ver.get("error") else ""), flush=True)
        for r in results:
            if r["outcome"] == "matched" and r["symbol"] in ver.get("rejected", []):
                r["outcome"] = "link-mismatch"
    matched = [r for r in results if r["outcome"] == "matched"]
    released = [r for r in results if r["outcome"].startswith("released")]
    other = [r for r in results if r not in matched and r not in released]
    models = sorted({r.get("model") for r in results if r.get("model")})
    summary = {"batch": a.batch, "harness": a.harness, "model": model, "models_seen": models, "n": len(results), "matched": len(matched),
               "link_rejected": len(ver.get("rejected", [])),
               "released": len(released), "failed": len(other), "cost_usd": round(spent, 3),
               "wall_s": round(time.time() - t0, 1), "results": results}
    # report + snapshot
    rep = ROOT / "docs" / "batches" / f"{a.batch}{'-shadow' if a.shadow else ''}.md"
    lines = [f"# Batch {a.batch}{' (shadow A/B trial)' if a.shadow else ''} — {a.harness}/{model}, {a.parallel} parallel",
             "", f"{len(results)} functions: {len(matched)} matched, {len(released)} released, {len(other)} failed; "
             f"${spent:.2f}; {summary['wall_s']} s wall.", "",
             "| Function | Outcome | % | Checks | Turns | $ | s |", "|---|---|---|---|---|---|---|"]
    for r in sorted(results, key=lambda r: r["symbol"]):
        lines.append(f"| {r['symbol']} | {r['outcome']} | {'' if r['percent'] is None else r['percent']} | "
                     f"{r['checks'] if r['checks'] is not None else ''} | {r['turns'] or ''} | {r['cost']:.3f} | {r['secs']} |")
    rep.write_text("\n".join(lines) + "\n")
    api.snapshot(p)
    subprocess.run(["git", "add", str(rep), str(ROOT / "state" / "ledger.json")], cwd=ROOT, capture_output=True)
    subprocess.run(["git", "commit", "-q", "-m", f"batch {a.batch}: {len(matched)}/{len(results)} matched ({a.harness}/{model})"],
                   cwd=ROOT, capture_output=True)
    print(json.dumps({k: v for k, v in summary.items() if k != "results"}))
    return 0


if __name__ == "__main__":
    sys.exit(main())
