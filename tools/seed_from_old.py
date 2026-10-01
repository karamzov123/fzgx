#!/usr/bin/env python3
"""Extract and sanitize candidate C functions from the old decomp repo.

Scans /home/armandofm/projects/fzero-gx-decomp/src/ for C function implementations,
maps them to unmatched functions in fzgx by symbol name and virtual address,
sanitizes them (stripping hardcoded addresses), and outputs donor seeds to
state/donor_seeds/<symbol>.c.
"""

import os
import re
import sqlite3
from pathlib import Path

FZGX_ROOT = Path(__file__).resolve().parent.parent
OLD_DECOMP_ROOT = Path("/home/armandofm/projects/fzero-gx-decomp")
OUT_DIR = FZGX_ROOT / "state" / "donor_seeds"


def load_old_symbols():
    sym_file = OLD_DECOMP_ROOT / "config" / "GFZE01" / "symbols.txt"
    by_name = {}
    by_addr = {}
    if not sym_file.exists():
        return by_name, by_addr
    sym_re = re.compile(r"^([a-zA-Z0-9_]+)\s*=\s*\.?[\w.$]+:(0x[0-9A-Fa-f]+)")
    for line in sym_file.read_text().splitlines():
        m = sym_re.match(line.strip())
        if m:
            name = m.group(1)
            addr = int(m.group(2), 16)
            by_name[name] = addr
            by_addr[addr] = name
    return by_name, by_addr


def load_unmatched_fzgx():
    db_path = FZGX_ROOT / ".fzgx" / "ledger.db"
    con = sqlite3.connect(db_path)
    cur = con.cursor()
    cur.execute("SELECT symbol, module, addr, size, best_percent FROM functions WHERE status = 'unmatched'")
    by_name = {}
    by_addr = {}
    for row in cur.fetchall():
        sym, mod, addr, size, best = row
        info = {"symbol": sym, "module": mod, "addr": addr, "size": size, "best": best}
        by_name[sym] = info
        if mod == "main" and addr:
            by_addr[addr] = info
    return by_name, by_addr


def extract_functions_from_c(path):
    text = path.read_text(errors="replace")
    # Match function header and opening brace
    # e.g., int foo(int a, void* b) {
    pattern = re.compile(
        r"(?P<header>^[a-zA-Z0-9_* \t\n]+?\b(?P<name>[a-zA-Z0-9_]+)\s*\([^;)]*\))\s*\{",
        re.M,
    )
    funcs = []
    for m in pattern.finditer(text):
        name = m.group("name")
        start = m.start()
        # Find matching closing brace
        brace_count = 0
        end = -1
        i = text.find("{", start)
        if i == -1:
            continue
        for idx in range(i, len(text)):
            if text[idx] == "{":
                brace_count += 1
            elif text[idx] == "}":
                brace_count -= 1
                if brace_count == 0:
                    end = idx + 1
                    break
        if end != -1:
            body = text[start:end]
            funcs.append((name, body))
    return funcs


def sanitize_code(name, body, origin_file, fzgx_symbol, addr):
    # Check for inline asm
    if "asm {" in body or "nofralloc" in body or re.search(r"\basm\s+void\b", body):
        return None, "contains_asm"

    # Strip address placements like `: 0x...;`
    clean = re.sub(r":\s*0x[0-9a-fA-F]+;", ";", body)
    clean = re.sub(r":\s*0x[0-9a-fA-F]+\b", "", clean)

    # Replace function name if fzgx symbol differs
    if name != fzgx_symbol:
        clean = re.sub(r"\b" + re.escape(name) + r"\b", fzgx_symbol, clean, count=1)

    header = (
        f'#include "types.h"\n\n'
        f"// Donor seed for {fzgx_symbol} (addr 0x{addr:08X})\n"
        f"// Extracted from {origin_file.name} (original name: {name})\n\n"
    )
    return header + clean + "\n", "ok"


def main():
    OUT_DIR.mkdir(parents=True, exist_ok=True)
    old_by_name, old_by_addr = load_old_symbols()
    unmatched_by_name, unmatched_by_addr = load_unmatched_fzgx()

    print(f"Loaded {len(unmatched_by_name)} unmatched symbols from fzgx ledger.")

    c_files = list((OLD_DECOMP_ROOT / "src").rglob("*.c"))
    print(f"Scanning {len(c_files)} C files from old repo...")

    extracted = 0
    skipped_asm = 0
    results = []

    for c_file in c_files:
        funcs = extract_functions_from_c(c_file)
        for orig_name, body in funcs:
            fzgx_info = None
            if orig_name in unmatched_by_name:
                fzgx_info = unmatched_by_name[orig_name]
            elif orig_name in old_by_name:
                addr = old_by_name[orig_name]
                if addr in unmatched_by_addr:
                    fzgx_info = unmatched_by_addr[addr]

            if not fzgx_info:
                continue

            fzgx_symbol = fzgx_info["symbol"]
            addr = fzgx_info["addr"] or 0

            code, status = sanitize_code(orig_name, body, c_file, fzgx_symbol, addr)
            if status == "contains_asm":
                skipped_asm += 1
                continue

            out_file = OUT_DIR / f"{fzgx_symbol}.c"
            out_file.write_text(code)
            extracted += 1
            results.append((fzgx_symbol, orig_name, fzgx_info["module"], fzgx_info["size"], c_file.name))

    print(f"\n[Summary]")
    print(f"  Sanitized clean C donor seeds: {extracted}")
    print(f"  Skipped (contains inline asm):  {skipped_asm}")
    print(f"  Seeds saved to: {OUT_DIR}\n")

    print("Sample extracted donor functions:")
    for sym, orig, mod, size, src in sorted(results, key=lambda x: x[3])[:20]:
        print(f"  {sym:24s} (was {orig:24s}) {mod:8s} {size:4d}B from {src}")


if __name__ == "__main__":
    main()
