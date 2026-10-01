# CW `entry` labels — stacking trap + extern requirement (savegpr.c, 2026-08-23)

Context: dolphin/msl/savegpr.c = __save_fpr/__restore_fpr/__save_gpr/__restore_gpr
(0x800797C0-0x800798F0). Retail calls land MID-STUB: bl _savegpr_25 targets
stub_start + 4*(n-14). ~90 such bls across the dol; all must resolve to exact
entry addresses.

## Trap F1: stacked `entry` directives collapse
18 consecutive `entry _xxx_NN` lines at the top of an asm fn body ALL bind to
the FIRST instruction of the body → every label gets the same offset in the .o.
The linker then resolves UND `_savegpr_25` refs to the stub START (first global
at that address) — link succeeds, gate goes red with ~90 bl-displacement diffs,
each retail-vs-gen delta = -(4*(n-14)).
FIX: interleave one instruction after each entry:
    entry _savegpr_14
        stfd    f14, -0x90(r11)
    entry _savegpr_15
        stfd    f15, -0x88(r11)
    ...

## Trap F2: `entry` labels need prior declarations under -lang=c
`entry _savefpr_14` errors "undefined identifier" unless declared first:
    extern void _savefpr_14(void);
(one decl per label, above the asm fns). OSInterruptMask.c's mid-fn entries
(_8000d8b8 etc.) worked WITHOUT decls because they are referenced by same-file
branch targets first; pure export-only labels need the decl.

## Method note
Verify stub regions via linked-dol slice AND check that gen bls targeting the
region match retail targets per-caller, not just byte-compare the stub itself
(stub bytes can be identical while all inbound displacements differ).
