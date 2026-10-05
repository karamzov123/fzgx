# 284 — a triage record for colchg_menu_disp: the cap was correct, and here is why

Date: 2026-10-04. Tools: `fzgx sweep` (31 candidates), `oracle.check` directly.

## Why this is written down

`colchg_menu_disp` (car_colchg, 172 B) is the closest function to closure anywhere in the
project: **94.74%** on the ledger, 22 attempts, and its module needs only 4 functions to be
complete. It is also `attempt cap 3 reached; needs triage` -- refused by `claim`. That is the
cap working as designed, not a bug: the docstring in `state/blocked.md` is explicit that
"unblocking any of these needs triage judgement, not another blind attempt."

So this is the triage. The short answer: **the cap was right, and the blocker is a register
liveness question the repair engine cannot reach.**

## The saved body and what it scores

Best saved body: `.fzgx/attempts/colchg_menu_disp.1790927643.c`, **90.093%**. Structure is
complete and correct: two calls, a byte compare, and a single-case switch over a loop.

The diff is one thing, repeated:

    ? 0054  extsb r5, r29        | extsb r3, r29
    ? 0058  cmpwi r5, 0x73       | cmpwi r3, 0x73
    ? 0068  addi r3, r30, 0xe8   | addi r4, r30, 0x98
    < 006C  addi r4, r30, 0x98   |
    ? 0070  cmpw r0, r5          | cmpw r0, r3

Retail sign-extends the loop counter into **r5**; our body puts it in **r3**, which shifts the
two `addi` by one slot.

## The cause, from the disassembly

Retail uses r5 twice, and that is the whole problem:

    00001174: lis r5, lbl_9_data_0@ha      <- r5 holds a data address
    00001188: addi r30, r5, lbl_9_data_0@l <- r30 is derived from it
    000011A4: li r29, 0x72                 <- loop counter in r29
    000011B0: extsb r5, r29                <- r5 REUSED for the switch test

So r5 is live across the whole loop body and only becomes the switch discriminant at the top of
it. Two further sign-extensions exist and must both be reproduced: `extsb r5, r29` at 0x1B0
(the switch test) and `extsb r0, r29` at 0x1E4 (the loop condition, `cmpwi r0, 0x79`). A
single `s8` counter used for both does not produce two `extsb`s.

## What was tried, with the scores

| body | idea | score |
|---|---|---:|
| saved best | one `s8 i`, `switch(i)` | **90.093** |
| separate `s8 k` for the switch test | two truncations | 90.093 |
| `s32` counter + `(s8)i` casts | r29 stays a full int | 90.093 |
| `do/while` with two `extsb`s | reproduce both | 87.767 |
| `u32 *baseu = &lbl_9_data_0` | keep r5 live across the loop | 0.000 (did not build) |

Four structurally different sources converge on **exactly 90.093025** with a byte-identical
diff. That is not the harness being insensitive: an empty body scores 2.326 and the `do/while`
variant scores 87.767 through the same path, so the compile-and-diff does discriminate.

The convergence is the finding. MWCC assigns the `extsb` result to r3 because nothing in the
source makes r5 the better home for it at that point; reaching r5 requires the register to be
*already occupied* at that instruction by the address `lis r5` established, which is a register
allocation and scheduling decision rather than a source-shape one. The last row is the honest
attempt at forcing it by keeping an explicit address in a live local, and it does not compile
at all.

## What this means for the 461 unsearched near-misses

`unsearched_near_misses` will hand this function to the deterministic search, and
`fzgx sweep colchg_menu_disp --min-percent 90 --rounds 4` did exactly that: **31 candidates,
0 improved, 0 matched**. This is the same result as finding 282 at one-function scale, and it
is the expected outcome for this class.

So the honest disposition: **the ≥90% band is not short of search, it is short of a source
shape that reproduces MWCC's register choice.** A model session reading this diff has a real
hypothesis to work with -- keep an address in a live local across the loop so r5 is occupied
at the `extsb` -- but `sweep`'s families do not generate that shape, and no amount of
re-running them will. Recorded here so the next attempt starts from the r5-liveness question
rather than from the diff.

## Cost and state

Four compiles, no tree change, `git status` clean. `16 files OK` unaffected. The saved best
body remains the best body; nothing was made worse, and nothing was claimed or submitted.