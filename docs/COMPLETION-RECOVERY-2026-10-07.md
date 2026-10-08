# Completion recovery: protected data ownership

The owner requested targeted completion work while keeping the GPT fleet lane disabled. This cohort recovers existing, previously uncommitted data-source work; it is not newly decompiled executable code.

## Reproduced pending imports

- 61 added `declared-bss` provenance records across 48 C translation units, totaling 19,084 bytes.
- Modules: main, main_rel, customize, interview, movie_module, winning.
- Re-ran `fzgx.dataimport.import_data(Project(), names, regenerate=True)` without applying changes. All 61 objects passed; none were skipped.
- Every generated source was byte-identical to the pending source, and saved address, size, section, alignment, source ownership, definition, method and SHA-256 fields agreed with the fresh compiler proof.
- Ran configure, Ninja at two jobs, explicit DTK check of `config/GFZE01/build.sha1` (16 files OK), and lint (0 findings).
- Validated all 61 records again with `dataimport.validate` against the actual Ninja-produced objects, not just importer probe objects.
- Included the dependent pending DOL/interview/movie_module/winning splits and DOL force-active symbol annotations. main_rel/customize ownership was already committed but their source files/provenance were not, so they are included for complete reproducibility.

Local detailed receipts: `.fzgx/completion-existing-data-reprobe.json` and `.fzgx/completion-existing-data-gate.log`.

## Remaining source-ownership warnings

Configuration still reports missing `src/dol/fn_80036AC4.c` and missing configuration for `rel/title/fn_8_704.c`, `rel/sel/fn_10_266AC.c`, `rel/movie_module/fn_12_23410.c`, and `rel/pilotpoint/pilotpoint/fn_14_C850.c`. A green retail hash gate does not prove these C bodies participate in the build. The pending pilotpoint body and unrelated fleet/tooling edits are deliberately excluded from this data commit.

Small-module closure remains structural work, not easy-work attribution: replay's last function has a saved register/address-promotion residual; car_colchg's menu function has plateaued across prior attempts; sample's entrypoint requires a fresh residual diagnosis. No attempt caps or provider cooldowns are reset by this recovery.
