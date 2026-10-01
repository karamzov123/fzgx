# `AXVPBInitChannelState`: bounded static evidence dossier

- Unit: `main/dolphin/ax/AXVPB`
- Symbol: `AXVPBInitChannelState`
- Evidence action: static manifest, relocation-aware xrefs, and context-only inspection
- Conversion attempt: none

## Verified evidence

`python3 tools/natc_reference_manifest.py --unit main/dolphin/ax/AXVPB` reports:

- `src/dolphin/ax/AXVPB.c` has 14 functions remaining in asm.
- `AXVPBInitChannelState` has `reference_count: 0` and no natural-C reference body.
- The unit has `reference_backed: 0`.

`python3 tools/find_xrefs.py --cslice AXVPBInitChannelState --json` reports the canonical definition as:

- Object: `build/GFZE01/obj/dolphin/ax/AXVPB.o`
- Section: `.text`
- Value: `3488` (`0xda0`, unit-relative)
- Size: `272` bytes (`0x110`)
- Binding/type: global function (`bind: 1`, `stype: 2`)
- Callers: `ADXTServerStateRequest`, `SndInitVoiceParams`, `SndProcessVoiceEnvelope`
- Callees: `OSDisableInterrupts`, `OSRestoreInterrupts`
- Globals referenced: none
- Relocation histogram: empty
- One alternate definition exists in `build/GFZE01/obj/coarse/text_8001E480.o`, also global and size `272` bytes.

`python3 tools/natc_loop.py --unit main/dolphin/ax/AXVPB --symbol AXVPBInitChannelState --context-only` reproduces a `272`-byte function with this exact instruction behavior:

```text
0x00  mflr r0
0x04  stw r0, 4(r1)
0x08  stwu r1, -0x20(r1)
0x0C  stw r31, 0x1c(r1)
0x10  stw r30, 0x18(r1)
0x14  addi r30, r4, 0
0x18  stw r29, 0x14(r1)
0x1C  addi r29, r3, 0
0x20  addi r31, r29, 0x1a6
0x24  bl 0x24   R_PPC_REL24 OSDisableInterrupts +0
0x28  lwz r0, 0(r30)
0x2C  stw r0, 0(r31)
0x30  lwz r0, 4(r30)
0x34  stw r0, 4(r31)
0x38  lwz r0, 8(r30)
0x3C  stw r0, 8(r31)
0x40  lwz r0, 0xc(r30)
0x44  stw r0, 0xc(r31)
0x48  lhz r0, 2(r30)
0x4C  cmpwi r0, 0xa
0x50  beq 0x70
0x54  bge 0x64
0x58  cmpwi r0, 0
0x5C  beq 0xd4
0x60  b 0xd4
0x64  cmpwi r0, 0x19
0x68  beq 0xa4
0x6C  b 0xd4
0x70  li r4, 0
0x74  stw r4, 0x10(r31)
0x78  lis r0, 0x800
0x7C  stw r4, 0x14(r31)
0x80  stw r4, 0x18(r31)
0x84  stw r4, 0x1c(r31)
0x88  stw r4, 0x20(r31)
0x8C  stw r4, 0x24(r31)
0x90  stw r4, 0x28(r31)
0x94  stw r4, 0x2c(r31)
0x98  stw r0, 0x30(r31)
0x9C  stw r4, 0x34(r31)
0xA0  b 0xd4
0xA4  li r4, 0
0xA8  stw r4, 0x10(r31)
0xAC  lis r0, 0x100
0xB0  stw r4, 0x14(r31)
0xB4  stw r4, 0x18(r31)
0xB8  stw r4, 0x1c(r31)
0xBC  stw r4, 0x20(r31)
0xC0  stw r4, 0x24(r31)
0xC4  stw r4, 0x28(r31)
0xC8  stw r4, 0x2c(r31)
0xCC  stw r0, 0x30(r31)
0xD0  stw r4, 0x34(r31)
0xD4  lwz r0, 0x1c(r29)
0xD8  rlwinm r0, r0, 0, 0x13, 0xe
0xDC  stw r0, 0x1c(r29)
0xE0  lwz r0, 0x1c(r29)
0xE4  oris r0, r0, 2
0xE8  ori r0, r0, 0x1000
0xEC  stw r0, 0x1c(r29)
0xF0  bl 0xf0   R_PPC_REL24 OSRestoreInterrupts +0
0xF4  lwz r0, 0x24(r1)
0xF8  lwz r31, 0x1c(r1)
0xFC  lwz r30, 0x18(r1)
0x100  lwz r29, 0x14(r1)
0x104  addi r1, r1, 0x20
0x108  mtlr r0
0x10C  blr
```

The live definition at `src/dolphin/ax/AXVPB.c:1014-1089` preserves the same body. It copies four 32-bit words from `r4+0x0`, `r4+0x4`, `r4+0x8`, and `r4+0xc` to `r3+0x1a6`, `r3+0x1aa`, `r3+0x1ae`, and `r3+0x1b2`. It then reads a 16-bit selector from `r4+2`. Selector `10` clears destination offsets `0x10` through `0x2c` (word stride), stores `0x08000000` at `0x30`, and clears `0x34`; selector `25` performs the same initialization with `0x01000000` at `0x30`. Selectors `0` and all other values skip that initialization. Finally it clears bits represented by `rlwinm r0,r0,0,0x13,0xe` in the word at `r3+0x1c`, sets `0x00021000`, restores interrupts, and returns.

The source call sites are `src/game/gamehead_8005C120.c:3273, 4160, 6409, 6588` and `src/game/adxt_8005A24C.c:1225`; these correspond to the three xref caller symbols reported above. The visible call-site setup loads the destination voice/channel object into `r3` and passes a stack or caller-owned parameter block in `r4`.

## Disposition

This static action establishes the exact `272`-byte ABI/control-flow shape, interrupt bracketing, source offsets, selector cases, and xref set. It does not establish semantic field types or a natural-C reference body. Context-only readiness is `blocked-evidence` because there is no natural-C reference and no verified runtime fact. No candidate was generated, compiled, submitted, gated, or converted; no source, queue, lease, or fleet state was changed.
