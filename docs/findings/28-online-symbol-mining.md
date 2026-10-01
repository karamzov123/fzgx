# GFZE01 symbol-name mining: fzero-gx-online findings/ + docs/mode-scene-tables.md

Sources: 66 findings files + mode-scene-tables.md. Cross-checked against config/GFZE01/symbols.txt.

## A. In-main.dol (0x80003100-0x801A7930) function addresses with semantic meaning

| Address | Online-project name | Evidence | symbols.txt status |
|---|---|---|---|
| 0X800032A8 | `count_zero_write_target` | writes 0 during init; target of 0x8000564C call chain (VS entry-count source hunt) (06-count-source.md:70) | **unmatched** (free rename) |
| 0X8000564C | `vs_count_init_caller` | caller chain 0x8000564C -> 0x800032A8 (06-count-source.md:70) | **unmatched** (free rename) |
| 0X800059E4 | `main_init_callstack_frame` | in init callstack 0x802402C4->0x801F2A5C->0x801BBAD4->0x801BAE54->0x800059E4 (06-count-source.md:69) | **unmatched** (free rename) |
| 0X8000C558 | `archive_load_loop_lr` | LR frame of _e archive loop; multiplex-crash LR (07-ceiling.md:206) | **unmatched** (free rename) |
| 0X8000DCC0 | `irq_disable_blrl_restore_dispatcher` | disable -> blrl -> restore dispatcher (13-results-screen-hang.md:65) | **unmatched** (free rename) |
| 0X8000E9FC | `dev_console_path` | dev-console path (simulated vs physical time) (07-ceiling.md:255) | **unmatched** (free rename) |
| 0X80034634 | `gx_state_flush` | GX state-flush containing innermost hang frame 0x8003466C (13-results-screen-hang.md:250) | **unmatched** (free rename) |
| 0X8003466C | `gx_state_flush_inner` | innermost frame of results-screen hang stack (13-results-screen-hang.md:245) | **unmatched** (free rename) |
| 0X8003762C | `render_cluster_callee_a` | bl from 0x8020CDE4 (differential render cluster) (17-differential-render-cluster.md:34) | **unmatched** (free rename) |
| 0X8003A1F8 | `render_cluster_callee_b` | bl from 0x8020CD80 (differential render cluster) (17-differential-render-cluster.md:31) | **unmatched** (free rename) |
| 0X80050690 | `sole_xref_caller_of_8012D9B8` | exactly one xref to lbl_8012D9B8 (01b-static-analysis.md:174) | **unmatched** (free rename) |
| 0X8006A420 | `arc_read_crash_frame` | frame in Invalid-read crash chain w/ 0x8000C558 (07-ceiling.md:206) | **unmatched** (free rename) |
| 0X8015CA8C | `os_timer_globals_region` | OS timer globals keep advancing inside heap 0 (13-results-screen-hang.md:48) | **unmatched** (free rename) |

### Empirically confirmed existing names (already correct — no action needed)

- 0X80008E84 online name `OSSetCurrentHeap_thunk` — symbols.txt: `OSSetCurrentHeap_thunk`
- 0X80008EC8 online name `fn_80008EC8` — symbols.txt: `fn_80008EC8`
- 0X80008FB0 online name `fn_80008FB0` — symbols.txt: `fn_80008FB0`
- 0X80009830 online name `OSAllocFromHeap` — symbols.txt: `OSAllocFromHeap`
- 0X8006A3DC online name `ARCInitHandle` — symbols.txt: `ARCInitHandle`
- 0X8006A3F8 online name `(ARC crash PC)` — symbols.txt: `NOT PRESENT`

## B. In-dol data-region map (useful for data-section carving)

| Address | Current name | Region | Evidence |
|---|---|---|---|
| 0X8008FF40 | `lbl_8008FF40` | data seg (size 0x20) | 01b-static-analysis.md DOL layout table |
| 0X80095EA0 | `lbl_80095EA0` | data seg 0x092ea0 size 0xC5A80 (saves live here; syncsaves analysis) | 01b + 43 |
| 0X8012AA08 | `lbl_8012AA08` | inside data seg 80095EA0..8015F91F | 43-syncsaves-analysis.md |
| 0X8015B920 | `lbl_8015B920` | BSS start (BSS 0x4C00C bytes) | 01b-static-analysis.md |
| 0X801A63C0 | unmatched | data seg 0x158920 size 0x2E0 (settings/lang globals nearby) | 01b-static-analysis.md |
| 0X801A6730 | `lbl_801A6730` | global set at 0x80003334 (asset-budget global) | 10-asset-budget.md |
| 0X801A6744 | `lbl_801A6744` | global set at 0x80003334 | 10-asset-budget.md |
| 0X801A66B4 | `lbl_801A66B4` | *(u32*) language/global index read | 48-difficulty-and-settings-row.md |
| 0X801A6E40 | `lbl_801A6E40` | data seg 0x158C00 size 0xAC0 | 01b-static-analysis.md |
| 0X801A792C | unmatched | BSS ends 0x801A792C; heap arena ptr R = *(u32*)0x800030C8 points past it | 01-racer-array.md |

Other in-dol unmatched low-level frames (weak semantics, low rename value): 0X800032AC, 0X80003334, 0X80005650, 0X800059E8, 0X800091A1, 0X8000DCE4, 0X8000EA2C, 0X8001071C, 0X80021A28, 0X8002917C, 0X8006A3F8, 0X801A63C0, 0X801A792C

Counts: 61 unique in-dol addresses referenced; 35 already present in symbols.txt (mostly SDK/lib names), 26 unmatched.

## C. RESERVED FOR M4 — non-main.dol addresses (0x801A7930 < a, incl. REL stubs & >=0x80300000)

967 unique overlay/high addresses referenced across the corpus. Key clusters:

### Mode/scene machinery (permanent side, from mode-scene-tables.md)

| Address | Proposed name |
|---|---|
| 0x801BBCF0 | `scene_noop_init (SMD_*_TOP sentinel)` |
| 0x801BBD38 | `scene_noop_main` |
| 0x801BBD3C | `scene_noop_exit` |
| 0x801BB360 | `MD_ERRORDISP init` |
| 0x801F0D6C | `MD_GAME init` |
| 0x801F154C | `MD_GAME f36` |
| 0x801F24E4 | `MD_GAME f40` |
| 0x801F28D4 | `MD_GAME exit` |
| 0x801EAA90 | `raceSceneInit` |
| 0x803283C0 | `modeTable (48-byte records {name[32];init;f36;f40;exit})` |
| 0x80328720 | `sceneTable (44-byte records {name[32];init;main;exit})` |
| 0x80264138 | `rel_stub_mode_init` |
| 0x802641A0 | `rel_stub_mode_f36` |
| 0x802641CC | `rel_stub_mode_f40` |
| 0x802641F8 | `rel_stub_mode_exit` |
| 0x80264220 | `rel_stub_scene_init` |
| 0x8026424C | `rel_stub_scene_main` |
| 0x80264278 | `rel_stub_scene_exit` |
### GAME scene handlers (all permanent, from mode-scene-tables.md)

| Address | Proposed name |
|---|---|
| 0x801F2D00 | `SMD_GAME_COURSE_INTRO init` |
| 0x801F2F24 | `SMD_GAME_COURSE_INTRO main` |
| 0x801F2FE0 | `SMD_GAME_COURSE_INTRO exit` |
| 0x801F30B8 | `SMD_GAME_CUSTOMIZE init` |
| 0x801F35F8 | `SMD_GAME_CUSTOMIZE main` |
| 0x801F3730 | `SMD_GAME_CUSTOMIZE exit` |
| 0x801F3AC8 | `SMD_GAME_COURSE_VIEW init` |
| 0x801F3C2C | `SMD_GAME_COURSE_VIEW main` |
| 0x801F3E5C | `SMD_GAME_COURSE_VIEW exit` |
| 0x801F4008 | `SMD_GAME_RACE_START init` |
| 0x801F469C | `SMD_GAME_RACE_START main` |
| 0x801F4A80 | `SMD_GAME_RACE_START exit` |
| 0x801F4D00 | `SMD_GAME_RACE init` |
| 0x801F4D6C | `SMD_GAME_RACE main` |
| 0x801F5900 | `SMD_GAME_RACE exit` |
| 0x801F5B58 | `SMD_GAME_RACE_RESULT init` |
| 0x801F6210 | `SMD_GAME_RACE_RESULT main (incl. hook site 0x801F6628 vicinity)` |
| 0x801F72BC | `SMD_GAME_RACE_RESULT exit` |
| 0x801F77EC | `SMD_GAME_FAIL_RESULT init` |
| 0x801F78D0 | `SMD_GAME_FAIL_RESULT main` |
| 0x801F85D8 | `SMD_GAME_FAIL_RESULT exit` |
| 0x801F87E8 | `SMD_GAME_OVER init` |
| 0x801F88BC | `SMD_GAME_OVER main` |
| 0x801F900C | `SMD_GAME_OVER exit` |
| 0x801F9064 | `SMD_GAME_PILOTPOINT init` |
| 0x801F90AC | `SMD_GAME_TICKET init` |
| 0x801F9114 | `SMD_GAME_TICKET main` |
| 0x801F94AC | `SMD_GAME_TICKET exit` |
### ACSETUP (REL 0x8059xxxx region)

| Address | Proposed name |
|---|---|
| 0x80310F0C | `MD_ACSETUP init` |
| 0x80310F4C | `MD_ACSETUP f36` |
| 0x80310F6C | `MD_ACSETUP f40` |
| 0x80310F70 | `MD_ACSETUP exit` |
### Task pool / module dispatcher (findings 13,14,18,30)

| Address | Proposed name |
|---|---|
| 0x801BE618 | `taskPool_start_timer(5)` |
| 0x801BE654 | `taskPool_stop_timer(5)` |
| 0x801BE72C | `taskPool_init(buffer,capacity)` |
| 0x801BE9CC | `task system access (lwz r9,22348(r7))` |
| 0x801BEA4C | `task dispatch helper` |
| 0x801BEAB8 | `task dispatch entry` |
| 0x801BB908 | `module dispatcher (indexes table by active module)` |
| 0x801BAE54 | `callstack frame above main-loop` |
| 0x801BBAD4 | `callstack frame (results path)` |
| 0x801BB1F0 | `20-entry table initialiser (VS results)` |
| 0x801BB520 | `loop-bounded-by-18 A` |
| 0x801BB6C0 | `loop-bounded-by-18 B` |
| 0x801BBC78 | `normal-path state loader 801BBC78..801BBCA4` |
| 0x801BBCC8 | `scene-machine call target` |
### Heap/memory globals (findings 01,07,10,11,12)

| Address | Proposed name |
|---|---|
| 0x801AE3C0 | `r13 small-data base: HeapArray + __OSCurrHeap` |
| 0x801BA960 | `heap arena ptr R (stable boot value; alt 0x801BA720)` |
| 0x801B7940 | `arena lo (~22MB to 0x817E0680)` |
| 0x801BDDCC | `writer of 0x803753A0 in 4-controller loop (results input gate)` |

Note: hook site 0x801F6628 and scene-table pointer 0x80352B0C from the task brief do NOT appear verbatim in the mined docs; the tables doc gives scene table @ **0x80328720** and modes @ **0x803283C0** instead (mem1-inrace build; doc warns bases move per dump — re-derive by signature `MD_ADV\0` / `SMD_ADV_TOP\0`).

Full raw hit list: /tmp/sa-mining/all_hits.txt
