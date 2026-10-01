# `PPCSetFpNonIEEEMode`: bounded static evidence dossier

- Unit: `main/dolphin/os/OSPPC`
- Symbol: `PPCSetFpNonIEEEMode`
- Evidence action: static manifest, relocation-aware xrefs, context-only disassembly, and caller/dependency inspection
- Conversion attempt: none

## Reproduction

Run from the repository root:

```sh
python3 tools/natc_reference_manifest.py --unit main/dolphin/os/OSPPC
python3 tools/find_xrefs.py --cslice PPCSetFpNonIEEEMode --json
python3 tools/natc_loop.py --unit main/dolphin/os/OSPPC --symbol PPCSetFpNonIEEEMode --context-only
```

The commands were run against the current checkout while generating this dossier. The
context-only command returned the exact two-instruction body shown below; no candidate,
compiler, queue, lease, runtime, or gate action was run.

## Identity and reference evidence

`natc_reference_manifest.py` reports the live definition in
`src/dolphin/os/OSPPC.c`, which has `asm_functions: 27` and
`reference_backed: 0`. `PPCSetFpNonIEEEMode` has `reference_count: 0`.
The same-name bodies found by the manifest are assembly definitions, not natural-C
reference bodies:

- `mkdd:libs/dolphin/base/PPCArch.c:333` (`is_asm: true`)
- `sms:src/dolphin/base/PPCArch.c:118` (`is_asm: true`)

The canonical symbol table identifies the target as a global `.text` function at
`0x8000A0F4`, size `0x8`:

```text
PPCSetFpNonIEEEMode = .text:0x8000A0F4; // type:function size:0x8 scope:global
```

## Exact body

`find_xrefs.py --cslice PPCSetFpNonIEEEMode --json` identifies:

- Object: `build/GFZE01/obj/dolphin/os/OSPPC.o`
- Section: `.text`
- Unit-relative value: `0x138` (`312`)
- Size: `8` bytes (`0x8`)
- Binding/type: global function (`bind: 1`, `stype: 2`)
- Alternate definition: `build/GFZE01/obj/coarse/text_800055E0.o`, `.text`, value `19220`, size `8`, global function
- Relocations: none

The context-only disassembly is:

```text
FUNCTION PPCSetFpNonIEEEMode  size 8 B (0x8)  section .text
0x00  mtfsb1 0x1d
0x04  blr
```

The generated retail assembly at `build/GFZE01/asm/dolphin/os/OSPPC.s:191-195`
confirms the address, bytes, and operand spelling:

```text
# .text:0x138 | 0x8000A0F4 | size: 0x8
.fn PPCSetFpNonIEEEMode, global
/* 8000A0F4 000070F4  FF A0 00 4C */\tmtfsb1 cr7gt
/* 8000A0F8 000070F8  4E 80 00 20 */\tblr
.endfn PPCSetFpNonIEEEMode
```

The live source at `src/dolphin/os/OSPPC.c:215-220` preserves the same body:

```text
asm void PPCSetFpNonIEEEMode(void)
{
    nofralloc
    mtfsb1  29
    blr
}
```

`mtfsb1 29` sets FPSCR bit 29. On the Gekko/F-Zero GX implementation this is the
non-IEEE-mode control bit named by the function; the instruction does not read a
GPR or memory operand and has no callable callee. `blr` returns with the incoming
link register and stack unchanged.

## Caller and register-state relationship

The relocation-aware xref report gives exactly one caller, `OSInit`, and no callees,
globals, or relocations for the target. The caller sequence in
`src/dolphin/os/OS.c:238-253` is:

```text
bl      OSDisableInterrupts
...
bl      PPCMtpmc4
bl      PPCDisableSpeculation
bl      PPCSetFpNonIEEEMode
li      r0, 0
```

Thus `OSInit` invokes the target after clearing the performance-monitoring counters,
and immediately after `PPCDisableSpeculation`; there is no argument setup because the
target has a `void(void)` ABI and consumes no GPR argument.

`PPCDisableSpeculation` is a separate 40-byte function at `0x8000A0CC`. Its verified
body reads HID0 with `PPCMfhid0`, sets HID0 mask `0x200` using `ori r3,r3,0x200`,
and writes HID0 with `PPCMthid0`. It has `R_PPC_REL24` relocations to those two
helpers. The two functions are therefore adjacent initialization steps with distinct
state domains:

| Function | Hardware state | Operation | Direct callees | Relocations |
|---|---|---|---|---|
| `PPCDisableSpeculation` | HID0 | read, set `0x200`, write | `PPCMfhid0`, `PPCMthid0` | two `R_PPC_REL24` calls |
| `PPCSetFpNonIEEEMode` | FPSCR | set bit 29 (`mtfsb1 29`) | none | none |

There is no HID0/HID2 access in `PPCSetFpNonIEEEMode`. There is also no MSR access:
`PPCMfmsr`/`PPCMtmsr` are separate OSPPC/OSCache helper paths and do not appear in
the target's xrefs or instruction body. Likewise, `PPCMffpscr` and `PPCMtfpscr` are
separate full-register FPSCR accessors; they are not callees of this bit-setting
leaf.

## Disposition

The static evidence establishes the exact address, size, bytes, instruction semantics,
void/no-argument ABI, sole caller, empty relocation/callee/global set, FPSCR-versus-HID0
relationship, and absence of an MSR dependency. It does not provide a natural-C twin
or verified runtime fact. No source conversion is authorized: no candidate was
generated, compiled, submitted, gated, or landed, and no queue, lease, runtime, or fleet
state was changed.
