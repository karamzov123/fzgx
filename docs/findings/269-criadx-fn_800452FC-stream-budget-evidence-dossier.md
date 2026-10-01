# CRIADX `fn_800452FC` stream-budget getter evidence dossier

- Findings number: `269`
- Unit: `main/game/criadx_800452FC`
- Source: `src/game/criadx_800452FC.c`
- Target: `fn_800452FC`
- Evidence action: exact-body extraction, relocation-aware xrefs, and caller data-flow inspection in both CRIADX decode loops
- Conversion attempt: none

## Scope and reproducibility

All observations were reproduced from the canonical worktree with:

```text
python3 tools/natc_reference_manifest.py --unit main/game/criadx_800452FC
python3 tools/find_xrefs.py --cslice fn_800452FC --json
python3 tools/natc_loop.py --unit main/game/criadx_800452FC --symbol fn_800452FC --context-only
llvm-objdump -dr --disassemble-symbols=fn_800452FC build/GFZE01/obj/game/criadx_800452FC.o
```

The manifest reports 34 assembly functions in this unit, `reference_count: 0` for this symbol, and `reference_backed: 0`. The context-only run reports `blocked-evidence`: no natural-C reference body and no identity-verified runtime fact. No source conversion, candidate generation, compilation, submission, gate, lease, or fleet operation was performed.

## Exact target definition

The canonical object defines one global `.text` function at unit-relative value `0x00`, corresponding to address `0x800452FC`, with size `0x8` bytes. The coarse object contains one matching global duplicate of size `0x8` at value `0x3e9c` (`16028`). The exact body is:

```text
0x00  lwz r3, 0x90(r3)
0x04  blr
```

The target takes one incoming object pointer in `r3`, loads a 32-bit word at offset `0x90`, and returns that word. There is no frame, call, branch, relocation, or global reference. The direct source definition remains an inline-assembly reconstruction at `src/game/criadx_800452FC.c:43-48`.

A bounded C shape is therefore:

```c
/* Semantic field name and declared type remain unresolved. */
return *(unsigned int *)((char *)arg0 + 0x90);
```

This is an ABI/offset transcription only, not a source-conversion proposal.

## Xrefs and caller data flow

`find_xrefs.py --cslice fn_800452FC --json` reports two callers and no callees or globals:

```text
callers: fn_80041700, fn_80041990
callees: (none)
globals: (none)
relocation histogram: (none)
```

Both callers use the same inner CRIADX object obtained from the outer object's `+0x04` pointer and the same decode-loop sequence. In `fn_80041700` (`src/game/criadx_80041460.c:328-348`):

```text
inner = *(outer + 0x04)
r27 = criadx_get_stream_ptr(inner)       // inner + 0x18
r30 = fn_80045304(inner)                 // inner + 0x94
r3  = fn_800452FC(inner)                 // inner + 0x90
r27 = r27 - *(outer + 0x34)
if (r3 < r27) r27 = r3
svm_ringbuf_read(outer + 0x14, r30, ...)
slwi loop_step, r27, 1
```

The parallel path in `fn_80041990` (`src/game/criadx_80041460.c:502-522`) repeats the same operations with the same offsets and comparison. After the read, each loop consumes `2 * r27` bytes, iterating once per signed `inner+0x0E` channel count (`:539-579`), and advances the outer counters by the clamped `r27` value (`:580-595`).

This gives the unique, caller-backed role evidence absent from the adjacent getter-family dossier: `inner+0x90` is the per-call upper bound for the amount of stream data consumed by the CRIADX decode loops. The actual read amount is the lesser of this field and the available span computed as `inner+0x18 - (outer+0x34)`. The sibling `inner+0x94` value is passed separately as the ring-buffer read argument and accumulated independently, so `+0x90` is not merely another stream pointer/value getter. Static evidence does not establish whether the field is a byte count, frame count, signed quantity, ownership-managed value, or a specific C typedef; the `lwz` and compare establish only a 32-bit word used as this upper bound.

## Relation to existing CRIADX evidence

The getter-family dossier (finding `262`) records `+0x18`, `+0x0E`, `+0x14`, and `+0x98`, and mentions that `fn_80041700`/`fn_80041990` call this target. It does not record the target's exact body or the two-call-site clamp/data-flow proof for `+0x90`. Findings `264`, `267`, and `268` cover format dispatch, FORM parsing, and SPSD probing; none covers this stream-consumption bound.

The result remains compatible with the shared inner-object handoff established by finding `262`: the target receives the inner pointer, while the available-span baseline is held in the outer object at `+0x34`. This dossier intentionally does not infer a complete struct declaration or parser ownership model.

## Disposition

The exact 8-byte body, address/size, caller set, coarse duplicate, and two independent decode-loop uses establish a reusable static fact: `fn_800452FC` returns the inner-object `+0x90` word as the decode stream read-budget cap. This is non-redundant evidence beyond the existing format-dispatch/probe/getter/FORM dossiers, but it does not justify natural-C generation because no reference body or verified runtime fact exists. The target remains `blocked-evidence`.

No candidate was generated, compiled, submitted, gated, or landed. This commit contains only this evidence dossier.
