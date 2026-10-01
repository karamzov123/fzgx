# Finding 269 — ADXT `fn_80057378` stream-shutdown lifecycle evidence dossier

- Unit: `main/game/adxt_800570DC`
- Symbol: `fn_80057378`
- Address: `0x80057378`
- Evidence scope: static only
- Evidence action: reference manifest, relocation-aware xrefs, exact context-only disassembly, and caller/lifecycle inspection
- Conversion/lease/candidate/compile/submit/gate/runtime action: none

## Reproduction

Run from the repository root:

```sh
python3 tools/natc_reference_manifest.py --unit main/game/adxt_800570DC
python3 tools/find_xrefs.py --cslice fn_80057378 --json
python3 tools/natc_loop.py --unit main/game/adxt_800570DC --symbol fn_80057378 --context-only
```

These are read-only evidence commands. The checkout was dirty before this
finding; only this dossier is included in the commit.

## Identity and exact body

The reference manifest reports `41` remaining asm functions in
`src/game/adxt_800570DC.c`, `reference_backed: 0`, and no natural-C reference
for `fn_80057378`. The canonical target object is
`build/GFZE01/obj/game/adxt_800570DC.o`; the symbol is a global `.text`
function with section-relative value `0x29c` and size `164` bytes (`0xa4`).
The coarse mirror has the same size (`164` bytes).

The context-only disassembly reproduces this body:

```text
0x00  stwu r1, -0x20(r1)
0x04  mflr r0
0x08  stw r0, 0x24(r1)
0x0C  addi r3, r1, 8
0x10  stw r31, 0x1c(r1)
0x14  stw r30, 0x18(r1)
0x18  bl 0x18   R_PPC_REL24 gccicrit_enter +0
0x1C  lis r3, 0
0x20  addi r4, r3, 0
0x24  lwz r3, 0(r4)
0x28  addic. r0, r3, -1
0x2C  stw r0, 0(r4)
0x30  bne 0x84
0x34  lis r3, 0
0x38  li r30, 0
0x3C  addi r31, r3, 0
0x40  lbz r0, 0(r31)
0x44  cmpwi r0, 1
0x48  bne 0x54
0x4C  mr r3, r31
0x50  bl 0x50   R_PPC_REL24 fn_80057114 +0
0x54  addi r30, r30, 1
0x58  addi r31, r31, 0x238
0x5C  cmpwi r30, 0x10
0x60  blt 0x40
0x64  lis r3, 0
0x68  li r4, 0
0x6C  addi r3, r3, 0
0x70  li r5, 0x2380
0x74  bl 0x74   R_PPC_REL24 memset +0
0x78  li r3, 0
0x7C  li r4, 0
0x80  bl 0x80   R_PPC_REL24 gcci_set_critical_value +0
0x84  addi r3, r1, 8
0x88  bl 0x88   R_PPC_REL24 gccicrit_leave +0
0x8C  lwz r0, 0x24(r1)
0x90  lwz r31, 0x1c(r1)
0x94  lwz r30, 0x18(r1)
0x98  mtlr r0
0x9C  addi r1, r1, 0x20
0xA0  blr
```

## Verified lifecycle evidence

`find_xrefs.py --cslice fn_80057378 --json` reports one relocatable caller,
`fn_80041164`, and five callees: `gccicrit_enter`, `fn_80057114`, `memset`,
`gcci_set_critical_value`, and `gccicrit_leave`. Its global relocations are
ADDR16 HA/LO pairs for:

- `lbl_80188A88`, `.bss`, 4 bytes;
- `lbl_80188A8C`, `.bss`, 9092 bytes.

The body establishes a guarded shutdown transition:

1. Enter the `gccicrit` critical section using the stack record at `r1+8`.
2. Decrement the word at `lbl_80188A88`. If the resulting value is nonzero,
   skip stream teardown and only leave the critical section.
3. When the result is zero, scan exactly `0x10` records beginning at
   `lbl_80188A8C`, advancing by `0x238` bytes per record. For each record whose
   byte at offset `0` equals `1`, call `fn_80057114(record)`.
4. Clear `0x2380` bytes from `lbl_80188A8C` with `memset`, then call
   `gcci_set_critical_value(0, 0)`.
5. Leave the critical section and return.

The `0x10 * 0x238 = 0x2380` arithmetic matches the cleared pool span. The
static evidence proves the refcount/last-release guard and synchronized pool
reset; it does not prove the semantic type or ownership meaning of
`lbl_80188A88` beyond its observed decrement-and-zero gate.

## Relationship to `fn_80057114`

Finding 262 already records that `fn_80057114` is a `216`-byte cleanup body
called by `fn_80057378`, and that it clears an individual active record before
zeroing that record with `memset(record, 0, 0x238)`. This dossier adds the
caller-side lifecycle condition: those per-record cleanup calls occur only on
the final decrement of `lbl_80188A88`, inside `gccicrit_enter`/`gccicrit_leave`.
The complete pool clear and `gcci_set_critical_value(0, 0)` follow the sixteen
record visits while still in that critical section.

The only indexed caller, `fn_80041164` at `src/game/tail_800410A4.c:127-166`,
decrements a separate word at `lbl_80178CA8` and, when that result is zero,
executes `fn_8004EF68`, `fn_8004F6B0`, `fn_8004B79C`, then
`fn_80057378`, followed by server-lock, callback deletion, server exit, and
additional ADXT/SVM cleanup calls. Thus `fn_80057378` is statically positioned
as the ADXT stream-pool cleanup step in the final server-shutdown path. This
is a call-order observation, not proof that the two counters have the same
semantic owner or that every shutdown path reaches this caller.

## Disposition

This target adds non-redundant, high-value static evidence: exact function
identity and relocations, the final-decrement guard on `lbl_80188A88`, the
critical-section boundary around pool teardown, the post-loop global reset,
and the caller's placement in the final server-shutdown sequence. It remains
`blocked-evidence` for candidate generation because there is no natural-C
reference body and no verified runtime fact. No source, queue, lease, or fleet
state was changed.
