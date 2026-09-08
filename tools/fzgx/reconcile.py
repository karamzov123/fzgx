"""Deterministic reconciliation of a TU's blocks into one compile environment.

No agent decides anything here. For every block of a TU file the question is
"does it still match retail when compiled under the file's prologue", and
that is a compile plus an objdiff. So:

  1. Private typedef names that collide (two blocks, or a block and a header)
     are prefixed with the function's name. A rename never changes code.
  2. The TU's headers (globals.h, <tu>.h) join the prologue.
  3. For every symbol declared privately by blocks, the candidate declarations
     are the definition's own signature (when the TU defines it), then the
     variants the blocks wrote. The first candidate under which every block
     still matches goes to the prologue and the private copies go; if none
     does, the symbol stays private and is reported as contested.
  4. A block that cannot compile under the prologue at all is stored
     self-contained (`noprologue`) and reported: that list, and only that
     list, is what a rewrite agent is for.

Every verdict is memoised by the generated unit's text (finish.Verifier).
"""

from __future__ import annotations

import re
import subprocess
from collections import Counter, OrderedDict
from typing import Dict, List, Optional, Set, Tuple

from . import tufile, tutidy
from .project import ROOT, Project

DEF_RE = re.compile(r"^((?:[A-Za-z_][\w\s\*]*?)\b([A-Za-z_]\w*)\s*\(([^;{}()]*)\))\s*\{", re.M)
TYPEDEF_NAME_RE = re.compile(r"^\}\s*([A-Za-z_]\w*)\s*;", re.M)
TAG_RE = re.compile(r"^typedef\s+struct\s+([A-Za-z_]\w*)\s*\{", re.M)


def _decls(body: str) -> List[Tuple[str, str]]:
    """(symbol, line) for every extern declaration line of a block body."""
    out = []
    for ln in body.splitlines():
        if tutidy.DECL_LINE_RE.match(ln):
            n = tutidy._decl_name(ln)
            if n:
                out.append((n, ln.strip()))
    return out


def _idents(text: str) -> Set[str]:
    return set(re.findall(r"[A-Za-z_]\w*", text))


def isolate_typedefs(tf: tufile.TuFile, header_typedefs: Set[str]) -> List[Tuple[str, str, str]]:
    """Prefix colliding private typedef/tag names with the block's function name. Returns
    (block, old, new) renames applied (in memory)."""
    owners: Dict[str, List[str]] = {}
    for b in tf.blocks:
        for n in set(TYPEDEF_NAME_RE.findall(b.body)) | set(TAG_RE.findall(b.body)):
            owners.setdefault(n, []).append(b.name)
    renames = []
    for n, bs in owners.items():
        if len(bs) > 1 or n in header_typedefs:
            for bname in bs:
                b = tf.get(bname)
                new = f"{bname}_{n}"
                b.body = re.sub(rf"\b{re.escape(n)}\b", new, b.body)
                renames.append((bname, n, new))
    return renames


def _committed_block(p: Project, tu_source: str, name: str, body: str, flags: List[str]):
    cp = subprocess.run(["git", "show", f"HEAD:src/{tu_source}"], cwd=ROOT, text=True, capture_output=True)
    if cp.returncode == 0:
        try:
            b = tufile.parse(cp.stdout).get(name)
            if b is not None:
                return b.body, list(b.flags)
        except ValueError:
            pass
    return body, flags


def reconcile_tu(p: Project, tu_source: str, v) -> Dict[str, object]:
    module = tu_source.split("/")[1] if tu_source.startswith("rel/") else "main"
    stem = tu_source.rsplit("/", 1)[-1][:-2]
    units = {u["symbols"][0]: u for u in p.load_units() if u.get("tu") == tu_source}
    path = tufile.tu_path(p, tu_source)
    tf = tufile.load(p, tu_source)
    out: Dict[str, object] = {"tu": tu_source, "renamed": 0, "hoisted": [], "contested": {}, "unresolved": [],
                              "included": []}

    def verify(names: List[str]) -> bool:
        for n in names:
            u = units.get(n)
            if u is None:
                continue
            tufile.write_gen(p, u, tf)
            if not v.matches(p, n):
                return False
        return True

    def restore_gens(names: List[str]) -> None:
        for n in names:
            if n in units:
                tufile.write_gen(p, units[n], tf)

    # every block starts as "under the prologue": the self-contained form is only a last resort
    for b in tf.blocks:
        b.flags = [f for f in b.flags if f != "noprologue"]

    # 1. headers
    for h in (f"rel/{module}/globals.h", f"rel/{module}/{stem}.h"):
        line = f'#include "{h}"'
        if (ROOT / "include" / h).exists() and line not in {ln.strip() for ln in tf.prologue.splitlines()}:
            tf.prologue = tufile.merge_prologue(tf.prologue, [line])
            out["included"].append(h)
    header_names, header_typedefs = tutidy._header_names(p, tf.prologue)

    # 2. private typedefs that collide
    out["renamed"] = len(isolate_typedefs(tf, header_typedefs))

    # 3. declarations, symbol by symbol
    definitions: Dict[str, str] = {}
    for b in tf.blocks:
        for m in DEF_RE.finditer(b.body):
            definitions[m.group(2)] = "extern " + m.group(1).strip() + ";"
    variants: Dict[str, Counter] = OrderedDict()
    where: Dict[str, Set[str]] = {}
    for b in tf.blocks:
        private_types = set(TYPEDEF_NAME_RE.findall(b.body)) | set(TAG_RE.findall(b.body))
        for n, ln in _decls(b.body):
            if _idents(ln) & private_types:
                continue  # names a block-private type: cannot leave the block
            variants.setdefault(n, Counter())[ln] += 1
            where.setdefault(n, set()).add(b.name)
    prologue_decl_names = set(tufile._header_items(tufile.prologue_decls(tf)))
    all_names = [b.name for b in tf.blocks]
    for n, cnt in variants.items():
        users = sorted(where[n])
        cands: List[Optional[str]] = []
        if n in header_names or n in prologue_decl_names:
            cands.append(None)  # already declared for everyone: just drop the private copies
        if n in definitions:
            cands.append(definitions[n])
        cands += [t for t, _ in sorted(cnt.items(), key=lambda kv: (-kv[1], -len(kv[0])))]
        saved = {b.name: b.body for b in tf.blocks}
        saved_pro = tf.prologue
        done = False
        for cand in cands:
            for bname in users:
                b = tf.get(bname)
                b.body = "\n".join(ln for ln in b.body.splitlines()
                                   if not (tutidy.DECL_LINE_RE.match(ln) and tutidy._decl_name(ln) == n))
                b.body = re.sub(r"\n{3,}", "\n\n", b.body).strip("\n") + "\n"
            if cand is not None:
                tf.prologue = tf.prologue.rstrip("\n") + "\n" + cand + "\n"
            # the prologue changed: every block of the file must still match, not only the users
            if verify(all_names if cand is not None else users):
                out["hoisted"].append(n)
                done = True
                break
            for bname in users:
                tf.get(bname).body = saved[bname]
            tf.prologue = saved_pro
        if not done:
            out["contested"][n] = sorted(cnt)
            restore_gens(all_names)

    # 4. what still cannot live under the prologue
    for b in tf.blocks:
        u = units.get(b.name)
        if u is None:
            continue
        tufile.write_gen(p, u, tf)
        if not v.matches(p, b.name):
            b.flags.append("noprologue")
            inc, body = tufile.split_includes(b.body)
            incs = [ln.strip() for ln in tf.prologue.splitlines() if tufile.INCLUDE_RE.match(ln)]
            keep = list(dict.fromkeys(incs + [ln.strip() for ln in inc]))
            decls = tufile.materialize_old_decls(p, keep, body, tufile.prologue_decls(tf))
            b.body = "\n".join(keep) + "\n\n" + (decls + "\n\n" if decls else "") + body
            tufile.write_gen(p, u, tf)
            if not v.matches(p, b.name):
                # keep the tree green with the committed block, whatever it was
                b.body, b.flags = _committed_block(p, tu_source, b.name, b.body, b.flags)
                tufile.write_gen(p, u, tf)
            out["unresolved"].append(b.name)
    tufile._write_atomic(path, tf.render())
    return out
