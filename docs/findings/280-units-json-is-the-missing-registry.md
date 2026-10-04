# 280 — a split range alone does not make a function linkable: `units.json` is the missing registry

Date: 2026-10-04. Tool: `tools/fzgx/splitgaps.py`, `configure.py`, `fzgx check`.

Build state while investigating: `ninja` + `build/tools/dtk shasum -q -c
config/GFZE01/build.sha1` prints `16 files OK`.

## The gap in finding 279

Finding 279 established that a body for a `.text` range no split covers still checks
at 100% and cannot link, and that adding the split is the fix. It names `splits.txt`
as the only registry involved. **That is incomplete.** A split range is necessary but
not sufficient: a second file, `config/GFZE01/units.json`, decides whether the range
is built from C at all, and nothing in the check path looks at it.

There are three registries, and only the first is checked by `splitgaps.py`:

| Registry | Meaning | Checked by `check`? |
| --- | --- | --- |
| `config/GFZE01/<mod>/symbols.txt` | retail address + size | yes |
| `config/GFZE01/<mod>/splits.txt` | which range a unit owns | **no** (finding 279) |
| `config/GFZE01/units.json` | that the unit is *built* from C | **no** (this finding) |

## What each missing registry does, and they fail differently

**Missing `splits.txt` range** — `check` reports 100% and submit fails at link. This is
finding 279; `splitgaps.py` finds it.

**`units.json` entry, no source file.** Registering the unit in `units.json` *without*
putting a body in the TU file makes the link fail the other way:

    Failed: While resolving relocations in 'build/GFZE01/main_rel/main_rel.plf'
    Caused by:
        Failed to find symbol fn_1_17A9C in any module

The split now claims the range, so the retail auto-object no longer provides the
function, and the new unit has nothing to compile. Registering the five `main_rel`
units of the object-100%-but-link-rejected cohort this way broke all five modules at
once. The fix is not more registration, it is a body.

**Neither registry, source only.** Adding the block to the TU file with no split and no
`units.json` entry compiles nothing: `configure.py` prints `Missing configuration for
<unit>` (`tools/project.py:1155`) and skips it, `build/<V>/gen/<unit>.c` is never
written, and the function is simply absent. The build stays green.

## The dangerous middle case: a green build that proves nothing

This is the part worth writing down. With a body spliced into the TU file and
`units.json` updated but `gen/` **not** regenerated, `ninja` still prints `16 files OK`
and the REL is byte-identical to retail — because the unit was never compiled and the
bytes came from the retail auto-object as before.

The check that exposed it is worth making routine: deliberately corrupt the body and
see whether the hash moves.

    sed -i 's/    b\[2\] = c;/    b[2] = c + 1;/' src/rel/customize/editor.c
    ninja && build/tools/dtk shasum -q -c config/GFZE01/build.sha1   # still 16 files OK

`16 files OK` on a body that provably cannot produce those bytes means the body is not
in the link. A green build is only evidence about code that is actually compiled.

`gen/` is written by `tufile.regenerate`, keyed on the mtime of `units.json` and the TU
files, and `configure.py` does not call it. After editing either, run it explicitly:

    python3 -c "import sys; sys.path.insert(0,'tools'); from fzgx import tufile; \
        from fzgx.project import Project; tufile.regenerate(Project())"

## A fourth gate: a new split can create a link-order cycle

Closing a split gap is not always safe, and the failure is a fourth one, found by trying
to bulk-close all 92 gaps in the >=99% no-split class at once:

    Cyclic dependency encountered while resolving link order:
    rel/main_rel/fn_1_5C83C.c -> auto_00_0005CA00_text

Giving a function its own split turns it into its own link unit. If the functions it
*calls* are still supplied by auto units that the linker must place **after** it, the
order is circular and the whole module fails to resolve. This is a real ownership
question, not a missing range: the callee's storage is owned by an unmatched auto unit
that sits later in the module.

Of 92 candidates, **52 linked cleanly and 40 were cyclic**. Every removal was validated
by rebuilding, so the 52 are known-good. Examples of the cyclic class:

    colchg_selmate_disp (car_colchg)  fn_16_E20 (profile)      fn_15_5460 (winning)
    fn_17_3F0 fn_17_3E08 fn_17_6C30 fn_17_7728 (interview)    fn_8_5C54 fn_8_FC5C (title)
    fn_10_3B44 fn_10_7130 fn_10_A90C fn_10_22760 (sel)         fn_3_104C4 fn_3_1AE40 fn_3_1AEE4

Note the shape of the trap: the cyclic ones are **cross-referencing functions** — the
kind that call each other or sit inside a matched unit's dependency cone. They need a
call-graph-aware split, not a range. `splitgaps.py` cannot tell them apart today, so the
pruning loop is the honest way to separate the two classes.

The right durable fix is for `splitgaps.py` to consult the call graph and exclude a
function whose callees are all still in auto units positioned after it. Until that
exists, treat "no split range" as a *candidate* reason, not a guaranteed fix, and
validate every batch of additions with a build.

## A split on an unmatched function *lowers* the headline number

Landing the 32 surviving ranges built and hashed clean (`ninja` exit 0, `16 files OK`)
and still cost 32 functions:

    matched_functions  5656 -> 5624      complete_units  5697 -> 5665

Nothing had regressed. The 32 were never decompiled — the ledger has every one of them as
`unmatched`. They were being counted only because they sat inside a **retail auto object**,
whose bytes match by construction. Giving a function its own split moves it out of that
auto unit into a unit with no source, and objdiff stops crediting it.

So `matched_functions` and `complete_units` include functions that were never written in
C, and the correct move toward a body for such a function *decreases* both. A split alone
is not neutral: without a body in the same change it is a metric regression.

This is the sharpest edge of finding 280, and it means the obvious metric to watch while
working this class is actively misleading. The 32 ranges were reverted for this reason;
the `cyclic_splits.json` set and the `--registries` output are kept, since they are pure
diagnosis and cost nothing.

The correct unit of work for one of these functions is all four steps in one change --
split range, `units.json` entry, `tufile.regenerate`, and a body -- and the metric only
moves when the last one lands.

### What the cycle class looked like

Of 92 candidates, 52 linked and 64 symbols were confirmed cyclic across two passes (the
second pass re-tested the first pass's survivors against a moved baseline). The confirmed
set lives in `config/GFZE01/cyclic_splits.json`, and `splitgaps.py` reports those symbols
as `CYCLIC` rather than offering them as an addable gap, so the class is not
rediscovered. That matters because the default output prints an "add this range" line
taken from the first row, which would otherwise keep proposing a range that breaks the
link.

The second pass produced a *candidate* predictor: the cyclic cases looked like functions
whose range is contiguous with the next split unit and that call into it.

    fn_12_3A64  0x3A64..0x3DB8      fn_12_3DB8 starts at 0x3DB8
    fn_1_8CA70 -> fn_1_8CAF4 -> fn_1_8CB78 -> auto_... -> fn_1_8CED0

**That predictor does not work, and it is worth recording so nobody rebuilds it.** Scoring
two versions of it against the 64 measured symbols, over all 1,377 no-split candidates:

| model | TP | FP | FN | precision | recall |
| --- | ---: | ---: | ---: | ---: | ---: |
| contiguity with a **split** range + call edge | 0 | 67 | 64 | 0% | 0% |
| contiguity with an **auto** unit + call edge | 0 | 0 | 64 | n/a | 0% |

The first model is not merely imprecise, it is anti-correlated: it flags 67 symbols and
none of them are in the measured set. The reason is structural. The cycle partner is an
**auto unit** — a maximal uncovered `.text` span that `dtk rel` turns into
`auto_NN_<addr>_text` — not a split range, so a contiguity test against `splits.txt` is
looking at the wrong objects. Modelling auto units does not rescue it either: a typical
candidate sits in the *interior* of one large auto span, so it has no adjacent span at
all, and every one of the 64 reports "no adjacent auto unit".

Splitting a candidate out of a large auto unit leaves **two** remainders, and the cycle is
a property of the call graph *across the new boundary inside that span*, not of adjacency
at its edges. The real test is whether F calls something in the remainder that
transitively reaches F: a graph query over the split remainder, not a structural one. Two
structural heuristics were tried and both scored zero; do not spend more time on
neighbour-based proxies.

## What it takes to land one object-100% function

`fn_3_17098` (customize, 72 B) was at 100% object / link-rejected with 31 attempts.
Landing it took, in order:

1. the split range `0x17098..0x170E0` (bounded on both sides by existing units);
2. a `units.json` entry naming the unit, its symbol, and its `tu`;
3. `tufile.regenerate`;
4. a body written against the **typed** header, not a private layout.

Step 4 is the one that matters for cost. The preserved body carried 19 file-scope
objects standing in for `lbl_3_bss_A2410`, and recompiling it displaced the shared
`.data` base by 0x76 bytes, breaking an unrelated TU's relocation:

    0x0020  ours=0x000a252c  retail=0x000a24b6

`include/rel/customize/editor.h` already declares that storage with a recovered layout
(`Obj_3_bss_A2410`, 0x28 B; `Obj_3_data_35C0`, 0x58 B). Using the typed fields removed
every one of those definitions and fixed the module-wide displacement in one edit. A
private re-declaration of storage a module header already owns is always wrong: it
cannot match per-object and it corrupts the module layout.

Two further per-object lessons from the same function:

- retail stores a **word** at `0x4` (`stw r3, 0x4(r5)`) but the recovered struct has
  `u8 unk_4; u8 unk_5; u8 unk_6;` there. A typed field write emits `stb` and costs the
  match; `*(u32 *)((u8 *)b + 4)` recovers it.
- retail reuses **one** materialised zero for the index, the byte store and every
  halfword store. Separate literals let the allocator pick a second zero and add a row.
  One `u32 z = 0` local shared by all of them is what reproduces it.

Result: 22.8% -> **91.4%**, module displacement gone, remaining diff 10 regalloc rows
and 1 instruction row. Not yet linked as a match.

## The regalloc class is *reachable*; my earlier reading of it was wrong

The plateau is large: 238 functions sit at >=98% (126 KB), 139 at >=99%. They release with
notes like "99.9%, 2 rows, both regalloc" and, in the worst case, `fn_10_3B44` after
**3217 search variants** concluding the swap "is an allocator tie-break not reachable from C".

I took that at face value and wrote into this file that the class was over-instrumented and
its captured colours "routinely not expressible in the C the tools generate". **That was an
inference I had not tested, and it is wrong.** Measured directly:

**1. A witness exists for every capture.** Running `mwgraph.selection_order` on all 379
captures, against the captured (retail) colours:

    reachable 496      unreachable 0      skipped 1

**2. The witnesses are not trivial.** A witness equal to the compiler's own simplify order
would prove nothing, since that is the order it already used:

    non-trivial (reorders the compiler)  540
    trivial                               8
    positions moved: min 2, median 40, max 213

A median of 40 reordered positions is a substantial rewrite of the selection order, and it
is achievable in every measured case.

**3. A quarter of them project onto real C.** `mwconstraints.declaration_projection` turns a
witness into an edited function body. Instrumenting every guard in it, over 463
capture/body pairs:

| guard | count | share |
| --- | ---: | ---: |
| **produces a candidate** | **120** | **25.9%** |
| has initializers (conservative guard) | 156 | 33.7% |
| fewer than 2 movable locals | 105 | 22.7% |
| movable absent from witness | 28 | 6.0% |
| not the reverse-declaration stratum | 24 | 5.2% |
| no locals in body | 18 | 3.9% |
| colours still differ after projection | 12 | 2.6% |

So 120 byte-level C candidates exist in effect, each reproducing the retail register
colours by construction, and **the projection stage had never been run at scale** --
`.fzgx/fixup/replay` did not exist.

The bottleneck was never "can the allocator get there". It is that nobody runs
`fzgx fixup --capture` / `--replay` over the corpus and compiles the results. Treat the
plateau as unbuilt work, not as a frontier.

The largest single guard is `has-initializers` at 33.7%. It is deliberately conservative --
"moving them can change program behaviour" -- so relaxing it is a real decision and not a
free win, but it is where the next ~150 candidates are.

Treating this class as "more agents, more attempts" has already cost roughly $369 and is
the single largest line item in the ledger.
