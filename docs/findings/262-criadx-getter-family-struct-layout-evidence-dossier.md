# CRIADX getter family: bounded struct-layout evidence dossier

- Findings number: `262`
- Unit: `main/game/criadx_800452FC`
- Source: `src/game/criadx_800452FC.c`
- Symbols: `criadx_get_field_0E`, `criadx_get_field_14`, `criadx_get_status`, `criadx_get_stream_ptr`
- Evidence action: exact-body extraction, relocation-aware xrefs, caller/context inspection, and shared-layout assessment
- Conversion attempt: none

## Scope and reproducibility

All observations below were reproduced from the canonical worktree with:

```text
python3 tools/natc_reference_manifest.py --unit main/game/criadx_800452FC
python3 tools/find_xrefs.py --cslice <symbol> --json
python3 tools/natc_loop.py --unit main/game/criadx_800452FC --symbol <symbol> --context-only
```

The last two commands were run once for each of the four symbols. No source conversion, candidate generation, compilation, submission, gating, lease operation, or fleet mutation was performed.

## Exact target definitions

The relocation-aware object index identifies all four definitions in `build/GFZE01/obj/game/criadx_800452FC.o`, section `.text`, global function binding (`bind: 1`, `stype: 2`):

| Symbol | Unit-relative value | Size | Exact body |
|---|---:|---:|---|
| `criadx_get_stream_ptr` | `0x210` (`528`) | `0x8` | `lwz r3, 0x18(r3)`; `blr` |
| `criadx_get_field_0E` | `0x28c` (`652`) | `0xc` | `lbz r3, 0x0e(r3)`; `extsb r3, r3`; `blr` |
| `criadx_get_field_14` | `0x298` (`664`) | `0x8` | `lwz r3, 0x14(r3)`; `blr` |
| `criadx_get_status` | `0x2a0` (`672`) | `0x8` | `lha r3, 0x98(r3)`; `blr` |

The live source contains the same instruction sequences at `criadx_800452FC.c:263-268` and `:313-333`. Each xref query reported zero callees, zero referenced globals, and an empty relocation histogram. Each symbol has one alternate global `.text` definition in `build/GFZE01/obj/coarse/text_80041460.o`, with matching size (values `16556`, `16680`, `16692`, and `16700`, respectively); these are duplicate coarse definitions, not natural-C reference bodies.

The reference manifest reports `34` assembly functions in the unit, `reference_count: 0` for each target, and `reference_backed: 0` for the unit. The context-only runs returned `blocked-evidence` for each target (no natural-C reference and no verified runtime fact).

## Xrefs and caller evidence

`find_xrefs.py` reported these canonical callers:

- `criadx_get_stream_ptr`: `criadx_get_stream_ptr_wrapper`, `fn_80041700`, `fn_80041990`, `fn_80041BF8`.
- `criadx_get_field_0E`: `criadx_set_field_48`, `fn_80041700`, `fn_80041990`, `fn_80041E00`.
- `criadx_get_field_14`: `criadxGetValue`.
- `criadx_get_status`: `ADXB_DecodeHeader`, `fn_800416A8`, `fn_80041BF8`.

The wrappers in `src/game/criadx_80041460.c` consistently perform `lwz r3, 4(r3)` before calling the leaf getter:

- `criadx_get_stream_ptr_wrapper` lines `172-183` calls `criadx_get_stream_ptr`.
- `criadx_set_field_48` lines `214-225` calls `criadx_get_field_0E`.
- `criadxGetValue` lines `228-239` calls `criadx_get_field_14`.
- `fn_800416A8` lines `242-253` calls `criadx_get_status`.

This establishes a common wrapper convention: the public/outer CRIADX object stores a pointer at offset `0x04`, and the leaf getters operate on that pointed-to inner object.

Additional caller slices provide the following offset/use evidence:

- In `fn_80041700` (`criadx_80041460.c:314-337`), `r31 = *(r28 + 4)` and the same `r31` is passed to `fn_80045414`, `criadx_get_stream_ptr`, `fn_80045304`, and `fn_800452FC`. The returned stream value is subtracted from `*(r28 + 0x34)` and used as a byte-count bound. The loop at `:402-405` reloads `*(r28 + 4)` and calls `criadx_get_field_0E`; its signed result controls the loop count.
- In `fn_80041990` (`criadx_80041460.c:488-511`), `r31 = *(r27 + 4)` and `r29 = *(r27 + 4)` are passed to the stream getter and adjacent CRIADX leaves. This is the same inner-object path as `fn_80041700`.
- In `fn_80041BF8` (`criadx_80041BF8.c:40-45`), `r31 = *(r29 + 4)`; at `:81-84` that value is passed to `criadx_get_status` and the signed return is compared with zero. At `:104-107`, `*(r29 + 4)` is passed to `criadx_get_stream_ptr`, whose return is compared with `*(r29 + 0x34)`. At `:120-129`, the same inner object is passed to `fn_80045514`, which reads offset `0x10`.
- `criadx_get_field_14` is reached through `criadxGetValue` with the same `*(outer + 4)` handoff; the xref result has no other canonical caller.

## Struct-layout conclusion

The four leaves establish a shared inner-object stream layout at the level supported by static evidence:

```text
inner + 0x0e : signed 8-bit value (lbz followed by extsb)
inner + 0x14 : 32-bit word (lwz)
inner + 0x18 : 32-bit word (lwz), consumed as a stream position/pointer by callers
inner + 0x98 : signed 16-bit value (lha), used as a status code by callers
outer + 0x04: pointer to the inner object supplied to all four getter wrappers
```

The layout relationship is strong because the wrappers use the identical `outer + 0x04` load and the direct callers pass the same `r31`/`r29` inner-object register to multiple leaves. The stream getter's `0x18` word is used in pointer-like subtraction and position comparisons, but this dossier does not prove its C type, ownership, or lifetime. Likewise, names such as `field_14` and `status` do not establish semantic types beyond the decoded load width and signedness.

## Disposition

This bounded action records reusable struct-offset and ABI evidence, but does not justify source introduction: there are no natural-C twins, no verified runtime facts, and no relocation/type metadata. The family remains `blocked-evidence` for candidate generation. No candidate was generated, compiled, submitted, gated, or landed.
