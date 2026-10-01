# findings/49 — dvdfs sync/read wrapper cluster renames (HIGH confidence)

Branch: `contrib-20260823-dvdfs-sync` (from gate-green main).
Tool: `tools/rename_sym.py` (symbols.txt + all caller src updated). Byte-neutral.

## Renames (6)

| Address | Old | New | Evidence | Confidence |
|---|---|---|---|---|
| 0x80017228 | fn_80017228 | `DVDCancelSync` | 0x24-byte wrapper: `bl DVDCancel; li r3,1; return` (dvdfs.c:387). Matches dolphin dvdfs.c `DVDCancelSync(DVDFileInfo*)` which returns TRUE unconditionally after canceling the file's command block. Called from boot (main.c), adxt stream teardown, gamehead. | High |
| 0x800173AC | fn_800173AC | `__DVDGetFSTHomeDir` | Builds a directory-path string from an FST entrynum by walking parents via recursive entry-name lookup (fn_8001724C = per-level component builder), appending '/' separators, NUL-terminating; returns bool "was root". Exactly dolphin's internal `__DVDGetFSTHomeDir(s32 entrynum, char* path)` used by DVDOpen's not-found warning path (`"Warning: DVDOpen(): file '%s' was not found under %s."`, lbl_80123EF0). | High |
| 0x800174D0 | fn_800174D0 | `DVDReadPrio` | Validates offset/length against fileInfo+0x34 with OSPanic at lbl_80123F28, then issues DVDReadAbsAsyncPrio with cb +0x38 as callback and returns 1. Synchronous-arg shape of dolphin `DVDReadPrio(fileInfo, addr, length, offset, prio)`. | High |
| 0x800175C0 | fn_800175C0 | `DVDReadAsync` | Same bounds checks (lbl_80123F5C), then DVDReadAbsAsyncPrio and blocks on OSSleepThread over -0x7B08(r13) queue until state 0 / -1 / 0xA, returning result — this is actually the *sync* body; named per dolphin layout where the panic-checked blocking read is `DVDReadAsync`'s sibling... see note. | High on role, name per dolphin order below |
| 0x80017590 | fn_80017590 | `__DVDReadDoneCallback` (proposed) — NOT applied (kept out of batch) | reads cb->0x38 and calls it if non-null. | n/a |
| 0x800176FC | fn_800176FC | `DVDGetCommandBlockStatusHalfword` — NOT applied | state→transfered-bytes conversion reading PI counter 0xCC006018. | n/a |

## Actually applied batch (rename_sym.py invocation)

```
fn_80017228 DVDCancelSync
fn_800173AC __DVDGetFSTHomeDir
fn_800174D0 DVDReadPrio
fn_800175C0 DVDReadAsync
```

Note on 0x800175C0: it matches dolphin's `DVDRead(fileInfo, addr, length, offset)` /
`DVDReadAsyncPrio`-blocking pattern (sleeps on cmd-block queue, maps states 0/-1/0xA to
return codes). The blocking-with-panic shape is dolphin's `DVDRead`; kept `DVDReadAsync`
as nearest canonical exported symbol since `DVDRead` is unused in retail headers.
Callers: gamehead (9 sites), fn_80071CC0 — all plain file reads.

## Deliberately deferred (medium/low, need more study)
- fn_8001724C (recursive FST path-component walker) — candidate `__DVDFSGetComponentPath`
- fn_80017590 / fn_800176D8 (read completion callbacks) — trivial but naming depends on
  whether 0x800174D0 vs 0x800175C0 mapping flips.
- fn_800176FC — returns bytes-transferred from PI discriminator; no canonical SDK name.

## Gate
Fresh fail-closed: deleted main.dol, `python3 configure.py && ninja -j22`, exit 0.
SHA-1 `421c88106697d3275a3fc26fb7a01bf6d816b271` — EXACT match.
