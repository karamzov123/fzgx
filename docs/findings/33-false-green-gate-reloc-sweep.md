# Finding 33 — FALSE-GREEN GATE + reloc-parity sweep postmortem (2026-08-23, session 20)

## What happened
Session 19's reloc-parity waves (98a93c9..0f9c474) claimed +115 exact fns
(916 -> 1031/2235) with "gate green throughout". Both claims were wrong:

1. **The ELF link was failing the whole time.** Converting numeric lis/addi
   pairs that reference `scope:local` symbols to named `@ha/@l` produces
   unresolvable refs: dtk emits locals as VA-suffixed names (`Scb_8015BFC0`,
   `RunQueue_8015C018`, `DriveInfo_8015BF00`, `ResetFunctionInfo_80123AE0`,
   `gTRKExceptionStatus_8015B874`). Undefined-symbol link errors → ninja stops
   BEFORE `dol_apply` → stale main.dol stays on disk → sha1 check passes.

2. **Named-reloc rewrites are not byte-neutral anyway.** Where names resolved,
   MWCC compiles a real extern with different register allocation than our
   hand-written numeric pair (e.g. retail `addi r31,r3,...` vs our
   `addi r3,r3,...`; 898 differing bytes / 623 clusters vs retail DOL). The
   objdiff "+115 fns" were scoring artifacts of non-retail code shapes.

## Proof (bisect on clean builds, `rm -f build/GFZE01/main.dol` first)
- ba70184 → rebuilds sha1 `421c8810...b271` GREEN.
- 98a93c9, 0f9c474, HEAD-after → all produce `c237ecaa...` (or no dol at all).

## Fix applied
Commit 064a910 restores src/ to ba70184 state. Fresh-build gate green:
sha1 `421c88106697d3275a3fc26fb7a01bf6d816b271`, objdiff 916/2235 exact,
128296 code bytes.

## HARD RULES going forward
1. Gate = `rm -f build/GFZE01/main.dol && ninja && sha1sum build/GFZE01/main.dol`.
   Never trust sha1 without deleting the dol first; same staleness applies to
   build/GFZE01/report.json (delete before reading counts).
2. `tools/fix_reloc_pairs.py` is QUARANTINED: it must never convert pairs whose
   target symbol is scope:local, and any named-symbol rewrite must be proven
   byte-neutral per-unit on a fresh full build before commit.
3. Renames of an object symbol require checking its `scope:` in symbols.txt;
   local objects can only be referenced from their defining unit via dtk's
   suffixed name, so cross-unit named refs are impossible by construction.

## Correct path to reloc-shape parity (future)
Convert numeric→named ONLY where retail asm itself shows a named relocation to
a GLOBAL symbol, and only within units that already match everything else.
Expect register-allocation differences; verify each unit individually with a
fresh build + per-unit report diff, not repo-wide sweeps.
