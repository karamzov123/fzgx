# 110: pragma-isolated asm restores retail scheduling; fuzzy-100 vs the DOL

Date: 2026-09-06. Follows findings/11 (asm poisons peephole) and findings/06
(-O4,p scheduler hoists into the prologue gap).

## Result

dolphin/ax/AXOut.c: fn_80021B84 (0x80021B84, 0x58) landed as natural C at
100.000%, DOL sha1 421c8810...b271, commit 3780197e (batch
batch-axout-b84-v2). The unit keeps its retail-ordered asm blocks; every
top-level `asm` function is now wrapped in `#pragma push` / `#pragma pop`.

Route to the landing, because every step taught something:
- probe v_h3 (fn_80021B84 alone, natc ledger attempt 13): 100% isolated.
- E1/E3: probe body spliced into the real unit — objdiff fuzzy 100% but
  GATE RED: the linked DOL sha changed. Fuzzy-100 is necessary, not
  sufficient.
- E10: single-delta test on the last source difference (label declared
  `u32 lbl_801A6B30` scalar vs `unsigned char lbl_801A6B28[4]`-style array).
  Real effect: 1 differing .text byte and 14 differing .rela.text bytes —
  the SDA21 relocs sat at different instruction offsets because the load
  FORM differs (lwz table@l vs addi rX, rY@l inside the jumptable-holder
  sibling). Equal-length, differently-encoded addresses read as "same
  function" to fuzzy pairing while the bytes differ.
- E14: no C change at all — wrapped every top-level asm block in
  `#pragma push`/`#pragma pop`. fn_80021B84 flipped 63.6% -> 100.000% and
  the DOL gate went green.

## Finding 1: asm blocks poison the SCHEDULER for later C, and pragma isolation fixes it

findings/11 showed an `asm` function disables clrlslwi fusion for all later
C in the TU, fixed by moving asm to the end of the file. AXOut cannot do
that: retail's address order puts asm functions before natural C. Symptom
with unwrapped asm present: the -O4,p scheduler hoists independent
argument-setup `li`s of the LATER natural function into its prologue gap
(between `mflr r0; stw r0, 4(r1)` and `stwu r1, -X(r1)`), exactly the T19
shape from findings/06 — while the same body scores 100% in isolation.

Fix that works: `#pragma push` before, `#pragma pop` after EACH top-level
asm block. With isolation, scheduling of the natural functions returns to
retail shape with no other change. RULE for mixed units: wrap every
top-level asm block, even when retail order forbids moving them.

Mechanical application (the loop that landed this):
regex-wrap `asm void name(void)\n{\n...\n}\n` blocks, recompile, re-score
the WHOLE unit. Script sketch kept at /tmp/wrap_asm.py this session —
candidate for promotion into tools/natc_splice.py or a natc_wrap_asm.py.

## Finding 2: objdiff fuzzy-100 can hide real byte diffs; the DOL sha is the arbiter

E1/E3 scored fuzzy-100 per symbol while the object differed from the head
object by 68 bytes, almost all in .rela.text (reloc offsets moved) plus a
single .text operand byte. Linked DOL caught it (GATE RED). When a
candidate says 100% but you can cheaply byte-compare the .o against the
head/ninja object, do it; if anything differs, diff .rela.text too —
moved relocs are invisible in the instruction text. Never trust fuzzy-100
over the sha1 gate; never trust the sha1 gate without a full rebuild
(gate deletes and rebuilds the batch objects itself).

## Finding 3: address-holding labels follow the unsized-label rule too

`lbl_801A6B30` holds an address that retail loads with one instruction
form; declaring it `unsigned char lbl_801A6B30[4]` vs `u32` scalar changes
that form (and reloc placement) even though both "work". Section 4's rule
("treat every 4-8 byte label as unsized until proven otherwise") applies to
labels that HOLD addresses, not only labels that ARE data.

## State after this session

- AXOut: 6 pragma-wrapped asm blocks, natural fn_80021B84, gate green
  (3780197e). The stale `fn_80021B84` fresh-row in the ledger is now
  resolved; 28 stale 'fresh' rows across the fleet were deleted the same
  day (symbols already C at head but left 'fresh' in symbol_states — the
  source-of-truth audit is `asm\s.*name\(` on the definition line, not a
  substring grep, which false-matches provenance comments like
  "asm-relocation-fix").
