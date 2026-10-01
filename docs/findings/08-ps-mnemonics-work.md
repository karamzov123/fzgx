# 08: PS mnemonics DO work in mwcceppc inline asm; dtk listings are ground truth

Date: 2026-08-22. Supersedes findings/04's claim "mwcceppc rejects
paired-single inline asm in ALL 20 versions" — that was a SYNTAX artifact.

## Finding 1: paired-single mnemonics compile fine

mwcceppc GC/1.2.5n accepts Gekko PS instructions inside `asm void f(void)
{ nofralloc ... }` bodies when written with REGISTER NAMES:
```c
psq_l f0, 0(r3), 0, 0     // NOT `qr0` (rejected: undefined symbol), plain 0 for GQR field
ps_mr f1, f0
mtfsf 255, f0             // the mystery word 0xFDFE058E at OSFPR+0x120
```
Rejected forms: bare numbers (`psq_l 0,0(3),0,0` → "expected a register name"),
`qrN` field names. Data directives `.long` / `dw` / `dc.l` are ALL rejected
inside inline-asm function bodies ("unknown assembler instruction mnemonic").

## Result

- dolphin/os/OSFPR.c @0x8000A0FC–0x8000A224 (__OSFPRInit, 296B): **EXACT**
  pre-link via unit_check GC/1.2.5n; gate sha1 green after integration.
  lfd source hardcoded -0x7C68(r13) (=ZeroF .sbss@0x801A6758, SDA base
  0x801AE3C0); lis/addi literals hardcode ZeroPS 0x801A6760 — bytes exact,
  relocs missing (objdiff fuzzy penalty only; consider sym@ha/@l + scope flips
  later for 100%).
- SDK Code now 10/10 files, 59/62 funcs counted matched (OSFPR excluded from
  fuzzy-100% numerator due to missing relocs), 100% linked.

## Workflow wins

- dtk-generated listings (build/GFZE01/asm/coarse/*.s) ARE ground-truth
  disassembly incl. PS mnemonics and symbol annotations (@ha/@l/@sda21) — read
  them BEFORE hand-decoding. They even name scratch symbols (ZeroPS/ZeroF).
- GNU clang can't encode Gekko PS; don't bother. unit_check IS the validator.
- bl target math: li=(insn>>2)&0xFFFFFF, sign-ext 24-bit, disp=li*4,
  target=CIA+4+disp. Trust a script, not mental hex arithmetic (cost ~30 min).

## OPEN: __OSPSInit @0x8000AB54 (84B, body fully decoded)

Retail calls land MID-FUNCTION: bl→0x8000A0AC (blr inside PPCMfhid2),
bl→0x8000A0B4 (blr inside PPCMthid2), bl→0x8000B718 (2nd instr of
ICFlashInvalidate). dtk listing prints them as `bl PPCMfhid2` etc., but true
targets are entry+4 (verified twice by script). Body otherwise byte-exact vs
clang-GNU validation (prologue/oris/sync/GQR0-7 stores/epilogue).
Unblock plan: add CW `entry lbl_8000A0AC` (and A0B4/B718) directives at those
addresses inside src/dolphin/os/OSPPC.c and OSCache.c bodies (precedent:
findings/06 OSException vector labels), add matching GLOBAL symbols to
symbols.txt, then write OSPSInit.c with real `bl lbl_8000XXXX`. Alternative:
find an MWCC syntax for absolute/bl literal (untested: `bl 0x8000A0AC`).
Rest of body can also be transcribed directly; only the three bl words block.
