# 285 — the "regalloc" class is not one shape: 395 differing rows split three ways

Date: 2026-10-04. Tools: `fzgx stuck --min-percent 99`, `oracle.check` over the 60 closest
functions in the >=99% band.

## Why this census exists

Findings 282 and 284 both ended at the same place: the >=99% band does not yield to the
deterministic repair engine (40,208 candidates, 0 matches), and on `colchg_menu_disp` four
structurally different bodies converge on exactly 90.093025 with a byte-identical diff. Both
concluded "the residue is which register MWCC picks". That is a claim about a *class*, so it
needs measuring before anyone builds a generator for it -- which is the mistake move 5 would
have been.

`fzgx stuck --min-percent 99` reports the band as mostly `regalloc`:

    pure mode      functions   mean %      multi-label row categories
    regalloc            62     99.5       regalloc 109, imm 31, op 25, ins 19, reloc 13, frame 5
    mixed              47     99.3
    imm                15     99.5
    reloc               3     99.4
    ext                 2     99.3
    schedule            2     99.7
    frame               1     99.8
    regalloc+schedule   1     99.3

109 of 135 flagged `regalloc`. If that is one shape, one generator could address it.

## What the rows actually say

Parsed every differing objdiff row across the 60 closest functions: **395 rows**.

| divergence class | rows | share |
|---|---:|---:|
| same mnemonic, **register** differs | 232 | 59% |
| **mnemonic** differs | 11 | 3% |
| same mnemonic, **immediate** differs | 33 | 8% |
| rows needing relocation/scheduling context | rest | ~30% |

So `regalloc` is directionally right — 59% of differing rows really are the same instruction
with a different register. But the top register-only divergences are **not one pattern**:

    lwz   r0            -> r3          7
    addi  r28, r27       -> r28, r3     6
    add   r28, r0, r28   -> r28, r27, r28  6
    mr    r3, r29        -> r3, r30      4
    addi  r3, r23        -> r3, r29      4
    lis   r3             -> r4          4

These are unrelated: a return-value register (r0 vs r3), a callee-saved pair (r27/r23 vs
r29/r30), an address base (lis r3 vs r4). Each has a different cause and a different fix, and
none of them is "keep an address live so r5 is occupied" -- the colchg hypothesis, which is
correct *for colchg_menu_disp* and generalises to nothing.

The 11 true mnemonic differences are equally heterogeneous: `mr`->`li`, `extrwi`->`srawi`,
`stmw`->`addi` (prologue shape), `clrlwi`->`slwi`, `ble`->`beq` (a real control-flow bug in
one candidate), `sth`->`stw`, `cmpw`->`cmplw`. One function's prologue disagreement is not
another function's shift-vs-mask disagreement.

## The finding

**`regalloc` is a bucket, not a shape.** It is the objdiff label for "this row's bytes differ
in a register", which covers at least three unrelated causes: a value landing in a different
register because it was re-derived instead of held in a local (the `MWCC_IDIOMS.md` rules at
141-166), a callee-saved allocation order difference, and an address base choice. A generator
keyed on `regalloc` would have to try all three per function, which is what the 40,208
candidates already did and why it returned 0.

This is the same lesson as the size bands (283) and the level scan: **the label is not the
shape.** Measure the population, find the population, then build for it.

## Actionable item 1, worked: fn_12_23410's prologue (finding 285 follow-up)

The census named two items worth acting on. Both were taken as far as the evidence allows.

`fn_12_23410` (movie_module, 752 B, ledger 100.0%, honestly 97.181%) differs in exactly its
prologue and epilogue:

    ~ 000C  stmw r27, 0x1c(r1)        | addi r11, r1, 0x30
    > 0010                            | bl _savegpr_27
    > 0014                            | mr r30, r4
    ~ 02E4  lmw r27, 0x1c(r1)        | addi r11, r1, 0x30
    > 02E8                            | bl _restgpr_27

Retail saves r27-r31 with a multi-word `stmw`/`lmw`. Ours calls `_savegpr_27`. Per
`docs/MWCC_IDIOMS.md:121`, the prologue form reports how many non-volatile registers are live
across calls -- so this says retail keeps **five or more** values alive across the whole body
and our body keeps fewer.

**Is `stmw` itself unusual?** Measured over matched functions, which are ground truth:

    matched sample (400):  stmw 5   | bl _savegpr_ 21  | neither 374
    >=99% band:            stmw 17  | bl _savegpr_ 43  | neither 76

So `_savegpr_` is the normal form and `stmw` is the rare one. `fn_12_23410` is genuinely
unusual, not misread.

**Tried, all 97.181 -- identical prologue, no change:**

- `fixup_source.inline_helpers` on the `find_entry` helper: produces 1 candidate, no change.
  The helper is already `static` and MWCC already inlines it, so inlining is not the lever.
- Hoisting the three parameters into locals (`n0`, `d0`, `s0`) to keep more state live across
  the calls: identical object.

**What is left, stated as a hypothesis rather than a guess:** retail's body keeps r27..r31 live
simultaneously, which means the real source holds five or more values across the
`memcpy`/`fn_12_215F4` calls -- most likely the four candidate offsets plus a cursor, structured
so the four retry calls share one loop rather than four near-identical call sequences. Our body
expresses four separate `find_entry(...)` calls, each of which frees its registers at the call.
That is a control-flow shape question (four call sites vs one loop), not a register-allocation
one, and it is the kind of rewrite a model session does by reading the whole body -- no
statement-level family generates it.

**Correction to my own census line above:** I first read `stmw` as having "appeared" in a
variant, by grepping the whole diff row. That row contains *both* sides, so it matched retail's
target text, not our output. Any variant check here must look only at the text after the `|`.
With that fixed, both variants produce byte-identical prologues to the baseline, which is what
the table says.

## Actionable item 2, closed without work

The `ble`->`beq` divergence is a signed/unsigned comparison defect, and
`fixup_source.signedness_flips` is already registered in `Engine.proposals` for exactly this
(alongside `cmpw`->`cmplw`, which also appears in the census). The family exists; it was not
missing. No new work.

Per-function, not per-class. A candidate that is one instruction away should be read as that
one instruction -- which is what a model session does with a diff, and what no single
generator can do for a 135-function bucket containing three unrelated causes.

## The one measurement caveat worth recording

`oracle.check(p, sym, 0, ...)` returns `ok=True` with **zero diff rows**, because
`max_diff_lines=0` means "no rows", not "unlimited". My first two census scripts therefore
reported "0 rows examined" and looked like a null result rather than a bug in the harness.
Any analysis that classifies diff rows must pass a real limit. `fzgx check` defaults to 80
for this reason; a script that hardcodes 0 measures nothing and looks like a finding.