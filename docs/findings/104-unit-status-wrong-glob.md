# 104: RETRACTION of findings/103 — unit_status.py compared retail-vs-retail

Date: 2026-08-24 (v2-audit enactment). Supersedes findings/103.

## The defect
`tools/unit_status.py` globbed `build/GFZE01/obj/**` — the dtk-extracted TARGET
(retail) objects — instead of `build/GFZE01/src/**` (OUR compiled objects).
Every "function" it classified was a retail object compared against the retail
DOL slice. Trivially near-100% exact, hence "blocker queues EMPTY".

Proof: `build/GFZE01/src/dolphin/os/OSSram.o` (sha1 39dab06a…) differs from
`build/GFZE01/obj/dolphin/os/OSSram.o` (24e6efb2…). Comparing OUR OSSram
objects word-wise against the DOL gives real diffs in every function
(e.g. __OSInitSram 65/77 words equal) — the "all exact" result was an artifact.

## Consequences
- findings/103's headline ("blocker queues empty; GLOBAL-NAMED bank stale;
  ADDR16 queue done") is FALSE.
- The real ADDR16 queue stands as the v2 audit measured it independently via
  objdiff instruction sweeps: **423 sites / 178 funcs / 68,004 B (+11.80 pp)**,
  plus 14 symbol-name fixes and 5 symbols.txt size fixes (audit App A/B).
- The claim "MWCC asm auto-emits sda21 for r13-relative operands" (STATE handoff
  v3) is also retracted: measured ZERO sda21 relocs in our asm .o vs 6,009 in
  the targets.

## Fix applied (this commit)
unit_status.py now globs `build/GFZE01/src/**/*.o` and derives unit names from
the src path. Regenerated TSV (our objects):
- functions=2218, byte_exact_or_reachable_only=2215
- TOLERATED 1539 (reloc-shaped diffs incl. the unconverted-but-reachable ADDR16
  sites), NO-VA 2 (CARDDelete, SystemCallVector), SIZE 1 (InitMetroTRK — audit
  Appendix B), REAL-DIFF 0.
Note TOLERATED means "shape-correct under reloc tolerance", NOT converted:
the per-unit work queue lives in audit Appendix C (objdiff sweep), which is the
authoritative batch list for the mechanical sweep.

## Standing rule adopted
Any tool whose output steers a directive gets a review against ground truth
before its first findings/*.md citation.
