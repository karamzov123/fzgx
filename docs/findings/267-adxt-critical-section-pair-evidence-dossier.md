# Finding 267 — `svmEnterCritical` / `svmExitCritical`: ADXT critical-section pair evidence dossier

- Findings number: `267`
- Unit: `main/game/adxt_800570DC`
- Symbols: `svmEnterCritical`, `svmExitCritical`
- Paired relationship target: `ADXT_ProcessStreamUpdate`
- Evidence action: static reference manifest, relocation-aware xrefs, exact context-only disassembly, source/symbol inspection, and caller/nesting analysis
- Conversion attempt: none

## Scope and reproduction

Run from the repository root:

```sh
python3 tools/natc_reference_manifest.py --unit main/game/adxt_800570DC
python3 tools/find_xrefs.py --cslice svmEnterCritical --json
python3 tools/find_xrefs.py --cslice svmExitCritical --json
python3 tools/natc_loop.py --unit main/game/adxt_800570DC --symbol svmEnterCritical --context-only
python3 tools/natc_loop.py --unit main/game/adxt_800570DC --symbol svmExitCritical --context-only
readelf -r build/GFZE01/obj/game/adxt_800570DC.o
```

These are read-only evidence actions. They do not generate a candidate or change source,
queue, lease, runtime, submission, or gate state.

## Unit and symbol identity

The reference manifest reports `src/game/adxt_800570DC.c` with `41` functions remaining
in assembly and `reference_backed: 0`; both critical-section symbols have
`reference_count: 0`. Their context-only readiness verdict is `blocked-evidence` because
no usable natural-C reference body or identity-verified runtime fact is available.

The canonical symbol table (`config/GFZE01/symbols.txt:1421-1422`) identifies:

| Symbol | Retail address | Object-relative value | Size |
|---|---:|---:|---:|
| `svmExitCritical` | `0x800576DC` | `0x600` (`1536`) | `0x4C` (`76`) |
| `svmEnterCritical` | `0x80057728` | `0x64C` (`1612`) | `0x4C` (`76`) |

Both are global `.text` functions in `build/GFZE01/obj/game/adxt_800570DC.o`. The
relocation-aware xref reports each with one equal-size global alternate definition in
`build/GFZE01/obj/coarse/text_80053EA0.o`:

- `svmExitCritical`: alternate value `14396`, size `76`.
- `svmEnterCritical`: alternate value `14472`, size `76`.

## Exact body: `svmEnterCritical`

The context-only disassembly reproduces the complete `0x4C`-byte body:

```text
FUNCTION svmEnterCritical  size 76 B (0x4c)  section .text
0x00  stwu r1, -0x10(r1)
0x04  mflr r0
0x08  lis r3, 0
0x0C  stw r0, 0x14(r1)
0x10  lwz r0, 0(r3)
0x14  cmpwi r0, 0
0x18  bne 0x28
0x1C  bl 0x1c   R_PPC_REL24 OSDisableInterrupts +0
0x20  lis r4, 0
0x24  stw r3, 0(r4)
0x28  lis r3, 0
0x2C  addi r4, r3, 0
0x30  lwz r3, 0(r4)
0x34  addi r0, r3, 1
0x38  stw r0, 0(r4)
0x3C  lwz r0, 0x14(r1)
0x40  mtlr r0
0x44  addi r1, r1, 0x10
0x48  blr
```

The source definition is `src/game/adxt_800570DC.c:533-556`. The function uses a
`0x10`-byte stack frame, checks the nesting counter at `lbl_8018AE10`, and, only when
that counter is zero, calls `OSDisableInterrupts` and stores the returned interrupt token
in `lbl_8018AE14`. It then increments `lbl_8018AE10` and returns. The static body proves
an outermost-entry-only interrupt disable and a nesting-depth increment; it does not
prove a source-level type beyond the observed four-byte storage and ABI use.

## Exact body: `svmExitCritical`

The context-only disassembly reproduces the complete `0x4C`-byte body:

```text
FUNCTION svmExitCritical  size 76 B (0x4c)  section .text
0x00  stwu r1, -0x10(r1)
0x04  mflr r0
0x08  lis r3, 0
0x0C  stw r0, 0x14(r1)
0x10  addi r4, r3, 0
0x14  lwz r3, 0(r4)
0x18  addi r0, r3, -1
0x1C  stw r0, 0(r4)
0x20  lwz r0, 0(r4)
0x24  cmpwi r0, 0
0x28  bne 0x3c
0x2C  lis r3, 0
0x30  addi r3, r3, 0
0x34  lwz r3, 0(r3)
0x38  bl 0x38   R_PPC_REL24 OSRestoreInterrupts +0
0x3C  lwz r0, 0x14(r1)
0x40  mtlr r0
0x44  addi r1, 0x10
0x48  blr
```

The source definition is `src/game/adxt_800570DC.c:508-531`. The function decrements
`lbl_8018AE10`; only when the resulting value is zero does it load the saved token from
`lbl_8018AE14` and call `OSRestoreInterrupts`. It then returns through the same `0x10`-byte
frame. The static body proves an outermost-exit-only interrupt restore paired with the
counter decrement. (The context printer's `addi` epilogue line omits the destination
register in this presentation; the source and surrounding instruction encoding identify
it as `addi r1, r1, 0x10`.)

## Shared state, nesting, and pair invariant

Both functions access the same two `.bss` objects as 32-bit words:

- `lbl_8018AE10`: nesting/depth state, read and written by both functions.
- `lbl_8018AE14`: saved `OSDisableInterrupts` return token, written by enter on a
  zero-to-one transition and read by exit on a one-to-zero transition.

The bounded state transition established by the instructions is:

| Entry state | `svmEnterCritical` effect | `svmExitCritical` effect |
|---|---|---|
| counter `0` | call `OSDisableInterrupts`, save token, set counter to `1` | decrement to `0`, restore saved token |
| counter `>0` | skip disable, increment counter | decrement nonzero, skip restore |

This is a nesting counter, not a per-level token stack: the sole saved token is retained
until the outermost exit. The evidence establishes the intended matched-pair invariant
for balanced calls. It does not establish behavior for underflow, unbalanced calls, or
interrupt-state corruption; those cases are not normalized by either body.

## Relocations, globals, and callees

`find_xrefs.py --json` reports the following direct dependencies:

| Symbol | Direct callee | Globals | Relocation histogram |
|---|---|---|---|
| `svmEnterCritical` | `OSDisableInterrupts` | `lbl_8018AE10`, `lbl_8018AE14` | `R_PPC_ADDR16_HA=2`, `R_PPC_ADDR16_LO=2`, `R_PPC_REL24=1` |
| `svmExitCritical` | `OSRestoreInterrupts` | `lbl_8018AE10`, `lbl_8018AE14` | `R_PPC_ADDR16_HA=2`, `R_PPC_ADDR16_LO=2`, `R_PPC_REL24=1` |

The corresponding object relocation records (the relocation applies to the
instruction's immediate field) are:

- Enter: `0x656/0x65E` for `lbl_8018AE10`, `0x66E/0x672` for
  `lbl_8018AE14`, and `0x676/0x67A` for `lbl_8018AE10`.
- Exit: `0x606/0x612` for `lbl_8018AE10`, and `0x62E/0x632` for
  `lbl_8018AE14`.

The authoritative call relocations are:

```text
0x00000668  R_PPC_REL24  OSDisableInterrupts + 0
0x00000638  R_PPC_REL24  OSRestoreInterrupts + 0
```

The target object's exact critical-pair call relocations are all `R_PPC_REL24` and are
listed in the caller table below. No SDA21 relocation is used by either pair.

## Callers and call sites

Each xref report identifies exactly these 15 callers (the two wrapper names are also
present in the object-level report):

`ADXT_ProcessStreamUpdate`, `fn_80057774`, `fn_8005782C`, `fn_8005795C`,
`fn_800579F0`, `fn_80057DB8`, `fn_80057EC4`, `fn_80058070`, `fn_800581CC`,
`fn_800583DC`, `fn_80058448`, `fn_80058630`, `fn_80058680`, plus
`gccicrit_enter`/`svm_enter_critical_wrapper` for enter and
`gccicrit_leave`/`svm_exit_critical_wrapper` for exit. Thus the named direct callers are
13 shared ADXT helpers plus the corresponding low-level wrapper aliases and
`ADXT_ProcessStreamUpdate`.

For the functions in this unit, the relocation records establish these critical-pair
call sites (offsets are relative to each caller's function start):

| Caller | `svmEnterCritical` | `svmExitCritical` |
|---|---:|---:|
| `fn_80057774` | `0x2C` | `0x88` |
| `fn_8005782C` | `0x40` | `0x110` |
| `fn_8005795C` | `0x40` | `0x74` |
| `fn_800579F0` | `0x2C` | `0xC4` |
| `fn_80057DB8` | `0x2C` | `0xDC` |
| `fn_80057EC4` | `0x40` | `0x18C` |
| `fn_80058070` | `0x40` | `0x13C` |
| `fn_800581CC` | `0x2C` | `0x190` |
| `fn_800583DC` | `0x14` | `0x40` |
| `fn_80058448` | `0x14` | `0x38` |
| `ADXT_ProcessStreamUpdate` | `0x28`, `0x140` | `0x16C`, `0x170` |
| `fn_80058630` | `0x0C` | `0x3C` |
| `fn_80058680` | `0x14` | `0x50` |

The table is derived from the complete `readelf -r` relocation listing and the object
symbol ranges; all offsets are exact caller-relative call-instruction offsets.

## Relationship to `ADXT_ProcessStreamUpdate`

`ADXT_ProcessStreamUpdate` is `0x80058498`, size `0x198` (`408`) per the symbol table and
Finding 264. Its exact relocations call:

- `svmEnterCritical` at function-relative `0x28` and `0x140`;
- `svmExitCritical` at function-relative `0x16C` and `0x170`.

The first enter at `0x28` precedes the pool scan and allocation. If a free slot is found,
the function initializes the selected `0x40`-byte pool record, then enters again at `0x140`
to perform the reset writes at slot offsets `+0x0C`, `+0x14`, `+0x18`, `+0x28`, `+0x2C`,
`+0x30`, and `+0x34`, while copying the saved `+0x20` value to `+0x10`. It exits the
inner section at `0x16C` and exits the outer section at `0x170`. On pool exhaustion it
returns null after the outer exit. Therefore the pair's counter semantics explain the
nested call sequence: the inner exit decrements but does not restore interrupts, and the
final outer exit restores the token saved by the first enter.

This establishes a static synchronization relationship, not a claim about scheduler
semantics or runtime timing. The surrounding ADXT lifecycle evidence shows the returned
slot is later used by callers and that the stream-update function initializes and clears
its slot state; those facts do not add a natural-C reference body for either critical
function.

## Disposition and quality gate

The static action establishes both exact `0x4C` bodies, retail addresses, object identity,
shared global state, outermost disable/restore behavior, nesting transitions, direct
callees, relocation classes, caller sets, and the complete nested relationship to
`ADXT_ProcessStreamUpdate`. Both targets remain `blocked-evidence`: no natural-C twin and
no identity-verified runtime fact were found. No candidate was generated, compiled,
submitted, gated, or converted.

Only this dossier is intended to be committed; no source, lease, candidate, queue, runtime,
or fleet state is part of the change.
