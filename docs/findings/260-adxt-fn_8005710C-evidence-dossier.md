# `fn_8005710C`: bounded setter evidence dossier

- Unit: `main/game/adxt_800570DC`
- Symbol: `fn_8005710C`
- Evidence action: static manifest, relocation-aware xrefs, and context-only inspection
- Conversion attempt: none

## Verified evidence

`python3 tools/natc_reference_manifest.py --unit main/game/adxt_800570DC` reports:

- 41 functions remain in asm in `src/game/adxt_800570DC.c`.
- `fn_8005710C` has `reference_count: 0` and no natural-C reference body.
- The unit has `reference_backed: 0`.

`python3 tools/find_xrefs.py --cslice fn_8005710C --json` reports the canonical definition as:

- Object: `build/GFZE01/obj/game/adxt_800570DC.o`
- Section: `.text`
- Value: `48` (`0x30`, unit-relative)
- Size: `8` bytes (`0x8`)
- Binding/type: global function (`bind: 1`, `stype: 2`)
- Callers: `ADXT_Create` (one)
- Callees/globals/relocations: none
- One alternate definition exists in `build/GFZE01/obj/coarse/text_80053EA0.o`, also size `8`.

`python3 tools/natc_loop.py --unit main/game/adxt_800570DC --symbol fn_8005710C --context-only` reproduces the exact body:

```text
FUNCTION fn_8005710C  size 8 B (0x8)  section .text
0x00  stw r4, 0x28(r3)
0x04  blr
```

The live source preserves the same two instructions at
`src/game/adxt_800570DC.c:79-84` inside an `asm void fn_8005710C(void)` definition.
The sole caller in `src/game/adxt_8004CD70.c:284-286` loads `r3` from `0x94(r31)`,
loads `r4` from `8(r31)`, and branches to the target. Therefore the bounded static
behavior is a 32-bit store of the caller-provided `r4` value at offset `0x28` of
the object addressed by `r3`, followed by return; the surrounding object field
types are not established by this evidence action.

## Disposition

This is sufficient evidence to record the exact operation and ABI shape, but not to
introduce source: there is no accepted natural-C twin, no verified runtime fact, no
relocation/type evidence beyond the instruction operands, and the context-only
readiness verdict is `blocked-evidence` (`no natural-C reference and no verified
runtime fact`). No candidate was generated, compiled, submitted, or gated.
