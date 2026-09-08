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
    for h in (f"{p.module_src_prefix(module)}/globals.h", f"{p.module_src_prefix(module)}/{stem}.h"):
        line = f'#include "{h}"'
        if (ROOT / "include" / h).exists() and line not in {ln.strip() for ln in tf.prologue.splitlines()}:
            tf.prologue = tufile.merge_prologue(tf.prologue, [line])
            out["included"].append(h)
    header_names, header_typedefs = tutidy._header_names(p, tf.prologue)

    # 2. private typedefs that collide
    out["renamed"] = len(isolate_typedefs(tf, header_typedefs))

    # 3. declarations, symbol by symbol
    definitions: Dict[str, str] = {}
    definer: Dict[str, str] = {}
    private_typed: Set[str] = set()  # defined with a block-private parameter type: never hoisted
    for b in tf.blocks:
        private_types = set(TYPEDEF_NAME_RE.findall(b.body)) | set(TAG_RE.findall(b.body))
        for m in DEF_RE.finditer(b.body):
            definer[m.group(2)] = b.name
            if _idents(m.group(1)) & private_types:
                private_typed.add(m.group(2))
            else:
                definitions[m.group(2)] = "extern " + m.group(1).strip() + ";"
    variants: Dict[str, Counter] = OrderedDict()
    where: Dict[str, Set[str]] = {}
    for b in tf.blocks:
        private_types = set(TYPEDEF_NAME_RE.findall(b.body)) | set(TAG_RE.findall(b.body))
        for n, ln in _decls(b.body):
            if _idents(ln) & private_types or n in private_typed:
                continue  # names a block-private type: cannot leave the block
            variants.setdefault(n, Counter())[ln] += 1
            where.setdefault(n, set()).add(b.name)
    # every block that names the symbol is affected by its prologue declaration: the definer,
    # and callers that never declared it (an implicit declaration is a declaration too)
    for n in list(variants):
        if n in definer:
            where[n].add(definer[n])
        pat = re.compile(rf"\b{re.escape(n)}\b")
        for b in tf.blocks:
            if b.name not in where[n] and pat.search(b.body):
                where[n].add(b.name)
    prologue_decl_names = set(tufile._header_items(tufile.prologue_decls(tf)))

    def strip_decl(b, n):
        b.body = "\n".join(ln for ln in b.body.splitlines()
                           if not (tutidy.DECL_LINE_RE.match(ln) and tutidy._decl_name(ln) == n))
        b.body = re.sub(r"\n{3,}", "\n\n", b.body).strip("\n") + "\n"

    def candidates(n):
        cands: List[Optional[str]] = []
        if n in header_names or n in prologue_decl_names:
            cands.append(None)  # already declared for everyone: just drop the private copies
        if n in definitions:
            cands.append(definitions[n])
        cands += [t for t, _ in sorted(variants[n].items(), key=lambda kv: (-kv[1], -len(kv[0])))]
        return cands

    # phase A: every symbol gets its best-guess declaration at once; one verification per block
    chosen: Dict[str, Optional[str]] = {n: candidates(n)[0] for n in variants}
    original = {b.name: b.body for b in tf.blocks}
    base_prologue = tf.prologue
    for b in tf.blocks:
        for n in variants:
            if b.name in where[n]:
                strip_decl(b, n)
    def render_prologue():
        have = {ln.strip() for ln in base_prologue.splitlines()}
        lines = list(dict.fromkeys(c for n, c in chosen.items() if c is not None and c.strip() not in have))
        return base_prologue.rstrip("\n") + ("\n" + "\n".join(lines) + "\n" if lines else "\n")
    tf.prologue = render_prologue()
    failing = [b.name for b in tf.blocks if not verify([b.name])]
    # phase B: for a failing block, try the other candidates of the symbols it uses, one symbol at a time
    for bname in failing:
        b = tf.get(bname)
        fixed = False
        for n in [n for n in variants if bname in where[n]]:
            for cand in candidates(n)[1:]:
                prev = chosen[n]
                chosen[n] = cand
                tf.prologue = render_prologue()
                # the change must keep every other user of the symbol matching too
                if verify([bname]) and verify([u for u in sorted(where[n]) if u != bname]):
                    fixed = True
                    break
                chosen[n] = prev
                tf.prologue = render_prologue()
            if fixed:
                break
        if not fixed:
            # the block keeps its own declarations; the symbols it declares are contested
            b.body = original[bname]
            for n in [n for n in variants if bname in where[n]]:
                out["contested"][n] = sorted(variants[n])
                if chosen[n] is not None:
                    chosen[n] = None  # no prologue declaration for a contested symbol
            tf.prologue = render_prologue()
    out["hoisted"] = [n for n, c in chosen.items() if n not in out["contested"]]
    # every block is re-checked once more under the final prologue in step 4
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
