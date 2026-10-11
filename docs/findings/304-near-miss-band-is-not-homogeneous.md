# The >=98% near-miss band is not homogeneous: classification and where to aim

Audit of every unmatched function at best_percent >= 98.0 whose body is present locally
(228 functions, fleet-held symbols excluded), re-checked with
`oracle.check(source=<saved body>, mw_version='GC/1.3.2')`.

## Headline: the oracle-bug class is now empty

**0 of 228** remaining candidates have byte-identical instruction words with a non-empty
defect set. Before finding 303 there were 3 (`fn_1_9DB04`, `fn_1_15EC40`, `fn_8_2124`).
That class is exhausted, not merely reduced. Any *new* near-miss claiming to be
"relocation-only" or "no source-owned differing rows" is therefore a fresh defect worth
investigating on its own terms, not a recurrence.

## Distribution

| differing rows | functions |
| ---: | ---: |
| 1 | 26 |
| 2 | 34 |
| 3-8 | 74 |
| 9-32 | 87 |
| 33+ | 7 |

60 of 228 (26%) sit within 2 instruction rows of a match.

## The 26 one-row functions, by actual cause

| class | count | nature |
| --- | ---: | --- |
| retail `mr rD, rS`, ours `li rD, 0x0` | 7 | genuine register-allocation tie-break (finding 301's class) |
| mnemonic differs | 7 | struct/type reconstruction, NOT allocation |
| offset and/or base register wrong | 5 | struct-offset error |
| operand order | 1 | source expression order |

### The mnemonic class is the actionable one

    fn_80043798   retail extrwi r9, r10, 8, 16   ours srawi r9, r10, 8
    fn_800446E4   retail extrwi r9, r10, 8, 16   ours srawi r9, r10, 8
    fn_1_135D7C   retail mr    r4, r3           ours extsb r4, r3
    movie:_prolog retail sth   r11, 0x0(r10)    ours stw   r11, 0x0(r10)
    fn_8008983C   retail addi  r3, r31, 0x0     ours mr    r3, r31

`extrwi` vs `srawi` is unsigned vs signed extraction: the C is producing the wrong
field type, so two functions share one root cause. `sth` vs `stw` is a 16-bit vs 32-bit
store, i.e. a `u16` field typed as `u32`. These are **type-reconstruction** defects, not
allocator floors — a single correct type change plausibly closes several at once.

### The offset class

    fn_17_7728   retail lwz r6, 0x14(r7)    ours lwz r6, 0x74(r4)
    fn_1_2D038   retail lbz r0, 0x5(r3)     ours lbz r0, 0x35(r31)
    fn_12_331A8  retail stw r3, 0x4(r7)     ours stw r3, 0x0(r5)
    fn_80067DE4  retail addi r3, r31, 0x0   ours addi r3, r31, 0x4

Wrong base register *and* wrong offset means the wrong struct is being indexed, not a
within-struct offset typo. `fzgx structmap` is the right tool.

### The genuine tie-break class (7)

    fn_12_72F4  retail mr r30, r25   ours mr r30, r27
    fn_1_4270C  retail mr r30, r29   ours li r30, 0x0
    fn_12_21C00 retail mr r29, r31   ours li r29, 0x0
    fn_1_1384C  retail mr r27, r30   ours li r27, 0x0
    fn_80077BD4 retail mr r30, r31   ours li r30, 0x0
    fn_12_2F730 retail mr r31, r0    ours li r31, 0x0
    fn_12_2F7AC retail mr r31, r6    ours li r31, 0x0

This is the only subset where "beyond the current automated tooling" is a defensible
claim. Note it is 7 functions, not 68.

## What this means for the residual

The "68 near-miss functions" figure conflates at least four distinct causes. Roughly a
third of the one-row set is a type or struct reconstruction error that ordinary
decompilation reasoning closes; roughly a quarter is a genuine allocator tie-break. Aim
the fleet at the mnemonic and offset classes first — they are real C work with a known
root cause, and one type fix can land several functions.
