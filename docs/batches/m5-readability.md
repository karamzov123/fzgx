# M5 — readability program: translation units, types, revise (2026-09-08)

Trigger: after ~800 cheap-tier matches the tree was byte-exact but unusable:
generated names everywhere, 100 units defining private one-off structs,
45 units doing `(u8 *)` pointer arithmetic on globals declared 10–16 times
with no shared definition.

## Translation units recovered from `__FILE__` strings

CodeWarrior keeps each TU's literals in that TU's `.data` block and links every
section in one TU order. main.rel embeds 62 source file names (`camera.c`,
`coli.c`, `driver.c`, `bg_*.c`, ...); 50 of them are referenced from code.
Their `.data` addresses and the `.text` addresses of their referencing
functions are monotonic together (zero order violations), so each string
anchors one TU. `tools/seeds/debug_strings.py` writes
`config/GFZE01/main_rel/tus.json`: 50 TUs, all 3,230 functions assigned (72
runtime/prolog functions before the first anchor). Boundary refinement uses
only *private* data votes (a literal referenced by ≤ 3 functions), since
shared globals are referenced from every file.

`fzgx tu-organize` moved 965 units into `src/rel/main_rel/<tu>/` with one
verified relink.

## Types recovered from the disassembly

`fzgx structs <global>` follows every `lis/addi` materialisation of a global
through the base register (with `addi`/`mr` deltas) and records each field
access (offset, width, float, loads/stores) across the whole module, matched
or not; pointer fields are followed one level, so the objects behind them get
their own typedefs. `fzgx headers --write` emits
`include/rel/main_rel/globals.h` for every global referenced by ≥ 20
functions (28 globals, 21 typedefs) and compiles an `offsetof` self-check
under MWCC: every field is proven to sit at the offset its name claims.
Misaligned accesses are emitted as bytes; objects are padded to dtk's symbol
size.

Known limit: dtk under-sizes some data symbols (`fzgx oversize` lists 30+,
e.g. `lbl_1_bss_3C00` is 4 bytes but accessed up to +0x180C). Those need
symbols.txt size corrections by the librarian; the header stays clamped to
dtk's sizes meanwhile. The rodata entries in that report are constant pools
addressed through a shared high half, not structs.

## Revise mode

Agent ids prefixed `revise-` may claim a matched function; `write_unit` +
`submit` replace the source only if the unit still fully matches, otherwise
the original is restored. Proven by hand on `fn_1_150518`: the
`(MainObject *)*(u32 *)((u8 *)lbl_1_data_2A7E0 + 0x3c)` version became
`lbl_1_data_2A7E0.unk_3C->unk_84 = 0;` against the header at 100%.
The matcher context bundle now shows header typedefs for referenced globals.

## Naming

`fzgx naming-bundle <tu>` gives a librarian model the TU's matched sources,
callers/callees, pointer-table references and the TU's own string literals;
`fzgx naming-apply` validates and applies renames with one relink. Pilot on
`ghost.c` (Sonnet 5): 6 conservative names for $0.71, no structs; the matched
functions were mostly stubs and getters, so there was little to name. Names
become meaningful once whole TUs are matched against shared types, which is
why types came first.

## Revise batch r1 (Luna, 28 cast-heavy units)

28/28 rewritten against `globals.h` and kept at 100%, one relink, $0.15, 79 s.
Before/after example (font.c):

    s16 index = *(s16 *)((u8 *)lbl_1_bss_4B9CC + 0xc);              ->  s16 index = *(s16 *)&lbl_1_bss_4B9CC.unk_C;
    *(f32 *)((u8 *)lbl_1_bss_4B9CC + 0x1c) = value1 * scale;        ->  lbl_1_bss_4B9CC.unk_1C = value1 * scale;

The residual cast exposed a header limit: the emitter took the widest access
per offset. It now keeps the narrower width when the neighbouring slot is
used or the narrow accesses dominate, and `lha` loads become `s16` (28 signed
halfword fields in the header). Arrays of fixed-stride records
(`lbl_1_data_1AEA8[index * 14 + 13]`, 0x38-byte font metrics) are the next
analyzer target.

Luna naming pilot on effect.c (64 matched functions): 11 renames of the
`effect_queue_event_2` kind for ~$0.02. Same verdict as Sonnet on ghost.c: not
applied; naming waits until a TU is matched as a whole.

## Next

1. Revise batch over the 45 cast-heavy and 100 private-struct units (Luna,
   oracle-guarded), then re-run naming per TU.
2. Match by TU: batches select whole files (all functions of `camera.c`), with
   the header in every bundle.
3. Librarian: symbols.txt size corrections from `fzgx oversize`, field naming
   in `globals.h`, promotion of per-unit structs into `include/rel/main_rel/<tu>.h`.
