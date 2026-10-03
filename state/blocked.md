# Blocked large near matches

Triage stays local. Candidates and compiler settings are preserved in
`state/repairs/large_near_frontier_20260914.json.gz`. The measured differences
below do not imply that declaration order alone can repair a function.

| Function | Words | Shape edits |
| --- | ---: | ---: |
| `fn_12_975C` | 12 | 0 |
| `fn_12_3DB8` | 16 | 0 |
| `fn_1_D123C` | 16 | 2 |
| `fn_8_2660` | 155 | 10 |
| `fn_12_BBC0` | 28 | 13 |
| `fn_3_1D338` | 112 | 6 |
| `fn_12_364CC` | 95 | 3 |
| `fn_1_9012C` | 111 | 26 |
| `fn_16_4E90` | 139 | 35 |
| `fn_1_AFC8` | 48 | 4 |
| `fn_1_15C6C0` | 254 | 43 |
| `fn_3_255DC` | 240 | 27 |
| `fn_1_13B98` | 241 | 17 |
| `fn_12_23F5C` | 87 | 5 |
| `fn_1_7FD7C` | 3 | 0 |

The 2026-09-15 shared-state pass linked `fn_8001B42C` from C.
`fn_3_1D338` now has a retail zero palette and corrected multiplication
association; the 109-word/8-shape alternative is also preserved in
`state/repairs/shared_regions_20260915.json.gz`. Neither customization
candidate is a full match. See `docs/batches/2026-09-15-shared-regions.md`.

The subsequent selection pass linked the 4,156-byte `fn_10_1C2D8` from C.
Its arithmetic, initialized data and all 867 pointer bindings are verified.
The adjacent `fn_10_1D314` remains unmatched at 121 words / 15 shape edits;
its corrected source is in `state/repairs/large_closures_20260915.json.gz`.
See `docs/batches/2026-09-15-large-closures.md`.
## src/rel/movie/ attempt-cap triage (cline-7)

All remaining unmatched functions in the movie module hit the claim attempt
cap before this session, so none could be worked. Blocked pending triage;
best objdiff score recorded for the next agent.

| Function | Bytes | Attempts | Best % |
| --- | ---: | ---: | ---: |
| `fn_5_6E8` | 184 | 5 | 94.85 |
| `fn_5_37C` | 332 | 6 | 93.34 |
| `fn_5_4C8` | 444 | 5 | 92.93 |
| `fn_5_7A0` | 3088 | 3 | 82.96 |
| `fn_5_1404` | 664 | 3 | 98.28 |
| `fn_5_169C` | 2656 | 3 | 68.02 |
| `fn_5_2C54` | 428 | 3 | 76.83 |
| `fn_5_3C44` | 528 | 3 | 94.81 |
| `movie:_prolog` | 512 | 3 | 90.70 |
| `fn_5_20FC` | 2904 | 3 | 40.59 |
| `fn_5_2E00` | 3576 | 3 | 61.01 |

This closes the module: all 10 remaining unmatched functions are recorded
here, so no `src/rel/movie/` function is left silently attempt-capped.
`fn_5_20FC` and `fn_5_2E00` were missing from the first table.

`fn_5_1404` is listed above but is no longer blocked: it has since been merged
into `src/rel/movie/_prolog.c` and the claim tool reports `status is matched`.
Only `fzgx inventory --module movie` still reports it as unmatched, so the
inventory status view is stale for symbols that are already merged into a
translation unit.

Correction (cline-7, later session): the `fzgx context` "attempts so far" line
is NOT authoritative for cap state, and the earlier claim in this file that it
is was wrong. The cap is enforced from the ledger counter, which `inventory`
reports. A symbol can show `attempts so far: 1` in `context` and still be
unclaimable: `fn_5_2E00` and `fn_5_6E8` both display 1-2 in `context` yet
`fzgx claim` refuses both with "attempt cap 3 reached". Judge claimability from
`fzgx inventory`, not from `context`, and do not read a low `context` counter as
an invitation to retry.

Unblocking any of these needs triage judgement, not another blind attempt: the
four near-misses (`fn_5_6E8` 94.85, `fn_5_37C` 93.34, `fn_5_4C8` 92.93,
`fn_5_3C44` 94.81) should be re-derived from the saved diff corpora rather than
resubmitted.

## car_colchg hand triage (2026-10-03 status report follow-up)

`colchg_selmate_disp` is **code-complete at 99.6%**. All 8 remaining rows are
`L` rows and the oracle reports zero rows owned by the matcher. The eight loads
(160.0, 50.0, 255.0, 0.5, 480.0 and one f64) belong to the module's *shared*
literal pool `lbl_9_rodata_0` at offsets 0x14/0x18/0x3c/0x40/0x44/0x48, but a
standalone carved unit compiles them into its own anonymous `.rodata.0`, so
only the base symbol differs. Rewriting the loads as field reads through the
shared `PoolRow` symbol *regresses* to 96.7%: it adds a `lis @27@ha`, reorders
`lis lbl_9_bss_14` / `lis lbl_1_rodata_26F8`, and swaps the `fmuls` operand
order. The release path already prints the needed binding as
`pool_map {"...rodata.0": "lbl_9_rodata_0", "@15": "lbl_9_rodata_14",
"@16": "lbl_9_rodata_18"}`. This needs the layout/pool priming path
(`fzgx fixup` with a seeded corpus, or `fzgx data-import` pool_objects), not
more matcher attempts. Best body:
`.fzgx/attempts/colchg_selmate_disp.1790882668.c`.

`colchg_menu_disp` stays at 94.7% (best body 90.1% locally). Two hand
variants both regressed and are recorded so they are not retried: the
inline ternary argument form gives 87.9%, and hoisting the format string into
its own local spills r28 and gives 81.7%. The 8 differing rows are the
`fn_1_496FC` argument materialisation order (retail schedules
`lfs f2 lbl_9_rodata_18[0]` before `stw r30`; the candidate schedules it after)
plus the switch guard register: retail keeps the sign-extended induction
variable in **r5** for both `cmpwi r5, 0x73` and `cmpw r0, r5`, while the
candidate reuses r3/r4. A fresh candidate should aim to keep the loop
induction in r5 and to let the second float load hoist with the first.

`fn_3_15A0` (customize, 7680 B) is **blocked as unrecoverable locally**. All 8
attempts' best bodies (peak 60.6%) live only under
`/Users/rayan/fzgx/.fzgx/attempts/`; neither `state/repairs/large_near_frontier_20260914`
nor `large_closures_20260915` contains it, so there is no seed to hand-decomp
from. 8 attempts across 4 model tiers all plateaued 54.7-60.6%, which is the
measured stopping condition. Unblocking requires copying the rayan-host bodies
into `state/repairs/` as a portable archive, or a fresh carve with a new seed.

## Link-rejected object-perfect bodies (2026-10-03, see docs/findings/279)

Two of the six are now matched (see docs/findings/279 for the tool and the
full result table). Six functions were object-perfect and link-rejected because their
`.fzgxpool` layout
primer is never stripped: `poolfix.apply` runs only under `matched_pool`, and a body
with `pool_rows: 0` never reaches it. Five are at exactly 100.0%: `fn_3_17098`,
`fn_1_7E8F4`, `fn_1_C6F8C`, `fn_1_FC760`, `fn_8_704`; plus `colchg_selmate_disp`
at 99.61%. The recipe, validated end to end on `fn_3_17098` (object 100%, module
byte-identical to retail, `16 files OK`): drop the `.fzgxpool` block but keep its
file-scope declarations, include `types.h` plus the module headers, then delete
exactly the globals the linker names `multiply-defined`. Corrected body archived at
`.fzgx/attempts/fn_3_17098.PRIMERLESS-100.c`.

Do not read "Section '.fzgxpool' is unknown" as the failure; every REL link prints it
and still returns 0. And a `15 files OK / 1 FAILED` line under fleet load is usually a
stale mid-write read: re-run `build/tools/dtk shasum -q -c config/GFZE01/build.sha1`
by hand before believing a rejection.

## Split-ownership gaps block a third class of object-perfect bodies (2026-10-03)

Two more 100.0% bodies are blocked for a reason unrelated to the `.fzgxpool` primer
(see docs/findings/279). Both fail at the REL resolve or at module size, not at link:

- `fn_12_23410` (movie_module, 752 B): object 100.0%, but the module grows by 280 bytes.
  The body declares a private `u8 lbl_12_bss_7250[2048]` scratch buffer. Retail does
  have `lbl_12_bss_7250` (`movie_module/symbols.txt:1053`, `.bss:0x00007250`, size
  0x800), but that symbol appears in **no** entry of `movie_module/splits.txt`, so
  linking the unit adds storage the retail module does not have. Fix is a split (or a
  reference to whichever unit ends up owning it), not a C edit.
- `fn_8_704` (title, 80 B): `Failed to find symbol fn_1_14F118 in any module`.
  `fn_1_14F118` is matched and defined in `src/rel/main_rel/sel_static_disp.c`, so the
  REL resolve cannot see an existing definition. Baseline without the unit is
  `16 files OK`, so this is not pre-existing breakage.

Both are object-perfect and neither can be advanced by any amount of matcher effort.
They need split ownership work, which is a different owner than the matcher.

## Verify's hash read races with the fleet

`verify` relinks and then runs `shasum -c` immediately. Under live fleet load that read
can land mid-write and report `<module>.rel: FAILED / 15 files OK` for a module that is
byte-identical to retail. Observed four times in this cohort; every one checked by hand
came back `16 files OK` with zero differing bytes. Two of them were then re-submitted and
verified normally. **Do not re-derive a body because of this line.** Confirm with:

    build/tools/dtk shasum -q -c config/GFZE01/build.sha1

and compare bytes against the retail file (`main_rel` is `files/enemy_line/main.rel`,
the others are `files/fze.<module>.rel`). A real layout problem shows a *size* delta or
non-zero differing bytes; `fn_12_23410` did, which is how it was told apart from the
races.
