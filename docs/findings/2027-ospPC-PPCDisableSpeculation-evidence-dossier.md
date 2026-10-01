# `PPCDisableSpeculation`: bounded speculation-disable evidence dossier

- Unit: `main/dolphin/os/OSPPC`
- Symbol: `PPCDisableSpeculation`
- Evidence action: static manifest, relocation-aware xrefs, and context-only inspection
- Conversion attempt: none

## Verified evidence

The bounded evidence action was run from the repository root as:

```sh
python3 tools/natc_reference_manifest.py --unit main/dolphin/os/OSPPC
python3 tools/find_xrefs.py --cslice PPCDisableSpeculation --json
python3 tools/natc_loop.py --unit main/dolphin/os/OSPPC --symbol PPCDisableSpeculation --context-only
```

`natc_reference_manifest.py` reports the live source as
`src/dolphin/os/OSPPC.c`, with `asm_functions: 27` and
`reference_backed: 0`. `PPCDisableSpeculation` has `reference_count: 0`:
the listed cross-project bodies are assembly definitions, not usable
natural-C reference bodies:

- `dolsdk2001:src/base/PPCArch.c:102` (`is_asm: true`)
- `melee:melee/extern/dolphin/src/dolphin/base/PPCArch.c:102` (`is_asm: true`)
- `melee:extern/dolphin/src/dolphin/base/PPCArch.c:102` (`is_asm: true`)
- `melee-src-tmpcopy:dolphin/base/PPCArch.c:102` (`is_asm: true`)
- `mkdd:libs/dolphin/base/PPCArch.c:321` (`is_asm: true`)
- `sms:src/dolphin/base/PPCArch.c:116` (`is_asm: true`)

`find_xrefs.py --cslice PPCDisableSpeculation --json` reports:

- Canonical object: `build/GFZE01/obj/dolphin/os/OSPPC.o`
- Section: `.text`
- Unit-relative value: `272` (`0x110`)
- Size: `40` bytes (`0x28`)
- Binding/type: global function (`bind: 1`, `stype: 2`)
- Caller: `OSInit` (one)
- Callees: `PPCMfhid0`, `PPCMthid0`
- Globals referenced: none
- Relocation histogram: empty
- One alternate definition: `build/GFZE01/obj/coarse/text_800055E0.o`, `.text`, value `19180`, size `40`, global function

The symbol manifest identifies the retail address as
`.text:0x8000A0CC`, size `0x28`, global function. The live source at
`src/dolphin/os/OSPPC.c:200-213` is:

```text
asm void PPCDisableSpeculation(void)
{
    nofralloc
    mflr    r0
    stw     r0, 4(r1)
    stwu    r1, -8(r1)
    bl      PPCMfhid0
    ori     r3, r3, 0x200
    bl      PPCMthid0
    lwz     r0, 0xc(r1)
    addi    r1, r1, 8
    mtlr    r0
    blr
}
```

The context-only disassembly reproduces the exact 40-byte body:

```text
FUNCTION PPCDisableSpeculation  size 40 B (0x28)  section .text
0x00  mflr r0
0x04  stw r0, 4(r1)
0x08  stwu r1, -8(r1)
0x0C  bl 0xc   R_PPC_REL24 PPCMfhid0 +0
0x10  ori r3, r3, 0x200
0x14  bl 0x14   R_PPC_REL24 PPCMthid0 +0
0x18  lwz r0, 0xc(r1)
0x1C  addi r1, r1, 8
0x20  mtlr r0
0x24  blr
```

The sole caller is `OSInit`; the live caller sequence at
`src/dolphin/os/OS.c:250-252` branches to `PPCDisableSpeculation`
between `PPCMtpmc4` and `PPCSetFpNonIEEEMode`.

Thus the bounded static behavior is: save the link register in an
8-byte stack frame, read HID0 via `PPCMfhid0`, set bit `0x200` in the
returned value, write it via `PPCMthid0`, restore the link register,
and return. The two calls carry `R_PPC_REL24` relocations; no global or
SDA/data relocation is present in this function.

The context-only report also gives `verdict exhausted`, with attempt
budget `12/12`, best score `100.000`, and reasons `attempt budget 12
spent` plus `no natural-C reference and no verified runtime fact`.

## Disposition

The static evidence is sufficient to record the exact body, ABI shape,
callee sequence, and HID0 mask, but does not authorize a source
conversion: all references are assembly, no natural-C twin is available,
and no runtime fact was available. No candidate was generated,
compiled, submitted, or gated; no source, lease, queue, or fleet state
was changed.
