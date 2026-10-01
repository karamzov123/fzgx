# 252 — tool worktree: uncommitted src/ is carve REGRESSION, not landed work (+ 7th-pass verify)

Date: 2026-08-26. Worker: tool. Branch: fleet/tool. Read-only w.r.t. canonical main.

## 1. Queue items 1-4 re-verified live (7th pass) — no code change needed

- `--self-test` rc=0 for all four:
  - find_xrefs  SELF-TEST OK (victim=OSRegisterVersion, callers=14, frag lines=2)
  - similar     SELF-TEST OK (2235 fns; probe=GXSetMisc; top=GXClearVtxDesc@0.050; 1 accepted-twin hit)
  - emit_m2c_asm SELF-TEST OK (50 lines, round-trip byte-identical)
  - natc_loop   SELF-TEST OK (context 3189 chars, attempts=0, uncarved-guard=unit_uncarved)
- Fixture suite: `uv run --with capstone --with pytest python3 -m pytest tests/test_{find_xrefs,similar,emit_m2c_asm,natc_loop}.py -q`
  -> **42 passed in 10.70s**.
- Compliance: `git ls-tree main -- <the four tools> | wc -l` = **0** (not promoted).
  `units.worker='tool'` = **0** (no lease held). natc_gate refusal logic and
  `.gateorig` backups untouched.
- Ratification: still **ABSENT**. The 6 keyword hits in `tools/REVIEW-queue-1-4.md`
  are prose plus the template line `## RATIFIED — hard-tail, <date>:` — a
  placeholder, not a sign-off. I cannot self-ratify.

## 2. NEW FINDING: three modified src/ files in this worktree are a regression

`git status --porcelain` was not clean (expected empty). Three files, all mtime
2026-08-26 18:31:31, differ from both HEAD and main (which agree byte-for-byte):

    src/dolphin/metrotrk/trk_80088B00.c   +300 -371
    src/dolphin/msl/tail_8008279C.c       +29  -39
    src/dolphin/mtx/MTXFused.c            +15  -17

The diff direction proves these are NOT new work — they **undo landed commits**:

- MTXFused.c loses the named-reloc form landed by 9e7ef78:
  `lis r6, lbl_801327F8+0x28@h` / `ori r6, r6, ...@l`  ->  `lis r6, -0x7fed` /
  `ori r6, r6, 0x2820`, and drops `extern unsigned char lbl_801327F8[];`.
  That commit is exactly what took MathSinCos to 100% / MathSin to 99.67%
  (+196 B, gate GREEN 421c8810). Reverting it is a parity loss.
- trk_80088B00.c loses the extern-decl type fixes landed by 619bdb7 and deletes
  ~50 extern declarations (e.g. `gTRKInterruptVectorTableEnd` -> `fn_80005518`).

Shape matches raw output of the carve generators that own these exact paths
(`tools/gen_pm10_carve.py` lines 232-234 write both metrotrk/msl paths;
`gen_mtxfused_carve.py` owns MTXFused). Conclusion: a carve generator was
re-run in this worktree and overwrote symbolised, landed sources with freshly
regenerated raw output. No tool in the queue (items 1-4) writes to `src/` —
verified: none of find_xrefs / similar / emit_m2c_asm / natc_loop has a src
write path, and all four are read-only by design.

## 3. Action taken — preserved, NOT discarded

Per contract ("never use git reset/checkout/stash to discard work") I did **not**
revert them. Full evidence preserved outside git at:

    ~/.cache/natc/scratch/tool/src-writeback-evidence-20260826/
      trk_80088B00.c  tail_8008279C.c  MTXFused.c  worktree.diff (2011 lines)

## 4. Consequences for integ (please read)

- Canonical `main` is **unaffected**; this is worker-tree-local dirt only.
- But a dirty `src/` makes `natc_gate.py --repair` refuse. If this same
  regeneration ever happens in the integrator tree, repair is blocked until the
  tree is clean — and the naive "clean it" reflex (`checkout --`) is the same
  reflex that discarded 239->234 on 8/26. The correct handling is: confirm the
  diff direction is a *revert of landed commits* (as here), preserve a copy,
  then restore from HEAD deliberately, not reflexively.
- Recommended guard (item 5 candidate, not built without assignment): a
  `--check-src-clean` precondition in the carve generators that refuses to
  overwrite a tracked `src/` file whose content differs from HEAD.

## 5. Status

Items 1-4: green, spec-complete, fixture-tested, NOT promoted.
BLOCKER unchanged: hard-tail ratification of `tools/REVIEW-queue-1-4.md`.
