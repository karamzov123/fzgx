# 09: .init section (0x80003100–0x80005544) fully matched from C

Date: 2026-08-22. Session 7. Follows findings/08.

## Result

coarse/init_80003100.c replaced by 9 C units + 1 coarse asm blob; gate
sha1 421c8810...b271 green after clean rebuild, DOL 1414848 B.
- start.c: __check_pad3 NATURAL C EXACT pre-link; __set_debug_bba/
  __get_debug_bba natural C (sda21 relocs normalize at link); __start
  nofralloc-asm weak body (GX-specific: BBA check + InitMetroTRK_BBA vs
  melee SDK source). fuzzy 100% incl. relocs.
- __init_registers.c: asm body, linked bytes exact; literals hardcoded
  (_stack_addr=0x801B7930, _SDA2_BASE_=0x801AEE40, _SDA_BASE_=0x801AE3C0)
  → fuzzy 99.17%, only unit <100 (see blocker).
- __init_data.c: asm body w/ _rom_copy_info/_bss_init_info@ha/@l (real
  relocs, fuzzy 100%). Retail has double-b loop heads + no hoisting;
  natural C under GC/1.2.5n -O4,p gives single-b + hoisted lis + A/B
  prologue diffs → not worth chasing (same class as findings/06).
- __init_hardware.c, __flush_cache.c: asm bodies, EXACT/fuzzy100.
- fill_mem.c: memset+__fill_mem asm transcriptions (melee Runtime/__mem.c
  C shape did NOT reproduce under 1.2.5n: regalloc v→r7/d→r6/i→r3 differs).
- memcpy.c: NATURAL C (melee Runtime/__mem.c verbatim minus includes)
  EXACT pre-link, fuzzy 100%. r3 survives as return value untouched.
- trk_stubs.c: fn_80003590/fn_800035C0 asm bodies (A-prologue + regalloc).
- trk_reset.c: fn_80005518 asm body (A-prologue stwu-first).
- coarse/init_trk_800035E4.c: gTRKInterruptVectorTable data blob left as
  generated asm (contains `b fn_80005518` cross-object branch).

## Finding 1: mwcceppc inline asm rejects label@h but allows @ha/@l

`lis r1, _stack_addr@h` → "illegal use of label (_stack_addr), can only
use label difference in this context". `sym@ha`/`sym@l` compile fine
(melee __ppc_eabi_init precedent). Consequence: retail's ADDR16_HI/LO
(plain @h/@l) pairs are NOT expressible with relocs in inline asm — only
hardcoded literals or @ha (which changes encoding when bit15 of the low
half is set).

## Finding 2: __declspec(section ".init") places C code in .init splits

All functions in a .init-range unit need `__declspec(section ".init")`
(asm form: `asm __declspec(section ".init") void f(void)`). Weak works:
`__declspec(weak)` on __start compiles under GC/1.2.5n. Linker-defined
_stack_addr IS available to mwldeppc (ldscript.lcf), so a future MWCC
that emits HI relocs could match __init_registers at 100%.

## Finding 3: A-pattern prologues confirmed unreachable via natural C on 1.2.5n

Retail memset/fn_80003590/fn_80005518 all open `stwu r1,-X(r1); mflr r0`
(stwu-first). 1.2.5n always emitted mflr-first for them regardless of
source shape → asm bodies. Extends findings/06 (B-from-version) with the
converse observation.

## Blocker

__init_registers fuzzy 99.17%: expected object carries 6 relocs
(_stack_addr/_SDA2_BASE_/_SDA_BASE_ × HI/LO — dtk reconstructs them);
inline asm can't emit HI type (finding 1) and @ha would corrupt bytes of
the two SDA bases (low halves 0xEE40/0xE3C0 have bit15 set). Linked DOL
bytes are EXACT; unblock requires either an MWCC asm syntax for @h on
extern labels or assembling this one function via mwasmeppc.
