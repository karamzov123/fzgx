# Register repair measurements, 2026-09-11

## GC/1.3.2 high-pressure simplify replay, 2026-09-15

The pinned compiler's simplify routine at `0x507B50` chooses an optimistic
removal when no low-degree node remains. Capture now retains its signed spill
cost at node offset 12, generated-register threshold, and fallback score.
Replay divides cost by the current degree and preserves the descending-rank
tie break of the compiler's linked list. This models allocation decisions;
it does not modify the compiler or its output.

Live validation covered **170 functions / 259 allocation passes**, with identical
stock objects and zero simplify/color replay errors. Three GPR passes previously
returned unsupported: `fn_80043798`, `fn_1_9D0EC`, and the 4,208-byte
`fn_12_364CC`. Capture took 14.206 seconds; the recorded replay took 36.398 ms.
The portable archive includes C, compiler settings and hashes, retail/baseline
words, complete captures, and validation results:

```sh
uv run tools/fzgx.py fixup --archive state/repairs/spill_graphs_20260915.json.gz
```

**No new C matches resulted.** Captured declaration-order projection produced
one candidate, which remained nonexact. Extending projection to nested scopes
also produced no new matches and was removed. The movie's 70 differing words
remain in seven inlined cleanup loops; their desired colors do not conflict
with the captured interference graph, but a source-realizable order is unresolved.
Older captures and compiler profiles without measured costs still report
unsupported high-pressure decisions. Existing cached captures need recapturing
to use the added fields.

## Loop-lifetime repair guard, 2026-09-15

`reuse_temporaries` previously used the last textual occurrence as a death point.
On `fn_1_13B98`, it therefore reused the live `for` counter `j` for a pointer
offset read inside the loop. This invalid rewrite improved the raw word score
from 241 differences to 230 while leaving 17 instruction-shape differences.

Loop uses now remain live through their enclosing back edges. Unbounded
statement bodies are conservative, and goto-bearing functions require control
flow analysis before this generator can reuse locals. Across all 2,171 saved
best bodies, proposals fell from 187,930 to 153,476: 34,454 proposals without a
sufficient liveness proof were excluded across 418 functions. This count is not
a count of proven miscompilations. The offending loader rewrite is excluded;
a retained post-lifetime reuse still compiles under stock MWCC. The original,
rejected and retained sources and their measured scores are preserved in
`state/repairs/reuse_liveness_20260915.json.gz`. Historical candidates are retained
as evidence; their similarity scores do not establish semantic correctness.

The stopped `deepseek-under1k-fast-512-20260911-021006` batch supplied
268 unmatched saved bodies at >=95%, totaling 79,616 bytes. Of these,
70 were classified as register-only from instruction spelling. That label
does not prove equivalent value flow.

## Findings

* The existing search enumerates up to 720 declaration orders before most
  other rewrites. Leading declarations exclude nested blocks. Its operand
  commutations, supplied by the separate spelling search, only recognize
  already-parenthesized simple expressions.
* Operand ordering, common subexpressions, declaration scopes, initialization
  order, and temporary materialization are coupled compiler decisions.
  A rewrite may repair some fields and disturb others; greedy improvement
  and independent-bit composition do not capture all these interactions.
* `fn_1_83CB0` is a concrete false allocation diagnosis: the stores at rows
  21 and 23 consume the wrong load results. Its 99.666664% similarity does
  not imply that physical register reassignment can repair it. The new
  `regflow` diagnostic distinguishes this from reordered intermediate
  producers, such as `fn_800581CC`.

`regflow` follows supported instructions within linear regions, discards
facts at branch targets and unknown operations, and respects call clobbers.
It reports only known non-stack store-value conflicts. It is neither a
whole-function equivalence proof nor permission to accept object differences.
The oracle's matcher output now includes these conflicts; `stuck` labels
them `value-flow`. Match acceptance is unchanged.

## Bounded compiler-response experiment

The original bounded compiler-response experiment preceded consolidation into
`fzgx fixup`. Its candidate generators and response algebra now share the engine. It parses expression precedence, handles
casts and repeated expression sites, probes nested declaration order, scopes,
materialization and optimizer state, then attempts to combine observed
repairs. It uses both disjoint repaired bits and bounded GF(2) response
equations. The compiler is nonlinear: every proposed combination is compiled
again and must pass the complete relocation-aware oracle.

Compiles are grouped across functions by module and recorded compiler options.
The input C hash is checked, and settings are preserved in output manifests.

| Pass on the same 70 bodies | Candidates | Wall time | Exact object matches |
| --- | ---: | ---: | ---: |
| Initial bounded probes | 1,274 | 3.941 s | 0 |
| Nested declarations, optimizer state, response equations | 1,625 | 7.027 s | 1 |
| Final version including value-flow diagnostics | 1,624 | 9.904 s | 1 |

The final pass improved 17 word scores, including its one exact match,
`fn_10_260D4`. That match was also found by the existing repair pass.
Final amortized time was **141.5 ms/function**, including 805.9 ms total
for proposals/reading diffs, 8,355.3 ms for probes, 621.5 ms for composition,
and 117.5 ms for combined compilation. This is throughput, not a per-function
latency guarantee. Frozen baseline compilation is excluded.

This does **not** solve register allocation generally. The old repair pass
found two exact objects in this register-only subset; the bounded pass found
one. It must not silently replace the old pass or claim equivalent coverage.
No equation-composed candidate closed in this corpus. Isolated commutations,
simultaneous repeated commutations, and explicit shared floating temporaries
also failed to close `fn_80015D7C`; more spellings alone lack evidence here.

Run the consolidated engine on the frozen diagnosis directory (the measurements
above describe the original experiment, not this expanded candidate set):

```sh
uv run tools/fzgx.py fixup --corpus .fzgx/near95-fast-batch \
    --output .fzgx/fixup/replay --max-candidates 32
```

Per-function hashes, attempt IDs, settings and measurements are committed in
`state/repairs/register_solver_20260911.json`. A coverage improvement needs
constraints connecting C expression/lifetime decisions to compiler allocation,
including decisions whose intermediate probes do not improve the score.
The current experiment measures responses; it does not recover MWCC's allocator.

## Integrated repairs from the full near-miss pass

The existing deterministic pass found four exact objects. Two passed source
lint and all 16 target hashes: `fn_10_260D4` and `fn_1_FC60C`, totaling
612 bytes. `fn_8006EFB4` remains unintegrated because of a literal pointer
to locked-cache memory; `fn_80083BCC` needs justified shared-control-flow
annotations. Neither is counted as an integrated match.

The batch remains stopped. No model sessions were launched for this work.

## Exact allocator capture and replay

`fzgx fixup --capture` reads the **actual** interference graph before
and after `SelectColors` in SHA-256-pinned GC/1.2.5n and GC/1.3.2. Native Wibo
runs under local LLDB; the compiler and generated instructions are not patched.
Sources with identical compiler settings share an invocation. Function-entry
breakpoints bind captures to function names, excluding emitted helper functions.
No personal debugger or Codex configuration is changed.

`mwgraph.py` implements the non-spilling simplify pass and color selection.
It preserves coalesced ghost edges in degree counts and reads neighbors'
physical fields directly, as the compiler does. It does **not** recursively
resolve coalesced parents while coloring. Unsupported simplify spill decisions
are explicit, not guessed. A constructive inverse finds sufficient selection
orders without enumerating permutations. Failure to construct an order is
inconclusive; a witness is not proof that a C edit can realize it.

Measured on the same 70 frozen candidates:

| Operation | Total | Per function |
| --- | ---: | ---: |
| Cold capture, one compiler process per source | 37.18 s | 531 ms amortized |
| Cold capture, five grouped compiler invocations | 4.45 s | 63.6 ms amortized |
| Simplify + color replay | 3.56 ms | 0.048 ms median |
| Reconstruct captured-color order witnesses | 25.44 ms | 0.257 ms median, 2.44 ms maximum |
| Target constraints and declaration projection | 34.34 ms | 0.415 ms median |

All **70 emitted function bodies are unchanged**, including 100% relocation-aware
object comparisons against their frozen baseline compiles. All **90 allocation passes**
for those named functions replay exactly. The earlier 93-pass count included
three passes from helpers emitted alongside `fn_1_579A0`; grouped capture now
separates those identities. Cold times include LLDB startup/capture, not corpus
loading; algorithm times exclude compilation and graph input decoding. These
are corpus measurements, not a general latency guarantee.

Target analysis finds 10 consistent fixed-graph hypotheses, 54 cases requiring
value-web alignment, and six with unsupported instructions. The ten hypotheses
produce nine selection witnesses across eleven graphs. **Zero declaration
projections pass the graph prediction; zero new functions are matched.**
Parameters, generated temporaries, coalescing, and scope-dependent creation
order cannot be arbitrarily reordered by shuffling declarations. We now check
that restriction before proposing a compile. Unsupported cases remain available
for further reconstruction; this does not prune them as impossible.

`fn_80015D7C` specifically needs reversed multiply operands at rows 8 and 20,
with an otherwise identity register map. Normal matcher check output now names
such operand-order differences separately from value-flow conflicts. Capturing
a graph does not recover the frontend expression/CSE decisions that produced it.
The remaining integration work is PCode value-web/source-origin alignment and
source transformations that realize those constraints. This tooling is an exact
allocator model and diagnostic foundation, **not a completed universal repair**.

The implementation is local and has no donor imports. Algorithm/layout research
used the CC0 [inspiredrobot/mwcc reconstruction](https://github.com/inspiredrobot/mwcc/tree/ef08e865561446c072f457eaf89bde9030cccb30),
particularly `Coloring.c`, `allocator_snapshot.py`, and `coloring_model.py`.
Its license is retained in `tools/licenses/mwcc-CC0.txt`. GC/1.3.2 addresses and
layouts were recovered from the installed PE and validated by live captures;
`mwgraph.PROFILES` records exact binary identities. Only owned Python and
measurement data are included here, not compiler binaries or external-project
includes.

The committed compressed archive contains real graph captures, saved C seeds,
compiler flags/hashes, and header/source hashes. It is a repair corpus, not a
unit-test suite. Replay it without MWCC, LLDB, or the original scratch directory:

```sh
uv run tools/fzgx.py fixup --archive state/repairs/register_graphs_20260911.json.gz
```

Capture a frozen diagnosis corpus, or recompute constraints from its cache:

```sh
uv run tools/fzgx.py fixup --capture --corpus .fzgx/near95-fast-batch --output .fzgx/mwgraph
uv run tools/fzgx.py fixup --capture --corpus .fzgx/near95-fast-batch --output .fzgx/mwgraph --replay
```

Capture requires macOS's `xcrun lldb` and the repository's native Wibo with its
loader symbols. Cached replay rejects changed source hashes, header hashes or
compiler settings. The normal relocation-aware oracle and all 16 target hashes
remain the acceptance authority. No model batch was resumed.


## PCode-directed source repairs (2026-09-11)

The next pass connects captured PCode operands and source lines to virtual value
webs, aligns their emitted instructions with retail, and proposes C changes for
the implicated locals. Commutative operand choices, conflicting constraints and
unresolved rows remain explicit hypotheses. Declaration orders are evaluated in
the captured allocator before compilation; bounded exhaustion is reported as
such. One-field scalar carriers change MWCC's scalarization/temporary creation
order while retaining the declared scalar type. Source-line provenance also
finds locals hidden behind compiler-generated `@` temporaries. Generated C must
still pass the stock compiler, relocation-aware oracle and all target hashes.

This integrated **9 generated repairs / 2,944 bytes**. A target-binding correction
also recovered **5 already-exact saved bodies / 868 bytes**: **14 functions and
3,812 bytes total**, verified in commits `d684864`, `b1e4500`, and `44b5f02`.

The auto-object index previously keyed functions globally by bare symbol name.
Consequently, `title:_prolog` was compared against car_colchg's 664-byte entrypoint
instead of title's 296-byte entrypoint. Index keys, normalized-object caches and
caller/context lookups now retain module identity. The oracle rejects foreign
frozen targets, including same-sized ambiguous targets with different bindings.
One generated candidate was recovered for its actual owner, car_colchg; no
foreign-target score is accepted as a match. The historical audit invalidated
48 attempt records across 15 functions and re-scored 47 saved sources. Original
scores are preserved in `state/repairs/target_binding_v4.json`; replacement
attempts carry correct-module scores. The migration cutoff protects new attempts.

Measured on 265 frozen near misses:

| Pass | Compiler probes | Exact candidate functions | Improved functions, including exact |
| --- | ---: | ---: | ---: |
| Named scalar carriers | 228 | 5 | 21 |
| Source-origin and initialized carriers | 545 | 9 | 34 |
| Add generated block-copy temporaries | 1,073 | 9 | 40 |

The final pass spent **12.605 s generating candidates and 3.085 s compiling**;
cold grouped PCode capture took **11.860 s** separately. These are corpus totals,
not per-function latency guarantees. The nine candidate functions include the
misbound entrypoint subsequently recovered for its correct owner. Parameter-home
rewrites added 32 probes with zero improvements and were removed. A further pass
on the 31 nonexact improvements used 240 probes, improved five seeds, and closed
zero additional functions. These results do not imply universal register repair.

Live capture now supports hash-identified GC/1.2.5, GC/1.2.5n, GC/1.3 and GC/1.3.2.
The remaining three GC/1.3 corpus functions also captured and replayed exactly;
14 repair probes yielded no improvements. All captured objects stayed unchanged.
There is no runtime dependency on donor sources or modified compiler binaries.

```sh
uv run tools/fzgx.py fixup --capture --corpus CORPUS --output CAPTURES
uv run tools/fzgx.py fixup --corpus CORPUS --captures CAPTURES --output REPAIRS
uv run tools/fzgx.py fixup --corpus CORPUS --output REPAIRS --saved --apply
```

Recapture after changing source, headers or compiler settings. Improved corpora
preserve compiler settings and replace seeds only on a strictly improved word
score; that score is not relocation-aware match acceptance. Integration rechecks
current module targets and performs the complete link verification.

The portable archive preserves all 14 seeds, generated hashes, the nine repairs'
PCode/allocator captures and baseline/retail words, plus the best 31 nonexact
improved seeds (including the five further improvements). Reproduce the accepted
source transformations without scratch files or a compiler:

```sh
uv run tools/fzgx.py fixup --archive state/repairs/mwgraph_repairs_20260911.json.gz
```

This reproduces all 14 source hashes; compilation and link acceptance are separate
checks. `state/repairs/mwgraph_imports.json` records compiler settings, transforms,
owning source and verification commits. No unit tests were added and no model
batch was resumed.


## Consolidated engine

`fzgx fixup` is the only repair command. Session release, lifter repair and the
orchestrator API invoke the same engine in `tools/fzgx/fixup.py`.
The former regalloc, spell, line-repair, lab, response-solver and graph-repair
search loops are removed. Their source transformations live in
`fixup_source.py`; retail-derived candidates and line diagnostics live in
`fixup_evidence.py`. These helpers do not compile, run searches, cache or submit.
Stock allocator capture/replay lives in `mwgraph.py` and is dispatched through
this command too. SDK/data import and `stuck` remain separate because they import
sources/data or diagnose failures, rather than running competing fixup searches.

The engine batches compilation across symbols by module/compiler/flags, caches
by source, headers, compiler flags/binary, retail object and oracle fingerprint, and
confirms potential matches with the relocation-aware oracle. A bounded frontier
retains distinct emitted bodies; all candidate families share its limits.
Compiler-response compositions return to that same evaluator. There is one
report and one cache in the output directory; `--saved --apply` verifies and
integrates completed results without repeating search. Frozen sources and their
compiler settings survive every round. Historical scores select candidates;
current module targets determine their actual scores.

```sh
uv run tools/fzgx.py fixup --min-percent 95 --output .fzgx/fixup/near95
uv run tools/fzgx.py fixup SYMBOL --body candidate.c --output .fzgx/fixup/one
uv run tools/fzgx.py fixup --output .fzgx/fixup/near95 --saved --apply
```

The default corpus includes every distinct saved C/compiler combination above
the threshold, not only one best body per function. `--rounds 0` rechecks and
freezes that corpus without searching. `inputs.json`/`results.json` expose the
best unfinished seeds for graph capture; `report.json` retains all variants.
Use `--corpus PATH` to repair an existing frozen corpus. `--rounds`, `--beam`,
`--max-candidates`, and optional total `--budget` bound the single shared search.


Consolidation validation used 4,242 distinct saved C/compiler combinations across
399 unfinished functions. One round evaluated 22,896 additional candidates:
299.11 seconds cold, including 261.36 seconds in compilation/oracle evaluation.
The cached repeat, after replacing whole-body character LCS in edit composition
with line alignment and local span trimming, reused 27,138 results and compiled
one new combination: 14.29 seconds total, 0.132 seconds in compiler evaluation.
These are different cold/warm workloads, not a claimed 21x compiler speedup.
The round improved 153 best word scores and found 12 function-level matches.

Nine passed all target hashes immediately (2,596 bytes). Two other candidates
emitted 1,004 bytes of unused static helper copies outside their matching
functions. The unified engine now rejects such extra REL functions before
integration and generates explicit inline helpers. The two repairs passed the
stock object comparison in 0.49 seconds for 34 compiles, then one additional
cached-pass compile selected the all-helpers rewrite. Both subsequently passed
all 16 hashes: **11 functions / 3,564 bytes integrated** in total during
consolidation, with source and recipe provenance in `state/repairs/fixup_imports.json`. The remaining original
candidate is lint-rejected; it is not counted as integrated.

Known link-rejected source/compiler combinations are retained with header and
oracle fingerprints so unchanged saved inputs are not resubmitted repeatedly.
Best bodies also retain current relocation-aware percentages; the shared saved
candidate collector consumes those reports for subsequent repair/model batches.
The one-time target-index history migration is complete: its standalone auditor
is removed, and its immutable migration/provenance records remain in `state/`.
All future compilation goes through module-qualified targets in the engine.

## Expanded corpus and binding repairs

A three-round continuation covered 387 unfinished functions / 161,896 bytes.
It evaluated 57,127 new candidate records, with 44,912 actual compiles and
12,602 cache hits including baselines. Four functions (724 bytes) reached the
object oracle; 80 best instruction-word scores improved. Wall time was
1,045.16 seconds, including 824.21 seconds in compiler/oracle evaluation.
This broader search is not a millisecond-per-function result. Its four closures
used response composition, field order, a scalar carrier, and graph-guided
declaration order.

Graph evidence now follows changed frontier bodies across rounds. Captures
retain stock object/color replay validation; an unsupported spilling simplify
path is reported separately from a replay mismatch. Three such graphs previously
aborted an otherwise valid 380-function capture. Binding quality participates in
frontier selection, so identical instruction words no longer automatically erase
better data bindings.

The same engine now derives string-pool boundaries and padding from emitted
symbols and retail bytes and recovers named floating initializer anchors. Whole readonly pools require complete byte
and padding equality with no internal relocations. Newly recognized DOL switch
tables require every entry to resolve to the correct containing-function offset.
REL pool reads are section-qualified, including local-symbol suffix resolution.

Shared BSS uses the existing relocation-retargeting path: symbol identity,
object size, zero-initialized section and addends are checked before copies are
removed. Data already owned by the function's unit remains defined there.
This preserves MWCC's code generation for shared section bases and handles its
compiler-generated `$N` local-static symbols without inventing invalid C names.
Exact bodies with unresolved shared definitions or source-lint failures remain
searchable. Branch comments are generated only after a full object match; lint
rules and address restrictions are not waived.

The expanded historical eligibility threshold is 90%, selecting 595 functions
and 7,063 distinct source/compiler combinations. All were recompiled against
current module targets; historical percentages are eligibility, not acceptance.


The 595-function pass evaluated 57,644 bodies including baselines in 984.13
seconds (864.49 seconds in compiler/oracle evaluation). It found 22 object
matches and improved 150 best instruction-word scores. A final pass over those
595 best bodies evaluated 5,163 bodies in 44.42 seconds, including 21.33 seconds
in compiler/oracle evaluation. It reached **25 functions / 6,844 bytes**, all
subsequently linked from C and verified against all 16 target hashes. Code
progress moved from 21.80% to **22.03% (647,044 / 2,937,044 bytes)**.

Direct extern conversions, byte-pointer aliases and typed array views did not
close the remaining scalar-alias examples: their instruction sequences changed.
Those added generators were removed rather than retained as another low-yield
search family. Shared BSS was instead repaired by the verified binding path.

Integration exposed two additional symbol-promotion gaps: existing pool mappings
retained stale suffixed targets, and renamed source definitions also needed their
private mapping keys updated. Promotion now updates both, including `@`/`$`
identifiers. Ninja mappings quote and escape compiler-generated `$N` symbols
through both expansion layers. The verifier filters absent, untracked rejected
files from its dependency journal before staging, while retaining tracked deletions.
The final 24-function and one-function link checks both used the fast path.

`state/repairs/fixup_imports.json` retains all 25 original seeds and 51 successive
source-repair steps, in addition to generated C, compiler settings and verification
commits. No model sessions or standalone repair tools were added.


## Whole-corpus near misses, 2026-09-11

The post-batch audit selected 598 historical >95% candidates; 592 remained above
95% after recompilation. This pass integrated **18 functions / 7,348 bytes**.
All 18 passed the 16-target hash check without bisection or rejection (9.229 s).
Tooling, headers, source and initial provenance landed together in `f7bf82d`.
`state/repairs/fixup_imports.json` retains the original seeds, final C, compiler
settings, source-repair chains and link results. Local detailed evidence is under
`.fzgx/fixup/near95-*/` and `.fzgx/reports/near95-current/`.

| Closing mechanism | Functions | Bytes |
| --- | ---: | ---: |
| Previously exact bodies: helper ownership and hardware declarations | 5 | 3,096 |
| Interior object/entrypoint bindings | 3 | 1,152 |
| Correct store values, then repair their code generation | 2 | 320 |
| Preserve aggregate size while correcting its field origin | 1 | 244 |
| Allocator capture and source-realizable carrier/declaration changes | 3 | 1,096 |
| Coupled operand order and optimizer policy | 3 | 1,316 |
| Existing block-declaration generator | 1 | 124 |

The important machinery changes are shared by the CLI and session/lifter adapter:

- Keep a proven value-correct candidate on the frontier even if its first word
  score decreases. `fn_1_83CB0` dropped from 93.33% exact words to 80% after fixing
  the swapped stores, then reached 100% with propagation disabled. The analogous
  `fn_1_72318` correction dropped from 96% to 88%, then reached 100% with an
  argument temporary. Both now use the shared hardware map instead of literal
  pointer casts. Mixed instruction differences no longer suppress all value-flow
  diagnostics; fused product operands commute in the diagnostic without moving
  the addend.
- Preserve coupled operand changes across optimizer policies. A commutation with
  zero machine-code response cannot participate in the existing linear response
  solver. Explicitly combining it with policy changes closed `fn_1_530C8` (four
  additions), `fn_1_E594C` and `fn_15_5864`. The original 80-proposal probe closed
  none; a targeted 155-combination probe found exact code for `fn_1_530C8`.
- Generate and prove symbolic interior bindings. `OWNER__fzgx_offset_HEX` retains
  the C declaration and uses the existing pool map to bind to `OWNER+offset`.
  Validate module, section, owner bounds, relocation kind, operand agreement and
  resolved retail target before acceptance. Only ELF relocation addends change;
  code/data bytes are not rewritten. Negative probes with wrong offsets were
  rejected on all three real functions. Symbol promotion updates the owning
  names in saved mappings. All three bindings linked exactly.
- Reject extra helper definitions in both DOL and REL objects. A public helper
  can become a private inline definition; mixed inlined/out-of-line uses need
  separate spellings with external calls bound to the already-owned function.
  This repaired all three earlier link rejections, including the movie function
  whose unused helper copy added 164 bytes. The session/lifter adapter no longer
  bypasses ownership/lint checks when the supplied function oracle is already
  exact; the real `fn_80010CB0` adapter probe repaired its duplicate helper.
- Derive a selection-order witness first, then validate it through stock
  simplify/color replay and source declaration order. Bound the small-stratum
  permutation fallback to 720 orders. A witness alone is never accepted C.
- Move false leading aggregate padding to the tail when retail stack stores
  identify a shifted field origin; preserve object extent. `fn_14_A2A4` closed
  by moving eight bytes, without changing its total frame size.

Measured passes (some ran concurrently with the blanket compiler sweep; these
are workload totals, not isolated speedup comparisons):

| Pass | Functions | Compiles | Wall seconds | Outcome |
| --- | ---: | ---: | ---: | --- |
| Blanket first round | 598 | 49,264 | 802.94 | 212 word-score improvements; three usable exact candidates |
| Captured allocator repair | 160 | 5,203 | 32.15 | Three exact, 13 improved; capture separately took 9.80 s |
| Up to four differing rows | 175 | 12,225 | 92.42 | Nine exact, 38 improved |
| Operand-order witnesses | 31 | 5,442 | 35.77 | Three exact, eight improved |
| Value-correction bridges | 2 | 346 | 1.95 | Both exact code; one then needed the hardware-map rewrite |

These pass outcomes overlap; only the 18 link-verified functions above are counted
as closures. The blanket second round expanded to roughly 144,000 additional
candidates and was interrupted. Its uncheckpointed work is excluded from the
reported completed-pass results. The first-round checkpoint and improved bodies
remain available to the saved-candidate collector.

Remaining limits: `fn_80048340` still needs its larger stack layout recovered;
packing scalar locals did not close it (that generator improved two other corpus
functions). `fn_1_59B00` still has a shared conversion-pool layout mismatch.
Recovery now considers equal-valued literals at different offsets, but generated
shared-field references did not reproduce its implicit conversion-bias addressing.
An explicit union-conversion experiment also regressed and was removed. Scalar
array spellings for interior references changed code generation and were replaced
by the proven relocation-binding path. Register allocation is not universally
solved, and the remaining high similarity scores must not be reported as closure.

## The 2026-10-04 gate census: "none qualify" does not reproduce

The withdrawn claim (git 59ba2e44) correctly retired the 496/0 reachability
number — `mwgraph.py` built `desired` from `after['nodes']`, the colouring of our
*own* SelectColors pass, so the question was whether an order reproduces what the
compiler just did. That circularity is real.

The follow-on "none qualify" claim did not survive measurement. Measured
directly — retail words vs our own compiled objects in `.fzgx/work/`
(2542 objects), restricted to `status != 'matched'`:

| Outcome | Count |
| --- | ---: |
| compared | 842 |
| equal length (clears the `mwconstraints.py:23` gate) | 661 |
| &nbsp;&nbsp;`fixed-graph-hypothesis` | 40 (34 distinct functions) |
| &nbsp;&nbsp;`needs-web-alignment` | 517 |
| &nbsp;&nbsp;`unsupported-instruction` | 104 |
| length mismatch | 1103 |

The gate fires on the plateau. `needs-web-alignment` at 517 is the actual wall,
not the length precondition — and that is the module's own documented position
("Conflicts require PCode/web alignment, not a broader permutation search").

Of the 34 reaching `fixed-graph-hypothesis`, six objdiff at exactly 100.0% while
the ledger still reads `unmatched`:

    colchg_selmate_disp   fn_1_128B60   fn_1_3F4B8
    fn_1_611EC            fn_1_FC760    fn_3_17098

These are `link-mismatch` bodies quarantined by `verify.py:160-162`: the object
matched, the batch relink did not, so the unit was uncarved and the body parked in
`.fzgx/attempts/<sym>.linkfail.<ts>.c` with `percent=100, link_fail=true`.

**Five of the six are now `matched` / `link_state=verified`.** Two distinct
repairs, both of which the engine already had:

**1. Missing TU context (four functions).** `fn_3_17098` (`6695b5e3`),
`fn_1_3F4B8` (`ca986079`), `fn_1_611EC` (`924f2754`), `fn_1_FC760` (`5607407a`).
The parked bodies are standalone copies, and a standalone body does not get the
TU's headers — the recarve path supplies them and the quarantine path does not.
Each failed to compile on its own evidence alone:

    ';' expected                              (fn_1_3F4B8, no types.h)
    undefined identifier 'lbl_9_bss_8'         (colchg_selmate_disp)

Prepending the owning TU header fixed all four with no source edit beyond the
include. `fn_3_17098` needed `rel/customize/editor.h` specifically because its
body *defines* 16 `lbl_3_bss_A2410*` fragment globals while that header already
declares `extern Obj_3_bss_A2410 lbl_3_bss_A2410` — without the header the
fragments collide; with it they are the layout primer they are meant to be.

**2. Non-inline static helper (`fn_1_128B60`, `a48591fa`).** Its body declared
`static s32 next_rand(void)` — the shape of finding 273. `inline_helpers` already
had the fix but never ran: it is gated on `row['score'] == 100` (fixup.py:432) and
the quarantined body was invisible to the corpus. It also needed `#include
"types.h"`.

**`colchg_selmate_disp` needed a real repair, and finding it exposed two tooling
gaps** (both fixed, both silent). It reaches 100% at the object but
`verify` rejected it on relink; its last agent note was *"All 8 remaining rows are
L-rows (private literal pool base only)"*, which was accurate.

Its literals bind to our own anonymous rodata while retail's bind to the module's
pooled `lbl_9_rodata_*`. Sibling units in the same TU carry exactly that mapping in
`units.json` and compile through the `mwcc_pool` rule, which retargets the literals
on every build. Three things had to line up, and each was broken in a different
place:

1. **`Engine` never retargeted the pool.** `oracle.check` retargets and drops the
   primer on all three of its paths, but the fixup scoring path calls
   `oracle._diff` directly, so a word-identical candidate was recorded as
   `matched` with its private literals still in place. `fixup.Engine.retarget_pool`
   now does the same repair, and restores the object if the re-diff disagrees.
2. **`api.submit` records the mapping**, so `units.json` gets the `pool` dict and
   the unit is accepted as `link: pending` rather than the terminal `pool: True`
   ("retail object still linked", `link_state=pool`, 35 functions today).
3. **`configure.add_pool_rules` silently skipped this unit.** Its regex required
   `mwcc` on the `build` line, but Ninja wraps the rule onto a continuation line
   when the output path is long:

       build .../colchg_selmate_disp.o: $
           mwcc build/.../colchg_selmate_disp.c | $      <- not matched

   Both spellings occur in one file, so the retargeting was skipped for exactly the
   longest-named units, with no error. The regex now accepts either form and is
   idempotent (439 units, 439 poolmap lines, no duplicates on re-run).

So: five functions were byte-identical and waiting on a repair that existed but was
gated off, and the sixth was waiting on two silent tooling gaps. All six are now
`matched` / `link_state=verified`; `matched` went 5682 -> 5689 and linked files
5704 -> 5706.

### The parked body now keeps its TU context

`verify` quarantines a link-mismatched body by copying it out of its unit
(`verify.py`, the `bad` loop). That copy is standalone, and the recarve path that
would normally supply the TU's headers does not apply to it — so the parked body
routinely cannot be recompiled on its own evidence, and the function gets
re-derived from scratch. Four of the six here needed nothing but the includes.

`_with_tu_context` now prepends the TU's own `#include` block, read from the TU file
rather than guessed from the unit path. A header is added only when the body does not
already carry it, and skipped when the body defines a name the header also declares
(`_include_is_safe`) — a parked body legitimately reconstructs a struct or defines
the `lbl_*` fragment globals a layout primer needs, and including that header would
then make the compiler reject the file. The guard was checked against the exact case
that motivated it: a body defining `lbl_3_bss_A2410` does not pull in
`rel/customize/editor.h`, which declares it. The rewrite is idempotent, and the sidecar
`.json` records the headers alongside the digest.

The header list is derived from `rec['tu']`, so a standalone unit (no TU) is left
alone rather than guessed at.

### Register repair is not a viable lever at this population size (2026-10-04)

This closes the line opened by the withdrawn reachability claim. Measured over the
`fzgx stuck` census (816 functions analysed), the 99-100% band is **44% pure
`regalloc`** — 75 functions where register assignment is the *only* difference. That
is exactly the population register repair is for, so it was measured properly:

| `mwconstraints.target_mapping` verdict | Count |
| --- | ---: |
| `needs-web-alignment` / **structural** | **40** |
| `unsupported-instruction` | 6 |
| `needs-web-alignment` / commutative-only | 2 |
| `fixed-graph-hypothesis` | 2 |
| (length mismatch / no object) | 25 |

Three conclusions, each earned:

1. **75% of the band is a source defect, not a register problem.**
   `structural` means no consistent renaming exists, and the module's own docstring is
   explicit that this is *"evidence of a source defect to fix first — aligning the webs
   would mean aligning two different programs."* One value is live across a span retail
   splits, or the reverse. Web alignment is the **wrong tool** here; that corrects the
   earlier suggestion that PCode/web alignment was the prerequisite.

2. **Where a map does exist, nothing applies it.** `target_mapping` returns a
   register map for the two `fixed-graph-hypothesis` functions, but no transform
   consumes it: `declaration_projection` only accepts a reorder that preserves our own
   colours, which is what made the original reachability number vacuous.
   `fn_1_10161C` is the live case — 99.40% with **0 shape errors** and 16 word errors,
   a map in hand, and three rounds of `fixup` proposals that cannot move it.

3. **So the population does not justify the machinery.** Two applicable functions
   (one of which, `fn_1_131194`, is already matched) do not justify building a
   register-mapping applier, and the 40 structural ones need value-flow repair at the
   source, which is ordinary matcher work rather than a register pass.

Note `mwconstraints.py` has since gained a `subkind` split (`cfebd280`), which is what
separates rows 1 and 2 above; this census imports the live module.

### Two more link-mismatch recoveries, and one that needs re-derivation

`fn_1_17A9C` -> `matched`/`verified` (`b37ca544`) and `fn_1_32600` -> `matched`
(`82ef5b7a`). Both were parked at exactly 100% with the object already perfect.

**`fn_1_17A9C`: the helper carried a comment.** Its parked body declares
`static f32 vec_dist(...) /* locks the load order of the three components */ {`,
and `inline_helpers` required `{` immediately after the parameter list, so it never
matched — the shape most likely to need inlining (a hand-named reconstruction a
matcher wrote down and explained) was the one it never saw. `fn_1_32600`'s primer had
the same shape (`static void fzgx_bss_layout(void)`) and did match.

**`fn_1_32600`: one file-scope object violated the body's own convention.** A `.bss`
layout primer reconstructs the retail TU's file-scope objects in address order, and
each gets a private `fzgx_obj_` copy so it cannot collide with whatever unit really
owns it. Five of its six did:

    u32 fzgx_obj_lbl_1_bss_50EC[5];
    u32 fzgx_obj_lbl_1_bss_5100;
    u32 fzgx_obj_lbl_1_bss_5104[13];
    u32 fzgx_obj_lbl_1_bss_5138[65];
    u32 lbl_1_bss_523C[8];          <- real retail name, no prefix

`lbl_1_bss_523C` is a global 0x20-byte object referenced by `fn_1_3F250`, a different
function. Defining it here is a duplicate definition of another unit's symbol, which
the `extra_data` guard correctly refused. Adding the prefix its five siblings already
use fixed it, and the six objects then land in `.bss` at retail's exact sizes
(3C30=5308=0x14B0, 523C=32=0x20, 5138=260=0x104, 5104=52, 5100=4, 50EC=20).

It landed as `link_state=pool`, not `verified`: the private `.bss` copies cannot be
emptied, so the retail object is still linked. That is the documented pool-match
terminal state, and it counts as matched but does **not** advance linked-files — worth
knowing before reading a `matched` count as progress on linking from C.

**`fn_12_23410` needs re-deriving, not repairing.** Its parked body only reaches
96.84%; the 100% in the ledger came from an earlier body that is no longer on disk.
Attempted, and the structure is now known exactly — retail unrolls **four** search
passes over the same 0x800-byte staging buffer, at source offsets **0, 2, 1, 3** in
that order, each with its own 4-byte argument slot (`r1+0x14`, `+0x10`, `+0xc`,
`+0x8`), each computing `n = min(size - k, 0x800)` with a *signed* compare, and each
storing on a hit:

    st->unk_0c = lbl_12_rodata_A18; st->unk_28 = p[7];
    st->unk_2c = (p[8]<<24)|(p[9]<<16)|(p[10]<<8)|p[11];

It is **not** a called helper: the parked body's `static int find_entry(...)` models
something retail does not have. The assembled replacement reproduces the row count
exactly (192 target / 192 ours) and its field stores match, so the control flow is
right — but it scores **54%** on words against the parked body's **96.8%**.

The blocker is register allocation, not structure. Retail brackets the frame with
`stwu r1, -0x30(r1)` + `stmw r27, 0x1c(r1)` — five saved registers — and keeps the
hit flag in volatile `r6`, assigned only on the two loop exits. Every C spelling
tried instead allocates `hit` or `n`/`p` to callee-saved registers and emits
`bl _savegpr_23`/`_26` over a `-0x40` frame: scoping `n`/`p` inside each pass block
made it *worse* (9 saved registers), hoisting `n`/`p` with a per-block `hit` cost four
extra 4-byte slots, and a single hoisted `hit` with early returns still lands on 15
instruction-shape differences. Closing this needs the source spelling MWCC's
allocator actually chose, which the assembly alone does not pin down.

**The parked body is therefore left in place** — it is the better C, and replacing it
with a structurally more faithful but word-worse derivation would be a regression. A
machine doing this should iterate compile→diff→reshape, not apply one edit.

That empties the re-drivable pool: of 141 symbols with a `link-mismatch` history only
18 ever had a body parked on disk, and all 18 have now been driven.

The has-initializers guard is still moot for the reason given: it raises the
candidate count of a path gated earlier. That part of the withdrawal stands.

Caveat: `.fzgx/work/*.o` holds only what was last compiled locally, so this
census is a lower bound on the plateau, not the full fleet corpus. The
"317 captures with a saved body" set is not reconstructible from this tree, so
the original 317 figure could not be re-derived directly — only contradicted on
a comparable population.
