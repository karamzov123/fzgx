# Finding 268 — ADXT unit initializer (`fn_800570DC`) evidence dossier

- Unit: `main/game/adxt_800570DC`
- Symbol: `fn_800570DC`
- Address: `0x800570DC`
- Evidence scope: static only
- Evidence action: reference manifest, relocation-aware xrefs, exact context-only disassembly, object symbol/relocation inspection, and adjacent-call/lifecycle inspection
- Conversion/lease/candidate/compile/submit/gate/runtime action: none

## Reproduction

Run from the repository root:

```sh
python3 tools/natc_reference_manifest.py --unit main/game/adxt_800570DC
python3 tools/find_xrefs.py --cslice fn_800570DC --json
python3 tools/natc_loop.py --unit main/game/adxt_800570DC --symbol fn_800570DC --context-only
readelf -sW build/GFZE01/obj/game/adxt_800570DC.o
readelf -r build/GFZE01/obj/game/adxt_800570DC.o
```

These are read-only evidence commands. The target checkout was dirty before this
finding; only this dossier is included in the commit.

## Identity and reference status

The reference manifest identifies `src/game/adxt_800570DC.c` and reports `41`
remaining asm functions, `reference_backed: 0`, and `fn_800570DC` with
`reference_count: 0`. The context-only readiness report is `blocked-evidence`
because there is no natural-C reference body and no verified runtime fact. This
dossier records the static behavior; it does not claim conversion readiness.

The canonical symbol table records:

```text
fn_800570DC = .text:0x800570DC; // type:function size:0x30
```

`readelf -sW` independently reports a global `.text` function at section-relative
value `0`, size `48` bytes. The target object is
`build/GFZE01/obj/game/adxt_800570DC.o`. The coarse mirror contains an identical
symbol at `build/GFZE01/obj/coarse/text_80053EA0.o`, value `0x323c` within that
object, size `48` bytes.

## Exact body

The target and coarse disassembly agree byte-for-byte on this 0x30-byte body:

```text
0x800570DC  stwu r1, -0x10(r1)
0x800570E0  mflr r0
0x800570E4  lis r6, 0x10
0x800570E8  li r5, 0
0x800570EC  stw r0, 0x14(r1)
0x800570F0  addi r7, r6, -1
0x800570F4  li r6, 0
0x800570F8  bl gcci_register_filename
0x800570FC  lwz r0, 0x14(r1)
0x80057100  mtlr r0
0x80057104  addi r1, r1, 0x10
0x80057108  blr
```

The source definition at `src/game/adxt_800570DC.c:62-77` preserves this same
instruction sequence. The call at `0x800570F8` is the only call in the body;
there are no conditional branches, global-address materializations, or local
state stores other than the link-register spill.

## ABI-level behavior

The function has no declared parameters in the current source spelling, but the
PowerPC body consumes the incoming values already present in `r3` and `r4` and
constructs a four-argument call as follows:

| Call register | Value at `gcci_register_filename` call | Evidence-bounded role |
|---|---:|---|
| `r3` | incoming `r3` | forwarded unchanged; likely owning ADXT context/record, semantic type not proven here |
| `r4` | incoming `r4` | forwarded unchanged; callee treats it as a non-null string/data pointer in its own body |
| `r5` | `0` | explicitly supplied constant |
| `r6` | `0` | explicitly supplied constant |
| `r7` | `0x000FFFFF` | `lis r6,0x10; addi r7, r6, -1`, then `r6` is cleared |

The wrapper does not inspect the callee return value. It restores `lr`, pops
its 16-byte stack frame, and returns `void` to its caller. The stack allocation
is sufficient for the saved link register at `0x14(r1)` (the normal PPC frame
area is above the allocated local area).

The `r7` value is specifically `0xFFFF`, not `-1`: `lis r6, 0x10` yields
`0x00100000`, and `addi r7, r6, -1` yields `0x000FFFFF`? This instruction
encoding's disassembly is authoritative: `lis r6, 0x10` loads `0x00100000`, so
`addi r7,r6,-1` yields `0x000FFFFF`. Thus the forwarded fourth auxiliary value
is `0x000FFFFF` (the source-level equivalent is `0xFFFFF`), while the fifth and
sixth auxiliary values are zero. No normalization to `0xFFFF` is justified.

## Callee and indirect global-state effect

`find_xrefs.py --cslice fn_800570DC --json` reports:

```text
callers: []
callees: [gcci_register_filename]
globals_referenced: []
reloc_histogram: {}
```

The empty caller set is also consistent with a repository-wide search: the only
references to `fn_800570DC` are its target/coarse definitions and the source
definition; no `bl fn_800570DC` or data reference was found. This means the
function is not statically shown as reached by a relocatable caller in the
indexed objects. It does not prove that no external handoff or linker-rooted
entry can reach it.

The function has no direct global references and no data relocations. Its one
text relocation is:

```text
0000001c  R_PPC_REL24  gcci_register_filename + 0
```

This relocation is at function-relative offset `0x1c`, exactly the call
instruction at `0x800570F8`. `readelf -sW` marks `gcci_register_filename` as an
undefined symbol in this object, so the call is resolved across the split source
unit rather than through a local definition.

The callee's source body (`src/game/adxt_80055708.c:1867` onward) shows why the
wrapper is state-adjacent but not itself a global initializer: it validates the
forwarded `r3` record and `r4` pointer, derives per-record offsets from the
record's `+0x24` field, writes a filename/metadata entry into the record-relative
area, and computes/stores a string length/checksum-like value. Those effects are
performed by `gcci_register_filename`; they must not be attributed to direct
writes by `fn_800570DC`.

## Relation to ADXT `ProcessStreamUpdate` and critical-section facts

`ADXT_ProcessStreamUpdate` begins at `0x80058498`, size `0x198`, in the same
unit. Its exact body (Finding 264) enters `svmEnterCritical`, scans the global
ADXT stream pool in 0x40-byte slots, initializes a selected slot, and exits via
`svmExitCritical`. The critical-section pair is therefore a property of the
stream-slot allocator/update path, not of `fn_800570DC`:

- `fn_800570DC` has no `svmEnterCritical`/`svmExitCritical` relocation.
- `fn_800570DC` has no global pool address relocation and performs no slot scan.
- `ADXT_ProcessStreamUpdate` has explicit critical-section call relocations and
  direct stores into the pool-backed slot.
- The two functions are adjacent within the same ADXT compilation unit but are
  not statically connected by a direct call edge in the indexed object graph.

The nearby `fn_8005710C` at `0x8005710C` is a separate 8-byte leaf that stores
`r4` at `0x28(r3)`; it is not part of `fn_800570DC` despite the adjacent address.
Likewise, adjacency to `ADXT_ProcessStreamUpdate` does not establish an
initializer/lifecycle call relationship. The defensible relationship is only
unit-level: the wrapper forwards registration metadata into the ADXT support
code, while `ProcessStreamUpdate` owns the separately evidenced synchronized
stream-slot state transition.

## Findings and limits

1. `fn_800570DC` is an exact 48-byte, single-call forwarding wrapper.
2. Its only direct callee is `gcci_register_filename`; it has no direct global
   state access and no direct critical-section operation.
3. The wrapper supplies constants `r5=0`, `r6=0`, and `r7=0x000FFFFF`, while
   forwarding incoming `r3`/`r4` unchanged.
4. No relocatable callers are present in the indexed repository objects, so a
   caller/lifecycle claim beyond the callee edge would be speculation.
5. The static evidence does not justify a natural-C conversion candidate: the
   manifest and context-only readiness both report blocked evidence. The
   dossier intentionally leaves the source as asm and makes no lease or queue
   changes.
