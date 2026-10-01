# Finding 264 — `ADXT_ProcessStreamUpdate`: stream-slot allocator/update evidence dossier

- Unit: `main/game/adxt_800570DC`
- Symbol: `ADXT_ProcessStreamUpdate`
- Evidence action: static manifest, relocation-aware xrefs, exact context-only disassembly, object relocation inspection, and caller/lifecycle inspection
- Conversion attempt: none

## Reproduction

Run from the repository root:

```sh
python3 tools/natc_reference_manifest.py --unit main/game/adxt_800570DC
python3 tools/find_xrefs.py --cslice ADXT_ProcessStreamUpdate --json
python3 tools/natc_loop.py --unit main/game/adxt_800570DC --symbol ADXT_ProcessStreamUpdate --context-only
readelf -r build/GFZE01/obj/game/adxt_800570DC.o
```

These commands were run against the checkout while generating this dossier. They are
read-only evidence actions; no candidate, compiler, queue, lease, runtime, or gate
action was run.

## Identity and reference status

The reference manifest reports `src/game/adxt_800570DC.c` with `41` functions remaining
in asm, `reference_backed: 0`, and `ADXT_ProcessStreamUpdate` with `reference_count: 0`.
No usable natural-C reference body was found. The context-only readiness verdict is
`blocked-evidence` because there is neither a natural-C reference nor a verified runtime
fact.

The canonical symbol table identifies:

```text
ADXT_ProcessStreamUpdate = .text:0x80058498; // type:function size:0x198
```

The target object is `build/GFZE01/obj/game/adxt_800570DC.o`; its function-relative
value is `0x13bc` (`5052`), section `.text`, global function (`bind: 1`, `stype: 2`),
and its exact size is `408` bytes (`0x198`). One alternate definition is present in
`build/GFZE01/obj/coarse/text_80053EA0.o`, section `.text`, value `17912`, also size
`408`.

## Exact body

`natc_loop.py --context-only` reproduces this exact 408-byte body:

```text
FUNCTION ADXT_ProcessStreamUpdate  size 408 B (0x198)  section .text
0x00  stwu r1, -0x20(r1)
0x04  mflr r0
0x08  stw r0, 0x24(r1)
0x0C  stw r31, 0x1c(r1)
0x10  mr r31, r5
0x14  stw r30, 0x18(r1)
0x18  mr r30, r4
0x1C  stw r29, 0x14(r1)
0x20  mr r29, r3
0x24  stw r28, 0x10(r1)
0x28  bl 0x28   R_PPC_REL24 svmEnterCritical +0
0x2C  lis r3, 0
0x30  li r0, 0x20
0x34  addi r3, r3, 0
0x38  li r4, 0
0x3C  mtctr r0
0x40  lwz r0, 4(r3)
0x44  cmpwi r0, 0
0x48  beq 0xe4
0x4C  lwz r0, 0x44(r3)
0x50  addi r4, r4, 1
0x54  addi r3, r3, 0x40
0x58  cmpwi r0, 0
0x5C  beq 0xe4
0x60  lwz r0, 0x44(r3)
0x64  addi r4, r4, 1
0x68  addi r3, r3, 0x40
0x6C  cmpwi r0, 0
0x70  beq 0xe4
0x74  lwz r0, 0x44(r3)
0x78  addi r4, r4, 1
0x7C  addi r3, r3, 0x40
0x80  cmpwi r0, 0
0x84  beq 0xe4
0x88  lwz r0, 0x44(r3)
0x8C  addi r4, r4, 1
0x90  addi r3, r3, 0x40
0x94  cmpwi r0, 0
0x98  beq 0xe4
0x9C  lwz r0, 0x44(r3)
0xA0  addi r4, r4, 1
0xA4  addi r3, r3, 0x40
0xA8  cmpwi r0, 0
0xAC  beq 0xe4
0xB0  lwz r0, 0x44(r3)
0xB4  addi r4, r4, 1
0xB8  addi r3, r3, 0x40
0xBC  cmpwi r0, 0
0xC0  beq 0xe4
0xC4  lwz r0, 0x44(r3)
0xC8  addi r4, r4, 1
0xCC  addi r3, r3, 0x40
0xD0  cmpwi r0, 0
0xD4  beq 0xe4
0xD8  addi r3, r3, 0x40
0xDC  addi r4, r4, 1
0xE0  bdnz 0x40
0xE4  cmpwi r4, 0x100
0xE8  bne 0xf4
0xEC  li r28, 0
0xF0  b 0x170
0xF4  lis r3, 0
0xF8  lis r5, 0
0xFC  slwi r6, r4, 6
0x100  lis r4, 0
0x104  addi r3, r3, 0
0x108  li r0, 1
0x10C  add r28, r3, r6
0x110  lis r3, 0
0x114  stw r0, 4(r28)
0x118  addi r5, r5, 0
0x11C  addi r4, r4, 0
0x120  addi r0, r3, 0
0x124  stw r5, 0(r28)
0x128  stw r29, 0x1c(r28)
0x12C  stw r30, 0x20(r28)
0x130  stw r31, 0x24(r28)
0x134  stw r4, 8(r28)
0x138  stw r0, 0x38(r28)
0x13C  stw r28, 0x3c(r28)
0x140  bl 0x140   R_PPC_REL24 svmEnterCritical +0
0x144  li r3, 0
0x148  stw r3, 0xc(r28)
0x14C  lwz r0, 0x20(r28)
0x150  stw r0, 0x10(r28)
0x154  stw r3, 0x14(r28)
0x158  stw r3, 0x18(r28)
0x15C  stw r3, 0x28(r28)
0x160  stw r3, 0x2c(r28)
0x164  stw r3, 0x30(r28)
0x168  stw r3, 0x34(r28)
0x16C  bl 0x16c   R_PPC_REL24 svmExitCritical +0
0x170  bl 0x170   R_PPC_REL24 svmExitCritical +0
0x174  lwz r0, 0x24(r1)
0x178  mr r3, r28
0x17C  lwz r31, 0x1c(r1)
0x180  lwz r30, 0x18(r1)
0x184  lwz r29, 0x14(r1)
0x188  lwz r28, 0x10(r1)
0x18C  mtlr r0
0x190  addi r1, r1, 0x20
0x194  blr
```

The source definition at `src/game/adxt_800570DC.c:1592-1701` preserves the same
instruction sequence. The disassembly listing above is authoritative for control flow;
the apparent one-instruction label displacement in the generated context output is a
printer presentation detail around the branch at `0xe4`.

## State fields and bounded operation

The function takes three incoming words in `r3`, `r4`, and `r5`; it preserves them in
`r29`, `r30`, and `r31`. Under the outer `svmEnterCritical`, it scans the global pool
`lbl_8018B2A4` in `0x40`-byte slots. The first slot's `word +0x04` is tested, then up to
seven following slots' `word +0x44` values are tested as the scan advances. The loop
counter is `0x20`, and the accumulated count is compared with `0x100`, establishing a
maximum of `0x100` counted records and a pool span of `0x4000` bytes (consistent with
the declared `lbl_8018B2A4[16388]`, including the four-byte boundary/adjacency detail
reported by the source declarations).

If the count reaches `0x100`, `r28` is set to null and the function returns null after
`svmExitCritical`. Otherwise it computes the selected slot as
`&lbl_8018B2A4 + (count << 6)` and initializes it:

| Slot offset | Operation | Evidence-bounded interpretation |
|---:|---|---|
| `+0x00` | store `&lbl_80132300` | callback/table-like pointer; semantic type unproven |
| `+0x04` | store `1` | marks slot active/nonzero |
| `+0x08` | store `&lbl_80092330` | pointer-like field; semantic type unproven |
| `+0x1c` | store saved `r3` | first caller-supplied word |
| `+0x20` | store saved `r4` | second caller-supplied word; then copied to `+0x10` |
| `+0x24` | store saved `r5` | third caller-supplied word |
| `+0x38` | store `&fn_800586E4` | error/callback-like pointer; semantic type unproven |
| `+0x3c` | store slot address | self/back-pointer-like field |
| `+0x0c` | store `0` | reset |
| `+0x10` | store prior `+0x20` | mirrors the second input |
| `+0x14` | store `0` | reset |
| `+0x18` | store `0` | reset |
| `+0x28` | store `0` | reset; this is the field used by the existing setter dossier |
| `+0x2c` | store `0` | reset |
| `+0x30` | store `0` | reset |
| `+0x34` | store `0` | reset |

The inner critical-section pair surrounds the reset writes. The outer pair surrounds
both the pool scan and the allocation/update, so the successful path has a nested
enter/exit followed by the final outer exit. The return value is the selected slot
address on success and null on pool exhaustion.

## Relocations, globals, and callees

`find_xrefs.py --json` reports two direct callees, three direct callers, and four
address-relocation global references:

- Callees: `svmEnterCritical`, `svmExitCritical`.
- Globals: `lbl_8018B2A4` (`.bss`, 16388 bytes), `lbl_80132300` (`.data`, 48 bytes),
  `lbl_80092330` (`.rodata`, 16 bytes), and `fn_800586E4` (`.text`, 40 bytes).
- Relocation histogram: `R_PPC_ADDR16_HA=4`, `R_PPC_ADDR16_LO=4`.

The function-relative address-forming pairs are:

| Function-relative instruction pair | Symbol | Relocation types |
|---|---|---|
| `0x2c/0x34` | `lbl_8018B2A4` | `R_PPC_ADDR16_HA` + `R_PPC_ADDR16_LO` |
| `0xf4/0x104` | `lbl_8018B2A4` | `R_PPC_ADDR16_HA` + `R_PPC_ADDR16_LO` |
| `0xf8/0x118` | `lbl_80132300` | `R_PPC_ADDR16_HA` + `R_PPC_ADDR16_LO` |
| `0x100/0x11c` | `lbl_80092330` | `R_PPC_ADDR16_HA` + `R_PPC_ADDR16_LO` |
| `0x110/0x120` | `fn_800586E4` | `R_PPC_ADDR16_HA` + `R_PPC_ADDR16_LO` |

The first table row is the scan base; the second is the selected-slot base. The
object relocation records are at offsets `0x13ea/0x13f2`, `0x14b2/0x14c2`,
`0x14b6/0x14d6`, `0x14be/0x14da`, and `0x14ce/0x14de`, respectively, relative to the
object's `.text` section. The four call relocations are `R_PPC_REL24` to
`svmEnterCritical` at function-relative `0x28` and `0x140`, and to `svmExitCritical`
at `0x16c` and `0x170`.

## Callers and lifecycle relationship

Relocation-aware xrefs identify exactly three callers:

- `ADXT_Create` in `src/game/adxt_8004CD70.c`. It computes three stream parameters,
  calls `ADXT_ProcessStreamUpdate` with them in `r3/r4/r5`, and stores the returned slot
  in the ADXT object's `+0x10`. A null return enters the existing cleanup path.
- `fn_8004768C` in `src/game/criadx_80047464.c`. Its stream-creation path calls the
  target and stores the returned slot in its owning record; null propagates to cleanup.
- `fn_8005B534` in `src/game/adxt_8005A24C.c`. The lifecycle/resource path also calls
  the target; the xref establishes the call but does not by itself establish higher-level
  semantic types for the three arguments.

The existing setter dossier (`findings/260-adxt-fn_8005710C-evidence-dossier.md`)
proves that `fn_8005710C` performs `stw r4, 0x28(r3); blr`, with `ADXT_Create` as its
sole caller. In `ADXT_Create`, the returned stream slot is passed as `r3` and the
owner's `+0x08` value as `r4`, so the bounded relationship is:
`ADXT_ProcessStreamUpdate` creates/initializes the slot and clears slot `+0x28`, then
`fn_8005710C` later writes the caller-provided word into that same offset. This dossier
does not prove whether that word is a voice pointer or another semantic type.

The existing cleanup dossier (`findings/262-adxt-fn_80057114-evidence-dossier.md`)
proves that `fn_80057114` operates on a different `0x238`-byte ADXT record: it may
consume that record's `+0x28`, clears selected fields, then clears the record with
`memset(..., 0x238)`. Thus the shared `+0x28` offset is an offset convention across two
ADXT object families, not proof that the pool slot and the cleanup record have the same
layout. `fn_800583DC` similarly resets selected fields in its own object, while
`fn_80058448` clears a `0x40`-byte object under a critical section; these neighboring
lifecycle helpers do not call the target. `fn_80058630` and `fn_80058680` clear the
whole `lbl_8018B2A4` pool under its lifecycle counter, providing corroborating pool
lifetime evidence without adding a direct xref to the target.

## Disposition

Static evidence establishes the exact `0x80058498` address, `0x198`/408-byte body,
three-input ABI, pool scan and `0x100` capacity test, per-slot state-field accesses,
four address-relocation pairs, four critical-section call relocations, two callees,
three callers, and the bounded setter/lifecycle relationships above. No natural-C twin
or verified runtime fact exists, so readiness remains `blocked-evidence`. No candidate
was generated, compiled, submitted, gated, or converted; no source, queue, lease, or
fleet state was changed.
