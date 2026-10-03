#!/usr/bin/env python3
"""Strip the `.fzgxpool` layout primer from a saved body that is object-perfect but
link-rejected. See docs/findings/279.

`poolfix.apply` drops the primer, but it runs only under `matched_pool`. A body whose
object matches with `pool_rows: 0` never reaches it, so its primer reaches the module
link and the linker rejects the private copies as `multiply-defined`. The primer exists
to pin anonymous-section layout order for the *per-object* oracle; once the object is
proven byte-perfect the primer is pure link risk.

The primer block is removed but its file-scope declarations are kept -- they define the
storage the function body addresses. `--drop` removes named globals the linker reports as
multiply-defined (private copies of storage a split data object already owns).

Usage:
    python3 tools/fzgx/primerless.py <symbol> [--source PATH] [--drop NAME ...] [--out PATH]

Without --source the newest `.fzgx/attempts/<symbol>.linkfail.*.c` is used.
"""

from __future__ import annotations

import argparse
import re
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent.parent))

from fzgx.project import ROOT, STATE_DIR  # noqa: E402

PRAGMA_POOL = '#pragma section code_type ".fzgxpool"'
PRAGMA_TEXT = '#pragma section code_type ".text"'


def newest_linkfail(symbol: str) -> Path:
    """Newest preserved body that still carries a `.fzgxpool` primer.

    Once a primerless body has been saved, later saves inherit it, so "newest" is the
    wrong pick for this tool: prefer the newest body that still has a primer, and only
    fall back to the newest overall when none does (already-clean input is idempotent).
    """
    key = symbol.replace(":", "__")
    found = sorted((STATE_DIR / "attempts").glob(f"{key}.linkfail.*.c"),
                   key=lambda p: p.stat().st_mtime)
    if not found:
        raise SystemExit(f"{symbol}: no preserved linkfail body in {STATE_DIR / 'attempts'}")
    with_primer = [p for p in found if PRAGMA_POOL in p.read_text()]
    return with_primer[-1] if with_primer else found[-1]


def strip_primer(text: str) -> str:
    """Remove the `.fzgxpool` function block, keeping every file-scope declaration."""
    if PRAGMA_POOL not in text:
        return text
    out, removed = text, 0
    while PRAGMA_POOL in out:
        i = out.index(PRAGMA_POOL)
        j = out.find(PRAGMA_TEXT, i)
        if j < 0:
            raise SystemExit("unterminated .fzgxpool block: no matching .text pragma")
        j += len(PRAGMA_TEXT)
        # keep the newline that followed the closing pragma
        while j < len(out) and out[j] in "\r\n":
            j += 1
        out = out[:i] + out[j:]
        removed += 1
    return out


def drop_globals(text: str, names) -> str:
    """Delete the file-scope *definition* of each named global.

    Only a line that defines the symbol is removed -- never a line that uses it, never an
    `extern` or `typedef`. Collapsing the whole declaration (line 29 of a repaired body
    was `u32 *b = (u32 *)&lbl_3_bss_A2410;`) would delete real code, so a definition
    must be a standalone type+name+`;` line at brace depth 0.
    """
    lines = text.split("\n")
    for name in names:
        rx = re.compile(r"^(?!(?:extern|typedef)\b)\s*[A-Za-z_][\w \t*]*\b%s\b[^;(){}]*;\s*$"
                        % re.escape(name))
        hits = [i for i, l in enumerate(lines) if rx.match(l)]
        if not hits:
            print(f"  note: no standalone definition of {name} to drop", file=sys.stderr)
            continue
        for i in reversed(hits):
            del lines[i]
    return "\n".join(lines)


def extern_globals(text: str, specs) -> str:
    """Declare the named globals `extern` instead of defining them.

    Used when the linker reports `multiply-defined` for a symbol the body both defines
    and uses: the storage is owned by a split data object, so the unit must *reference*
    it, not define it. Unlike `--drop`, this keeps every use in the body compiling.

    Each spec is `NAME` or `TYPE:NAME` (`TYPE:NAME` is required: the original
    definition's type is load-bearing -- `extern s32` for a `u8` global turns a `stb`
    into a `stw` and costs the match).
    """
    if not specs:
        return text
    decls = []
    for spec in specs:
        if ":" in spec:
            ty, name = spec.split(":", 1)
        else:
            # recover the declared type from the body's own definition
            m = re.search(r"^([A-Za-z_]\w*)\s+%s(?:\[[^\]]*\])*;\s*$" % re.escape(spec),
                          text, re.M)
            ty, name = (m.group(1), spec) if m else ("s32", spec)
        decls.append(f"extern {ty} {name};\n")
    block = "".join(decls)
    m = list(re.finditer(r"^[A-Za-z_][\w]*\s+(\w+)(?:\[[^\]]*\])*;\s*$", text, re.M))
    if m:
        last = m[-1]
        return text[:last.end()] + "\n" + block + text[last.end():]
    return text + "\n" + block


def ensure_includes(text: str, headers) -> str:
    """The preserved bodies are fragments; prepend the headers they need.

    `types.h` is always safe. Module headers are NOT guessed -- the body's struct types
    (Obj_3_bss_A2410 and friends) live in the module's per-unit header, so adding
    <module>.h would not supply the type and risks shadowing it. Pass them with --header
    once the compiler names the type it wants.
    """
    lines = [h for h in headers if f'#include "{h}"' not in text]
    if not lines:
        return text
    return "\n".join(f'#include "{h}"' for h in lines) + "\n\n" + text.lstrip()


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("symbol")
    ap.add_argument("--source", type=Path)
    ap.add_argument("--drop", action="append", default=[],
                    help="global to delete (linker-reported multiply-defined)")
    ap.add_argument("--extern", action="append", default=[],
                    help="global to declare extern instead of defining (owned by a split "
                         "data object but still used by this body)")
    ap.add_argument("--header", action="append", default=[],
                    help="header to ensure included (repeatable; types.h when none given)")
    ap.add_argument("--out", type=Path)
    a = ap.parse_args()

    src = a.source or newest_linkfail(a.symbol)
    text = src.read_text()
    print(f"{a.symbol}: source {src}")

    before = text.count(PRAGMA_POOL)
    text = strip_primer(text)
    print(f"  stripped {before} .fzgxpool block(s); remaining {text.count(PRAGMA_POOL)}")

    # Resolve --extern types against the *original* text: a name given to both --drop and
    # --extern has already lost its definition by the time extern_globals() would look.
    externs = []
    for spec in a.extern:
        if ":" in spec:
            externs.append(spec)
            continue
        m = re.search(r"^([A-Za-z_]\w*)\s+%s(?:\[[^\]]*\])*;\s*$" % re.escape(spec),
                      text, re.M)
        externs.append(f"{m.group(1)}:{spec}" if m else spec)

    if a.drop:
        text = drop_globals(text, a.drop)
        print(f"  dropped globals: {', '.join(a.drop)}")
    if externs:
        text = extern_globals(text, externs)
        print(f"  declared extern: {', '.join(externs)}")

    from fzgx.project import Project
    sym = Project().resolve(a.symbol)
    module = sym.module if sym else "unknown"
    text = ensure_includes(text, a.header or ["types.h"])
    print(f"  module {module}; headers {', '.join(a.header)}; {len(text)} bytes")

    out = a.out or (STATE_DIR / "attempts" / f"{a.symbol}.PRIMERLESS.c")
    out.parent.mkdir(parents=True, exist_ok=True)
    out.write_text(text)
    print(f"  wrote {out}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())