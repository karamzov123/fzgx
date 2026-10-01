# 19: MTX library located at 0x8006D000–0x80080000; probe-carve of first 2 functions EXACT

Date: 2026-08-22. w7-mtxprobe subagent.

## Result

`dolphin/mtx/MTX.c` carved (2 funcs @0x8006E200–0x8006E324, 0x124 bytes,
nofralloc asm transcription, all words EXACT post-link): gate sha1 green
(421c8810...b271). Post-link word audit: 0 differing words across the
carved range. configure.py registered (`dolphin/mtx/MTX.c` after
PSMathFns.c); splits.txt split coarse/text_80069AE0.c at 0x8006E324 (new
coarse/text_8006E324.c).

## Finding 1: the probed 0x80041460–0x80050000 region is NOT MTX

It is game-side code: 0x80041460–0x80041700 is a run of 20+ small thunks
(0x8–0x70 B) that all dereference `*(r3+4)` and tail-call a vtable-like
function table at 0x800454B4–0x8004559C; 0x80041700–0x8004A560 is integer/
branch-heavy with bctrl dispatch. ZERO PS arithmetic or psq ops in the whole
range. The REAL MTX library is at **0x8006D000**:

- 0x8006D000: `mtspr 0x392..0x397` GQR0–GQR7 init (li/oris pairs) —
  PSMathFns-style QR setup, exactly the SDK MTXInit pattern
- 0x8006D044–0x8006D188: float NaN/Inf branch-trees (fabs/fmod helpers)
- 0x8006D088: frsqrte Newton-Raphson core (2 iterations)
- 0x8006D1C4+: dense op4 arithmetic + psq_l/st — PSMatrix families
- 0x8006E200 onward: clean small functions = PSMTXMultVec-type ops
- analyzer labels lbl_8006D0B4...lbl_8006E1C0 are internal branches of big
  fused functions, not functions (known dtk analysis warnings region)

## Finding 2: analyzer fragments shared-code MTX functions — merge before carving

fn_8006E250/E294/E2B0 (+lbl_8006E2D0) share tails via internal `b`
(0xE250→0xE270→0xE2D8 etc.). Carving them as separate symbols breaks;
they must be ONE function 0xE250–0xE324. Expect this pattern throughout
the MTX block (CW emitted fallthrough-shared variants).

## Finding 3: raw-word PS decoder additions to findings/18 table

- xo5=10 ps_sum0 / 11 ps_sum1 were missing from the findings/18 opcode list;
  they appear heavily in vec⟶scalar dot products.
- ps_sum0/ps_sum1 AND ps_madds0/1 use the SAME fused emission as madd:
  `{mn} fD, fA, fC, fB`. Verified by encode round-trip against raw DOL
  words (e.g. 0x10842114 → `ps_sum0 f4, f4, f4, f4`).
- CR-logic in this region: capstone prints `crset?\t2, 2, 2` for cror
  d,a,a (op19 XO289) and plain `crxor\t2, 2, 2` passes through fine.
- ps_mul here carries bit6=1 (xo10=57), i.e. the encoding is
  `op4|D|A|B|bit6|xo5<<1` — round-trip confirms `ps_mul f4,f4,f0` for
  word 0x10840072; don't "normalize" it to xo10=25 form.

## Traps

- dump_asm.py takes full vaddr (0x80041460), not offset.
- The generator must decode op4 from RAW WORDS even when capstone printed
  a plausible-looking mnemonic (vmhaddshs for ps_madds*, vsubeuqm for
  ps_nmadd) — mnemonic-based UNDEC filtering alone misses half the ops.
- fn sizes in symbols.txt for fragmented functions are wrong by design;
  derive carve ranges from branch structure, not symbol size fields.
