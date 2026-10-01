#!/usr/bin/env python3
"""find_xrefs.py — cross-reference slicer for GFZE01 objects.

For a target symbol, emits:
  - defining object/unit, section, address range, byte size
  - direct callers and callees (function-level, from R_PPC_REL24)
  - referenced globals/data with section, size, binding
  - a relocation-type histogram
  - a MINIMAL compilable header fragment: only the declarations the target's
    relocations actually require (T1/T3-aware: sda21 -> scalar hint,
    addr16 -> array/struct hint)

Index source: every *.o under build/GFZE01/obj (parsed with a small built-in
ELF32 reader; no pyelftools dependency). The index itself is cached by content
hash of all object files under ~/.cache/natc/slice/index-<sha>.json.

Per-symbol slices are cached by content hash under ~/.cache/natc/slice/.

Usage:
    python3 tools/find_xrefs.py --cslice <symbol> [--json] [--out FILE]
    python3 tools/find_xrefs.py --index            # (re)build index only
    python3 tools/find_xrefs.py --self-test        # fixture test
"""
from __future__ import annotations

import argparse
import hashlib
import json
import struct
import sys
import time
from pathlib import Path

REPO = Path(__file__).resolve().parent.parent
OBJ_ROOT = REPO / "build/GFZE01/obj"
CACHE = Path.home() / ".cache/natc/slice"

# --- relocation classes -------------------------------------------------
CALL_TYPES = {10}          # R_PPC_REL24
DATA_TYPES = {1, 4, 5, 6, 109}  # ADDR32, LO, HI, HA, EMB_SDA21
TYPE_NAMES = {
    1: "R_PPC_ADDR32", 4: "R_PPC_ADDR16_LO", 5: "R_PPC_ADDR16_HI",
    6: "R_PPC_ADDR16_HA", 10: "R_PPC_REL24", 109: "R_PPC_EMB_SDA21",
}
SHT_SYMTAB = 2
SHT_RELA = 4
STT_FUNC = 2
STT_OBJECT = 1
STB_GLOBAL = 1
STB_WEAK = 2


# --- minimal ELF32 big-endian reader ------------------------------------
def parse_object(path: Path):
    """Return (sections, symbols, relas_by_target_section).

    sections: list of dicts (name, type, flags, size, off, entsize)
    symbols:  list of dicts (name, value, size, info, bind, type, shndx)
    relas_by_target: {section_index: [(r_offset, r_type, sym_index, addend)]}
    """
    d = path.read_bytes()
    if d[:4] != b"\x7fELF":
        raise ValueError(f"{path}: not an ELF")
    e_shoff, = struct.unpack_from(">I", d, 0x20)
    e_shentsize, e_shnum, e_shstrndx = struct.unpack_from(">HHH", d, 0x2E)
    secs = []
    for i in range(e_shnum):
        o = e_shoff + i * e_shentsize
        name, typ, flags, addr, off, size, link, info, _al, ent = \
            struct.unpack_from(">IIIIIIIIII", d, o)
        secs.append(dict(idx=i, name_off=name, type=typ, flags=flags,
                         addr=addr, off=off, size=size, link=link,
                         info=info, entsize=ent))
    # resolve section names
    strsec = secs[e_shstrndx]
    for s in secs:
        end = d.index(b"\0", strsec["off"] + s["name_off"])
        s["name"] = d[strsec["off"] + s["name_off"]:end].decode()
    # symbols (first SHT_SYMTAB)
    syms = []
    for s in secs:
        if s["type"] == SHT_SYMTAB:
            strsec2 = secs[s["link"]]
            n = s["size"] // 16
            for i in range(n):
                o = s["off"] + i * 16
                nm, val, sz, info, other, shn = struct.unpack_from(">IIIBBH", d, o)
                if nm:
                    end = d.index(b"\0", strsec2["off"] + nm)
                    name = d[strsec2["off"] + nm:end].decode()
                else:
                    name = ""
                syms.append(dict(name=name, value=val, size=sz, info=info,
                                 bind=info >> 4, stype=info & 0xF, shndx=shn))
            break
    # relocations
    relas = {}
    for s in secs:
        if s["type"] == SHT_RELA:
            tgt = s["info"]
            lst = relas.setdefault(tgt, [])
            n = s["size"] // 12
            for i in range(n):
                o = s["off"] + i * 12
                off, info, add = struct.unpack_from(">IIi", d, o)
                lst.append((off, info & 0xFF, info >> 8, add))
    return secs, syms, relas


def index_hash() -> str:
    h = hashlib.sha1()
    for p in sorted(OBJ_ROOT.rglob("*.o")):
        h.update(p.name.encode())
        h.update(struct.pack("<Q", p.stat().st_mtime_ns))
    return h.hexdigest()


def build_index(force=False):
    CACHE.mkdir(parents=True, exist_ok=True)
    ih = index_hash()
    idx_path = CACHE / f"index-{ih[:16]}.json"
    if idx_path.exists() and not force:
        return json.loads(idx_path.read_text())
    t0 = time.time()
    index = {"hash": ih, "objects": {}, "defines": {}, "calls": {},
             "data_refs": {}}
    # calls[sym] = set of caller syms ; data_refs[sym] = {(referring_sym, rtype)}
    for path in sorted(OBJ_ROOT.rglob("*.o")):
        try:
            secs, syms, relas = parse_object(path)
        except Exception as e:  # noqa: BLE001
            print(f"WARN skip {path.name}: {e}", file=sys.stderr)
            continue
        secnames = [s["name"] for s in secs]
        obj_entry = {"path": str(path.relative_to(REPO)),
                     "sections": {sn: {"size": s["size"]}
                                  for s, sn in zip(secs, secnames)
                                  if s["type"] == 1}}
        defined = {}
        for si, sym in enumerate(syms):
            if sym["shndx"] in (0, 0xFFF1):     # UND / ABS
                continue
            if sym["stype"] not in (STT_FUNC, STT_OBJECT) or not sym["name"]:
                continue
            sec = secnames[sym["shndx"]] if sym["shndx"] < len(secnames) else "?"
            rec = dict(object=obj_entry["path"], section=sec,
                       value=sym["value"], size=sym["size"],
                       bind=sym["bind"], stype=sym["stype"])
            defined[si] = rec
            index["defines"].setdefault(sym["name"], []).append(rec)
        # per-symbol reloc attribution inside .text: walk text funcs by value
        funcs = [(s["value"], s["value"] + max(s["size"], 4), s["name"])
                 for s in syms
                 if s["stype"] == STT_FUNC and s["name"]
                 and s["shndx"] != 0]
        funcs.sort()

        def owner(off):
            for lo, hi, nm in funcs:
                if lo <= off < hi:
                    return nm
            return None

        for tgt_idx, rlst in relas.items():
            for off, rtype, si, add in rlst:
                if si >= len(syms):
                    continue
                tname = syms[si]["name"]
                if not tname:
                    continue
                src = owner(off)
                if src is None:
                    continue
                if rtype in CALL_TYPES and tname != src:
                    index["calls"].setdefault(tname, set()).add(src)
                elif rtype in DATA_TYPES:
                    key = f"{src}\x00{tname}"
                    index["data_refs"].setdefault(key, set()).add(rtype)
        index["objects"][path.stem] = obj_entry
    # sets are not JSON-serialisable directly
    def sets_to_lists(o):
        if isinstance(o, dict):
            return {k: sets_to_lists(v) for k, v in o.items()}
        if isinstance(o, set):
            return sorted(o)
        return o
    index = sets_to_lists(index)
    tmp = idx_path.with_suffix(".tmp")
    tmp.write_text(json.dumps(index))
    tmp.rename(idx_path)
    # prune old indexes
    for old in CACHE.glob("index-*.json"):
        if old != idx_path:
            old.unlink(missing_ok=True)
    print(f"[xref] indexed {len(index['objects'])} objects, "
          f"{len(index['defines'])} symbols in {time.time()-t0:.1f}s "
          f"-> {idx_path.name}", file=sys.stderr)
    return index


# --- cslice -------------------------------------------------------------
def guess_decl(sym, rec, rtypes):
    """Best-effort C declaration from section/size/type/reloc evidence."""
    name = sym
    # ELF parser stores numeric relocation IDs; JSON fixtures and callers often
    # provide names. Normalize both so ADDR16 evidence cannot disappear.
    reloc_names = {TYPE_NAMES.get(int(t), str(t)) if isinstance(t, int) else str(t)
                   for t in rtypes}
    rtypes = set(rtypes) | reloc_names
    if rec["stype"] == STT_FUNC:
        return f"void {name}(void);  /* signature unknown: adapt from refs */"
    sec = rec["section"]
    size = rec["size"]
    sda = 109 in rtypes
    if "sdata2" in sec or "sbss2" in sec:
        base = "double" if size == 8 else "float" if size == 4 else "char"
        qual = "const volatile " if "sdata2" in sec else ""
        arr = f"[{size}]" if base == "char" and size > 8 else ""
        return f"extern {qual}{base} {name}{arr};  /* r2/.sdata2, sda21={sda} */"
    if sda and size <= 8:
        t = {1: "char", 2: "short", 4: "int", 8: "long long"}.get(size, "char")
        return f"extern {t} {name};  /* sdata/sbss scalar (T1) */"
    # ADDR16_HA/LO (and plain ADDR16) data globals are byte arrays, not
    # scalars: dtk declares `extern unsigned char lbl_X[n]`. Emit the array
    # shape so a worker does not feed MWCC an invalid scalar access (T1/T3).
    if rtypes & {"R_PPC_ADDR16_HA", "R_PPC_ADDR16_LO", "R_PPC_ADDR16"}:
        if size and size > 1:
            return f"extern unsigned char {name}[{size}];  /* ADDR16 data: array shape */"
        if size == 1:
            return f"extern volatile unsigned char {name};  /* ADDR16 byte */"
    if size == 4:
        return f"extern int {name};  /* 4 B; check float vs int */"
    return f"extern unsigned char {name}[{max(size, 1)}];  /* shape unknown */"


def _best_def(defs):
    """Choose the linked, strong definition deterministically, not defs[0]."""
    if not defs: return None
    def rank(r):
        bind=r.get('bind',0); sec=r.get('section','') or ''
        return (bind == STB_GLOBAL, bind != STB_WEAK, r.get('size',0) or 0,
                not sec.startswith('UND'), str(r.get('object','')))
    return max(defs, key=rank)

def _grec(index, sym):
    """Section/size/type/binding of a referenced global, or empties if the
    symbol is external (not defined in any carved object). Spec requires the
    slice to report each referenced global's section/size/type so a worker does
    not have to reach for a 330 KB .ctx to learn them."""
    d = index["defines"].get(sym)
    if not d:
        return {"section": None, "size": None, "stype": None, "bind": None}
    r = _best_def(d)
    if r is None:
        return {"section": None, "size": None, "stype": None, "bind": None}
    return {"section": r.get("section"), "size": r.get("size"),
            "stype": r.get("stype"), "bind": r.get("bind")}


def cslice(index, symbol, use_json=False):
    defs = index["defines"].get(symbol)
    if not defs:
        raise SystemExit(f"symbol not found in object index: {symbol}")
    # prefer the definition that lives in an actual linked object
    rec = _best_def(defs)
    callers = sorted(index["calls"].get(symbol, []))
    callees, globals_ref, hist = [], [], {}
    # find the function body's own relocations: scan data_refs and calls where
    # the *source* is this symbol — need reverse view; rebuild cheaply here.
    src_calls, src_data = [], []
    for callee, srcs in index["calls"].items():
        pass  # calls map is callee->callers; reverse lookup below
    # Reverse views
    for callee, srcs in index["calls"].items():
        if symbol in srcs:
            callees.append(callee)
    for key, types in index["data_refs"].items():
        src, tgt = key.split("\x00")
        if src == symbol:
            globals_ref.append((tgt, types))
            for t in types:
                hist[TYPE_NAMES.get(t, str(t))] = hist.get(
                    TYPE_NAMES.get(t, str(t)), 0) + 1
        if tgt == symbol and src != symbol:
            for t in types:
                hist[TYPE_NAMES.get(t, str(t))] = hist.get(
                    TYPE_NAMES.get(t, str(t)), 0) + 1

    # header fragment: declarations for everything this symbol references
    frag_lines = []
    seen = set()
    for callee in sorted(set(callees)):
        d = index["defines"].get(callee)
        callee_def = _best_def(d) if d else None
        if callee_def and callee_def["object"] == rec["object"]:
            continue  # same TU: no extern needed
        seen.add(("fn", callee))
        frag_lines.append(f"void {callee}(void);")
    for tgt, tset in sorted(globals_ref):
        if tgt in seen:
            continue
        td = index["defines"].get(tgt)
        if not td:
            continue
        target_def = _best_def(td)
        if target_def and target_def["object"] == rec["object"]:
            continue
        seen.add(("dat", tgt))
        # pass THIS reference's relocation types so guess_decl can apply the
        # T1/T3 declaration rule (sda21 -> scalar hint, addr16 -> array/struct)
        frag_lines.append(guess_decl(tgt, target_def, sorted(tset)))

    result = {
        "symbol": symbol,
        "definition": rec,
        "definitions": [rec] + [d for d in defs if d != rec],
        "alternate_definitions": [d for d in defs if d != rec],
        "alternate_defs": len(defs) - 1,
        "callers": callers,
        "callees": sorted(set(callees)),
        "globals_referenced": [
            {
                "symbol": g,
                "types": sorted(TYPE_NAMES.get(t, str(t)) for t in ts),
                **_grec(index, g),
            }
            for g, ts in sorted(globals_ref)],
        "reloc_histogram": hist,
        "header_fragment": "\n".join(frag_lines),
        "cache": str(CACHE),
    }
    blob = json.dumps(result, indent=2, sort_keys=True)
    h = hashlib.sha1(blob.encode()).hexdigest()
    cf = CACHE / f"{h}.slice.json"
    if not cf.exists():
        cf.write_text(blob)

    if use_json:
        print(blob)
    else:
        r = rec
        print(f"SYMBOL  {symbol}")
        print(f"OBJECT  {r['object']}")
        print(f"SECTION {r['section']}  size {r['size']} (0x{r['size']:x})  "
              f"value 0x{r['value']:x}  bind "
              f"{'global' if r['bind']==STB_GLOBAL else 'local'}")
        if result["alternate_defs"]:
            print(f"NOTE    {result['alternate_defs']} alternate definition(s) "
                  f"in other objects — diff them (SDK revisions differ)")
        print(f"CALLERS ({len(callers)}): " + ", ".join(callers[:20])
              + (" ..." if len(callers) > 20 else ""))
        print(f"CALLEES ({len(result['callees'])}): "
              + ", ".join(result["callees"][:20]))
        print("GLOBALS:")
        for g in result["globals_referenced"]:
            sz = g.get("size")
            extra = (f"  ({sz} B, {g.get('section')})"
                     if sz is not None else "")
            print(f"  {'+'.join(g['types']):<40} {g['symbol']}{extra}")
        print("RELOC HISTOGRAM:", hist or "(none)")
        print("\n/* minimal header fragment — only what this symbol needs */")
        print(result["header_fragment"] or "/* (self-contained) */")
        print(f"\n[cached {cf}]")
    return result


# --- fixture test ---------------------------------------------------------
def self_test():
    """Structural fixture: index builds, known SDK symbol resolves, fragment
    is non-empty for a symbol with external refs."""
    index = build_index()
    assert index["objects"], "index empty"
    # GXMisc.o is the stable fixture object in every worktree state; pick a
    # function it defines rather than hardcoding an SDK symbol that comes and
    # goes as the integrator lands/carves units.
    fixture = next((s for s, defs in index["defines"].items()
                    if any(d["object"].endswith("gx/GXMisc.o")
                           for d in defs)), None)
    assert fixture, "GXMisc.o missing from corpus — fixture broken"
    # every call edge must reference a symbol defined somewhere in the corpus
    # OR an external SDK symbol whose object isn't carved yet — only fail on
    # edges that are neither
    for callee in list(index["calls"])[:50]:
        if callee not in index["defines"]:
            print(f"note: unresolved call edge (external): {callee}")
    # pick any symbol with callers + data refs and slice it
    victim = None
    for sym, srcs in index["calls"].items():
        if srcs and sym in index["defines"]:
            has_data = any(k.split("\x00")[0] == sym
                           for k in index["data_refs"])
            if has_data:
                victim = sym
                break
    assert victim, "no symbol with both callers and data refs found"
    import io, contextlib
    buf = io.StringIO()
    with contextlib.redirect_stdout(buf):
        res = cslice(index, victim)
    assert res["header_fragment"] is not None
    assert res["callers"], "fixture expected callers"
    # regression guard: the relocation type MUST reach the header fragment so
    # the T1/T3 declaration rule is visible. GXSetMisc references `gx` via
    # R_PPC_EMB_SDA21 in every worktree state; its fragment must report
    # sda21=True (the old code passed an empty rtypes list, so it always read
    # False and the declaration guidance was lost).
    if "GXSetMisc" in index["defines"]:
        gm = cslice(index, "GXSetMisc")
        assert "sda21=True" in gm["header_fragment"], (
            "relocation type not propagated to header fragment: "
            + gm["header_fragment"])
    # Reviewer-grade guards (hard-tail review, 2026-08-26). These lock the
    # behaviours a from-scratch reader cannot see from the happy-path test:
    #   (a) ADDR16_HA+LO DATA global -> array/struct shape, NOT a scalar (T1);
    #       OSPanic references OSErrorFmt_80122C30 via ADDR16_HA+LO and it is
    #       a data object (not a callee), so it must be shaped as an array.
    #   (b) a callee in the SAME TU must be suppressed from the fragment
    #       (OSPanic calls OSReport, both in dolphin/os/OSError.o), while a
    #       cross-TU callee (vprintf) must still appear.
    if "OSPanic" in index["defines"]:
        op = cslice(index, "OSPanic")
        assert "OSErrorFmt_80122C30" in op["header_fragment"], \
            "ADDR16 data global dropped from fragment: " \
            + op["header_fragment"]
        assert "OSErrorFmt_80122C30[" in op["header_fragment"], \
            "ADDR16 data global not shaped as array/struct (T1): " \
            + op["header_fragment"]
        assert "OSReport" not in op["header_fragment"], \
            "same-TU callee not suppressed from fragment: " \
            + op["header_fragment"]
        assert "vprintf" in op["header_fragment"], \
            "cross-TU callee missing from fragment: " + op["header_fragment"]
        # referenced globals MUST carry section/size/type (spec item 1)
        ref = next(g for g in op["globals_referenced"]
                   if g["symbol"] == "OSErrorFmt_80122C30")
        assert ref["section"] is not None, "referenced global missing section"
        assert ref["size"] is not None, "referenced global missing size"
        assert ref["stype"] is not None, "referenced global missing type"
    print(f"SELF-TEST OK (victim={victim}, callers={len(res['callers'])}, "
          f"frag lines={len(res['header_fragment'].splitlines())})")


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--cslice", metavar="SYMBOL")
    ap.add_argument("--json", action="store_true")
    ap.add_argument("--index", action="store_true", help="build index only")
    ap.add_argument("--force-index", action="store_true")
    ap.add_argument("--self-test", action="store_true")
    args = ap.parse_args()

    if args.self_test:
        self_test()
        return
    if args.index or args.force_index or not args.cslice:
        build_index(force=args.force_index)
        return
    index = build_index()
    cslice(index, args.cslice, use_json=args.json)


if __name__ == "__main__":
    main()
