# 245 — OSReset `ResetFunctionQueue` / `OSResetFunctionInfo` struct reconstruction

Date: 2026-08-28
Worker: hard (paid model — type reconstruction, no lease volume)
Unit: `main/dolphin/os/OSReset`  (compiler pin: GC/1.2.5n / mwcc_233_163n)
Reusable by: all OSReset family functions (`OSRegisterResetFunction`,
`OSResetSystem`, `__OSDoHotReset`) and any worker touching the reset-queue ABI.

## Root cause of the OSRegisterResetFunction plateau (66.9% -> 98.03%)

The canonical `src/dolphin/os/OSReset.c` declares the queue as:

    extern unsigned char ResetFunctionQueue_801A67D0[8];

That is the **T1/T3/T14 sda21 declaration-shape trap**. The symbol is an 8-byte
`.sbss` object, but it is NOT a byte array — it is a 2-pointer queue header.
Treating it as `char[8]` makes every `ResetFunctionQueue.head` / `.tail` access
index at `0(r5)` and `4(r5)` as if the queue were a bare list head, which
mis-routes the `lwzu r4,4(r5)` / `stw r3,0(r5)`/`stw r4,0xc(r3)` sequence and
drops the byte score to ~67%.

## Verified layout (derived from retail disasm of OSRegisterResetFunction,
132 B, R_PPC_EMB_SDA21 x1 on ResetFunctionQueue_801A67D0)

    struct OSResetFunctionInfo {
        s32  (*func)(s32);              // +0x00  (retail: lwz r4,4(r3) compares info->priority)
        s32   priority;                 // +0x04  (compared with q->priority in the scan loop)
        struct OSResetFunctionInfo* next; // +0x08  (stw r3,8(r4) / lwz r5,8(r5))
        struct OSResetFunctionInfo* prev; // +0x0C  (stw r4,0xc(r3) / lwz r4,0xc(r5))
    };
    struct OSResetFunctionQueue {
        struct OSResetFunctionInfo* head; // +0x00
        struct OSResetFunctionInfo* tail; // +0x04
    };
    extern struct OSResetFunctionQueue ResetFunctionQueue_801A67D0;

Evidence chain:
- Scan loop reads `q = head`, then `q = q->next` (off +0x08) while
  `q->priority >= info->priority` (off +0x04).
- Empty case: `t = tail` (+0x04); if NULL set `head = info`; else `t->next = info`.
  Then `info->prev = t` (+0x0C), `info->next = NULL` (+0x08), `tail = info`.
- Non-empty case: `info->next = q` (+0x08); `p = q->prev` (+0x0C); `q->prev = info`;
  `info->prev = p`; if `p == NULL` `head = info` else `p->next = info`.
  This is exactly `ENQUEUE_INFO_PRIO(info, &ResetFunctionQueue)`: priority-ordered
  insertion into a doubly-linked list with head/tail sentinels.

## Provenance (do NOT paste reference verbatim — SDK revisions differ)

Reference body (dolsdk2001:src/os/OSReset.c:65):

    void OSRegisterResetFunction(struct OSResetFunctionInfo* info) {
        ASSERTLINE(0x76, info->func);
        ENQUEUE_INFO_PRIO(info, &ResetFunctionQueue);
    }

Adapted to natural C (no macro, no ASSERTLINE — `-DNDEBUG=1` compiles asserts out):
see candidate at `~/.cache/natc/scratch/hard/OSReset_OSRegisterResetFunction.c`.

## Authoritative compile result

    natc_compile.py --unit main/dolphin/os/OSReset --symbol OSRegisterResetFunction \
        --src ~/.cache/natc/scratch/hard/OSRegisterResetFunction.c --json
    => ok=true, compiler=GC/1.2.5n, diff_score=98.03, fn size=0x84 (132 B, exact)

Prior best was 66.9% under the `char[8]` declaration. The +31pt swing is
wholly attributable to the corrected struct type — same algorithm, different
declaration class (T1). Residual ~2% is instruction scheduling in the insert
tail (`OSRegisterResetFunction.c` already C89 block-top locals; no NULL literal,
since `-nodefaults`).

## Action for integ / other workers

- Replace `extern unsigned char ResetFunctionQueue_801A67D0[8];` in
  `src/dolphin/os/OSReset.c` with the struct pairs above before landing any
  OSReset conversion. This unblocks `OSResetSystem`'s two `CallResetFunctions`
  loops (which walk `8(r27)`/`8(r28)`) and the queue-insert in sibling units.
- Node field offsets are shared with any other `ENQUEUE_INFO_PRIO`-style queue
  in the OS family; reuse verbatim.

## Status

Not yet registered as a batch (this worker produces information; integ runs the
gate). Candidate source SHA recorded by natc_compile: 95897f0d7688506c.
One bounded evidence-producing action per lease turn: DONE. The remaining ~2%
is a scheduling residual, not a class error — leave the lease on this symbol;
a free or integ worker can close the last two points after the struct lands
in canonical src/.
