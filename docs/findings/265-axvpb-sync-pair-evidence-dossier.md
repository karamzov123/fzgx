# `AXVPBSyncChannelA` / `AXVPBSyncChannelB`: paired synchronization and layout evidence dossier

- Findings number: `265`
- Unit: `main/dolphin/ax/AXVPB`
- Symbols: `AXVPBSyncChannelA`, `AXVPBSyncChannelB`
- Paired target: `AXVPBInitChannelState`
- Evidence action: static reference manifest, relocation-aware xrefs, context-only disassembly, retail assembly/source inspection, and caller/layout analysis
- Conversion attempt: none

## Scope and reproduction

Run from the repository root:

```sh
python3 tools/natc_reference_manifest.py --unit main/dolphin/ax/AXVPB
python3 tools/find_xrefs.py --cslice AXVPBSyncChannelA --json
python3 tools/find_xrefs.py --cslice AXVPBSyncChannelB --json
python3 tools/natc_loop.py --unit main/dolphin/ax/AXVPB --symbol AXVPBSyncChannelA --context-only
python3 tools/natc_loop.py --unit main/dolphin/ax/AXVPB --symbol AXVPBSyncChannelB --context-only
```

All commands were run against the current checkout while generating this dossier. The
context-only actions are evidence-only and do not generate candidates.

## Unit and symbol identity

`natc_reference_manifest.py --unit main/dolphin/ax/AXVPB` reports `14` assembly
functions and `reference_backed: 0`. Each target has `reference_count: 0` and no
natural-C reference body. `AXVPBInitChannelState` is in the same assembly-only set.

The canonical symbol table (`config/GFZE01/symbols.txt:611-614`) identifies the
contiguous layout:

| Symbol | Retail address | Unit-relative value | Size |
|---|---:|---:|---:|
| `AXVPBInitChannelState` | `0x80023284` | `0xDA0` (`3488`) | `0x110` (`272`) |
| `AXVPBSyncChannelA` | `0x80023438` | `0xF54` (`3924`) | `0x98` (`152`) |
| `AXVPBSyncChannelB` | `0x800234D0` | `0xFEC` (`4076`) | `0x98` (`152`) |

The two sync functions are adjacent in the retail object and have equal size. The
canonical definitions are global `.text` symbols in
`build/GFZE01/obj/dolphin/ax/AXVPB.o`. Each also has one equal-size global alternate
definition in `build/GFZE01/obj/coarse/text_8001E480.o`:

- A alternate: value `20408`, size `152`.
- B alternate: value `20560`, size `152`.

## AXVPBSyncChannelA: exact body and layout effect

The context-only disassembly is a `0x98`-byte function:

```text
0x00  mflr r0
0x04  stw r0, 4(r1)
0x08  stwu r1, -0x20(r1)
0x0C  stw r31, 0x1c(r1)
0x10  stw r30, 0x18(r1)
0x14  addi r30, r4, 0
0x18  stw r29, 0x14(r1)
0x1C  addi r29, r3, 0
0x20  addi r31, r29, 0x1de
0x24  bl OSDisableInterrupts
0x28  lhz r0, 0(r30)
0x2C  sth r0, 0(r31)
0x30  lhz r0, 2(r30)
0x34  sth r0, 2(r31)
0x38  lhz r0, 4(r30)
0x3C  sth r0, 4(r31)
0x40  lhz r0, 6(r30)
0x44  sth r0, 6(r31)
0x48  lhz r0, 8(r30)
0x4C  sth r0, 8(r31)
0x50  lhz r0, 0xa(r30)
0x54  sth r0, 0xa(r31)
0x58  lhz r0, 0xc(r30)
0x5C  sth r0, 0xc(r31)
0x60  lwz r0, 0x1c(r29)
0x64  rlwinm r0, r0, 0, 0xd, 0xb
0x68  stw r0, 0x1c(r29)
0x6C  lwz r0, 0x1c(r29)
0x70  oris r0, r0, 4
0x74  stw r0, 0x1c(r29)
0x78  bl OSRestoreInterrupts
0x7C-0x94  epilogue and blr
```

The retail assembly at `build/GFZE01/asm/dolphin/ax/AXVPB.s:1103-1143` confirms
address, bytes, operands, and the same `0x20`-byte stack frame. With `r3` as the
voice/channel object and `r4` as a source parameter block, A performs the following
under the interrupt-disabled interval:

- Copies seven unsigned halfwords from source offsets `0x00, 0x02, ..., 0x0C`.
- Stores them to the target object at `0x1DE, 0x1E0, ..., 0x1EA`.
- Reads the target control word at `+0x1C`, applies `rlwinm r0,r0,0,13,11`, and writes it back.
- ORs `0x00040000` into the same `+0x1C` control word.
- Passes the `OSDisableInterrupts` return value through the ABI to `OSRestoreInterrupts`.

The mask instruction is preserved literally here: its PPC mask operands clear the
control-bit lane selected by the instruction before A sets the `0x00040000` state bit.
No global data or relocations are present for A beyond its two `R_PPC_REL24` calls.

## AXVPBSyncChannelB: exact body and layout effect

The context-only disassembly is also `0x98` bytes:

```text
0x00  mflr r0
0x04  stw r0, 4(r1)
0x08  stwu r1, -0x28(r1)
0x0C  stfd f31, 0x20(r1)
0x10  fmr f31, f1
0x14  stw r31, 0x1c(r1)
0x18  stw r30, 0x18(r1)
0x1C  stw r29, 0x14(r1)
0x20  mr r29, r3
0x24  bl OSDisableInterrupts
0x28  lfs f0, 0(0)   R_PPC_EMB_SDA21 lbl_801A6F78 +0
0x2C  mr r30, r3
0x30  fmuls f31, f0, f31
0x34  fmr f1, f31
0x38  bl __cvt_fp2unsigned
0x3C  fmr f1, f31
0x40  mr r31, r3
0x44  bl __cvt_fp2unsigned
0x48  lis r0, 4
0x4C  cmplw r3, r0
0x50  ble 0x58
0x54  lis r31, 4
0x58  srwi r0, r31, 0x10
0x5C  sth r0, 0x1de(r29)
0x60  mr r3, r30
0x64  sth r31, 0x1e0(r29)
0x68  lwz r0, 0x1c(r29)
0x6C  oris r0, r0, 8
0x70  stw r0, 0x1c(r29)
0x74  bl OSRestoreInterrupts
0x78-0x94  epilogue and blr
```

The retail assembly at `build/GFZE01/asm/dolphin/ax/AXVPB.s:1145-1186` confirms
that B preserves `f1`, uses a `0x28`-byte frame, saves/restores `f31`, and has one
`R_PPC_EMB_SDA21` load of `lbl_801A6F78` plus two `R_PPC_REL24` calls to
`__cvt_fp2unsigned` and the interrupt pair.

With `r3` as the target object and `f1` as the input scalar, B:

1. Disables interrupts and saves the returned interrupt token in `r30`.
2. Multiplies the input by the single-precision value loaded from
   `lbl_801A6F78` (`.sdata2`, 8-byte symbol; the relocation is the verified static
   dependency).
3. Converts the scaled value to unsigned twice. The first result is not retained;
   the second result is retained in `r31` and compared with `0x00040000`.
4. Saturates `r31` to `0x00040000` when the unsigned comparison is greater.
5. Stores the high halfword (`r31 >> 16`) at target `+0x1DE` and the low halfword at
   target `+0x1E0`.
6. ORs `0x00080000` into target control word `+0x1C`, restores interrupts using the
   saved token, and returns.

Static evidence does not establish a source-level type for `lbl_801A6F78`, the scalar
argument, or the two halfwords. The safe statement is the exact scaled conversion,
unsigned comparison/saturation, and split-halfword stores above.

## Paired synchronization/layout interpretation

The pair shares a destination sublayout beginning at `AXVPB + 0x1DE` and a control
word at `AXVPB + 0x1C`:

| Operation | Source/input | Target writes | Control-word effect | Critical section |
|---|---|---|---|---|
| `AXVPBSyncChannelA` | `r4`, seven halfwords | `+0x1DE..+0x1EA` at 2-byte stride | literal `rlwinm` update, then OR `0x00040000` | `OSDisableInterrupts` → `OSRestoreInterrupts` |
| `AXVPBSyncChannelB` | `f1`, scaled by `lbl_801A6F78` | `+0x1DE` high halfword and `+0x1E0` low halfword | OR `0x00080000` | same pair, with token preserved across FP helpers |

Thus A transfers a seven-halfword channel/control block, while B updates the first
32-bit lane of that same destination region from a floating-point-derived unsigned
value. B does not touch A's remaining offsets `+0x1E2..+0x1EA`; A does not touch B's
floating-point conversion path. Both publish their update by setting distinct bits in
the shared control word at `+0x1C` while interrupts are disabled.

## Relationship to `AXVPBInitChannelState`

The preceding `AXVPBInitChannelState` definition is `0x110` bytes at
`0x80023284`; the paired sync functions begin immediately after the intervening
`fn_80023394` at `0x80023394`. The existing static evidence for init establishes
that it copies four 32-bit words into `+0x1A6, +0x1AA, +0x1AE, +0x1B2`, initializes
selector-dependent words at `+0x1B6..+0x1DA`, and normalizes the control word at
`+0x1C` before setting `0x00021000`, all under the same interrupt-bracketing
pattern.

The direct source call sequences show the lifecycle relationship:

- `src/game/gamehead_8005C120.c:6392-6409` constructs a seven-halfword block at
  `r1+0x48`, calls `AXVPBSyncChannelA`, then calls `AXVPBInitChannelState` with a
  separate block at `r1+0x58`.
- `src/game/gamehead_8005C120.c:6588-6603` calls init first and then calls A with
  the block at `r1+0x28` before voice-type setup.
- `src/game/adxt_8005A24C.c:597-602` and `:1895-1898` set voice state and call A
  with a stack block at `r1+8`.
- `src/game/gamehead_8005C120.c:6732-6737` passes a bounded scalar in `f1` to B.
- The relocation-aware xref report gives A callers
  `SndInitVoiceParams`, `fn_8005A7F8`, and `fn_8005B534`; B callers are
  `SndInitVoiceParams`, `axmix_device_ctrl_accumulate_mix`, and
  `axmix_update_voice_state`. These names are the report's canonical symbol names;
  the two `fn_` names remain intentionally unresolved here.

The evidence supports an initialization-then-publication model at the layout level:
init establishes the base voice/channel state and initial control flags, A publishes a
multi-halfword channel block, and B publishes a scalar-derived 32-bit channel value.
The call order is not globally fixed—one visible path invokes A before init and
another invokes init before A—so this dossier does not claim an unconditional call
ordering or semantic field names beyond the verified offsets and operations.

## Disposition and quality gate

This paired static action establishes both exact retail bodies, addresses, equal sizes,
stack/FP ABI differences, direct callees, B's SDA21 dependency, A's mask-and-flag
operation, the shared `+0x1DE` layout, control-word flag distinction, caller sets,
and the bounded relationship to `AXVPBInitChannelState`.

Readiness is `blocked-evidence` for both targets: there is no natural-C reference body
and no identity-verified runtime fact. No candidate was generated, compiled, submitted,
gated, or converted. No source, queue, lease, candidate, runtime, or fleet state was
changed. This file is the only intended artifact of the action.
