# 286 — 26 of the >=99% band's 48 "relocation-only" rows are one function, and it is ONE WORD from matching

Date: 2026-10-04. Tools: `fzgx shapecensus`, `oracle.words`, `oracle.check`.

## How this was found

Finding 285 classified 48 of 395 differing rows as "relocation-only" -- identical mnemonic,
registers and immediates on both sides, carrying objdiff's `p` flag. That was correct as far
as it went, and it invited the obvious question: is `p` a *binding* problem (fixable, and the
rows were hiding real work) or an artifact?

**Grouping them by function answered it immediately:**

    fn_1_2D038   26 rows      <-- over half, in one function
    fn_1_53E40    5
    fn_800288C4   2
    fn_1_149E2C   2
    fn_17_3F0     2
    ... 11 more functions, 1 row each

A single function holding 26 of 48 is not a population of 48 problems. So the `p` rows were
aggregated noise, and the way to settle it is to stop reading objdiff's diff and compare the
**raw words**:

    retail words 315   ours 315
    differing word indices: [65]   ... total 1

    [ 65] retail 88030005   ours 881F0035

**`fn_1_2D038` is one word out of 315.** Decoded, both are `lbz`:

    retail:  lbz r0, 0x5(r3)      (rA=3,  d=0x05)
    ours:    lbz r0, 0x35(r31)    (rA=31, d=0x35)

That is the entire difference in the function. The ledger has it at 99.68% after 8 attempts,
and `sweep` had it in the band it converted 0 of.

## Why objdiff showed 26 rows for one differing word

Because objdiff's diff is row-**aligned**, not word-aligned. Once our code loses a register
at row 65, every subsequent row is "paired" against retail's and the float loads that follow
(`lfs f0, 0x8(r30)` and 20-odd identical pairs) are reported with a relocation flag -- the
rows are individually fine. **A row count from objdiff is not a defect count.** The word
comparison is the ground truth and it is one instruction.

This retro-corrects part of finding 285's own framing: the "12% relocation noise" is noise in
the *diff*, but it was also masking a genuinely close function. A census that counts diff rows
without checking word counts will rank this function as a 26-row problem instead of a 1-word
one.

## The residue, stated exactly

Retail around that instruction:

    lbz   r0, 0x1634(r31)
    cmplwi r0, 0x0
    bne   ...
    addi  r3, r31, 0x30        <- the hdr address, materialised into r3
    lbz   r0, 0x5(r3)          <- loaded through r3
    cmplwi r0, 0x2
    bne   ...
    bl    fn_1_F22D4

Our body evaluates `((struct fn_1_2D038_hdr *)p_rod)->unk_5 == 2` inline, and MWCC folds the
`addi` into the load, giving `0x35(r31)`. So the whole question is: **why does retail keep the
hdr address in r3 across both the load and the subsequent call?**

Tried, all producing an identical object or worse:

| variant | score |
|---|---:|
| baseline | 99.537 |
| `h->pad_4[1]` (same offset, different member spelling) | did not compile (1-byte pad overrun) |
| `*((u8 *)h + 5)` | 99.537 |
| `u8 *p5 = (u8 *)h + 5; *p5` | 99.537 |
| `*(u8 *)&h->pad_4[0] + 1` | 99.537 |
| `((struct fn_1_2D038_hdr *)(void *)p_rod)->unk_5` | 99.537 |
| `*((u8 *)(void *)p_rod + 5)` | 99.537 |
| `&p_bss->unk_30` spelled directly | 98.921 (worse) |

Seven spellings of the same offset, all identical. MWCC's folding is driven by the *address
provenance*, not the C spelling: as long as the only use is the one load, the `addi` is folded.
Retail's version needs `r3` to hold that address across **two** uses -- the load and the
argument to `fn_1_F22D4` -- and once the source expresses the second use, the address stops
being foldable. Note also that `fn_1_2D038_hdr_chk` exists in our body but is **never called**;
the live site is line 77, inside a `||` with `p_bss->unk_1634 != 0`.

So the hypothesis for a model session is specific and falsifiable: the header check in retail
is a **separate statement that passes the header pointer to `fn_1_F22D4`**, i.e. `r3` is the
argument register for that call, and the `lbz` reads through the argument. Our body inlines the
check into a boolean expression, which cannot produce that. That is a control-flow statement
(hoist the check, call it, branch on the result), not a rewrite of the load.

## Why this is the highest-value item in the band

One word. Everything else in the >=99% band is a bucket (285), but this one is a named
instruction with a named cause and a stated next experiment. It should be handed to a model
session with this finding rather than re-derived.

## The generalisable lesson

**Before ranking a near-miss by its diff-row count, count differing words.** objdiff's row
alignment inflates a single late divergence into many flagged rows, and its `p` flag makes them
look like binding problems. One `oracle.words` comparison separates "one instruction away" from
"structurally different" in one call, and that distinction is worth more than any candidate
generator: it says which of the two the work actually is.

## What that immediately produced: seven one-word functions

`fzgx shapecensus --words` ranks the band by true defect count. Of 41 functions it could
score, **seven differ by exactly ONE word**:

    symbol            diff  words  the single divergence
    fn_17_7728          1    211  w11  80C70014 r6,r7   / 80C40074 r6,r4
    fn_1_135D7C         1    254  w185 7C641B78 r3,r4  / 7C640774 r3,r4
    fn_1_2D038          1    315  w65  88030005 r0,r3  / 881F0035 r0,r31   (analysed above)
    fn_1_53E40          1    105  w69  C87F0060 r3,r31 / C87F0058 r3,r31
    fn_1_5D1B8          1    111  w32  EC0007F2 r0,r0  / EC1F0032 r0,r31
    fn_1_7FD7C          1    157  w14  579F103A r28,r31 / 579F143A r28,r31
    movie:_prolog      1    128  w87  B16A0000 r11,r10 / 916A0000 r11,r10

Six more sit at two words: `fn_10_A90C`, `fn_1_4068C`, `fn_1_466B0`, `fn_1_F7F48`,
`fn_3_1AE40`, `fn_800288C4`.

**This is the actionable list, and none of it was visible from the row census.** `fn_1_53E40`
showed 5 flagged rows and looked like a medium problem; it is one word in a 105-word function.
`fn_1_7FD7C` is one word. Conversely some functions with far fewer rows have dozens of
differing words and are genuinely structural.

A shared cause is visible in several of them, and it is *not* register allocation. Decoding the
single differing word in each (opcode, destination, base, displacement) gives a specific
diagnosis per function rather than a guess:

| symbol | words | the one divergence | what it is |
|---|---:|---|---|
| `fn_1_53E40` | 105 | `lfd f3, 0x60(r31)` vs `0x58(r31)` | one **f64 field 8 bytes later** than ours. 8 bytes is exactly one `f64` slot, so this is a struct-layout offset, not a register choice. `fzgx structmap` is the tool that measures which offsets retail touches. |
| `fn_1_7FD7C` | 157 | op 21 (`rlwinm`) same dest/base, mask bits differ | rotate/mask width. `fixup_evidence.optimizer_pragmas` and the `optimize off/on` pragmas are the existing lever. |
| `fn_17_7728` | 211 | op 32 (`lwz`), same dest, base `r7` vs `r4` | base register choice for one load. |
| `fn_1_135D7C` | 254 | op 39 (`stw`) `r28,r30` vs `r28,r31` | callee-saved store pair: ours saves r31, retail saves r30. One saved-register allocation. |
| `fn_1_5D1B8` | 111 | op 34 (`extsb`), `r0,r0` vs `r0,r31` | sign-extension source register. |
| `fn_1_466B0` | 81 | **two words swapped**: w67/w68 exchange `rlwinm.` and `addi` | an instruction *ordering* difference, not a value one -- and it is the only one here with no register disagreement at all. Scheduling. |
| `movie:_prolog` | 128 | op 44 (`b`) vs op 36 (`stw`) | retail branches where ours stores. The prologue/epilogue is a `blr` early-return in one and a store in the other; the likeliest cause is an early `return` in our source that retail expresses as a branch. |

Only `fn_1_466B0` and `movie:_prolog` are control-flow questions. The other five are single
operand fields — an offset, a mask, a base register, a store pair, an extension source — which
is the cheapest possible class of fix and the one `structmap`/`sweep`'s evidence families are
closest to. `fn_1_53E40` in particular should be tried with `fzgx structmap --body` before
anything else: one field being 8 bytes off is exactly the measurement that tool exists for.

This is why the word-count view matters beyond bookkeeping: it turned a bucket of 135 into a
list where **five of seven have a named single operand** to change.

## fn_1_53E40 tested: the layout is already right

`fzgx structmap fn_1_53E40 --body`, on the one-word `lfd f3, 0x60(r31)` vs `0x58(r31)`:

    STRUCTMAP COMPARE fn_1_53E40 (main_rel)  retail 105 words, ours 99.048%
      base registers differ (r4,r5,r29): offsets pooled, so a missing/extra pair may be one
        field reached through a differently-based pointer
      every field offset retail uses is present in ours

So the struct layout is **not** the defect: every offset retail touches already exists in our
body. The 8-byte gap is not a missing field, it is the same field reached through a different
base -- which is the tool's own hint. That turns the cheapest-looking item in the list from
"try a field offset" into "find which pointer retail keeps in r31 where we keep a different
one", and it rules out a family of guesses in one command.

Worth recording because it is the opposite of what the arithmetic suggested. 0x58 to 0x60 is
exactly one `f64` slot, so "our field is one slot early" was the obvious reading, and structmap
refutes it. Measuring beat reasoning -- the same result as the level scan and the size bands,
and the reason those two were cheap to settle.