# `AXSetVoiceState_cached` / `AXSetVoiceType_cached`: paired cached voice-state/type evidence dossier

- Findings number: `266`
- Unit: `main/dolphin/ax/AXVPB`
- Symbols: `AXSetVoiceState_cached`, `AXSetVoiceType_cached`
- Evidence action: static reference manifest, relocation-aware xrefs, context-only disassembly, retail assembly/source inspection, and AXVPB layout analysis
- Conversion attempt: none

## Scope and reproduction

Run from the repository root:

```sh
python3 tools/natc_reference_manifest.py --unit main/dolphin/ax/AXVPB
python3 tools/find_xrefs.py --cslice AXSetVoiceState_cached --json
python3 tools/find_xrefs.py --cslice AXSetVoiceType_cached --json
python3 tools/natc_loop.py --unit main/dolphin/ax/AXVPB --symbol AXSetVoiceState_cached --context-only
python3 tools/natc_loop.py --unit main/dolphin/ax/AXVPB --symbol AXSetVoiceType_cached --context-only
```

These commands were run against the current checkout while generating this dossier.
The context-only actions are evidence-only and do not generate candidates.

## Unit and symbol identity

`natc_reference_manifest.py --unit main/dolphin/ax/AXVPB` reports `14` assembly
functions and `reference_backed: 0`. Both targets report `reference_count: 0` and
have no natural-C reference body. The canonical symbol table
(`config/GFZE01/symbols.txt:607-608`) identifies:

| Symbol | Retail address | Unit-relative value | Size |
|---|---:|---:|---:|
| `AXSetVoiceState_cached` | `0x800230A4` | `0xBC0` (`3008`) | `0xC4` (`196`) |
| `AXSetVoiceType_cached` | `0x80023168` | `0xC84` (`3204`) | `0x5C` (`92`) |

The definitions are global `.text` functions in
`build/GFZE01/obj/dolphin/ax/AXVPB.o`. Each has one equal-size global alternate
definition in `build/GFZE01/obj/coarse/text_8001E480.o` (values `19492` and
`19688`, respectively). `find_xrefs.py` reports no data globals and an empty
relocation histogram for either symbol; the two direct calls in each body are
`R_PPC_REL24` calls to `OSDisableInterrupts` and `OSRestoreInterrupts`.

## `AXSetVoiceState_cached`: exact body and cache fields

The context-only disassembly reproduces the complete `0xC4`-byte body:

```text
0x00 mflr r0                 0x04 stw r0,4(r1)
0x08 stwu r1,-0x18(r1)      0x0C stw r31,0x14(r1)
0x10 addi r31,r4,0          0x14 stw r30,0x10(r1)
0x18 addi r30,r3,0          0x1C bl OSDisableInterrupts
0x20 cmpwi r31,2            0x24 addi r4,r30,0x138
0x28 beq 0x68               0x2C bge 0x40
0x30 cmpwi r31,0            0x34 beq 0x50
0x38 bge 0x5C               0x3C b 0x9C
0x40 cmpwi r31,4            0x44 beq 0x8C
0x48 bge 0x9C               0x4C b 0x78
0x50 li r0,2                0x54 sth r0,8(r4)
0x58 b 0x9C                 0x5C li r0,1
0x60 sth r0,8(r4)           0x64 b 0x9C
0x68 li r0,0                0x6C sth r0,8(r4)
0x70 sth r0,0xA(r4)         0x74 b 0x9C
0x78 li r0,0                0x7C sth r0,8(r4)
0x80 li r0,1                0x84 sth r0,0xA(r4)
0x88 b 0x9C                 0x8C li r0,0
0x90 sth r0,8(r4)           0x94 li r0,2
0x98 sth r0,0xA(r4)         0x9C lwz r0,0x1C(r30)
0xA0 ori r0,r0,1            0xA4 stw r0,0x1C(r30)
0xA8 bl OSRestoreInterrupts
0xAC-0xC0 epilogue and blr
```

The retail listing at `build/GFZE01/asm/dolphin/ax/AXVPB.s:838-896` and the
source-side asm body at `src/dolphin/ax/AXVPB.c:866-925` agree instruction-for-instruction
on addresses, control flow, operands, and the `0x18`-byte stack frame. With `r3`
as the AXVPB object and `r4` as the state selector, the exact effects under the
interrupt-disabled interval are:

| Input `r4` | Target writes (AXVPB-relative) |
|---:|---|
| `0` | `+0x140` = halfword `2` |
| `1` | `+0x140` = halfword `1` |
| `2` | `+0x140` = halfword `0`; `+0x142` = halfword `0` |
| `3` | `+0x140` = halfword `0`; `+0x142` = halfword `1` |
| `4` | `+0x140` = halfword `0`; `+0x142` = halfword `2` |
| other | no writes to `+0x140/+0x142` |

Every path then reads and writes the AXVPB control word at `+0x1C`, ORing
`0x00000001`, and restores interrupts using the token returned by
`OSDisableInterrupts`. The cached state subrecord therefore begins at `+0x138`,
with the two observed halfword fields at `+0x140` and `+0x142`; semantic field
names are not asserted by this static evidence.

## `AXSetVoiceType_cached`: exact body and cache fields

The context-only disassembly reproduces the complete `0x5C`-byte body:

```text
0x00 mflr r0                 0x04 stw r0,4(r1)
0x08 stwu r1,-0x18(r1)      0x0C stw r31,0x14(r1)
0x10 addi r31,r4,0          0x14 stw r30,0x10(r1)
0x18 addi r30,r3,0          0x1C bl OSDisableInterrupts
0x20 sth r31,0x146(r30)     0x24 clrlwi. r0,r31,0x10
0x28 lwz r4,0x1C(r30)       0x2C ori r0,r4,4
0x30 stw r0,0x1C(r30)       0x34 bne 0x40
0x38 li r0,1                0x3C stw r0,0x20(r30)
0x40 bl OSRestoreInterrupts
0x44-0x58 epilogue and blr
```

The retail listing at `build/GFZE01/asm/dolphin/ax/AXVPB.s:898-924` and the
source-side asm body at `src/dolphin/ax/AXVPB.c:927-954` agree byte-for-byte.
With `r3` as the AXVPB object and `r4` as the input value, it stores the input
as a halfword at `+0x146`, ORs `0x00000004` into the control word at `+0x1C`,
and, only when the input's low 16 bits are zero (`clrlwi. r0,r31,16` yields
zero), stores word `1` at `+0x20`. No other AXVPB fields are written.

## Callers, callees, and relocation evidence

`find_xrefs.py --cslice AXSetVoiceState_cached --json` reports six callers:
`SndInitVoiceParams`, `SndProcessVoiceEnvelope`,
`axmix_device_ctrl_accumulate_mix`, `axmix_update_voice_state`, `fn_8005A7F8`,
and `fn_8005B534`. It reports the two callees above, no globals, and the
canonical definition size/value `196`/`3008`.

`find_xrefs.py --cslice AXSetVoiceType_cached --json` reports six callers:
`ADXTServerStateRequest`, `SndInitVoiceParams`, `SndKillChannelVoice`,
`SndProcessVoiceEnvelope`, `SndStopAllChannelVoices`, and `fn_80069B10`. It
reports the same two callees, no globals, and canonical size/value `92`/`3204`.

The direct call sites are also visible in the checked-in source-side assembly:
`src/game/axmix_80026EE0.c:1625,1974` (state),
`src/dolphin/mtx/MTXHead.c:156,206` (type),
`src/game/gamehead_8005C120.c:1022,1085,1151,1219,1286,1356,4175,4219,4249,4400,6608,6637`
(type), `:3283,4170,6419,6598` (state), and
`src/game/adxt_8005A24C.c:599,1895` (state), `:1228,1252` (type).
The relocation-aware object report is authoritative for the deduplicated caller
sets; source grep additionally shows the individual call-site occurrences.

## Relationship to AXVPB init and synchronization dossiers

`findings/261-axvpb-init-channel-state-evidence-dossier.md` establishes that
`AXVPBInitChannelState` writes the initialization sublayout beginning at
`+0x1A6`, normalizes `+0x1C`, and sets `0x00021000` under the same interrupt
bracketing. `findings/265-axvpb-sync-pair-evidence-dossier.md` establishes that
`AXVPBSyncChannelA` publishes halfwords at `+0x1DE..+0x1EA` and sets control bit
`0x00040000`, while `AXVPBSyncChannelB` publishes the split value at `+0x1DE`
and `+0x1E0` and sets `0x00080000`.

The cached pair is earlier in the contiguous AXVPB text layout (`+0xBC0` and
`+0xC84`) and updates a distinct voice-control region: state uses
`+0x138/+0x140/+0x142` and control bit `0x1`; type uses `+0x146`, optional
`+0x20`, and control bit `0x4`. Thus the dossiers establish complementary,
non-overlapping static field effects: init establishes the later channel-state
region, cached state/type publish voice-control inputs, and sync A/B publish the
channel block. Static call evidence does not prove a universal lifecycle order,
so no unconditional ordering or semantic names are claimed here.

## Disposition and quality gate

Quality gate passed for a static dossier: both canonical definitions were found;
retail addresses/sizes match the symbol table; context-only disassembly matches
the retail assembly and source-side asm; every write offset and control flag is
accounted for; direct callees and relocation form are recorded; caller sets were
obtained from the relocation-aware xref report; and relationships to the init
and sync dossiers are bounded to verified offsets/operations.

Readiness remains `blocked-evidence` for both targets: no natural-C reference body
and no identity-verified runtime fact exists. No candidate was generated,
compiled, submitted, gated, or converted. No source, queue, lease, candidate,
runtime, or fleet state was changed. This file is the only intended artifact.
