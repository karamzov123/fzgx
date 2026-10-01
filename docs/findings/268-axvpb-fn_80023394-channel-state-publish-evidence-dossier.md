# Finding 268 — `fn_80023394`: AXVPB channel-state publish evidence dossier

- Findings number: `268`
- Unit: `main/dolphin/ax/AXVPB`
- Symbol: `fn_80023394`
- Evidence action: static reference manifest, relocation-aware xrefs, context-only disassembly, retail/source assembly inspection, and caller/layout analysis
- Conversion attempt: none

## Scope and reproducibility

Run from the repository root:

```sh
python3 tools/natc_reference_manifest.py --unit main/dolphin/ax/AXVPB
python3 tools/find_xrefs.py --cslice fn_80023394 --json
python3 tools/natc_loop.py --unit main/dolphin/ax/AXVPB --symbol fn_80023394 --context-only
```

These commands were run against the current checkout while generating this dossier.
They are evidence-only: no candidate, compile, submission, gate, lease, queue, or
runtime operation is involved.

## Unit and symbol identity

The reference manifest reports `14` assembly functions in
`src/dolphin/ax/AXVPB.c`, `reference_backed: 0`, and `reference_count: 0` for
`fn_80023394`. The canonical symbol table at
`config/GFZE01/symbols.txt:612` identifies:

| Symbol | Retail address | Unit-relative value | Size |
|---|---:|---:|---:|
| `fn_80023394` | `0x80023394` | `0xEB0` (`3760`) | `0xA4` (`164`) |

The relocation-aware report identifies the canonical global `.text` definition in
`build/GFZE01/obj/dolphin/ax/AXVPB.o` (`value: 3760`, `size: 164`) and one equal-size
global alternate in `build/GFZE01/obj/coarse/text_8001E480.o` (`value: 20244`,
`size: 164`). No data globals are referenced and the target relocation histogram is
empty because its only external dependencies are direct `R_PPC_REL24` calls.

## Exact body and ABI

The context-only disassembly reproduces the complete `0xA4`-byte body:

```text
0x00  mflr r0
0x04  stw r0, 4(r1)
0x08  stwu r1, -0x20(r1)
0x0C  stw r31, 0x1c(r1)
0x10  stw r30, 0x18(r1)
0x14  addi r30, r4, 0
0x18  stw r29, 0x14(r1)
0x1C  addi r29, r3, 0
0x20  addi r31, r29, 0x1b6
0x24  bl 0x24   R_PPC_REL24 OSDisableInterrupts +0
0x28  lwz r0, 0(r30)
0x2C  stw r0, 0(r31)
0x30  lwz r0, 4(r30)
0x34  stw r0, 4(r31)
0x38  lwz r0, 8(r30)
0x3C  stw r0, 8(r31)
0x40  lwz r0, 0xc(r30)
0x44  stw r0, 0xc(r31)
0x48  lwz r0, 0x10(r30)
0x4C  stw r0, 0x10(r31)
0x50  lwz r0, 0x14(r30)
0x54  stw r0, 0x14(r31)
0x58  lwz r0, 0x18(r30)
0x5C  stw r0, 0x18(r31)
0x60  lwz r0, 0x1c(r30)
0x64  stw r0, 0x1c(r31)
0x68  lwz r0, 0x20(r30)
0x6C  stw r0, 0x20(r31)
0x70  lwz r0, 0x24(r30)
0x74  stw r0, 0x24(r31)
0x78  lwz r0, 0x1c(r29)
0x7C  oris r0, r0, 2
0x80  stw r0, 0x1c(r29)
0x84  bl 0x84   R_PPC_REL24 OSRestoreInterrupts +0
0x88  lwz r0, 0x24(r1)
0x8C  lwz r31, 0x1c(r1)
0x90  lwz r30, 0x18(r1)
0x94  lwz r29, 0x14(r1)
0x98  addi r1, r1, 0x20
0x9C  mtlr r0
0xA0  blr
```

The retail listing at `build/GFZE01/asm/dolphin/ax/AXVPB.s:1008-1049`
and the source-side body at `src/dolphin/ax/AXVPB.c:1091-1135` agree on the
instruction sequence, addresses, operands, and frame size. The ABI evidence is
bounded as follows:

- `r3` is retained as the AXVPB/channel object (`r29`).
- `r4` is retained as the source parameter block (`r30`).
- `r31` is the destination cursor initialized to `r3 + 0x1B6`.
- The `0x20`-byte frame saves `r29`, `r30`, and `r31`; no floating-point register
  or additional argument is used.
- The return value of `OSDisableInterrupts` remains in `r3` and is passed directly
  to `OSRestoreInterrupts` by the ABI; the body has no explicit return value.

## Exact data movement and publication flag

After interrupts are disabled, the function copies ten contiguous 32-bit words:

| Copy | Source (`r4`-relative) | Destination (`r3`-relative) |
|---:|---:|---:|
| 0 | `+0x00` | `+0x1B6` |
| 1 | `+0x04` | `+0x1BA` |
| 2 | `+0x08` | `+0x1BE` |
| 3 | `+0x0C` | `+0x1C2` |
| 4 | `+0x10` | `+0x1C6` |
| 5 | `+0x14` | `+0x1CA` |
| 6 | `+0x18` | `+0x1CE` |
| 7 | `+0x1C` | `+0x1D2` |
| 8 | `+0x20` | `+0x1D6` |
| 9 | `+0x24` | `+0x1DA` |

These are ten literal `lwz`/`stw` pairs, not a runtime loop. The destination
range is therefore `AXVPB + 0x1B6` through `AXVPB + 0x1DD` inclusive. No
source-level struct type, alignment promise beyond the observed word accesses, or
semantic names for these fields are asserted.

The final update reads the control word at `AXVPB + 0x1C`, applies
`oris r0,r0,2`, and writes it back. In PPC terms this sets the upper-halfword
immediate `0x0002`, i.e. ORs control bit mask `0x00020000`. The update occurs
before the interrupt token is restored. The static body proves a ten-word
state-block publication followed by the `0x00020000` control-word flag; it does
not prove what consumer owns or clears that flag.

## Callers, callees, and relocations

`find_xrefs.py --cslice fn_80023394 --json` reports:

```text
OBJECT  build/GFZE01/obj/dolphin/ax/AXVPB.o
SECTION .text  size 164 (0xa4)  value 0xeb0  bind global
CALLERS (2): SndInitVoiceParams, SndProcessVoiceEnvelope
CALLEES (2): OSDisableInterrupts, OSRestoreInterrupts
GLOBALS (0):
RELOC HISTOGRAM: (none)
```

The two direct calls are `R_PPC_REL24` relocations at target-relative offsets
`0x24` (`OSDisableInterrupts`) and `0x84` (`OSRestoreInterrupts`). There are no
`ADDR16_*`, `SDA21`, or other data relocations in this function. The equal-size
coarse duplicate corroborates the body identity but is not a separate semantic
implementation.

Source-side assembly grep exposes four call-site occurrences in
`src/game/gamehead_8005C120.c`: lines `3278`, `4165`, `6414`, and `6593`.
The relocation-aware xref report deduplicates those call sites to the two
canonical caller symbols `SndInitVoiceParams` and `SndProcessVoiceEnvelope`.
Each visible setup loads an AXVPB pointer into `r3`, forms a caller-owned stack
block in `r4`, and invokes this function; the stack offsets are `r1+0x38` at the
first two sites and `r1+0x68` at the latter two sites.

## Channel-state/cache relationships

`AXVPBInitChannelState` is the immediately preceding `0x110`-byte definition at
`0x80023284`; `fn_80023394` occupies `0x80023394-0x80023437`, followed by
`AXVPBSyncChannelA` at `0x80023438`. The neighboring static dossiers establish
these complementary effects:

- `findings/261-axvpb-init-channel-state-evidence-dossier.md` records init writes
  beginning at `+0x1A6`, selector-dependent initialization through `+0x1DA`, and
  control-word normalization followed by `0x00021000`.
- `findings/265-axvpb-sync-pair-evidence-dossier.md` records channel publication
  beginning at `+0x1DE` and control flags `0x00040000`/`0x00080000`.
- This function writes the intervening ten-word region `+0x1B6..+0x1DD` and
  publishes with `0x00020000`.

Together these are non-overlapping, adjacent AXVPB subregions at the verified
offset level. The shared interrupt bracket indicates that each operation publishes
its own block atomically with respect to interrupt handlers. Static evidence does
not establish a universal lifecycle order: visible code calls init then this
function on some paths, while other paths call this function after a prior init or
sync operation. It also does not establish whether the `+0x1B6..+0x1DD` block is
a cache, hardware shadow, or another channel-state representation; “channel-state
publish block” is descriptive of the observed copy-and-flag behavior only.

At `gamehead_8005C120.c:6404-6414`, one visible path calls
`AXVPBSyncChannelA`, then `AXVPBInitChannelState`, then `fn_80023394`; at
`:3273-3278` and `:4160-4165`, init precedes this function. These sequences show
that the function participates in voice/channel setup, but they do not by
-themselves establish field semantics or a required global ordering.

## Value of further evidence and disposition

The target remains `blocked-evidence`. No natural-C reference body or
identity-verified runtime fact is available, and the manifest reports zero usable
references. The exact body is already available as source-side assembly, so further
static disassembly is unlikely to resolve the remaining semantic/type questions.
The highest-value next evidence would be:

1. An SDK/Dolphin AXVPB reference body or verified source twin matching the ten-word
   copy and `0x00020000` flag, to test whether ordinary C can reproduce the required
   unrolled code under the unit's compiler settings.
2. Relocation-aware consumer/caller analysis for reads or clears of
   `AXVPB + 0x1B6..0x1DD` and control bit `0x00020000`, to identify the block's
   lifecycle without inventing a struct type.
3. If runtime work is authorized, an identity-verified read-only trace across the
   paired DOL/map/profile/ISO identity, observing the block and control word before
   and after callers invoke the function. Runtime evidence must remain observation
   only.

No candidate was generated, compiled, submitted, gated, converted, or landed. No
source, queue, lease, runtime, or fleet state was changed. This dossier is the only
intended artifact.
