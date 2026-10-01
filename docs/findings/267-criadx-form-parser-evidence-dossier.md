# CRI FORM parser evidence dossier

- Findings number: `267`
- Unit: `main/game/criadx_800452FC`
- Source: `src/game/criadx_800452FC.c`
- Target: `CRI_FORM_parser`
- Evidence action: exact-body extraction, relocation-aware xrefs, parser phase/control-flow inspection, rodata/string inspection, and relation to CRIADX format dispatch/getter layout
- Conversion attempt: none

## Scope and reproducibility

All observations were reproduced from the canonical worktree. The bounded evidence commands were:

```text
python3 tools/natc_reference_manifest.py --unit main/game/criadx_800452FC
python3 tools/find_xrefs.py --cslice CRI_FORM_parser --json
python3 tools/natc_loop.py --unit main/game/criadx_800452FC --symbol CRI_FORM_parser --context-only
python3 tools/find_xrefs.py --cslice criadx_format_dispatch --json
python3 tools/find_xrefs.py --cslice ADXB_DecodeHeader --json
llvm-objdump -dr --disassemble-symbols=CRI_FORM_parser build/GFZE01/obj/game/criadx_800452FC.o
llvm-nm -S build/GFZE01/obj/coarse/rodata_8008FF40.o
strings -tx build/GFZE01/obj/coarse/rodata_8008FF40.o
```

The manifest reports 34 assembly functions in this unit, `reference_count: 0` for `CRI_FORM_parser`, and `reference_backed: 0`. The context-only run reports `blocked-evidence`: no natural-C reference body and no identity-verified runtime fact. No source conversion, candidate generation, compilation, submission, gate, lease, or fleet operation was performed.

## Exact target definition

The relocation-aware object index identifies one canonical definition and one matching coarse duplicate:

| Definition | Section | Binding | Unit-relative value | Size |
|---|---|---:|---:|---:|
| `build/GFZE01/obj/game/criadx_800452FC.o` | `.text` | global (`bind: 1`, `stype: 2`) | `0x3a4` (`932`) | `0x69c` (`1692`) |
| `build/GFZE01/obj/coarse/text_80041460.o` | `.text` | global (`bind: 1`, `stype: 2`) | `0x4240` (`16960`) | `0x69c` (`1692`) |

The live source definition is `src/game/criadx_800452FC.c:419-650` and is an inline-assembly reconstruction. `llvm-objdump -dr` verifies the body from target offset `0x00` through `0x698`, followed by `blr` at `0x698`. The exact body is preserved in the source and object; its semantically bounded phases are:

1. **Prologue and header decode (`0x00-0x64`)**: allocates `0x50` bytes, saves LR and `r26-r31`, preserves incoming `r3/r4/r5` as `r29/r30/r31`, forms `lbl_800900A0` and `lbl_8017A288`, stores `1` at `arg0+0x02`, and calls `fn_80046C28` with the input/context plus pointers to `arg0+0x0c`, `+0x0d`, `+0x0f`, `+0x0e`, `+0x14`, and stack `sp+0x12`. A negative result returns `0`.
2. **Unsupported/high-channel fallback (`0x78-0xf4`)**: if signed `arg0+0x0c > 4`, checks `arg0+0xb0`; if null, reports an error using `lbl_800900A0+0x838` and `+0x858`, returns `-1`; otherwise initializes the FORM state (`+0x0d=8`, `+0x0f=(u8)(+0x0e*0xc0)`, `+0x10=0x60`, `+0x98=10`, clears `+0x1c/+0x20/+0x24/+0x26/+0x28/+0x2c/+0x30/+0x34/+0x88`) and joins the finalization block.
3. **Secondary header decode (`0xf8-0x140`)**: calls `fn_80046B90(r30,r31,&sp+0x11,&sp+0x10)`; a negative result returns `0`. Channel count below four selects zero coefficient lanes. For sample-width/format `sp+0x10 >= 0x10`, it calls `sprintf` with `lbl_800900A0+0x830`, increments the one-time counter at `lbl_8017A288+0` when zero, then executes the fixed-point coefficient/table transform over the decoded bytes. For `8 <= sp+0x10 < 0x10`, it lazily copies three signed halfwords from `lbl_8017A288+0xc/+0xe/+0x10` into `arg0+0x9c/+0x9e/+0xa0` when all three are zero; otherwise, or for widths below eight, it selects the corresponding stored/zero coefficient lanes.
4. **FORM state/application (`0x574-0x638`)**: the encoded `0 < 0` test is never taken in the exact body. It calls `fn_8004E2CC(arg0+0x08,var_r5,var_r6)`, then `fn_80046BE0(r30,r31,arg0+0x1c)` and returns `0` on failure; calls `fn_80046AC0(r30,r31,&sp+0x18,&sp+0x14)` and likewise returns `0` on failure; calls `fn_8004E324(arg0+0x08,arg0+0x14,arg0+0x1c)`, `fn_8004E300(arg0+0x08,&sp+0x18,&sp+0x14)`, `fn_800469A4` with output pointers `arg0+0x20/+0x24/+0x26/+0x28/+0x2c/+0x30`, and `fn_8004683C` with `arg0+0xbc/+0xc0/+0xd0/+0xd2`.
5. **Finalization/return (`0x63c-0x698`)**: clears `arg0+0x98`, sign-extends and copies `+0x0e/+0x0f/+0x10/+0x3c/+0x40/+0x44` into `+0x50/+0x54/+0x58/+0x5c/+0x60/+0x64`, clears `+0x8c`, returns the decoded signed halfword from `sp+0x12`, restores registers/LR/frame, and returns.

The coefficient transform is not treated as an invented mathematical replacement: the object disassembly records the repeated `mullw`, `slwi 22`, `srwi 31`, `subf`, `rotlwi 10`, `add`, `slwi 1`, and `lhax` sequence and its exact register/offset operands.

## Relocations, xrefs, and call graph

`find_xrefs.py --cslice CRI_FORM_parser --json` reports one caller, `criadx_format_dispatch`, and 11 callees:

```text
adx_err_report, fn_8004683C, fn_800469A4, fn_80046AC0,
fn_80046B90, fn_80046BE0, fn_80046C28, fn_8004E2CC,
fn_8004E300, fn_8004E324, sprintf
```

The canonical target has two `R_PPC_ADDR16_HA`/`R_PPC_ADDR16_LO` pairs, both confirmed by `llvm-objdump -dr`:

| Relocation-bearing instructions | Referenced symbol | Use |
|---|---|---|
| `lis` at `0x08`, `addi` at `0x28` | `lbl_800900A0` | FORM parser rodata base and message/format/table offsets |
| `lis` at `0x24`, `addi` at `0x2c` | `lbl_8017A288` | one-time counter and default coefficient halfwords |

`find_xrefs.py` reports `lbl_800900A0` as a `.rodata` global of 2184 bytes and `lbl_8017A288` as a `.bss` global of 20 bytes. The target relocation histogram is `R_PPC_ADDR16_HA=2`, `R_PPC_ADDR16_LO=2`; there are no direct call relocations omitted from the call list. The coarse duplicate has the same address-independent size and body.

The direct caller is the dispatch body. `criadx_format_dispatch` tests the input halfword at `arg1+0` against `0x8000`, branches to `CRI_FORM_parser` without reloading the incoming registers, and otherwise tests SPSD, RIFF/WAVE, FORM/AIFF, and `.snd` formats through the probe/parser chain. Thus the parser is the selected handler for the dispatch's `0x8000` FORM-family branch; it does not itself perform the dispatch's later RIFF/WAVE/AIFF signature tests.

`ADXB_DecodeHeader` is the only caller of `criadx_format_dispatch`. Its xref slice reports `adx_err_report`, `criadx_format_dispatch`, `criadx_get_status`, `memcpy`, and `svm_ringbuf_read`, with the format failure strings `E03010901_ADXB_DecodeHeader_str` (30 bytes) and `Can_not_decode_this_file_format_str` (33 bytes).

## FORM/AIFF and rodata evidence

The target's `lbl_800900A0` base is the same CRIADX rodata aggregate that contains the format tags. `llvm-nm -S` identifies five-byte symbols `FORM_str`, `AIFF_str`, `RIFF_str`, `WAVE_str`, `SPSD_str`, and `snd_str`; `strings -tx` locates the literal tag instances (`FORM` at rodata offset `0x180`, `AIFF` at `0x188`, `RIFF` at `0x170`, `WAVE` at `0x178`, `.snd` at `0x190`, and `SPSD` at `0xa28`). The older parser evidence inventory associates `lbl_800900A0`'s FORM tag with `CRI_FORM_parser`.

Static proof supports this narrower conclusion:

- `criadx_format_dispatch` recognizes the FORM-family branch using the big-endian `0x8000` halfword sentinel and calls `CRI_FORM_parser`.
- `CRI_FORM_parser` decodes the FORM payload through `fn_80046C28`, `fn_80046B90`, and the subsequent state/application helpers; it consumes the shared CRIADX rodata aggregate and performs no direct `strncmp`/literal comparison in its own body.
- `criadx_aiff_probe` is a separate dispatch probe for `FORM` at offset zero and `AIFF` at offset eight. Therefore AIFF recognition is a sibling dispatch path, not evidence that `CRI_FORM_parser` is an AIFF parser or that its C parameter types are known.

## Relation to the getter/layout evidence

The adjacent getter family in this unit establishes the relevant object handoff without proving a C struct declaration:

```text
criadx_get_stream_ptr: lwz r3, 0x18(r3); blr
criadx_get_field_0E:   lbz r3, 0x0e(r3); extsb r3, r3; blr
criadx_get_field_14:   lwz r3, 0x14(r3); blr
criadx_get_status:     lha r3, 0x98(r3); blr
```

Their wrappers load `lwz r3, 4(r3)` before calling the leaves, establishing `outer+0x04` as the inner CRIADX pointer. `CRI_FORM_parser` itself receives the inner parser/state object in `r3` (`r29`), the input/context pair in `r4/r5` (`r30/r31`), and writes the same inner-object offsets later read by the getter family, including `+0x0e`, `+0x14`, `+0x18`, and `+0x98`. This makes the parser's state updates and getter layout mutually consistent at the offset/width level, but does not prove ownership, lifetime, semantic field names, or ABI types.

## Disposition

The exact 1692-byte body, branch phases, xrefs, relocations, FORM/AIFF dispatch relationship, and getter-layout relation are now recorded. No natural-C twin or verified runtime fact was found, so this dossier does not justify source introduction or candidate generation. The target remains `blocked-evidence`. No candidate was generated, compiled, submitted, gated, or landed.
