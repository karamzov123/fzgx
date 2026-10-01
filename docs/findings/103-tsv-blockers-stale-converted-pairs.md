# 103: TSV blocker classes were stale — converted @ha/@l pairs mislabeled; band reality check

Date: 2026-08-24 late. Follows 102.

## Tooling fix (committed 855dce9)
`tools/unit_status.py::classify_pair` fired on lis/addi pairs whose OUR-side lis
already carried an R_PPC_ADDR16_HA reloc — i.e. pairs already converted to
`sym@ha`/`sym@l`. Because classify_pair runs BEFORE the site-level ADDR16_HA
tolerance, these done sites were labeled GLOBAL-NAMED/LOCAL-SCOPE blockers.
That manufactured the entire "185 GLOBAL-NAMED funcs / 87,808 B" queue in
docs/UNIT-STATUS.tsv and the mission directive item (c).

Fix: skip pair classification when rl_prev contains R_PPC_ADDR16_HA. After regen:
- all 2235 functions byte_exact_or_reachable_only = 2235
- TOLERATED 1799, LOCAL-SCOPE 0, GLOBAL-NAMED 0, UNNAMED-TARGET 0, REAL-DIFF 0

## Implication for the 90–99% objdiff band
Spot checks (SndMarkChannelVoicesForUpdate, EXIInit) show the residual pre-link
diffs are MWCC-auto-emitted `sym@sda21` relocs vs retail raw encodings — linked
DOL is byte-exact (gate green). The remaining ~300 KB matched_code gap is NOT a
relocation-symbolisation gap anymore; it lives in units that are fuzzy for real
reasons (unconverted logic, float/paired-single codegen). Next levers are
natural-C conversion of genuinely-wrong functions, not reloc sweeps.

## convert_global_named.py status
Ran across every unit named by the old directive queue: 0 conversions available
(all sites already converted). The tool remains valid for future new sites but
its bank is empty as of this commit.
