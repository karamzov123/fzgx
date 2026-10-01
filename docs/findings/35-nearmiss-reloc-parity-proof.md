# Finding 35 — AX/AR/pad/os near-miss units are ALREADY linked-exact (reloc-parity proof)

## Scope audited (w8-nearmiss-ax1, fresh-build gate green)
ax/{AXAlloc,AXVPB,AXSPB,AXAux,AXOut,AX,AXCL}, ar/{AR,ARQ}, pad/{padmisc,pad_8001C01C},
os/{OSError,OSAudioSystem,OSReset,OSPSInit,OSException,OSInterrupts,OSInterruptMask,SIBios}.

## Result
tools/reloc_parity_audit.py: 178/179 functions have ZERO non-relocation word
diffs vs retail DOL bytes. Every diff is one of:
- R_PPC_REL24 bl placeholders (obj encodes 0x48000001; link fills target)
- R_PPC_EMB_SDA21 / ADDR16_HA/LO zeroed immediates (link fills r13/r2 disp)
The 1 "BAD" was a script opcode-table gap (`lha` op 42 missing from d-form set),
fixed in the committed tool. SIBios AlarmHandler SIZE warning = duplicate symbol
name in symbols.txt (global @0x8001287C vs local @0x80018510), not a code issue.

## Gate (findings/33 hardened form)
Purged asm/obj/config.json/main.dol/report.json -> configure.py -> ninja:
sha1 421c88106697d3275a3fc26fb7a01bf6d816b271 GREEN,
objdiff "SDK Code ... 100.00% linked (192/192 files)".

## Consequence
Sub-100% fuzzy % on these units is objdiff pre-link reloc scoring, NOT real
mismatches. DO NOT re-carve or "fix" these units — there is nothing to fix.
Remaining real work in this region: semantic naming only (rename_sym.py,
byte-neutral per findings/34).
