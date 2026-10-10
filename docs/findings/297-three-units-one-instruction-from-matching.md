# 297 -- three units sit exactly one instruction from matching

Each residual below is fully characterized, byte-verified against retail, and reproducible
locally without a lease. None has been closed. The point of this finding is to record *exactly*
which source shapes were tried and failed, so the next session does not re-spend a compile
round on them.

All three are now carved (split range + units.json entry), so a body that matches will link
instead of duplicating the function. What remains is the single instruction.

---

## A. `fn_17_7728` (interview, 0x34C, 211 instructions, plateau 99.971565%, 21 attempts)

Single differing word at offset 0x2C:

    target  80 C7 00 14   lwz r6, 0x14(r7)
    ours    80 C4 00 74   lwz r6, 0x74(r4)

Both decode to opcode 32, rt=r6. **Same effective address** -- `addi r7, r4, 0x60` sits at 0x28,
so r4+0x74 == r7+0x14. Retail reloads through the r7 base the preceding `addi` materialized;
MWCC folds the offset into an r4-relative load. Semantically identical, only the addressing
form differs.

Best archived body: `.fzgx/attempts/fn_17_7728.1791070748.c` (attempt 21410, codex /
gpt-6.1-sol, 15 checks). Local compile reproduces 99.971565 under GC/1.3.2, GC/1.3 and GC/2.0.
**GC/1.2.5n scores 73.58** -- the 1.2.5n attempts were compiling under the wrong compiler.

r7 is genuinely live in our own build: it is used for the later `group->unk_0` loads at 0x290,
0x2A4, 0x2D4, 0x2E8 (`lwz r4, 0x0(r7)`), byte-identical to retail. Only the *first* load is
folded. This is an addressing-mode selection artifact, not a liveness problem.

Tried and failed (all still emit `0x80C40074` or worse):

    plain field           owner->unk_14                                  -> 0x80C40074
    u8 pointer + 0x14     *(Details **)((u8 *)owner + 0x14)               -> 0x80C40074
    u32 index             *(Details **)((u32 *)owner + 29)                -> 0x80C400D4
    trailing struct pad   adding `u32 unk_18;` to struct Group            -> no change
    Group* + 0x60 local   g = (Group *)((u8 *)owner + 0x60); return g->unk_14;
                                                                          -> 0xA9030044 (worse)
    separate extern sym   group = (Group *)&lbl_17_bss_60                 -> build shape breaks
    pragma barriers       opt_propagation / opt_loop_invariants /
                          opt_scheduling off around the group pointer     -> no change

Pointer-arithmetic variants that reach +0x74 all produce r4-relative loads. Variants that
materialize the +0x60 pointer independently destroy the surrounding code (differing rows jump
to 5 or 39).

---

## B. `fn_12_21C00` (movie_module, 0x130, 76 instructions, plateau 99.210526%, 65 attempts)

Single differing word at offset 0x24:

    target  7F FD FB 78   mr  r29, r31
    ours    3B A0 00 00   li  r29, 0

Best archived body: `.fzgx/checks/fn_12_21C00/000.c`.

Retail context, byte-verified via `dtk elf disasm`:

    /* 00021C14 00000054  3B E0 00 00 */   li  r31, 0x0
    /* 00021C20 00000060  93 A1 00 24 */   stw r29, 0x24(r1)
    /* 00021C24 00000064  7F FD FB 78 */   mr  r29, r31
    ...
    /* 00021C68 000000A8  7C 7F 1B 78 */   mr  r31, r3      <- r31 reassigned to call result
    /* 00021D14 00000154  7C 7F EA 14 */   add r3, r31, r29 <- return result + extra

r31 is `result` (initialized 0, later overwritten by the `read_count` vtable call); r29 is
`extra`. Retail emits a register *copy* where MWCC constant-folds to `li`. `opt_propagation
off` is already present in the body and does not prevent the fold.

Tried and failed (all still emit `0x3BA00000`):

    reference shape        result = 0; extra = result;              -> 0x3BA00000
    swapped order          extra = 0; result = 0;                   -> 0x3BA00000
    decl-init              int extra = result; result = 0;          -> build shape breaks
    address-of             extra = *(&result);                      -> 0x3BA00000
    reordered              extra = result after module computed     -> 0x3BA00000
    uninit extra           removing the early init                  -> 66 word diffs (worse)
    arithmetic wrappers    result * 1 / (int)result / +result /
                          result ^ 0                               -> 0x3BA00000
    asm barrier / &result  forcing opacity between the two          -> 0x3BA00000
    volatile qualifiers    on result, extra, or both                -> 58-72 word diffs (worse)
    call between them      fn_12_24950(&state); extra = result;     -> 72 word diffs (worse)
    compiler flags         -use_lmw_stmw on/off, -fp hardware, -char signed,
                          -fp_contract on, -common on, -sdata 0,
                          -align powerpc                           -> all 0x3BA00000

---

## C. `fn_12_72F4` (movie_module, 0xAC, 43 instructions, plateau 99.88372%, 17 attempts)

Single differing word at offset 0x24:

    target  7F 3E CB 78   mr  r30, r25
    ours    7F 7E DB 78   mr  r30, r27

Byte-verified against retail (`dtk elf disasm` on the retail object):

    /* 00007314 00000060  3B 79 00 44 */   addi r27, r25, 0x44
    /* 00007318 00000064  7F 3E CB 78 */   mr  r30, r25
    /* 0000731C 00000068  7F 5F D3 78 */   mr  r31, r26

Best archived body: `.fzgx/checks/fn_12_72F4/013.c`, flags `-use_lmw_stmw on`. A pure register
choice: retail copies r25 (the `movie` parameter), ours copies r27 (`&arg->... + 0x44`).

Tried and failed:

    reference shape        current = (u8 *)arg; current_2 = current;     -> 0x7F7EDB78
    direct init            current_2 = (u8 *)arg;                        -> 0x7F7EDB78

---

## Method note

All probes were read-only local compiles into `.fzgx/analysis/` (gitignored), with the fleet
off and no lease claimed. No ledger rows written. The build matched all 16 retail SHA1s
throughout.

The per-object oracle can be driven directly from Python:

    oracle.check(project, 'module:SYMBOL', source=<path>,
                 mw_version='GC/1.3.2', extra_cflags=['-use_lmw_stmw', 'on'])

and the exact differing word isolated by comparing `oracle.words()` on the retail object
against `.fzgx/work/<SYMBOL>.o`. That is how each residual above was reduced to one
instruction. `dtk elf disasm <retail.o> <out>` gives the byte-verified retail encoding.

**Two of the three residuals (B and C) are register-allocation choices, not source-shape
differences.** No source rewrite tested changes them, and the surrounding instructions are
byte-identical. These are candidates for a register-allocation flag or an accepted hand-edit,
not for another model session.
