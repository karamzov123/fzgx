# Finding 301: the pure-regalloc near-misses are allocator tie-breaks no single source lever closes

Measured 2026-10-10 against `fn_12_2F970` (120B, 30 instructions, 1 differing row,
99.83%) and `fn_1_10161C` (99.23%, a clean r8<->r12 / r10<->r11 loop permutation).

## The shape

`fn_12_2F970` differs in exactly one instruction at the top of the loop:

```
  li r31, 0x0          ; the zero used by the stores (stw r31, ...)
> mr r30, r31          | li r30, 0x0     ; retail copies the live 0; ours materialises a fresh li
```

`r30` is the loop counter `i` (`addi r30,r30,1; cmpwi r30,3`; `mr r5,r30` passes it to
the callee). Retail initialises the induction variable by **copying a zero already
live in r31** (the same zero the stores use); MWCC's allocator coalesces the counter
onto that existing value. Our compile materialises an independent `li r30,0`, so the
counter gets its own register and the init is a `li` not an `mr`. Same program, same
semantics, different allocator tie-break.

## Twelve source levers tried, none closed it

On `fn_12_2F970`: rematerialise the offset accumulator (`off+=2` -> `i*2`); recompute
the loop pointer from `arg0` each iteration; counter-form pointer (`i*0x10`); copy the
parameter into a local; `while` instead of `for`; initialise `i` from a separate
`zero` variable; share one `z=0` across the stores and the counter; `#pragma
opt_loop_invariants off` (already present). Every one held at 98.00% / 1 differing row
or made it worse (95.8%, 93.7%). On `fn_1_10161C`, rematerialisation moved the
permutation (retail r9/r8, ours r12/r8) but did not close it.

This is the measured confirmation of REGISTER_REPAIR.md ("allocator fixed point, not a
source-shape problem") and finding 299 (not pragma-reachable): the divergence is a
register *colouring tie-break*, and the source spelling does not determine which of the
tied registers MWCC picks.

## The gap in the tooling

`mwconstraints.target_mapping` computes the register bijection and
`mwconstraints.constrain` builds a selection witness, but `declaration_projection`
only reorders *leading movable locals with no initializers*, preserving our own
colours. It cannot realise a permutation where one of our registers is bound to two
retail registers. `induction_indexing` already tries the counter<->stepped rematerialisation
(that was variant A) and does not reach it either.

## The population and the lever that would

68 landable >=98% small functions are this tractable permutation set (see
`fzgx shapecensus`). The one lever that decides which spelling MWCC colours a given
way is a **region-crossing match**: if a function matches under the Pal or Japan
retail build but not the US build, the matching region contains the source construct
(induction spelling, declaration order, or a flag) that produces retail's colouring.
Diffing the matching region's function against ours reads off the exact construct,
collapsing the search from "try all spellings" to "copy the one that works."

Until that evidence exists, these are classified as allocator tie-breaks: do not
re-dispatch the stochastic fleet at them (finding 299's stopping condition), and do
not convert them to asm units (the project goal is matching C). The regional versions
are the decisive input.
