# `fn_80057114`: bounded static evidence dossier

- Unit: `main/game/adxt_800570DC`
- Symbol: `fn_80057114`
- Evidence action: static manifest, relocation-aware xrefs, and context-only inspection
- Conversion attempt: none

## Verified evidence

`python3 tools/natc_reference_manifest.py --unit main/game/adxt_800570DC` reports:

- `src/game/adxt_800570DC.c` has 41 functions remaining in asm.
- `fn_80057114` has `reference_count: 0` and no natural-C reference body.
- The unit has `reference_backed: 0`.

`python3 tools/find_xrefs.py --cslice fn_80057114 --json` reports:

- Canonical object: `build/GFZE01/obj/game/adxt_800570DC.o`
- Section: `.text`
- Unit-relative value: `56` (`0x38`)
- Function size: `216` bytes (`0xd8`)
- Binding/type: global function (`bind: 1`, `stype: 2`)
- Callers: `fn_8004CAC8`, `fn_80057378`
- Callees: `ADXT_StartVoice`, `gcciErrPrintf`, `memset`
- Referenced global: `E0003_lsc_null_str`, 35 bytes, `.rodata`, global/object; relocation pair `R_PPC_ADDR16_HA` + `R_PPC_ADDR16_LO`
- Relocation histogram: `R_PPC_ADDR16_HA=1`, `R_PPC_ADDR16_LO=1`
- One alternate definition exists in `build/GFZE01/obj/coarse/text_80053EA0.o`, `.text`, value `12916`, size `216`, global function.

`python3 tools/natc_loop.py --unit main/game/adxt_800570DC --symbol fn_80057114 --context-only` reproduces this exact 216-byte body:

```text
FUNCTION fn_80057114  size 216 B (0xd8)  section .text
0x00  stwu r1, -0x10(r1)
0x04  mflr r0
0x08  stw r0, 0x14(r1)
0x0C  stw r31, 0xc(r1)
0x10  or. r31, r3, r3
0x14  beq 0xc4
0x18  bne 0x30
0x1C  lis r3, 0
0x20  addi r3, r3, 0
0x24  crclr cr1eq
0x28  bl 0x28   R_PPC_REL24 gcciErrPrintf +0
0x2C  b 0xac
0x30  lbz r0, 1(r31)
0x34  extsb. r0, r0
0x38  beq 0xac
0x3C  li r0, 0
0x40  stb r0, 1(r31)
0x44  lwz r3, 0x28(r31)
0x48  cmplwi r3, 0
0x4C  beq 0x68
0x50  lbz r0, 2(r31)
0x54  cmpwi r0, 1
0x58  bne 0x68
0x5C  bl 0x5c   R_PPC_REL24 ADXT_StartVoice +0
0x60  li r0, 0
0x64  stb r0, 2(r31)
0x68  li r3, 0
0x6C  cmplwi r31, 0
0x70  stw r3, 0x2c(r31)
0x74  bne 0x8c
0x78  lis r3, 0
0x7C  addi r3, r3, 0
0x80  crclr cr1eq
0x84  bl 0x84   R_PPC_REL24 gcciErrPrintf +0
0x88  b 0xa4
0x8C  lbz r0, 1(r31)
0x90  extsb. r0, r0
0x94  bne 0xa4
0x98  stw r3, 0x1c(r31)
0x9C  stw r3, 0x20(r31)
0xA0  stw r3, 0x24(r31)
0xA4  li r0, 0
0xA8  stw r0, 0x34(r31)
0xAC  li r0, 0
0xB0  mr r3, r31
0xB4  stb r0, 0(r31)
0xB8  li r4, 0
0xBC  li r5, 0x238
0xC0  bl 0xc0   R_PPC_REL24 memset +0
0xC4  lwz r0, 0x14(r1)
0xC8  lwz r31, 0xc(r1)
0xCC  mtlr r0
0xD0  addi r1, r1, 0x10
0xD4  blr
```

The live definition at `src/game/adxt_800570DC.c:86-149` preserves the same instruction sequence. The incoming `r3` is copied to `r31`. For a non-null object whose signed byte at offset `1` is nonzero, the function clears byte `1`, loads the word at offset `0x28`, and calls `ADXT_StartVoice` only when that word is nonzero and byte `2` equals `1`; it then clears byte `2`. It clears the word at `0x2c`. The subsequent non-null path checks byte `1` again and, when zero, clears words at `0x1c`, `0x20`, and `0x24`, then clears word `0x34`. It clears byte `0`, calls `memset(object, 0, 0x238)`, restores `r31`/`lr`, and returns. The null/error branches load `E0003_lsc_null_str` and call `gcciErrPrintf`; the branch structure makes the second null check unreachable for a normally entered non-null path, but it is retained in the exact body above.

## Callers and relationship to `fn_8005710C`

The relocation-derived callers are:

- `fn_8004CAC8` in `src/game/adxt_8004C164.c:721-916`. Its cleanup path at lines `833-838` loads `r3` from `0x94(r31)`, clears that owner field, and calls `fn_80057114`.
- `fn_80057378` in `src/game/adxt_800570DC.c:263-310`. Its loop at lines `282-292` walks sixteen records at `lbl_80188A8C` with stride `0x238`; for each record whose byte `0` is `1`, it passes the record in `r3` to `fn_80057114`. It then clears the whole `0x2380`-byte record array with `memset`.

`fn_8005710C` is a separate 8-byte function at `.text` value `0x30` in the same object. Its exact body is `stw r4, 0x28(r3); blr`, and its sole caller is `ADXT_Create` (`src/game/adxt_8004CD70.c:283-286`). Thus the static evidence establishes a same-record offset relationship: `fn_8005710C` writes the incoming `r4` word into offset `0x28`, while `fn_80057114` later reads offset `0x28` and may pass it to `ADXT_StartVoice`. This is not a direct call relationship, and the evidence does not establish the field's semantic type or prove that the stored value is a voice pointer.

## Disposition

This static action establishes the exact `216`-byte ABI/control-flow shape, two relocation-derived callers, three callees, the `E0003_lsc_null_str` address relocation pair, and the bounded offset relationship to `fn_8005710C`. It does not establish semantic field types or provide a natural-C reference body. Context-only readiness is `blocked-evidence` because there is no natural-C reference and no verified runtime fact. No candidate was generated, compiled, submitted, gated, or converted; no source, queue, lease, or fleet state was changed.
