# Anonymous-pool reach must be measured through the pool base register

2026-10-10. `fn_1_9DB04` was recorded at 99.5% with the note "two tooling-owned
shared-pool base relocations; no source-owned differing rows remain", and finding
301 filed it under the allocator-tie-break residual alongside `fn_10_1B2F4` and
`fn_1_15EC40`. It was not an allocator problem. The body's instruction words are
byte-identical to retail; the oracle was rejecting the pool proof.

## Root cause

In `tools/fzgx/oracle.py::_pool_rows`, the anonymous-pool branch estimates how far
retail's pooled literal region extends by scanning every load/store row for a
displacement and taking the max. It did not check *which register* the displacement
was through. The stack frame is addressed through `r1` with displacements up to
`0xf4` (`lwz r0, 0xf4(r1)`, `lfd f31, 0xe0(r1)`), so the computed "reach" was the
stack-frame size, not the pool's extent. For `fn_1_9DB04` that gave 248 bytes where
the real pool reach through `r29` was 172.

The over-long window ran past retail's pool into the bytes our own unit appends after
the primed layout, `ours != retail` fired at byte 180, and the row was dropped as a
non-matching pool. Two `?` rows remained, `unit_fully_matches` returned "below 100%",
and the function was recorded as a residual.

## Fix

The pool base is the register the `addi rD, rS, sym@l` row materialises, i.e. the
**destination operand of the R_PPC_ADDR16_LO row that binds the same symbol** — not
anything appearing in the `lis ...@ha` row, and not a regex over the row text. Match
that LO row by `target_symbol == lrel['target_symbol']` (retail's index, not ours)
and read the first `opaque` arg of its `parts`. Then only count displacements through
that register when computing `reach`.

## Result

| function | before | after |
| --- | --- | --- |
| `fn_1_9DB04` | 99.5% unmatched | MATCH (pool), 100.0%, `lbl_1_rodata_41F0=initializer[172]` |
| `fn_1_15EC40` | 99.15% unmatched | MATCH (pool), 100.0% |
| `fn_8_2124` | 98.77% unmatched | MATCH (pool), 100.0% |

Project went 5755 -> 5758 matched. A 25-function random regression sample of
already-matched units shows 0 failures.

## Rule

When a near-miss is described as "no source-owned differing rows remain" and the
remaining flagged rows are all `?` or `p` relocations, treat it as an oracle or
plumbing defect first and prove the byte equality directly, before filing it under
any allocator, tie-break or register-allocation class. A "register bound to two
retail registers" description is not by itself evidence of an allocator floor.

Also: `shapecensus --words` crashed with `IndexError` on exactly these functions,
because `r[0]` is indexed when `differing == 0` leaves the rows list empty. The
flagged-row count now rides in the scored tuple.
