# Naming study: OS region fns (fn_800035C0, fn_8000C49C, fn_80012678, fn_80017160, fn_80017228)

Disassembly via `tools/dump_asm.py` (capstone PPC) on `orig/GFZE01/sys/main.dol`; caller graphs from
`findings/26-fn-profiles.json`; cross-checked against dolphin OS/VI/PAD/DVD library structure.
MERGE-LOG's documented single-word diff in `__OSUnhandledException` was not chased.

---

## fn_80035C0 — **`memcpy`** (ascending byte copy)
**Confidence: high**

- Body at 0x800035C0: `addi r4,r4,-1 / addi r6,r3,-1`, then loop
  `lbzu r0,1(r4); stbu r1(r6)` until count exhausted → forward (ascending) byte copy,
  args `(dst r3, src r4, len r5)`. 0x20 bytes, no alignment handling.
- Immediately preceded by the word/half/byte-aligned tail of a memset-style routine and by a
  backward-copy loop (`lbzu -1`) — classic mem-family cluster in this region.
- All 9 callers are MetroTRK (`dolphin/metrotrk/*`, `dolphin/trk/nubevent.o`). Example
  fn_80088B44 calls it as `fn(dst=buf, src=&buffer->data[cmd->pos], n=min(4,remaining))` to copy
  message payload bytes — plain memcpy usage, never overlapping.
- Note: distinct from the optimized `memcpy` at ~0x800034xx (word-stores with alignment prologue);
  this is the small byte-loop variant. Could alternatively be named per its TRK role
  (`TRKCopyBuffer`-ish), but shape+args are unambiguously memcpy.

## fn_8000C49C — **`OSPanic(const char* file, int line, const char* msg, ...)`**
**Confidence: very high**

- Size 0x12C at 0x8000C49C; vararg spill frame identical to OSReport (stfd f1–f8 under cr1 flag),
  then `vprintf(msg, va)`.
- Formats `"in \"%s\" on line %d.\n"` with (file=r28, line=r30), prints it via OSReport;
  then prints `"\nAddress:      Back Chain    LR Save\n"` and walks the stack:
  `bl fn_8000BFC0` where fn_8000BFC0 is literally `mr r3,r1; blr`
  (**OSGetStackPointer**) — loops max 16 frames printing `0x%08x:   0x%08x    0x%08x`,
  stopping on back-chain 0 / 0x0001FFFF sentinel. Ends `bl 80009ffc` = infinite
  `sync; nop` loop (halt spin).
- Strings confirmed in DOL at 0x80122C30. Fan-in 14 across vi/pad/dvd/game — matches OSPanic's
  ubiquity. Already referenced from `vi_8001AFB8.c` (VIConfigure mode-change error path).

## fn_80012678 — **`OSSetWirelessID(s32 chan_and_id)`** (SI wireless-ID setter)
**Confidence: medium-high**

- Body: disables interrupts (bl OSDisableInterrupts @0x8000D4F4), reads shadow word at
  `lbl_80123B94+4`, `rlwinm r31,r31,8,0x18,0x1b` (rotate arg byte into bits 24–27 mask position),
  clears that field, stores shadow back AND writes `0xCC006430` (SI register space).
- Companion fn_800125DC is the getter/setter pair inverse: extracts `srwi 24` + masks
  `rlwinm 0x1c,0x1c,0x1f`, writes `0xCC006430` and pokes `0xCC006438 = 0x80000000` (SI write
  trigger). This is exactly the OS `OSSetWirelessID/OSSetWirelessID`-family SI bitfield access.
- pad_8001C01C.o already declares `extern void OSSetWirelessID(void);` and calls fn_80012678 right
  beside SIGetResponse/SISetCommand code paths — consistent with PADInit wiring wireless IDs.
- Naming: recommend `fn_80012678 → OSSetWirelessID` only if the existing named symbol of the same
  name resolves elsewhere; safer: `__OSSetWirelessID` / keep as SI helper
  `SISetWirelessIDBitfield`. Flag for symbol-table collision check before committing.

## fn_80017160 — **`DVDOpen(const char* fileName, DVDFileInfo* fileInfo)`**
**Confidence: high**

- Args `(r3=path, r4=out info)`. Calls fn_80016DF8 which is verbatim
  `DVDConvertPathToEntrynum`: parses leading '/', "./", "../", splits name/ext at '.'/' '/'\0'
  against the FST (entries via `-0x7B1C(r13)` root table, 12-byte entries, dir flag = top byte).
- On failure (entrynum < 0): builds 128-byte string via fn_800173AC (FST entry-name lookup chain →
  builds directory path string) and reports
  `"Warning: DVDOpen(): file '%s' was not found under %s.\n"` (string verified at 0x80123EF0);
  returns 0.
- On success: fills out struct fields +0x30 (entrynum), +0x34 (parent/dir field), zeroes +0x38 and
  +0xC; returns 1. Matches dolphin `DVDOpen` semantics exactly.
- Callers: main.o fn_80005858 (boot-time file open after OSSetStringTable — loads .map/.txt
  string-table file), game/adxt & gamehead modules — all open files by path. High confidence.

## fn_80017228 — **`DVDClose`-style sync wrapper → actually `DVDGetCurrentDiskID`-adjacent? No:
   `DVDCancelAsync`-free sync stub — best fit: `DVDCheckCancel`? Recommend `fn_80017228 =
   DVDWaitCoverClose`… see below.** 

Actually, on evidence: fn_80017228(36 bytes) = `bl fn_80019B78; li r3,1; return 1`.
- fn_80019B78 takes (r3=cb) and validates command-block state (+0xC state vs values 1,3,4,5,0xA,0xD,0xF)
  after bl 800198FC (the giant DVD command dispatcher with jump table at 0x801240A8 over states
  0..11) — i.e. it is `DVDCancel(command block, callback)`-family logic operating on an enqueued
  cancel request.
- Pattern `wrapper(arg) { DVDCancel(cb); return 1; }` called from boot (main fn_80005858 right
  after DVDOpen success), adxt streaming teardown, gamehead — fits **`DVDCancelSync`-like
  "cancel current command" convenience wrapper**. In dolphin libs the closest match is
  `DVDCancel(cb)` returning BOOL.
**Confidence: low-medium** — recommend naming `fn_80019B78 = DVDCancelAsync` first, then
fn_80017228 falls out as its thin sync/BOOL wrapper (`DVDCancel`).

---

## Summary table

| VA | Proposed name | Evidence | Confidence |
|---|---|---|---|
| fn_800035C0 | `memcpy` (small byte variant) | fwd byte-copy loop, (dst,src,len); MetroTRK buffer copies | High |
| fn_8000BFC0 | `OSGetStackPointer` | `mr r3,r1; blr` | Very high |
| fn_8000C49C | `OSPanic` | vprintf + `" in \"%s\" on line %d."` + 16-frame back-chain dump + halt spin | Very high |
| fn_80012678 | `OSSetWirelessID` (or `SISetWirelessID`) | SI shadow + 0xCC006430 bitfield write, interrupt-guarded | Medium-high |
| fn_80017160 | `DVDOpen` | FST path parse, exact `"Warning: DVDOpen(): file '%s' was not found under %s."` string | High |
| fn_80017228 | `DVDCancel` (sync wrapper over fn_80019B78=`DVDCancelAsync`) | cb-state validation + dispatcher; returns BOOL 1 | Low-medium |

Bonus identifications made while tracing: fn_80016DF8 = `DVDConvertPathToEntrynum`;
fn_800173AC = FST path-string builder (dvd `dvdfs.c` internal); 80009FFC = halt-spin loop;
0x8000D4F4/0x8000D51C = already-named OSDisableInterrupts/OSRestoreInterrupts.
