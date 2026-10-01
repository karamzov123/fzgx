# Online-project addresses live above main.dol: they are overlay/REL symbols

2026-08-21, session 3. Amends the M1 assumptions in docs/PROGRESS.md;
does not supersede any finding's results (00 and 01 remain valid).

## Claim

Nearly all named addresses harvested from fzero-gx-online (loadStage,
MD_GAME_init, mode tables, g_cfg, ...) lie ABOVE main.dol's extent. They are
runtime-loaded REL/overlay code+data, not main.dol symbols. Only one seed,
OSSetCurrentHeap_thunk @ 0x80008E84, falls inside the DOL.

Consequence: M1-style symbol seeding into main.dol's symbols.txt is mostly
moot. The seed table matters at M4, once the overlay/REL binaries are split.

## Evidence A: DOL header extents

Decoded from `orig/GFZE01/sys/main.dol`
(sha1 421c88106697d3275a3fc26fb7a01bf6d816b271):

| field | value |
|---|---|
| .init (text[0]) off/addr/size | 0x100 / 0x80003100 / 0x24E0 |
| .text (text[1]) off/addr/size | 0x25E0 / 0x800055E0 / 0x8A920 |
| .text end | 0x8008FF00 |
| dtors (data[0]) off/addr/size | 0x8CF00 / 0x8008FF00 / 0x20 |
| sect1 (data[1]) off/addr/size | 0x8CF20 / 0x8008FF20 / 0x20 |
| .rodata (data[2]) off/addr/size | 0x8CF40 / 0x8008FF40 / 0x5F60 |
| .data (data[3]) off/addr/size | 0x92EA0 / 0x80095EA0 / 0xC5A80 |
| .sdata (data[4]) off/addr/size | 0x158920 / 0x801A63C0 / 0x2E0 |
| .sdata2 (data[5]) off/addr/size | 0x158C00 / 0x801A6E40 / 0xAC0 |
| bss addr/size | 0x8015B920 / 0x4C00C (single bss field spans sdata/sbss/sdata2/sbss2 — see PROGRESS Session 1) |
| entry | 0x80003154 (__start) |

NOTE (session-3 correction): an earlier version of this table misdecoded
several file offsets (big-endian field confusion); the addresses above are
cross-checked against a live MEM1 dump — every non-mutated section appears
verbatim at its linked address (tools/probe/dol_ram_recon.py). Max virtual
address covered by main.dol: **0x801A792C** (unchanged).

## Evidence B: seed-table coverage

`python3 tools/merge_seeds.py` over the full 63-entry SEEDS table reports:

- renamed/annotated: 1 — OSSetCurrentHeap_thunk @ .text:0x80008E84
- no covering symbol (NOT injected): 62

Lowest missed seed: modeManager @ 0x801BB908 > 0x801A792C. Every other named
address from the online project (mode tables 0x803283C0 / 0x80328720,
g_cfg 0x803785D0, g_modeState 0x80375300, loadStage 0x80235718, ...) sits in
the ~0x801B0000–0x804xxxxx window far above the DOL's last byte.

## Consequences for milestones

- M1 (symbol infrastructure): proceed with objdiff report + ledger
  automation only; mass seeding of main.dol is pointless.
- Keep tools/merge_seeds.py as the canonical seed table (names, roles,
  kinds); re-apply per overlay module at M4.
- The 62 misses are not errors to fix — they are the M4 work list.
- Open question (M4 prep): what maps 0x801B0000–0x802FFFFF? Working
  hypothesis: REL modules loaded into the arena above dol bss + static
  overlays at 0x80300000+. Binaries live in the ISO
  (/home/armandofm/projects/fzero-gx-online/build/fzgx-us.iso,
  sha1 c83fd6c2eca14955b3a87a6416a5ac1705c2de7e).

## Adjacent fixes this session (commit 1fe8150)

- dtk rejects unknown `key:value` attrs in symbols.txt comments
  ("Unknown symbol attribute 'seed'"): merge_seeds.py now emits only
  type:/size:/scope:. Seed provenance lives in SEEDS, not symbols.txt.
- `dtk shasum -c` requires `<sha1><SP><SP><path>` lines; a bare hash fails
  with "Invalid line". config/GFZE01/build.sha1 now checks the GENERATED
  build/GFZE01/main.dol, so the default ninja target self-verifies the
  byte-match on every full build.
