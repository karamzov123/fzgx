# Selection-loop continuation and refreshed closure routing

This is compiler-reproduced research, not accepted decompilation credit. No new exact function was submitted in this pass, no attempt caps/cooldowns were reset, and GPT remained owner-disabled. Existing AGY/OpenCode claims continued running; no shared source, split, header, compiler setting or controller implementation was changed.

## fn_1_7D6B8: isolate the actual remaining mechanism

The prior portable frontier freshly reproduces at seven differing rows with GC/1.3.2. A new stock allocator/PCode capture of that exact changed source and its replay both succeeded with zero errors. A bounded single projection round (32 candidates, beam one, 45-second ceiling) did not improve it and produced no exact match.

Retail materializes the mask value one inside the loop at 0x4B0; the old candidate hoists it into the loop preheader. Rewriting `temp_bit = 1 << temp_r4_2` as `temp_bit = 1; temp_bit <<= temp_r4_2` removes the hoist and aligns every instruction form. This is equivalent C mutation of the existing mask value, not a compiler flag change or register pin.

The resulting baseline has five differing rows, raw 99.95091%, adjusted 99.30%. Recaptured the changed source before a second bounded projection round (64 candidates, beam one, 60-second ceiling); that round yielded no improvement and no exact match, so it was stopped. The engine-generated portable `completion-mask-frontier-2026-10-07.json.gz` reproduced all four retained records through `fzgx fixup --archive`.

Reversing the addition source operands to the retail value order preserves the same five-row word residual, with raw 99.957924%. Remaining differences are physical register choices only: signed step r7 vs r5, signed category r5 vs r4, and shifted/tested mask r5 vs r6. It is not conversion-complete. The source, compiler identity and complete diff are retained in the separately labeled manual dossier `completion-cmp-frontier-2026-10-07.json.gz`.

Historical evidence was checked before considering another pragma: `opt_loop_invariants off` had already been tried in the old matcher and left 21 differing rows, including an incorrectly placed mask constant. No unchanged broad optimization-toggle sweep was repeated.

## Refreshed larger saved main_rel candidates

Four available unclaimed saved bodies over 1 KiB with historical scores at least 98% were recompiled against current retail with their archived compiler settings:

| Symbol | Retail bytes | Raw score | Adjusted score | Differing rows |
| --- | ---: | ---: | ---: | ---: |
| fn_1_12DAEC | 1484 | 99.66307 | 94.34 | 21 |
| fn_1_FE014 | 1456 | 98.95605 | 90.14 | 36 |
| fn_1_F4F08 | 1360 | 99.367645 | 88.53 | 39 |
| fn_1_D123C | 1348 | 99.64391 | 99.41 | 2 |

All four compile, none is exact, and none has outstanding literal-pool rows. Source bodies, hashes, settings and diffs are portable in `completion-cmp-frontier-2026-10-07.json.gz`. High raw similarity did not establish a small residual for the first three.

## fn_1_D123C: pointer-test form versus branch shape

Both residual instructions are retail `cmplwi r25,0` versus candidate `cmpwi r25,0` at the two allocation/null checks. The saved source expresses those checks as single-case switches over pointer values cast to u32.

Bounded causal probes:
- Native pointer `if` fixes signedness but consolidates the retail `beq; b` form, leaving four branch rows.
- Unsigned case labels preserve the original two-row mismatch; they do not change the compiler's chosen compare form.
- A pointer-predicate switch materializes cntlzw/srwi and regresses to 35 rows.
- A pointer-selection expression preserves unsigned comparison but introduces copies and different lifetimes, leaving 20 rows.
- Adjacent installed pins, with the original C unchanged: GC/1.3 leaves eight rows; GC/1.3.2r leaves the same two; GC/1.2.5n regresses to 172. No pin was changed in the build.

Stop this local shape/pin search. The original two-row candidate remains the retained frontier, not the prettier but worse pointer forms. The next investigation must account for signedness AND retail branch topology under the stock source pipeline; treating this as merely a register-allocation problem is incorrect.

## Other bounded checks and operating constraints

Explicit signed conversion temporaries for the missing DOL matrix setter regressed to 47 differing rows and were not installed; retain its older twelve-row frontier. The small trivial dry-run (max size 255, no lifter, limit 40) returned zero candidates. Existing SDK identification supplies one unique unresolved SI handler, whose recorded history already includes a large failed allocator search; no repeated donor-import sweep was launched.

The publisher's legitimate isolated clean build temporarily held build.lock for longer than a 120-second private-probe wait. A longer bounded wait succeeded without killing owners or weakening locks. This observation is not proof of a controller failure. Keep private compilation serialized and preserve worker mutations.

Acceptance for these research artifacts: engine archive replay (four records), validated manual archive source SHA-256s (five bodies), lint with zero findings, fresh ledger snapshot, scoped evidence commit, and the authorized publisher's exact-SHA clean sixteen-target hash/lint gate. Preserve live worker controls and report current claims separately from accepted matches.
