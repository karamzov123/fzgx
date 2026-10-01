# MSL/runtime naming study — fn_8008023C, fn_80083D6C, fn_80083DB0, fn_80088EB0, fn_8008A754, fn_8008D398

Method: disassembly via `tools/dump_asm.py` (with /tmp/fzvenv python, capstone), call-graph from
`/tmp/gx_fn_db.json`, comparison against standard MSL C library internals and melee's
`src/MSL/*` reference. Existing names verified against `config/GFZE01/symbols.txt`.

## Summary table

| VA | current | proposed | conf |
|---|---|---|---|
| 8008023C | fn_8008023C | `strncmp` | HIGH |
| 80083D6C | fn_80083D6C | `strncpy` | HIGH |
| 80083DB0 | fn_80083DB0 | `strcpy` (MSL aligned fast-path form) | HIGH |
| 80088EB0 | TRKAppendBuffer | keep `TRKAppendBuffer` (already named; identity confirmed) | — |
| 8008A754 | fn_8008A754 | `TRKGetTargetClock`-style accessor → propose `TRKTargetGetClock`... see note; safest: `TRK_GetSWTValue` unknown — recommend `__TRK_uart_msr_get` NO. Final: leave unnamed pending TRK unit mapping; structural name `TRKReadGlobal` LOW | LOW |

Correction: 8008A754 is a 2-instruction global getter (`lwz r3, 0x801A50B0`). Its setter twin
fn_8008A748 (`stw r3, 0x801A50B0`) sits immediately before it. Given callers are all EXI/USB
mouse/keyboard driver functions in the same region (fn_8008D7CC..fn_8008DC10) plus one TRK-side
caller, the variable is most plausibly the mouse/EXI interrupt-enable or device-presence flag.
Propose `TRKGetInterruptTimer`? — no unique match. **Recommend naming only after the surrounding
USB/EXI unit study; do not rename now.**

| VA | current | proposed | conf |
|---|---|---|---|
| 8008D398 | fn_8008D398 | `TRKDoWrite` (target→host write via function-pointer dispatch at 0x8015B8D8) | MED-HIGH |

## Evidence

### fn_8008023C = `strncmp` — HIGH
0x4C bytes. r6=src-1, r7=dst... precisely: compares byte-by-byte with lbzu pairs, early-out on
equality (`beq`), returns 1 / -1 / 0 via `bgelr`+`li r3,-1`, loop counter = n (r5+1 addic. chain).
Call site 0x80042588: `li r5,4; lis/addi r4,"string literal"; bl fn_8008023C` then branches on
result==0 — classic fixed-length tag comparison. Fan-in 14 across game code matches strncmp usage.
Layout identical to MSL `strncmp` (dolphin msl string.c): unsigned compare (cmplw), n-limited.

### fn_80083D6C = `strncpy` — HIGH
r4=n-1 (count), copies with lbzu/stbu until NUL, then pads destination with zero bytes for the
remaining count (`li r0,0; stbu r0,1(r6)` loop over remaining r5). This pad-to-n behavior is the
defining signature of `strncpy` vs plain copy. scope:global already set in symbols.txt. Sits between
other string.c fns (strlen right after at 80083E68).

### fn_80083DB0 = `strcpy` — HIGH
MSL's optimized strcpy: aligns dst/src low-2-bits equality check (`clrlwi ...,0x1e` pair + cmplw),
byte-copies prologue to alignment, then word-at-a-time copy loop using the classic
`(x - 0x01010101) & ~x & 0x80808080` NUL-detector (lis -0x7f7f / addi -0x7f80 build 0x80808080;
addis/addi -0x0101 build 0xFEFEFEFF), tail byte loop. Exactly MSL/dolphin strcpy structure.
scope:global already set.

### TRKAppendBuffer (80088EB0) — already named, confirmed
Byte-append into buffer struct {…, +0x08: length, +0xC: capacity-ish}: if used==0x880 clamps and
returns 0x301 (buffer-full error code), else stores byte at base+used+0x10, bumps +0xC and +0x08.
All 8 callers are TRK message handlers (fn_800894FC..fn_8008B8B4). Name is correct as-is.

### fn_8008A754 — global getter, LOW confidence
2 instructions: `lis/addi r3, 0x801A50B0; lwz r3,0(r3); blr`. Twin setter fn_8008A748.
Callers: fn_8008963C (TRK message path) and 7× USB/EXI gamepad-driver fns. Cannot uniquely name;
defer.

### fn_8008D398 = TRK host-write dispatcher — MED-HIGH
Calls function pointer loaded from global table at **0x8015B8D8** (+0x14) via bctrl, then folds
result to 0/-1 with `neg/or/srawi 31` (= `(ret<=0) ? -1 : 0` pattern: actually
`r3 = -( (neg|r) >> 31 )` → returns -1 if ret<=0 else 0). All 13 callers are TRK nub/message
functions (TRKMessageSend fn_80088B00 is caller #1). Shape matches Metrowerks MetroTRK
`TRKDoWrite`/`MessageWrite` style target→host I/O shim that goes through the board's
custom-function table. Propose `TRKDoWrite` MED-HIGH; alternative `TRKMessageBufferedWrite`.
Needs cross-check against dolphin `trk/` unit names before committing.

## Corrections to prior findings
None contradicted; expf/sin conclusions in findings/29 hold (symbols.txt now has `expf` named
at 80088578, consistent).
