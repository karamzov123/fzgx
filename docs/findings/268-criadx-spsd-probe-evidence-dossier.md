# CRIADX SPSD-probe evidence dossier

- Findings number: `268`
- Unit: `main/game/criadx_800452FC`
- Source: `src/game/criadx_800452FC.c`
- Target: `criadx_spsd_probe`
- Evidence action: exact-body extraction, relocation-aware xrefs, caller/callee and dispatch inspection, SPSD rodata inspection, and comparison with the adjacent format-probe family
- Conversion attempt: none

## Scope and reproducibility

All observations were reproduced from the canonical worktree. The bounded evidence commands were:

```text
python3 tools/natc_reference_manifest.py --unit main/game/criadx_800452FC
python3 tools/find_xrefs.py --cslice criadx_spsd_probe --json
python3 tools/find_xrefs.py --cslice criadx_format_dispatch --json
python3 tools/natc_loop.py --unit main/game/criadx_800452FC --symbol criadx_spsd_probe --context-only
llvm-objdump -dr --disassemble-symbols=criadx_spsd_probe build/GFZE01/obj/game/criadx_800452FC.o
llvm-objdump -dr --disassemble-symbols=criadx_format_dispatch build/GFZE01/obj/game/criadx_800452FC.o
llvm-nm -S build/GFZE01/obj/coarse/rodata_8008FF40.o
strings -tx build/GFZE01/obj/coarse/rodata_8008FF40.o
llvm-objdump -s -j .rodata build/GFZE01/obj/coarse/rodata_8008FF40.o
```

The manifest reports 34 assembly functions in this unit, `reference_count: 0` for `criadx_spsd_probe`, and `reference_backed: 0`. The context-only run reports `blocked-evidence`: no natural-C reference body and no identity-verified runtime fact. No source conversion, candidate generation, compilation, submission, gate, lease, or fleet operation was performed.

## Exact target definition

The relocation-aware object index identifies one canonical definition and one matching coarse duplicate:

| Definition | Section | Binding | Unit-relative value | Size |
|---|---|---:|---:|---:|
| `build/GFZE01/obj/game/criadx_800452FC.o` | `.text` | global (`bind: 1`, `stype: 2`) | `0xcf0` (`3312`) | `0x34` (`52`) |
| `build/GFZE01/obj/coarse/text_80041460.o` | `.text` | global (`bind: 1`, `stype: 2`) | `0x4b8c` (`19340`) | `0x34` (`52`) |

The canonical symbol table identifies the target as `0x80045FEC`, size `0x34`. The live source definition is `src/game/criadx_800452FC.c:1065-1081`; it preserves the target as an inline-assembly reconstruction. The exact target-relative body is:

```text
0x00  stwu r1, -0x10(r1)
0x04  mflr r0
0x08  lis r4, SPSD_str@ha
0x0c  li r5, 4
0x10  stw r0, 0x14(r1)
0x14  addi r4, r4, SPSD_str@l
0x18  bl strncmp
0x1c  cntlzw r0, r3
0x20  srwi r3, r0, 5
0x24  lwz r0, 0x14(r1)
0x28  mtlr r0
0x2c  addi r1, r1, 0x10
0x30  blr
```

The target has a 16-byte frame and saves only LR. Its incoming `r3` is not copied before the call, so it remains the first `strncmp` argument. The body replaces `r4` with the address of `SPSD_str` and sets `r5` to `4`; the call ABI is therefore `(input, SPSD_str, 4)`, subject to the repository's intentionally incomplete `strncmp` prototype. The return normalization is exact: `strncmp` result zero produces `cntlzw(0) = 32`, then `srwi 32,5 = 1`; any nonzero result produces a count in `0..31`, then `srwi = 0`. Consequently the target returns integer `1` iff the first four bytes at the input compare equal to `SPSD_str`, otherwise `0`.

This is a control-flow/ABI transcription of the verified body, not a proposed source replacement:

```c
/* ABI types intentionally not inferred from this evidence alone. */
return strncmp(input, SPSD_str, 4) == 0;
```

## Relocations, xrefs, and call graph

`find_xrefs.py --cslice criadx_spsd_probe --json` reports:

- one caller: `criadx_format_dispatch`;
- one callee: `strncmp`;
- one referenced global: `SPSD_str`, global `.rodata`, size 5 bytes;
- relocation histogram: `R_PPC_ADDR16_HA=1`, `R_PPC_ADDR16_LO=1`;
- no other data or call relocations;
- one alternate definition, the matching 52-byte coarse duplicate listed above.

The two data relocations are the `lis`/`addi` pair at target offsets `0x08` and `0x14`. They prove that the probe passes a pointer to the five-byte rodata object; they do not prove that the input is NUL-terminated, writable, or a particular C typedef. The only call relocation is `R_PPC_REL24 strncmp` at `0x18`.

The context-only xref slice supplies the machine header fragment:

```text
void strncmp(void);
extern unsigned char SPSD_str[5];  /* ADDR16 data: array shape */
```

The `void` prototype is a tooling seed, not a usable declaration. The instruction operands provide the stronger ABI facts above.

## SPSD data and sibling-probe evidence

`llvm-nm -S build/GFZE01/obj/coarse/rodata_8008FF40.o` identifies `SPSD_str` as a five-byte `.rodata` symbol at object offset `0x9e8`. The section bytes at that location are:

```text
53 50 53 44 00    "SPSD\\0"
```

`strings -tx` independently reports `SPSD` at rodata object offset `0xa28` as the string occurrence associated with the aggregate's string table; symbol and raw-string offsets differ because the object contains multiple section/metadata views. The symbol's size and the bytes shown by the section dump are the authoritative shape evidence for this target. The four-byte compare means the terminating NUL is not examined by this function.

The adjacent CRIADX probe family shows the same recognition convention:

- `criadx_wav_probe` compares four bytes at input offset `0` with `RIFF_str`, then four bytes at offset `8` with `WAVE_str`;
- `criadx_aiff_probe` compares four bytes at input offset `0` with `FORM_str`, then four bytes at offset `8` with `AIFF_str`;
- the dispatch's final parser/probe path uses `.snd`/`.sd` strings for the Sun audio case.

Those sibling bodies are corroboration of the four-byte signature-probe role, not natural-C reference bodies for this target. They also do not establish a stronger parameter type for `criadx_spsd_probe`.

## Format-dispatch relation

The target's only caller is `criadx_format_dispatch`, whose canonical definition is a 232-byte `.text` function at unit-relative value `0x2bc`. Its exact SPSD branch is:

```text
0x38  mr r3, r30             ; restore input pointer
0x3c  bl criadx_spsd_probe
0x40  cmpwi r3, 0
0x44  beq 0x5c               ; continue if probe returned zero
0x48  mr r3, r29             ; restore outer/state argument
0x4c  mr r4, r30             ; input/context argument
0x50  mr r5, r31             ; third incoming argument
0x54  bl fn_800462F8         ; SPSD handler
```

The dispatch saves incoming `r3/r4/r5` as `r29/r30/r31`; `r30` is passed to each one-argument format probe, while the selected handler receives the original triple. Before the SPSD branch, the dispatch handles the `lhz(arg1 + 0) == 0x8000` FORM-family sentinel. If SPSD fails, it tests RIFF/WAVE, FORM/AIFF, and then the `.snd` path; if all tests fail it returns `-1`.

Therefore the bounded format conclusion is strong: `criadx_spsd_probe` recognizes the SPSD four-byte signature at the dispatch input pointer, and a positive result selects `fn_800462F8`. The probe does not parse SPSD payload fields, update CRIADX state, or call `CRI_SPSD_parser`; `CRI_SPSD_parser` is a separate 488-byte function in `game/criadx_800462F8`, with its own callers (`fn_800478C0`, `fn_8004E324`, and `fn_8004E354`). The probe's handler relationship is dispatch-to-`fn_800462F8`, not a direct probe-to-parser call.

`ADXB_DecodeHeader` is the sole caller of `criadx_format_dispatch` according to its xref slice. Its source calls the dispatch after obtaining the input/header pointer and then uses the returned result in the decode/fallback path. This places SPSD recognition inside the top-level CRIADX format-selection path, while leaving the exact C ownership and parameter types unresolved.

## Disposition

The exact 52-byte body, normalized boolean result, `strncmp` argument roles, SPSD pointer relocations, caller/callee set, sibling-probe convention, and dispatch-to-handler relation are now recorded. The coarse duplicate is byte/size-equivalent evidence only; no accepted natural-C twin or verified runtime fact was found. This dossier does not justify source introduction or candidate generation. The target remains `blocked-evidence`.

No candidate was generated, compiled, submitted, gated, or landed. This commit contains only this evidence dossier.
