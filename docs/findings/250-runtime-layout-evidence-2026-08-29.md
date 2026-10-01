# 250 — Authoritative runtime layout evidence for OSReset, DVDF, PAD, and CARD

Date: 2026-08-29

## Identity

The read-only observation server launched the verified GFZE01 ISO and bound every trace to:

- retail DOL SHA-1: `421c88106697d3275a3fc26fb7a01bf6d816b271`
- symbol map: `map-sha256:b74a0d117ed93d1db4babb2a8ca6f3e8c6de3e0058d1594e75358eac71fbbf58`
- canonical revision: `e0f9f5d0f68757df6b7028ac4ea85314e25c8c5c`
- game ID: `GFZE01`
- MEM1: 24 MiB
- source: bounded localhost reads only; no input or write API was used

Validated bundles and plans:

- `~/.cache/natc/runtime/layout-audit.jsonl`
- `~/.cache/natc/runtime/layout-audit.jsonl.manifest.json`
- `~/.cache/natc/runtime/layout-audit.normalized.json`
- `~/.cache/natc/runtime/layout-deref.jsonl`
- `~/.cache/natc/runtime/layout-deref.jsonl.manifest.json`
- `~/.cache/natc/runtime/layout-deref.normalized.json`

Both bundles pass `tools/natc_runtime_import.py::import_trace` with the identity above.

## OSReset

`ResetFunctionQueue` at `0x801A67D0` is exactly two pointers:

- head: `0x80123AE0`
- tail: `0x8012AA50`

Dereferenced nodes confirm the existing 16-byte layout:

| offset | field | head node | tail node |
|---:|---|---:|---:|
| `0x0` | callback | `0x8000E7AC` | `0x8002AB70` |
| `0x4` | priority | `0x0000007F` | `0x0000007F` |
| `0x8` | next | `0x8012AD30` | `0x00000000` |
| `0xC` | prev | `0x00000000` | `0x801245E0` |

This confirms `OSResetFunctionInfo { callback, priority, next, prev }` and queue `{ head, tail }`; workers should not spend more attempts varying this layout.

## DVDF/FST

At boot:

- `BootInfo` (`0x801A68A0`): `0x80000000`
- `FstStart` (`0x801A68A4`): `0x817E0680`
- `FstStringStart` (`0x801A68A8`): `0x817EE1F8`
- `MaxEntryNum` (`0x801A68AC`): `0x0000124A`

The root FST entry at `FstStart` is:

- word 0: `0x01000000` (directory bit plus 24-bit name offset)
- word 1: `0x00000000` (parent)
- word 2: `0x0000124A` (next entry / entry count)

Subsequent entries advance by 12 bytes, confirming the standard three-word FST entry layout. The dereferenced string table begins with valid NUL-delimited asset names (`bg`, `bg_big.gma.lz`, ...). Workers should use a 12-byte entry and 24-bit name offset, not pointer-sized entries.

## PAD

`__PADSpec` at `0x801A699C` is `0` in the verified booted state. The captured `0xB0`-byte SBSS window around the PAD globals is preserved in `layout-deref.jsonl`. This establishes the active spec value and adjacent state addresses, but one static boot capture does not by itself prove every per-channel structure field. Use the bundle to reject candidates that assume a nonzero active spec; retain static xrefs for remaining offsets.

## CARD

`__CARDBlock` at `0x80177960` contains two control blocks with a confirmed `0x110` stride. Channel 0 nonzero words include:

- `+0x004 = 0xFFFFFFFD`
- `+0x008 = 0x00800000`
- `+0x00C = 0x00002000`
- `+0x010 = 0x08000000`
- `+0x014 = 0x00000004`
- pointers at `+0x080`, `+0x084`, `+0x088`, `+0x0B4`, and `+0x0C0`
- `+0x108 = 0x00000080`
- `+0x10C = 0x801B7860`

Channel 1 starts at `0x80177A70`; its `+0x004` is also `0xFFFFFFFD`, and `+0x10C = 0x801B7840`. The observed second-channel base and matching terminal fields independently confirm the `0x110` control-block size used by retail `mulli ..., 0x110` sequences.

## Routing consequence

Treat these layouts as authoritative runtime evidence for the listed revision/identity. They resolve the OSReset queue, DVDF FST entry, and CARD block-stride questions. PAD has authoritative state evidence but still needs dynamic transition captures before claiming every adjacent field.
