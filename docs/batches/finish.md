# The TU-finish pass, deterministic (2026-09-08)

## What was wrong

Blocks are matched one function at a time, and each block declares its own
view of the world: private externs, private typedefs, guessed prototypes.
Turning those into one translation unit means reconciling N independent
guesses. The first version of that reconciliation ran agents in rounds
(tidy, hoist, reflag, then revise agents on whatever still collided, then
again). It converged slowly (queue 271 → 165 → 120 → 58 → 49 → 43 over five
rounds, about $5) because the agents could not see the prologue they were
supposed to agree with, and because the tooling asked them to search a space
that is enumerable.

## What replaced it

`fzgx tu-finish --module M` (`tools/fzgx/reconcile.py`), no agents:

1. Private typedef names that collide across blocks, or with a header, are
   prefixed with the function's name. A rename never changes code.
2. The TU's headers join the prologue.
3. For every symbol declared privately, the candidates are the definition's
   own signature (when the TU defines it), then each variant the blocks
   wrote, most common first. All best-guess candidates go into the prologue
   at once and every block is verified once (compile + objdiff, memoised by
   the generated unit's hash). Only for a block that fails are the other
   candidates of the symbols it names tried, one symbol at a time, each
   re-verified on every block that names that symbol (including callers
   that never declared it and the defining block). A symbol no candidate
   satisfies stays private and is reported as contested.
4. A block that cannot compile under the prologue at all is stored
   self-contained and reported. That list is the only agent task the pass
   emits: one revise batch, with the prologue shown as "already in scope"
   and a PROLOGUE CONFLICT reported (and refused at submit) by every check.
   A block released twice is blocked, never re-queued.
5. The pass relinks with `ninja -k 0`; a failing unit is put back to its
   committed block (not its whole TU), and the hash is the oracle.

| pass over main.rel (51 TUs, ~700 blocks) | time |
|---|---|
| first version, serial, re-verifying everything | ~15 min |
| memoised, 12 workers, quadratic declaration search | 128 s |
| two-phase search, one verification per block | 24–40 s |

## Result on main.rel

| | before | after one pass + one agent batch |
|---|---|---|
| declarations hoisted into prologues | 0 | ~250 |
| colliding typedefs isolated | 0 | 111 |
| contested symbols (stay private) | – | 98 |
| blocks that need a rewrite | 271 (queue) | 21 |
| agent cost | ~$5 over five rounds | $0.53, once |

The tree links at 16 files OK after every pass; every pass commits.

## What the contested symbols are

Two valid views of one symbol: a pointer to a block-private struct where the
analyzer's header says `u32`; a prototype returning `void *` where callers
declared `void`; `char` versus `u8` strings. Both compile to the same bytes
in their own block, so per-function linking is unaffected; a whole-TU
compile needs one view. Choosing it is not computable from the bytes, which
is why the 21 blocks that cannot compile under their prologue go to an agent
once, and why contested symbols are reported rather than resolved.

## Rules that came out of it

- Agents only where the answer is not computable. Everything about declarations
  was computable once "still matches" became a memoised compile.
- The orchestrator picks a pool, runs a batch, runs the pass, reads the report.
- Never hand-edit blocks, headers or splits; never experiment on the live tree.
