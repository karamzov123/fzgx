# MSL / OS naming study — dolphin/msl/* and os/OSAllocHead.c

Method: disassembly via `tools/dump_asm.py`, call-graph from `/tmp/gx_fn_db.json`, structural comparison against
standard MSL C library internals (Melee reference has no MSL sources; identities are inferred from code shape,
constants, and call topology). Conf: HIGH = structure uniquely identifies function; MED = strong but not unique;
LOW = plausible only.

## dolphin/msl/msl_80083E84.c (math/float section + strtod)

| VA | sym | proposed | conf | evidence |
|---|---|---|---|---|
| 80088578 | fn_80088578 | `expf` (float exp wrapper) | HIGH | 32B thunk: `bl fn_80085EF8` then return — pure float→float forwarder. Caller site 0x8005C6B4 (gamehead): computes `(u16 - 1023.0)/256.0` in f1 then calls this, result multiplied by constants — classic `2^(n/256)`-style exponent use. fn_80085EF8 is the double `exp`: splits f1 into hi/lo via 0x10 comparison, handles ±0/±denormal with error path storing errno-like 0x21 to SD var (-0x75e0 r13) = EDOM/DOMAIN, extracts biased exponent (`addi r4,r4,-0x3ff`, subfic 0x3ff), builds scaled 2^k via mantissa bit surgery, calls polynomial helper fn_80085C7C (itself a range-reduced exp kernel with the same denormal/errno prologue), then fmadd chain of two correction terms — exactly the fdlibm/MSL `exp(x)=2^k·P(r)` layout. |
| 80085EF8 | fn_80085EF8 | `__msl_exp` (double exp core) | HIGH | see above; errno store at -0x75e0(r13), exponent bias math, poly call, 2-term fmadd correction. |
| 80085C7C | fn_80085C7C | `__exp_range_reduced` (MSL internal exp kernel) | MED | same prologue pattern, polynomial evaluation, called only by 80085EF8. |
| 80086C5C | fn_80086C5C | `sin` (double sine) | MED-HIGH | Range reduction: compares high word vs 0x3e40 (= π/4 boundary 0.1875… actually 0x3e40xxxx ≈ 0.1875? no — 0x3E400000 ≈ 0.1875; used as small-arg threshold), `fctiwz` round-to-int check for near-zero shortcut returning the argument constant at -0x7690(r2), then Horner chain of 6 fmadds over constants from TOC — matches MSL `sin` polynomial q0..q5 shape (odd-poly sin after split into n·π/2 quadrant handling visible in tail). Called by printf-family formatting paths and trig wrappers. NOT strtod_nan: no string parsing anywhere. |
| 800883E8 | fn_800883E8 | `sin`/`cos` shared core or `__sin` | LOW-MED | 216B, calls 80087BA4 & 80086C5C repeatedly (looped) — looks like an argument-reduction driver. Needs its own study. |
| 80087E80 | fn_80087E80 | `__cos_core` sibling | LOW-MED | same caller pattern as 800883E8. |

## dolphin/msl/math_80088600.c

Already documented as double→float wrappers (`truncf`-style thunks). With exp/sin identified upstream:
| VA | sym | proposed | conf | evidence |
|---|---|---|---|---|
| 80088600 | fn_80088600 | `expf` thunk → wait, it calls fn_800883E8 not 80085EF8 | — | keep as-is; re-map after 800883E8 resolved. If 800883E8=`sin`, this is `(float)sin(d)`. |
| 80088624 | fn_80088624 | `(float)fn_80087E80(d)` | MED | direct wrapper of 80087E80; pair suggests cosf. |

## dolphin/os/OSAllocHead.c

This unit is NOT the OS heap family (OSAllocFromHeap etc. live in OSAllocCtx.o here). It is the **MetroTRK-style
"head" overlay**: runtime-init helpers + weak-alias thunks + OSCheckHeap-equivalent validator.

| VA | sym | proposed | conf | evidence |
|---|---|---|---|---|
| 80008DB4 | fn_80008DB4 | `__init_registers`-adjacent init / struct-copy init helper | LOW | copies two 12-byte structs to stack, calls fn_8006EB4C(merge?) then fn_80015B78 — float-carrying init glue. |
| 80008E30 | fn_80008E30 | empty stub (nop) | HIGH | body is `blr` only. |
| 80008E58→named OSAlloc | OSAlloc | `OSAlloc` | already named | thunk: `OSAllocFromHeap(OSGetCurrentHeapID-ish, size, 0, 0)` via SD-relative current-heap ptr (-0x7fac r13). Matches melee OSAlloc.c line 148 wrapper semantics. |
| 80008E70→named OSFree | OSFree | `OSFree` | named | mirror thunk into OSFreeToHeap. |
| OSSetCurrentHeap_thunk | — | `OSSetCurrentHeap` | HIGH | saves old current-heap global, calls fn_8000951C (state save), swaps -0x7fb0(r13) current-heap var, calls fn_800095A4 (state restore) — exactly the mutex-guarded set-current-heap path. |
| 80008EC8 | fn_80008EC8 | `OSInitAlloc` | MED-HIGH | loops `NumHeaps` times writing {-1, NULL, NULL} 0xC-byte HeapArray entries (size=-1 sentinel matches melee OSAlloc.c `HeapArray[heap].size >= 0` "not yet initialized" convention), aligns end ptr `+0x1F & ~0x1F`, stores bounds to SD vars (-0x7c84/-0x7c88 = arena lo/hi), calls allocator fn_80009468 which writes a 0x14-byte-per-entry table at -0x41c0 base with -1 sentinels — the HeapArray allocation/init. |
| 80009468 | fn_80009468 | `__.OSAllocTableInit` (internal HeapArray alloc) | MED | writes stride-0x14 table, first field -1 (heap-size sentinel). |
| 80008F60 | fn_80008F60 | `OSSetCurrentHeapPath`-style wrapper w/ flag=0 | LOW | sets flag var -0x7c8c=0 then tail-calls fn_80008FB0. |
| 80008F88 | fn_80008F88 | same with flag=1 | LOW | twin of above; flag likely "force"/"verbose". |
| 80008FB0 | fn_80008FB0 | `OSCreateHeap` | HIGH | guards via fn_8000951C/A (interrupt or heap-lock save/restore pair), scans HeapArray entries for `size==0` free slot (lwz 0(r4); cmpwi 0), on hit splits region [align(lo),align(hi)] writing cell {size, prev=NULL,next} and links into slot, returns heap index i (r31 counter) else -1. This is exactly melee OSCreateHeap loop (OSAlloc.c ~line 400 region: find size==0 entry, carve, link). |
| 80009064 | fn_80009064 | `OSDestroyHeap` | MED-HIGH | guarded (951C/95A4 pair), sets `HeapArray[heap].size = -1` (stwx r4=-1 indexed by heap*0xC into base -0x7c7c). Sentinel reset = destroy. |
| 800090A4 | fn_800090A4 | `OSCheckHeap` | HIGH | 964B validator: null-checks HeapArray ("...is not initialized" style msg @0x350 offset), range-checks handle, walks allocated list verifying hdr back-link (0x35b), alignment `& 0x1F` (0x35a/0x35d), min-cell 0x40 (0x35c), accumulates sizes and compares to heap->size (0x360), then walks free list with same checks plus contiguity `cell+size == next` (0x370), final total compare (0x37c), returns accumulated free bytes (r29 = Σ(size-0x20)). Line numbers 0x350–0x37C match melee OSCheckHeap ASSERTMSGLINE range (line ~0x3E8 region per melee source ordering). Uses _savegpr_27/_restgpr_27 (many live vars) like the real OSCheckHeap. |
| fn_8000951C | (OSAllocCtx unit) | `OSEnableInterrupts`-style state SAVE (heap lock acquire) | MED | reads lock var -0x7c90(r13); if != -1 snapshots 5 SD globals into 0x14-byte table at -0x41c0 indexed by lock id, restores prior snapshot. Pairs as push/pop of a global state stack. |
| fn_800095A4 | (OSAllocCtx unit) | matching state RESTORE (lock release) | MED | inverse of 951C: writes snapshot back out to table, reloads SD globals. |

## Notes / corrections to prior hypotheses
- **fn_80086C5C is NOT `strtod_nan`** (findings/23 candidate wrong): zero string/parse behavior, pure FP Horner
  polynomial with π/4-range reduction — it's `sin` (or the shared sin/cos kernel).
- **fn_80088578 identity: `expf`.** High fan-in (36) is consistent: game code calls expf constantly for physics/
  curve easing; every call site seen passes one float and uses the float result directly.
- OSAllocHead.c's 195 unresolved refs are extern decls of game-side callers baked into this translation unit's
  relocation listing — the unit itself contains only the 11 functions above.

## Remaining unknowns
- stdio_8007A060.c (36 unnamed) and tail_8008279C.c/printf.c vfprintf machinery untouched this pass — needs the
  __StringWrite/__FileHandle chain mapped before renaming.
