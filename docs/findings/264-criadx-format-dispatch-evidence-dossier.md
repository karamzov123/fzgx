# CRIADX format-dispatch evidence dossier

- Findings number: `264`
- Unit: `main/game/criadx_800452FC`
- Source: `src/game/criadx_800452FC.c`
- Target: `criadx_format_dispatch`
- Evidence action: exact-body extraction, relocation-aware xrefs, caller/callee inspection, format-string/data inspection, and relation to the CRIADX getter layout
- Conversion attempt: none

## Scope and reproducibility

All observations below were reproduced from the canonical worktree. The evidence-only commands were:

```text
python3 tools/natc_reference_manifest.py --unit main/game/criadx_800452FC
python3 tools/find_xrefs.py --cslice criadx_format_dispatch --json
python3 tools/natc_loop.py --unit main/game/criadx_800452FC --symbol criadx_format_dispatch --context-only
python3 tools/find_xrefs.py --cslice <each dispatch callee> --json
llvm-objdump -dr --disassemble-symbols=criadx_format_dispatch build/GFZE01/obj/game/criadx_800452FC.o
llvm-nm -S build/GFZE01/obj/coarse/rodata_8008FF40.o
llvm-objdump -s -j .rodata build/GFZE01/obj/coarse/rodata_8008FF40.o
```

The manifest reports 34 assembly functions in the unit, `reference_count: 0` for `criadx_format_dispatch`, and `reference_backed: 0`. The context-only run is `blocked-evidence`: no natural-C reference and no verified runtime fact. No source conversion, candidate generation, compilation, submission, gate, lease, or fleet operation was performed.

## Exact target definition

The target object index identifies one canonical definition and one matching coarse duplicate:

| Definition | Section | Binding | Unit-relative value | Size |
|---|---|---:|---:|---:|
| `build/GFZE01/obj/game/criadx_800452FC.o` | `.text` | global (`bind: 1`, `stype: 2`) | `0x2bc` (`700`) | `0xe8` (`232`) |
| `build/GFZE01/obj/coarse/text_80041460.o` | `.text` | global (`bind: 1`, `stype: 2`) | `0x4158` (`16728`) | `0xe8` (`232`) |

The canonical source body at `src/game/criadx_800452FC.c:350-417` is an inline-assembly reconstruction. The exact instruction sequence, with target-relative offsets, is:

```text
0x00  stwu r1, -0x20(r1)
0x04  mflr r0
0x08  stw r0, 0x24(r1)
0x0C  stw r31, 0x1c(r1)
0x10  mr r31, r5
0x14  stw r30, 0x18(r1)
0x18  mr r30, r4
0x1C  stw r29, 0x14(r1)
0x20  mr r29, r3
0x24  lhz r0, 0(r4)
0x28  cmplwi r0, 0x8000
0x2C  bne 0x38
0x30  bl CRI_FORM_parser
0x34  b 0xcc
0x38  mr r3, r30
0x3C  bl criadx_spsd_probe
0x40  cmpwi r3, 0
0x44  beq 0x5c
0x48  mr r3, r29
0x4C  mr r4, r30
0x50  mr r5, r31
0x54  bl fn_800462F8
0x58  b 0xcc
0x5C  mr r3, r30
0x60  bl criadx_wav_probe
0x64  cmpwi r3, 0
0x68  beq 0x80
0x6C  mr r3, r29
0x70  mr r4, r30
0x74  mr r5, r31
0x78  bl fn_80043050
0x7C  b 0xcc
0x80  mr r3, r30
0x84  bl criadx_aiff_probe
0x88  cmpwi r3, 0
0x8C  beq 0xa4
0x90  mr r3, r29
0x94  mr r4, r30
0x98  mr r5, r31
0x9C  bl fn_80043B48
0xA0  b 0xcc
0xA4  mr r3, r30
0xA8  bl CRI_WAVE_parser
0xAC  cmpwi r3, 0
0xB0  beq 0xc8
0xB4  mr r3, r29
0xB8  mr r4, r30
0xBC  mr r5, r31
0xC0  bl criadx_snd_probe
0xC4  b 0xcc
0xC8  li r3, -1
0xCC  lwz r0, 0x24(r1)
0xD0  lwz r31, 0x1c(r1)
0xD4  lwz r30, 0x18(r1)
0xD8  lwz r29, 0x14(r1)
0xDC  mtlr r0
0xE0  addi r1, r1, 0x20
0xE4  blr
```

Equivalent control-flow form (ABI types intentionally not inferred) is:

```c
if ((u16)*arg1 == 0x8000) return CRI_FORM_parser();
if (criadx_spsd_probe(arg1)) return fn_800462F8(arg0, arg1, arg2);
if (criadx_wav_probe(arg1)) return fn_80043050(arg0, arg1, arg2);
if (criadx_aiff_probe(arg1)) return fn_80043B48(arg0, arg1, arg2);
if (CRI_WAVE_parser(arg1)) return criadx_snd_probe(arg0, arg1, arg2);
return -1;
```

This is a control-flow transcription of the verified body, not a proposed source replacement.

## Format branches and data/string evidence

| Order | Test | Data evidence | Selected handler |
|---:|---|---|---|
| 1 | `lhz(arg1 + 0) == 0x8000` | Big-endian halfword sentinel; `CRI_FORM_parser` parses the branch-selected form | `CRI_FORM_parser()` (no arguments reloaded; original ABI state is preserved) |
| 2 | `criadx_spsd_probe(arg1) != 0` | `criadx_spsd_probe` calls `strncmp` for four bytes against `SPSD_str`; `SPSD_str` is a five-byte rodata symbol whose bytes are `53 50 53 44 00` (`"SPSD\\0"`) | `fn_800462F8(arg0, arg1, arg2)` |
| 3 | `criadx_wav_probe(arg1) != 0` | Probe compares four bytes at `arg1` with `RIFF_str` (`"RIFF\\0"`) and four bytes at `arg1 + 8` with `WAVE_str` (`"WAVE\\0"`) | `fn_80043050(arg0, arg1, arg2)` |
| 4 | `criadx_aiff_probe(arg1) != 0` | Probe compares four bytes at `arg1` with `FORM_str` (`"FORM\\0"`) and four bytes at `arg1 + 8` with `AIFF_str` (`"AIFF\\0"`) | `fn_80043B48(arg0, arg1, arg2)` |
| 5 | `CRI_WAVE_parser(arg1) != 0` | Parser compares four bytes at `arg1` against `snd_str` (`".snd\\0"`) or `lbl_80090098`, whose rodata bytes are `2e 73 64 00` (`".sd\\0"`) | `criadx_snd_probe(arg0, arg1, arg2)` |
| fallback | All preceding tests fail | No format data access in the dispatch body | `-1` |

The rodata object `build/GFZE01/obj/coarse/rodata_8008FF40.o` reports five-byte symbols `RIFF_str`, `WAVE_str`, `FORM_str`, `AIFF_str`, `SPSD_str`, and `snd_str`. Its section dump shows the contiguous bytes `RIFF\\0`, `WAVE\\0`, `FORM\\0`, `AIFF\\0`, `.snd\\0`, `.sd\\0`; the source declarations also preserve five-byte arrays for the four-character strings. The dispatch itself has no direct data relocations: it reads the input halfword and delegates string recognition to the probe/parser callees.

## Relocations, callers, and callees

`llvm-objdump -dr` verifies eight `R_PPC_REL24` call relocations in the canonical target object:

| Target offset | Relocation | Symbol |
|---:|---|---|
| `0x30` | `R_PPC_REL24` | `CRI_FORM_parser` |
| `0x3c` | `R_PPC_REL24` | `criadx_spsd_probe` |
| `0x54` | `R_PPC_REL24` | `fn_800462F8` |
| `0x60` | `R_PPC_REL24` | `criadx_wav_probe` |
| `0x84` | `R_PPC_REL24` | `fn_80043050` |
| `0x9c` | `R_PPC_REL24` | `fn_80043B48` |
| `0xa8` | `R_PPC_REL24` | `CRI_WAVE_parser` |
| `0xc0` | `R_PPC_REL24` | `criadx_snd_probe` |

`find_xrefs.py --cslice criadx_format_dispatch --json` reports one canonical caller, `ADXB_DecodeHeader`, and these nine callees: `CRI_FORM_parser`, `CRI_WAVE_parser`, `criadx_aiff_probe`, `criadx_snd_probe`, `criadx_spsd_probe`, `criadx_wav_probe`, `fn_80043050`, `fn_80043B48`, and `fn_800462F8`. It reports no referenced globals and an empty data-relocation histogram for the target itself. Each probe/parser query reports `criadx_format_dispatch` as its only caller. The callee bodies are independently sized as: `criadx_spsd_probe` 52 bytes, `criadx_wav_probe` 104 bytes, `criadx_aiff_probe` 104 bytes, `CRI_WAVE_parser` 104 bytes, and `criadx_snd_probe` 376 bytes.

`ADXB_DecodeHeader` is defined in `src/game/criadx_80041BF8.c` and calls the dispatch at lines `277-280` after loading `r4` from its input/context and retaining `r3` as the outer CRIADX object. Its xref slice reports callees `adx_err_report`, `criadx_format_dispatch`, `criadx_get_status`, `memcpy`, and `svm_ringbuf_read`; its failure path uses the rodata strings `E03010901_ADXB_DecodeHeader_str` (30 bytes) and `Can_not_decode_this_file_format_str` (33 bytes).

## Relation to the CRIADX getter layout

The adjacent getter family in the same unit establishes the relevant object handoff, but not a new type for the dispatch arguments:

```text
criadx_get_stream_ptr: lwz r3, 0x18(r3)
criadx_get_field_0E:   lbz r3, 0x0e(r3); extsb r3, r3
criadx_get_field_14:   lwz r3, 0x14(r3)
criadx_get_status:     lha r3, 0x98(r3)
```

Their wrappers in `src/game/criadx_80041460.c` load `lwz r3, 4(r3)` before invoking the leaves, establishing `outer + 0x04` as a pointer to the inner CRIADX object. In `ADXB_DecodeHeader`, `r3` is the outer object and `r4` is loaded from the outer/context path before dispatch; the dispatch preserves the three incoming registers in `r29`, `r30`, and `r31`, passes the same triple to each format handler, and passes only the input buffer/header pointer (`r30`) to each probe/parser. Thus the dispatch is an outer-level format selector adjacent to, and consumed by, the same CRIADX object family; the getter evidence supports the object relationship but does not prove C parameter types, ownership, or the semantic type of `arg1`.

## Disposition

The target body and branch ordering are exact and independently relocation-backed. The format evidence is strong for the recognized signatures and the terminal `-1` result. However, there is no natural-C reference body and no verified runtime fact, so this dossier does not justify source introduction or candidate generation. The target remains `blocked-evidence`. No candidate was generated, compiled, submitted, gated, or landed.
