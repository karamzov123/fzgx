Starter notes on CodeWarrior for GameCube 1.x codegen at `-O4,p`. Grow this
file from what actually unblocked functions; keep each item one or two lines.

- Register allocation follows declaration order of locals and argument order.
  Reordering declarations moves registers; extra temporaries can force a spill
  or a different register.
- `int` vs `u8/s16` locals change sign/zero extension (`extsb`, `extsh`,
  `rlwinm ... 0xff`). Match the extension you see.
- `-inline auto`: small `static` functions in the same file are inlined into
  callers. If retail has the body inline, define the helper `static inline` in
  the same unit.
- Float compares: `fcmpu` + `cror` patterns come from `<=`/`>=`; a plain
  `fcmpo` from `<`/`>`. `frsp` appears when a double is narrowed to float:
  use `f32` literals (`1.0f`) to avoid double promotion.
- `switch` with dense cases produces a jump table (`.data`); sparse cases
  produce compare chains. A `default` placement can change the layout.
- Struct field access uses the field offset directly in `lwz r, off(base)`;
  when you see an offset, add the field at that offset in the struct.
- Loops: `bdnz` implies a counted loop the compiler could prove; `for (i = 0;
  i < n; i++)` with `n` loop-invariant. `mtctr` at loop entry.
- `stwu r1, -N(r1)` frame size N and `_savegpr_NN` tell how many non-volatile
  registers are live across calls: roughly how many values survive a call.
- String literals go to `.rodata` (`-str reuse` pools identical ones); float
  constants to `.rodata` in RELs (`-sdata2 0`) and `.sdata2` in the DOL.
- `lis/addi` (`@ha/@l`) is a normal address materialisation, not a constant:
  declare the symbol and take its address.
- A value loaded into `r4` (or `r5`...) right before a `bl`, while `r3` is left
  untouched, means the callee takes the caller's first argument as its own
  first parameter and the loaded value as the second: write
  `callee(arg0, table[idx])`, not `callee(table[idx])`. Register choice here is
  a signature question, never a compiler quirk.
- The same in reverse: if you need a parameter in `r4`, add the preceding
  parameter to the signature even when the function body ignores it.
