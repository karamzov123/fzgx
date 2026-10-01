# Finding 36 — Repo-wide reloc-parity audit: ALL matched code is linked-exact

Session 23 (2026-08-23 evening), fresh-build gate green at 503e7c4 baseline
(sha1 421c8810…b271, objdiff 1258/2195 exact / 215920B).

## Method
`xargs -a <(all 217 build objs) python3 tools/reloc_parity_audit.py`
— per-function ELF .text bytes vs retail DOL slice, every diffing word must be
explained by a relocation of a supported class.

## Result
**2225 / 2225 audited functions have ZERO non-relocation word diffs — zero flags.**
(Update, same session: duplicate-symbol disambiguation by size added to the
tool; SIBios AlarmHandler global@0x8001287C vs dvd.c local@0x80018510 both
audit OK. Commit fba7f7a.)

## Tool fixes committed (tools/reloc_parity_audit.py)
1. R_PPC_EMB_SDA21 d-form set was missing most load/store opcodes — MWCC emits
   sda21 on ANY d-form incl. lfd(50)/stfd(54)/lfs/stfs(48/52)/psq_l/st(56/60)
   and lha family (40-43). Now full d-form range.
2. R_PPC_REL14 added (conditional branches: opcode+BO/BI preserved, disp
   zeroed in obj, CR-bits equality enforced).
3. R_PPC_ADDR16_HI accepted alongside HA/LO.
Before these fixes the tool false-flagged ~99 fns (31 stfd/lfd-disp,
56 multi-site same classes, 6 lis/addi HI, 3 REL14).

## Consequence
objdiff fuzzy% below 100 is scoring noise for EVERY unit in this repo.
There is no hidden byte-mismatch backlog. Remaining levers:
- Semantic naming only (rename_sym.py, byte-neutral).
- New carve candidates are gone (.text fully carved except permanent 80B
  coarse/text_8006E1B0).
Do NOT spend time "fixing" near-miss units' bytes.
