"""Context bundle for a matcher: everything it needs, ordered by value per token."""

from __future__ import annotations

import json
from pathlib import Path
from typing import List, Optional

from .ledger import Ledger
from .project import ROOT, Function, Project, Symbol

RULES = ROOT / "docs" / "CODING_RULES.md"
IDIOMS = ROOT / "docs" / "MWCC_IDIOMS.md"


def _sig_hint(fn: Function) -> str:
    """Cheap hints from the prologue/epilogue: stack frame, saved regs, float use."""
    body = " ".join(fn.asm)
    hints = []
    if "stwu r1" in body:
        hints.append("has stack frame")
    else:
        hints.append("leaf, no stack frame")
    if any(m in body for m in ("fadds", "fmuls", "fdivs", "lfs ", "stfs", "fcmpu", "frsp", "fmr")):
        hints.append("uses float")
    if "mtctr" in body and "bctr" in body:
        hints.append("has switch/jump table or indirect call")
    if "_savegpr" in body or "stmw" in body:
        hints.append("saves many GPRs (many locals or calls)")
    return "; ".join(hints)


def _decl_for(sym: Symbol) -> str:
    if sym.kind == "function":
        return f"extern void {sym.name}(void);  // {sym.section}:0x{sym.addr:08X} size 0x{sym.size:X} (signature unknown)"
    dt = sym.attrs.get("data", "")
    ctype = {"byte": "u8", "2byte": "u16", "4byte": "u32", "8byte": "u64", "float": "f32",
             "double": "f64", "string": "char"}.get(dt, "u8")
    n = max(sym.size, 1)
    unit = {"u16": 2, "u32": 4, "u64": 8, "f32": 4, "f64": 8}.get(ctype, 1)
    arr = f"[{n // unit}]" if n // unit > 1 else ""
    return f"extern {ctype} {sym.name}{arr};  // {sym.section}:0x{sym.addr:08X} size 0x{sym.size:X} scope {sym.scope}"


def build_context(project: Project, ledger: Optional[Ledger], symbol: str,
                  budget_tokens: int = 6000) -> str:
    sym0 = project.resolve(symbol)
    fn = project.function_asm(sym0.module).get(sym0.name) if sym0 else None
    if fn is None:
        raise LookupError(f"{symbol}: no disassembly (is it a function? ambiguous? run ninja first)")
    sym = fn.symbol
    module = sym.module
    symbol = sym.name
    row = ledger.get(project.key(sym)) if ledger else None
    unit_src = project.unit_of(sym)
    units = {u["source"]: u for u in project.load_units()}
    unit_cfg = units.get(unit_src or "", {})

    parts: List[str] = []
    parts.append(f"# Match task: `{symbol}`")
    parts.append(
        f"- module: `{module}`  section: `{sym.section}`  address: `0x{sym.addr:08X}`  "
        f"size: {sym.size} bytes ({sym.size // 4} instructions)  scope: {sym.scope}"
    )
    parts.append(f"- unit: `src/{unit_src}`" if unit_src else "- unit: NOT CARVED (run `fzgx carve`)")
    if row:
        parts.append(f"- attempts so far: {row['attempts']}  best: {row['best_percent']:.1f}%")
    parts.append(f"- hints: {_sig_hint(fn)}")
    mw = unit_cfg.get("mw_version") or ("GC/1.2.5n" if module == "main" else "GC/1.3.2")
    parts.append(f"- compiler: `{mw}` `-O4,p -inline auto -fp hardware -enum int`"
                 + (" `-sdata 0 -sdata2 0`" if module != "main" else "")
                 + (f" extra: `{' '.join(unit_cfg.get('extra_cflags', []))}`" if unit_cfg.get("extra_cflags") else ""))

    parts.append("\n## Target assembly (retail)\n```asm")
    parts.extend(fn.asm)
    parts.append("```")

    if fn.refs:
        parts.append("\n## Referenced symbols (declare what you use; names are provisional)\n```c")
        for name in fn.refs:
            s = project.find_symbol(name, module) or project.find_symbol(name)
            if s:
                parts.append(_decl_for(s))
        parts.append("```")

    callers = project.callers(symbol)
    if callers:
        parts.append(f"\n## Callers: {', '.join(f'`{c}`' for c in callers)}")

    # Neighbouring matched C in the same module, nearest first (cheap, high value).
    matched_units = [u for u in project.load_units() if u["module"] == module and u["status"] == "matching"]
    neigh: List[str] = []
    for u in sorted(matched_units, key=lambda u: abs((project.find_symbol(u["symbols"][0]) or sym).addr - sym.addr))[:2]:
        p = ROOT / "src" / u["source"]
        if p.exists():
            body = p.read_text()
            if len(body) < 2500:
                neigh.append(f"### src/{u['source']}\n```c\n{body}\n```")
    if neigh:
        parts.append("\n## Nearby matched code (style and naming reference)")
        parts.extend(neigh)

    if unit_src:
        p = ROOT / "src" / unit_src
        if p.exists():
            parts.append(f"\n## Current file `src/{unit_src}`\n```c\n{p.read_text()}\n```")

    if ledger:
        att = ledger.db.execute(
            "SELECT * FROM attempts WHERE symbol=? AND ended IS NOT NULL ORDER BY final_percent DESC, id DESC LIMIT 1",
            (symbol,)).fetchone()
        if att and att["best_body_path"] and Path(att["best_body_path"]).exists():
            parts.append(f"\n## Best prior attempt ({att['final_percent'] or 0:.1f}%, notes: {att['notes'] or '-'})\n```c\n"
                         f"{Path(att['best_body_path']).read_text()}\n```")

    if IDIOMS.exists():
        parts.append("\n## MWCC idioms\n" + IDIOMS.read_text().strip())
    if RULES.exists():
        rules = RULES.read_text()
        parts.append("\n## Rules (excerpt)\n" + rules[: 1800].strip())

    text = "\n".join(parts)
    # crude token cap: ~4 chars per token
    limit = budget_tokens * 4
    if len(text) > limit:
        text = text[:limit] + "\n\n[context truncated to budget]"
    return text
