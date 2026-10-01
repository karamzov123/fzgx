# Finding 271 — `fn_80023568`: AXVPB three-halfword publish evidence dossier

- Findings number: `271`
- Unit: `main/dolphin/ax/AXVPB`
- Symbol: `fn_80023568`
- Evidence action: static reference manifest, relocation-aware xrefs, context-only disassembly, retail/source assembly inspection, and caller/layout analysis
- Conversion attempt: none

## Scope and reproducibility

Run from the repository root:

```sh
python3 tools/natc_reference_manifest.py --unit main/dolphin/ax/AXVPB
python3 tools/find_xrefs.py --cslice fn_80023568 --json
python3 tools/natc_loop.py --unit main/dolphin/ax/AXVPB --symbol fn_80023568 --context-only
```

These commands were run against the current checkout while generating this dossier.
They are evidence-only: no candidate, compile, submission, gate, lease, queue, or
runtime operation is involved.

## Unit and symbol identity

The reference manifest reports `14` assembly functions in
`src/dolphin/ax/AXVPB.c`, `reference_backed: 0`, and `reference_count: 0` for
`fn_80023568`. The canonical symbol table at
`config/GFZE01/symbols.txt:615` identifies:

| Symbol | Retail address | Unit-relative value | Size |
|---|---:|---:|---:|
| `fn_80023568` | `0x80023568` | `0x1084` (`4228`) | `0x6C` (`108`) |

The relocation-aware report identifies the canonical global `.text` definition in
`build/GFZE01/obj/dolphin/ax/AXVPB.o` (`value: 4228`, `size: 108`) and one equal-size
global alternate in `build/GFZE01/obj/coarse/text_8001E480.o` (`value: 20712`,
`size: 108`). No data globals are referenced and the target relocation histogram is
empty because its only external dependencies are direct `R_PPC_REL24` calls.

## Exact body and ABI

The context-only disassembly reproduces the complete `0x6C`-byte body:

```text
0x00  mflr r0
0x04  stw r0, 4(r1)
0x08  stwu r1, -0x20(r1)
0x0C  stw r31, 0x1c(r1)
0x10  stw r30, 0x18(r1)
0x14  addi r30, r4, 0
0x18  stw r29, 0x14(r1)
0x1C  addi r29, r3, 0
0x20  addi r31, r29, 0x1ec
0x24  bl 0x24   R_PPC_REL24 OSDisableInterrupts +0
0x28  lhz r0, 0(r30)
0x2C  sth r0, 0(r31)
0x30  lhz r0, 2(r30)
0x34  sth r0, 2(r31)
0x38  lhz r0, 4(r30)
0x3C  sth r0, 4(r31)
0x40  lwz r0, 0x1c(r29)
0x44  oris r0, r0, 0x10
0x48  stw r0, 0x1c(r29)
0x4C  bl 0x4c   R_PPC_REL24 OSRestoreInterrupts +0
0x50  lwz r0, 0x24(r1)
0x54  lwz r31, 0x1c(r1)
0x58  lwz r30, 0x18(r1)
0x5C  lwz r29, 0x14(r1)
0x60  addi r1, r1, 0x20
0x64  mtlr r0
0x68  blr
```

The retail listing at `build/GFZE01/asm/dolphin/ax/AXVPB.s:1188-1217`
and the source-side body at `src/dolphin/ax/AXVPB.c:1224-1254` agree on
instruction sequence, addresses, operands, and frame size. The bounded ABI facts
are:

- `r3` is retained as the AXVPB/channel object (`r29`).
- `r4` is retained as the source parameter block (`r30`).
- `r31` is a destination cursor initialized to `r3 + 0x1EC`.
- The `0x20`-byte frame saves `r29`, `r30`, and `r31`.
- The interrupt token returned by `OSDisableInterrupts` remains in `r3` and is
  passed directly to `OSRestoreInterrupts`; there is no explicit return value.

## Exact data movement and publication flag

After interrupts are disabled, the function copies exactly three unsigned halfwords:

| Copy | Source (`r4`-relative) | Destination (`r3`-relative) |
|---:|---:|---:|
| 0 | `+0x00` | `+0x1EC` |
| 1 | `+0x02` | `+0x1EE` |
| 2 | `+0x04` | `+0x1F0` |

The three literal `lhz`/`sth` pairs cover the destination range
`AXVPB + 0x1EC..0x1F1` inclusive. This is an unrolled fixed-width transfer, not a
runtime loop. The source evidence proves halfword access widths and offsets but does
not prove a source-level struct type or semantic field names.

The final update reads the AXVPB control word at `+0x1C`, applies
`oris r0,r0,0x10`, and writes it back. PPC `oris` sets upper-halfword immediate
`0x0010`, so the operation ORs control mask `0x00100000`. The flag is set before
the interrupt token is restored. Static evidence proves the three-halfword
publication and this control-word bit; it does not prove which consumer owns or
clears the bit.

## Callers, callees, and call-site input shape

`find_xrefs.py --cslice fn_80023568 --json` reports:

```text
OBJECT  build/GFZE01/obj/dolphin/ax/AXVPB.o
SECTION .text  size 108 (0x6c)  value 0x1084 bind global
CALLERS (2): SndInitVoiceParams, SndProcessVoiceEnvelope
CALLEES (2): OSDisableInterrupts, OSRestoreInterrupts
GLOBALS (0):
RELOC HISTOGRAM: (none)
```

The checked-in source-side assembly shows four direct call-site occurrences in
`src/game/gamehead_8005C120.c`: lines `3238`, `3814`, `6259`, and `6548`.
The relocation-aware report is authoritative for the deduplicated canonical caller
set. Each visible site loads an AXVPB pointer into `r3`, forms a caller-owned stack
block in `r4`, and invokes this helper. At lines `3235`, `3812`, `6256`, and `6546`,
the stack block bases are respectively `r1+0x10`, `r1+0x10`, `r1+0x20`, and
`r1+0x20`; the caller writes three halfwords at offsets `0`, `2`, and `4` before
the call. This confirms the input shape at each site without assigning semantic names
to the three values.

## Relationship to the neighboring AXVPB publish regions

The helper follows `AXVPBSyncChannelB` immediately in the canonical text layout:
`AXVPBSyncChannelB` occupies `0x800234D0..0x80023567`, and this helper occupies
`0x80023568..0x800235D3`; `fn_800235D4` follows at `0x800235D4`. The existing
sync dossier records that A/B publish at `AXVPB + 0x1DE` and `+0x1E0..+0x1EA` and
set control bits `0x00040000`/`0x00080000`. This helper publishes the next adjacent
three-halfword region `+0x1EC..+0x1F0` and sets the distinct `0x00100000` bit.

Thus the verified static layout extends the already observed channel publication
region through `+0x1F1`, with distinct control-word flags for the neighboring
operations. The adjacency and shared interrupt bracketing support a descriptive
multi-operation publication layout; they do not establish hardware ownership,
consumer behavior, semantic field names, or a universal lifecycle ordering.

## Novel-evidence disposition

This action adds non-redundant evidence beyond the cached state/type, init, sync,
and `fn_80023394` publish dossiers: it establishes the exact three-halfword transfer
at `+0x1EC..+0x1F0`, the distinct `0x00100000` control flag, the fixed `0x6C`-byte
ABI/body, and the caller-side three-halfword input construction. The target remains
`blocked-evidence`: no natural-C reference body or identity-verified runtime fact is
available, and the manifest reports zero usable references.

No candidate was generated, compiled, submitted, gated, converted, or landed. No
source, queue, lease, runtime, or fleet state was changed. This dossier is the only
intended artifact.
