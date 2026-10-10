# 297 -- two units sit exactly one instruction from matching

Both residuals below are fully characterized, byte-verified against retail, and reproducible
locally without a lease. Neither has been closed. The point of this finding is to record
*exactly* which source shapes were tried and failed, so the next session does not re-spend a
compile round on them.

Both bodies are archived attempts, not tree source (no `splits.txt` entry, no `units.json`
entry, no definition in `src/`). Closing either requires landing the registry entries first --
see findings 279 and 280.

---

## A. `fn_17_7728` (interview, 0x34C, 211 instructions, plateau 99.971565%, 21 attempts)

Single differing word at offset 0x2C:

    target  80 C7 00 14   lwz r6, 0x14(r7)
    ours    80 C4 00 74   lwz r6, 0x74(r4)

Both decode to opcode 32, rt=r6. **Same effective address** -- `addi r7, r4, 0x60` sits at
0x28, so r4+0x74 == r7+0x14. Retail reloads through the r7 base the preceding `addi`
materialized; MWCC folds the offset into an r4-relative load. Semantically identical, only
the addressing form differs.

Best archived body: `.fzgx/attempts/fn_17_7728.1791070748.c` (attempt 21410, codex /
gpt-6.1-sol, 15 checks). Local compile reproduces 99.971565 under GC/1.3.2, GC/1.3 and GC/2.0.
**GC/1.2.5n scores 73.58** -- the 1.2.5n attempts were compiling under the wrong compiler.

Tried and failed (all still emit `0x80C40074` or worse):

    plain field           owner->unk_14                                  -> 0x80C40074
    u8 pointer + 0x14     *(Details **)((u8 *)owner + 0x14)               -> 0x80C40074
    u32 index             *(Details **)((u32 *)owner + 29)                -> 0x80C400D4
    trailing struct pad   adding `u32 unk_18;` to struct Group            -> no change
    Group* + 0x60 local   g = (Group *)((u8 *)owner + 0x60); return g->unk_14;
                                                                          -> 0xA9030044 (worse)

Pointer-arithmetic variants that reach +0x74 all produce r4-relative loads. Variants that
materialize the +0x60 pointer independently destroy the surrounding code (differing rows jump
to 5 or 39).

Direction: MWCC picks the r7 form when the *caller* holds the +0x60 pointer as a live value
and the callee indexes it directly. The reference body computes
`struct Group *group = &state->group;` and passes it in -- backwards from what retail does.
Hoist the +0x60 pointer into a named local in `fn_17_7728` itself, not inside the inline
accessor.

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

r31 is `result` (initialized to 0, later overwritten by the `read_count` vtable call); r29 is
`extra`. Retail emits a register *copy* where MWCC constant-folds to `li`. `opt_propagation
off` is already present in the body and does not prevent the fold.

Tried and failed (all still emit `0x3BA00000`):

    reference shape        result = 0; extra = result;              -> 0x3BA00000
    swapped order          extra = 0; result = 0;                   -> 0x3BA00000
    decl-init              int extra = result; result = 0;          -> 0.0 (breaks build shape)
    address-of             extra = *(&result);                      -> 0x3BA00000
    reordered              extra = result after module computed     -> 0x3BA00000
    uninit extra           removing the early init                  -> 66 word diffs (worse)

Direction: the copy survives only if `result` is not visibly a constant at that point. A
barrier that MWCC respects (not `#pragma`, which is already present) between `result = 0` and
`extra = result` is the remaining untested lever.

---

## Method note

All probes above were read-only local compiles into `.fzgx/analysis/` (gitignored), with the
fleet off and no lease claimed. No ledger rows written, no `src/` changes, and the build still
matched all 16 retail SHA1s afterwards. The per-object oracle can be driven directly from
Python with `oracle.check(project, sym, source=<path>, mw_version=...)`, which is how these
residuals were isolated without touching the fleet.
