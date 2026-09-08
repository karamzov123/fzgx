# camera.c end to end (2026-09-08)

First translation unit taken through the whole readability pipeline: match by
TU, recover layouts, rewrite the matched units against the typed header, then
name functions, globals and typedefs from the coherent result.

## State of the TU

| | |
|---|---|
| functions in camera.c | 94 |
| matched | 44 (2,624 of 33,980 bytes) |
| revised against the typed header and kept at 100% | 43 of 44 (`camera-r2` 42/44, `camera-r2b` 1/1; `fn_1_C35C` released) |
| unmatched, attempted, best ≥ 90% | 17 |
| unmatched, attempted, best 70–90% | 16 |
| unmatched, mid-size 588–1260 B | 8 (0/8 in `camera-big`) |
| never attempted (524–6,056 B) | 5 |
| renamed | 44 functions, 6 globals, 4 typedefs |
| cost of the revise passes (Luna) | $0.25 |

Before and after for one unit (`fn_1_88D8`, now `game_camera_set_shake`):

```c
// before (matched, cheap tier)
camera = (CameraData *)((u8 *)lbl_1_bss_F6C + index * 0x1fc);
// after (header types the array, revise pass, naming pass)
camera = game_camera_entries + index;
if (mode != camera->unk_2) { fn_1_8A0C(index); return; }
camera->unk_12C += delta[0];
```

## What had to change in the tooling

- **Word-sized globals that hold a pointer are typed by their element.** The
  analyzer merges the two access paths (pointer loaded straight from the
  small-data area, and pointer loaded from a materialised address) into one
  element layout, drops the load of the pointer variable itself, and folds
  constant offsets past the stride (unrolled loops) back onto the element only
  when that produces no width clash. `lbl_1_bss_F6C` became
  `GameCameraEntry *game_camera_entries` with a 0x1FC-byte stride.
- **Strides are trusted only from the record start**; an index applied inside a
  record is a sub-array, and a stride smaller than the fields it would contain
  is dropped (a false 4-byte stride had collapsed a 0x60-byte record to two
  bytes).
- **Slots touched by both integer and float ops are typed by majority**; the
  old "any float access wins" rule retyped a slot to `f32`, grew one matched
  unit by four bytes and shifted every REL that links against main.rel.
- **Assert harvest** (`tools/seeds/asserts.py`): 54 assert calls in 44
  functions, each pinning the caller to `file.c:line` and usually naming the
  global it checks ("game_camera is NULL at set shake"). The naming bundle now
  shows them, plus the TU header and every string literal read from the retail
  bytes (dtk emits string tables as raw words, so the disassembly is not a
  reliable source).
- **TU anchors on relocation targets**, not on the regex match: eight file
  strings sat a few bytes after a path prefix or stray bytes. `bg_common.c`
  (40 functions, split out of `cloth.c`) was recovered this way, confirmed by
  the assert map; 51 TUs in main.rel now.
- **Typedef names persist** in `config/GFZE01/<module>/typedefs.json`
  (`{symbol: {"self"|"target"|"at_<off>": Name}}`), honoured by the header
  generator and carried across symbol renames. `fzgx naming-apply` records
  them, rewrites the identifiers in the sources, renames symbols, re-splits,
  regenerates every header of the module with the MWCC offset self-check and
  relinks once.
- **Disassembly index** keys functions by address and prefers the file named
  after the current symbol: dtk never deletes the `.s` of a renamed or moved
  unit, and the stale file used to shadow the fresh one.
- `orchestrate --revise --select-tu` now selects the TU's matched units.

## Observations

- Luna's names are conservative and offset-based where semantics are not
  visible (`camera_get_entry_field_0xa4`), and specific where the code shows
  them (`game_camera_set_shake`, `live_camera_get`, `camera_enable_flags`),
  which is the rule the bundle asks for. Field names were not proposed and are
  not applied yet.
- The two revise releases were both header mismatches, not model failures:
  a field typed `u16` where the retail code loads it signed (fixed by
  tracking indexed access through a derived pointer), and `lbl_1_bss_1010`
  declared as a lone `f32` where the function indexes across neighbouring
  bss symbols that dtk split apart (needs the `fzgx oversize` symbol merge).
- Header changes must be gated by the full link: a typedef change can alter a
  matched unit's code. Byte-diffing the linked REL against retail and mapping
  the first divergence to a function found the culprit in one step.

## Next

- Retry the 33 released camera functions that plateaued at 70–99% against the
  typed header (they were matched before it existed).
- Symbol merge for split bss records (`fzgx oversize`), then revise
  `fn_1_C35C`.
- Field naming: a scoped rename (per typedef, through the header generator's
  name map) so `unk_12C` can become `shake_offset.x` without touching other
  structs' `unk_12C`.
- Run the same pipeline on the next TUs by matched byte share.
