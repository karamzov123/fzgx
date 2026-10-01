# Finding 105: OS unit @1 sites are MWCC-unreachable; dtk split owns the name

Date: 2026-08-25 (session v6)

## Claim attempted
Symbolize the 4 ADDR16_HA/LO sites in `main/dolphin/os/OS` that target the
local `.data` object `@1 = .data:0x801225B0` (OSInit x2, OSExceptionInit x2).

## Result: NOT mechanically reachable — extend the $/@ lesson
1. Renaming `@1` -> `lbl_801225B0` in config/GFZE01/symbols.txt does NOT change
   the retail-side object: dtk's `dol split` generates local symbol names as
   `@<n>_<addr>` itself. The split obj always carries `@1_801225B0`.
2. Adding an alias line (`lbl_801225B0 = ...` alongside `@1`) makes dtk split
   PANIC ("no group named 'attrs'") — duplicates not supported.
3. MWCC inline asm cannot declare or reference leading-`@` symbols (extern decl
   is a syntax error; same class as findings v5 `$`/`@` lesson).
4. Net: linker "undefined lbl_801225B0" either way. Only exit was reverting.

## Side observations (for future natural-C work)
- OSInit in target calls DCInvalidateRange(r30, 0x20) before DVDInquiryAsync
  setup at _8000a74c; our asm transcription omits it yet DOL still links EXACT —
  because both orderings produce identical bytes? No: ours differed pre-link but
  post-link DOL was exact only with HEAD version. HEAD OS.c == gate-green.
- Remaining OS-unit pre-link diffs are @1 relocs + sda21 sites + those addi
  r31/r29 R_PPC_NONE addend-relocs against @1 (string-base offsets). All wait
  for natural-C conversion of OSInit/OSExceptionInit/ClearArena.
- If a symbols.txt rename is ever needed for split objects, remember: renames
  require re-split (delete build/GFZE01/config.json AND stale obj/*.o files;
  ninja does not track removed objs otherwise).

## Rule
Skip all worklist sites whose target symbol starts with `@` or contains `$`
(printf's `@stringBase0_*`, OS's `@1_801225B0`, OSCacheRest's `@69_80122828`).
Mark them natural-C-only in the queue tooling.
