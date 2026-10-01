# 04: C-pipeline green; MWCC/wibo tooling traps

Date: 2026-08-21. Follows findings/03.

## Milestone

First matched-from-source unit: `src/dolphin/os/OSInterrupts.c`
(OSDisableInterrupts/OSEnableInterrupts/OSRestoreInterrupts,
0x8000D4F4–0x8000D540, 76 bytes, 3 funcs). objdiff report: SDK Code
1/1 files 100%; retail sha1 gate still green (commit c1efcaa).
Workflow that works:
1. shrink coarse unit range in config/GFZE01/splits.txt, add fine unit block;
   dtk auto-splits remainder into auto_NN objects.
2. self-contained C in src/<path>.c mirroring splits path.
3. register `Object(Matching, ...)` under DolphinLib(...) in configure.py.
4. ninja → check "SDK Code ... matched" + sha1 gate.
Retail asm ground truth lives in build/GFZE01/asm/coarse/*.s (`.fn` labels
per symbol names from symbols.txt).

## Tooling traps (each cost real time)

- wibo+mwcceppc: `-I dir` / `-i dir` include paths NEVER resolve (space form
  embeds a leading space into the path; no-space form is unknown-option).
  Response files (@args.rsp, one token per line) parse fine but includes STILL
  fail. Conclusion: compile only self-contained C (typedefs inline, no SDK
  headers). gcc -E preflatten is an untested fallback.
- mwcceppc rejects `.s` inputs ("not compilable source") and its inline-asm
  dialect REJECTS paired-single mnemonics (psq_l/ps_mul/ps_muls0) in ALL 20 GC
  versions — melee's asm-block sources can't be compiled as-is by our tools.
- Assemble real asm with mwasmeppc.exe directly: `-proc gekko -c -o x.o x.s`
  (`-mgekko` does NOT exist; `--strip-local-absolute` unknown here). Needs
  explicit `.text` directive and `.set qr0, 0` etc. (macros.inc equivalents)
  or "Invalid special-purpose register" / "Instructions must occur within
  code section". Resulting ELF has one sized ".text" symbol, not per-func
  symbols (elf_func_bytes prints name ".text").
- Pilot cross-game blob transfer of PSVECAdd (assembled from melee source):
  NO hit in GX DOL. Melee-vintage MTX asm not present verbatim either.

## Consequence for compiler pin

Still unpinned; GC/1.3 remains default until a C-compiled game function
discriminates versions via the working diff loop.
