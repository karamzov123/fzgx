# findings/34 — rename-loss ROOT CAUSE SOLVED (Claude Opus 5 investigation, 2026-08-23)

## Verdict
`.note.split` is EXONERATED — it contains zero name-keyed metadata (only GENR/MODN/MODI/VIRT
address lists). The "tail unit" pattern was unit SIZE sensitivity: 4-fn units lose 25%+ match
per unpaired fn; 26-fn CARDDir dilutes the same shape to noise.

## Actual mechanism
objdiff matches relocations BY NAME: base-side = C identifier, target-side = symbols.txt.
Rename atomically in both via `tools/rename_sym.py` → byte-neutral (asm nofralloc blocks have
literal register tokens; MWCC does no allocation). Session 19's failure was a DIFFERENT
transform (fix_reloc_pairs.py rewriting numeric immediates to @ha/@l, changing encodings).

## Proof
`rename_sym.py fn_8002F140 __CARDSeek` → fresh-build sha1 421c8810...b271 GREEN,
CARDRead.c matched_functions 4/4 = 100% at baseline. Commit 277f077.

## Sprint consequence
THE RENAME BLOCKER IS DEAD. All M3 semantic naming waves can proceed through
rename_sym.py without objdiff penalties. Note: CARDWrite.c WIP moved to /tmp/CARDWrite.c.wip
(was red-gate reloc-parity edits of the findings/33 class — do not reapply as-is).
Also: resolve __CARDSeek identity contradiction between CARDRead.c:4-5 comment and
findings/30-card-names.md:123 before further card renames.
