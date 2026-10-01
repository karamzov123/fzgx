# Finding 22 — data-section string xref naming + MTXHead contains Dolphin ARC

Date: 2026-08-23 (session 17). Method: read `.symtab` + `.rela.text` of every
linked object under `build/GFZE01/obj/`, collect `lbl_*` relocations per
function, resolve each target VA against retail main.dol data sections and
keep printable ASCII runs ≥4 bytes.

## Results

1. **~85 `lbl_` data symbols are string-backed** and can be renamed safely
   (data units link from asm blobs; symbol renames in symbols.txt do not move
   bytes as long as names stay unique). Full map saved at
   `/tmp/lbl_strings.json` (regenerate: script inline in session log; needs
   pyelftools — `/tmp/fzvenv/bin/python`).
2. **fn_8006A3DC (in dolphin/mtx/MTXHead.o) is Dolphin `ARCInitHandle`**:
   asserts `"ARCInitHandle: bad archive format"` with `__FILE__` = lbl_801A6610
   = `"arc.c"`. MTXHead.c [0x80069AE0–0x8006CE44, 59 funcs] is therefore a MIX
   of Dolphin archive (ARC) code and MTX code. ARC reference source is NOT in
   melee's extern/dolphin (no ar/ARC there beyond stubs) — use
   libogc/ModernGekko vendor sources or decomp.dev dolphin headers for shape.
3. Cross-pollination audit: fzero-gx-online findings/docs name almost NO
   in-dol (<0x801A792C) addresses — its named symbols are overlay/REL (M4).
   Only usable nuggets: dev-console heap path @0x8000E9FC–0x8000EA2C (inside
   already-matched OSMutex region), CARD SDK stamp @0x8012AA08. The mode/
   scene tables ARE the M4 seed table (48-byte mode records @0x803283C0 etc.).

## Naming conventions for the wave

- String-backed: rename `lbl_XXXXXXXX` → derived snake_case name with
  `_str`-style suffix avoided; prefer semantic name from content
  (e.g. lbl_80090A28 → `ADXT_GetStatPause_param_err_str`). Keep it mechanical:
  `<SRC>_<verb-noun>_msg` style acceptable; uniqueness is what matters.
- Do NOT rename fn_ symbols without positive evidence (string assert inside
  the function naming its SDK identity, or melee/SDK source correspondence).

## TRAPS

- Renaming symbols referenced by OTHER objects' asm blobs requires the new
  name to exist in symbols.txt globally before rebuild — edit symbols.txt,
  then full clean rebuild (`rm -rf build/GFZE01/{config.json,asm,obj}` +
  configure.py + ninja). Gate must stay green; sha1 unchanged by renames.
