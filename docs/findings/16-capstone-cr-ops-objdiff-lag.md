# 16: capstone traps for GX asm transcription; objdiff lags on asm-heavy units

Date: 2026-08-22. w4-gxgeom agent, batches 3-4 of GXGeometry.c extension
(0x8003493C–0x800355B0 now fully carved into dolphin/gx/GXGeometry.c:
24 funcs / 0x1024B total, all EXACT post-link; gate sha1 green).

## Finding 1: this capstone build cannot decode ANY fcmpu/fcmpo

`fc010040`-class words return zero instructions from `Cs.disasm` regardless
of crfD. Manual decode works: X-form, opcode 63, XO 0=fcmpu/32=fcmpo,
crfD=(w>>23)&7, A=(w>>16)&31, B=(w>>11)&31 → `fcmpo crN, fA, fB`. Only 2
occurrences in the batch range; patch dump lines manually when hit.

## Finding 2: capstone prints CR-logic ops in syntax MWCC REJECTS

Word `4c401382` decodes to `cror cr0eq, cr0lt, cr0eq`; mwcceppc treats
`cr0eq` as an undefined symbol ("illegal forward label ... in constant
expression"). Fix: emit numeric CR-bit operands — `cror 2, 0, 2`
(bit = field*4 + {lt=0,gt=1,eq=2,so=3}); assembles to the identical word.
Scan dumps for `cr[0-9](lt|gt|eq|so)` before generating bodies. Note
XO-truth-table values: cror=449, crnor=33 (don't trust mnemonic guesses,
verify by encoding).

## Finding 3: jump-table holder transcribes fine

fn_80035420 ("jumptable holder") has NO computed jump instructions inside
(bctr appears later than expected); its bl out (to fn_80088624) plus plain
branches transcribed verbatim → EXACT first build. The actual table lives
in data sections handled by the coarse split. Don't fear holders until a
bctr-with-mtctr shows up IN range.

## Finding 4: objdiff matched-bytes lag on asm-body units (accounting only)

After batches 2-3: denominator grew +1184B exactly, numerator (matched)
unchanged at 20824B — asm-heavy files score <100% fuzzy from sda21/reloc
bookkeeping vs dtk base objects, so objdiff excludes them from matched
totals even though every function is EXACT in the linked dol (word-level
audit) and the gate sha1 is green. Real progress metric for these waves =
linked-DOL slice diff + gate, not the objdiff numerator. Same accepted
class as findings/09/10.

## Workflow notes

- Worktree cherry-pick onto a tree that already contains equivalent
  content conflicts silently (-q hides it): reset worktree to MAIN tip via
  `git reset --hard $(git -C main rev-parse HEAD)` instead of replaying
  branch commits.
- `-i include` missing-dir warning is NON-fatal noise (main lacks include/
  too); don't chase it when a build fails — read the SECOND error.
