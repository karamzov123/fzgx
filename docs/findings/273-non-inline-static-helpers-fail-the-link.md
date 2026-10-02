# 273 — every stranded 100% function closed; the cause was an inline transform that never ran

Date: 2026-10-02. Follows 272 (433cc19f). Matches: e9a812e4, 44f88d9c.

## Result

Unmatched functions at >=100%: **5 -> 0**. All five are now `matched` / `link_state=verified`:

| Function | Commit | Cause |
|---|---|---|
| `fn_1_207DC` | 16f2892e | reached the corpus once visible |
| `fn_1_21950` | b0ef3c3a | reached the corpus once visible |
| `fn_1_58248` | 24909e08 | `source_lint` (see 272) |
| `fn_1_41CB8` | e9a812e4 | non-inline `static` helper |
| `fn_1_D7C44` | 44f88d9c | non-inline `static` helper |

## The link failure: an out-of-line helper that retail does not have

`why-link` on `fn_1_41CB8` (the diagnostic that 433cc19f made reachable) isolated it:
`.text` was **+116 bytes** while every data section was exact, and the excess shifted
everything after it (`text_diffs` listed 12 downstream functions, `other_diffs` 413264).

Measuring the object rather than guessing which unit was responsible:

    .text size = 496
      fn_1_41CB8        size=380   <- exact
      fn_1_41CB8_walk   size=116   <- invented; not in symbols.txt

Retail's `fn_1_41CB8` is 106 instructions with **no call** to a walk helper: the linked-list
walk is an unrolled loop inline in the body. So the helper must be inlined:

    static       void fn_1_41CB8_walk(...)   -> .text 496, link fails
    static inline void fn_1_41CB8_walk(...)   -> .text 380, matches

`fn_1_D7C44` was the same shape: `.text 716 = 688 (fn_1_D7C44) + 28 (scale_to_byte)`, and
`scale_to_byte` is absent from `symbols.txt`. The file already declared a neighbouring
helper `static inline`, so the convention was local. Its preserved body was also missing
`#include "types.h"` and `rel/main_rel/globals.h` (for `Obj_1_bss_38458_Target`, already
declared in the header) — the recarve path supplies TU context, a standalone work copy does
not, so both had to be added before it would compile.

## The engine already had the transform

`source.inline_helpers` exists and its docstring names this exact failure: *"A REL retains
these copies even when the target function inlined every call. They shift its text and
every dependent relocation despite an exact function diff."* It is gated on `score == 100`
(fixup.py:427), and both bodies score 100% **at the object** — verified by rebuilding the
pre-fix bodies:

    fn_1_41CB8 pre-fix: base.matched=True pct=100.0 -> try_fix: matched=True
                        label='inline helper at 828'
    fn_1_D7C44 pre-fix: base.matched=True pct=100.0 -> try_fix: matched=True
                        label='inline helper at 4240'

So no new transform was needed. The engine could always have repaired both bodies; it never
ran because `verify` had quarantined them (`best_body_path` NULL), so `release()` and the
fixup corpus had nothing to hand it. **433cc19f is the fix** — visibility, not capability.

Worth stating plainly, because it is the trap here: **the object oracle reports
`matched=True, percent=100.0` for a body that cannot link.** Object equality says nothing
about .text size, and every downstream byte shifts from one extra function. Only the
16-target hash catches it.

## Method notes

- Diagnose by measuring section sizes and symbol tables, not by reading the diff. The
  `text_diffs` list is a symptom of an overflow somewhere earlier, never the cause.
- Prove the fix is load-bearing: rebuild the pre-fix body and confirm the transform fires,
  so the hand edit is known to match what the engine would have done.
- A preserved link-fail body may need `#include "types.h"` plus its module `globals.h`
  before it compiles outside a TU.