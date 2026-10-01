# Symbol seed review — 2026-08-28

Source: `findings/28-online-symbol-mining.md`, cross-checked against the
canonical `config/GFZE01/symbols.txt` and current ELF/xref tooling.

## Confirmed existing names

The six entries listed in finding 28 section “Empirically confirmed existing
names” are already represented where applicable. No changes are required.

## Proposed function names — useful, but not safe automatic seeds

These 12 names have semantic evidence from runtime traces/xrefs, but the finding
marks their addresses as unmatched rather than providing a definitive function
start/size/type record:

- 0x800032A8 `count_zero_write_target`
- 0x8000564C `vs_count_init_caller`
- 0x800059E4 `main_init_callstack_frame`
- 0x8000C558 `archive_load_loop_lr`
- 0x8000DCC0 `irq_disable_blrl_restore_dispatcher`
- 0x8000E9FC `dev_console_path`
- 0x80034634 `gx_state_flush`
- 0x8003466C `gx_state_flush_inner`
- 0x8003762C `render_cluster_callee_a`
- 0x8003A1F8 `render_cluster_callee_b`
- 0x80050690 `sole_xref_caller_of_8012D9B8`
- 0x8006A420 `arc_read_crash_frame`

Disposition: proposed names only. Keep them in findings/ until each has a
canonical function-start boundary, size, binding/type, and at least one
independent caller/relocation confirmation. Do not import them into
`symbols.txt` yet.

## Data/weak-semantic entries

The remaining unmatched entries are low-confidence labels or data objects:

`0x800032AC`, `0x80003334`, `0x80005650`, `0x800059E8`, `0x800091A1`,
`0x8000DCE4`, `0x8000EA2C`, `0x8001071C`, `0x80021A28`, `0x8002917C`,
`0x8006A3F8`, `0x801A63C0`, `0x801A792C`, plus the data labels at
`0x8008FF40`, `0x80095EA0`, `0x801A6730`, and `0x801A6744` described in the
source finding. These need data-size/relocation evidence before naming.

## Safe seed-file policy

`config/GFZE01/symbol_seeds.csv` is intentionally a valid empty input until a
candidate passes the above checks. This prevents speculative names from
changing dtk symbol ownership or build boundaries. The importer is healthy and
ready for reviewed rows.

## Next evidence action

For each proposed function, run `find_xrefs.py --cslice <symbol-or-address>`
against the authoritative object, record section/size/relocations/callers, and
only then promote a row to the seed CSV. A semantic label alone is not enough.
