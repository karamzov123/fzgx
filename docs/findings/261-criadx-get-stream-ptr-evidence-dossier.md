# `criadx_get_stream_ptr`: bounded getter evidence dossier

- Unit: `main/game/criadx_800452FC`
- Symbol: `criadx_get_stream_ptr`
- Evidence action: static manifest, relocation-aware xrefs, and context-only inspection
- Conversion attempt: none

## Verified evidence

`python3 tools/natc_reference_manifest.py --unit main/game/criadx_800452FC` reports:

- 34 functions remain in asm in `src/game/criadx_800452FC.c`.
- `criadx_get_stream_ptr` has `reference_count: 0` and no natural-C reference body.
- The unit has `reference_backed: 0`.

`python3 tools/find_xrefs.py --cslice criadx_get_stream_ptr --json` reports the canonical definition as:

- Object: `build/GFZE01/obj/game/criadx_800452FC.o`
- Section: `.text`
- Value: `528` (`0x210`, unit-relative)
- Size: `8` bytes (`0x8`)
- Binding/type: global function (`bind: 1`, `stype: 2`)
- Callers: `criadx_get_stream_ptr_wrapper`, `fn_80041700`, `fn_80041990`, and `fn_80041BF8` (four)
- Callees/globals/relocations: none
- One alternate definition exists in `build/GFZE01/obj/coarse/text_80041460.o`, also global `.text`, value `16556`, size `8`.

`python3 tools/natc_loop.py --unit main/game/criadx_800452FC --symbol criadx_get_stream_ptr --context-only` reproduces the exact body:

```text
FUNCTION criadx_get_stream_ptr  size 8 B (0x8)  section .text
0x00  lwz r3, 0x18(r3)
0x04  blr
```

The live source preserves the same two instructions at
`src/game/criadx_800452FC.c:263-267` inside an `asm void criadx_get_stream_ptr(void)` definition.
The relocation-aware xref result reports no relocation records, referenced globals, or callees for the target.

The wrapper at `src/game/criadx_80041460.c:172-183` loads `r3` from `4(r3)`, calls the target, and returns its `r3` result. The direct callers in that file are:

- `fn_80041700` at line 331: loads `r29` from `4(r28)`, calls the target, and saves the returned `r3` in `r27`.
- `fn_80041990` at line 505: loads `r29` from `4(r27)`, calls the target, and saves the returned `r3` in `r28`.
- `fn_80041BF8` at `src/game/criadx_80041BF8.c:104-107`: loads `r3` from `4(r29)`, calls the target, then compares the returned `r3` against `0x34(r29)`.

Therefore the bounded static behavior is a 32-bit load from offset `0x18` of the object addressed by incoming `r3`, returned in `r3`, followed by return. The callers establish that their incoming object is loaded from an outer object's offset `4`; this evidence does not establish the semantic field type beyond the 32-bit load or prove that the result is a pointer.

## Disposition

This is sufficient evidence to record the exact operation, ABI shape, and four relocation-aware callers, but not to introduce source: there is no accepted natural-C twin, no verified runtime fact, and no relocation/type evidence beyond the instruction operand. The context-only readiness verdict is `blocked-evidence` (`no natural-C reference and no verified runtime fact`). No candidate was generated, compiled, submitted, or gated.
