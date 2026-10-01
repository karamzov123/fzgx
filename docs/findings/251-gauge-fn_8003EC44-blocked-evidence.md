# findings/251-gauge-fn_8003EC44-blocked-evidence.md

TYPE: evidence dossier / blocked-evidence note
UNIT: main/game/gauge
SYMBOL: fn_8003EC44
WORKER: integ
TURN CONTEXT: lease-verified, 0 attempts

LEASED UNIT (natc_rank --status --worker integ):
  main/game/gauge -> leased (integ, 0.1h)

SELECTED SYMBOL (natc_symbol_state --unit main/game/gauge --next):
  next: fn_8003EC44
  attempts: 0
  excluded: []

LIVE DESTINATION (canonical HEAD, src/game/gauge.c):
  line 46: asm void fn_8003EC44(register void* a)

CONTEXT ASSEMBLED (natc_loop --unit main/game/gauge --symbol fn_8003EC44 --context-only):
  size: 3056 B
  section: .text
  callers (1): fn_8003FF88
  callees (2): GXBegin, GXSetChanMatColor
  globals (32): R_PPC_EMB_SDA21 to .sdata/.sbss/.sdata2, including
    lbl_801A65A8..lbl_801A6600 (scalar sdata/sbss)
    lbl_801A6C60..lbl_801A6C70 (scalar sbss)
    lbl_801A71C8 (const volatile float, sdata2)
    lbl_801A71D0 (const volatile double, sdata2)
    lbl_801A71E4, lbl_801A71F0, lbl_801A71F4, lbl_801A71F8,
    lbl_801A71FC, lbl_801A7200, lbl_801A7204, lbl_801A7208
    (const volatile float, sdata2)
  reloc histogram: R_PPC_EMB_SDA21 = 32
  traps flagged: T1/T3/T14 (sda21 declaration shape), T9 (fixed-coefficient ABI, noted but not used by this function's accesses)
  neighbors: fn_8003F834 (J=0.062), fn_8003FF88 (J=0.017)

REFERENCES (natc_reference_manifest + natc_refs --build + natc_refs --dump):
  reference_count: 0
  reference_backed: 0
  dump for fn_8003EC44: "-" (no natural-C body in indexed trees)
  ref-bodies dir: ~/.cache/natc/ref-bodies/main_game_gauge (empty)

VERIFIED READINESS (machine-validated):
  verdict: blocked-evidence
  ready for candidate generation: no
  reasons:
    - no natural-C reference
    - no verified runtime fact
  action: do NOT generate yet; obtain a reference body or a verified runtime trace first

BLOCKER:
  This symbol is in a stable blocked-evidence state. Generating and compiling
  candidates now would violate the unit readiness contract and risk wasted
  attempts / false parity. The live destination is ASM. No reference body
  exists in the indexed reference set, and no identity-verified runtime trace
  is available.

ONE ACTION TAKEN THIS TURN:
  wrote this dossier; did NOT generate, splice, compile, or codegen-search.

NEXT REQUIRED EXTERNAL INPUT (evidence-producing, not generated here):
  - a verified runtime trace (argument/return/global/branch coverage) with a
    durable artifact path, OR
  - a reference body for fn_8003EC44 (or a useable sibling/twin) indexed into
    the reference set so natc_refs --dump exposes a body.

STOP CONDITIONS MET:
  - ready for candidate generation: no
  - no verified runtime fact
  - no usable natural-C reference body
