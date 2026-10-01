# Toolchain bring-up: dtk + wibo + MWCC GC compilers

2026-08-21, session 1. Everything verified by running it, not reading about it.

## What works

- **dtk v1.8.3** (linux-x86_64 release binary). `dtk dol config`,
  `dtk dol split`, `dtk dol info` all run on our DOL.
- **MWCC GC compilers** obtained from the official decomp.dev mirror:
  `https://files.decomp.dev/compilers_20251118.zip` (same tag melee pins).
  Contains GC/1.0, 1.1, 1.1p1, 1.2.5, 1.2.5n, 1.3, 1.3.2, 1.3.2r, 2.0, 2.0p1,
  2.5, 2.6, 2.7, 3.0a3.x, 3.0a5.x — each with mwcceppc/mwasmeppc/mwldeppc +
  lmgr326b.dll.
- **wibo** (decompals/wibo) runs `mwcceppc.exe` fine: compiled a test C file
  with GC/1.3 and GC/2.0 to PowerPC objects (648 bytes each). No wine needed
  for the compiler.
- The dtk-template build system (`configure.py` + ninja) works end to end up
  to the link step: analysis → split → objdiff report → link invocation.

## What broke

### mwldeppc.exe under wibo

| linker | wibo 1.0.3 | wibo 1.2.0 |
|---|---|---|
| GC/1.0 | segfault (139) | segfault |
| GC/1.3 | segfault | segfault |
| GC/1.3.2 | — | segfault |
| GC/2.0 | — | segfault |
| GC/2.7 | — | exit 1: `internal linker error: File: 'ELF_linker.c' Line: 5257.` |

The GC/2.7 failure is a *linker* error, not a crash — plausibly caused by our
degenerate input (11 blob objects from an unseeded split, extabindex warning
before the alert). Do not conclude 2.7 is broken; retry after the split is
fixed.

The segfaults reproduce with both wibo versions on multiple linkers, so it may
be wibo's PE emulation hitting an mwld quirk (mwld is known to use unusual PE
features). Melee's CI uses wine (ubuntu winehq) historically; newer projects
use wibo successfully with *some* linker versions.

**Next lever:** Bottles' soda-9.0-1 wine (`~/.local/share/bottles/runners/soda-9.0-1/bin/wine`)
as `configure.py --wrapper`. multilib is present so 32-bit wine should work.
Set `WINEPREFIX` to a project-local prefix, `-WINEDEBUG=-all`, headless.

## Split state (unseeded)

First `ninja` ran `dtk dol split` against a config.yml with no symbols.txt /
splits.txt: analysis found **278 functions**, wrote **11 auto_* blob objects**
(whole-section chunks), warned:

- `Failed to locate extab/etabindex: Failed to locate section @ 0x8008FEE4`
- `Unknown section .bss2`
- `Unknown section .data7`

The DOL has nonstandard extra sections (.bss2, .data7) — expected for this
game (the online project's overlay work hints at custom section layout).
These need explicit handling in splits/config later.

278 functions is far below truth (Ghidra found ~2100 in the same DOL). The
documented first-run flow is `dtk dol config main.dol -o config.yml` which we
ran — but it emitted only object+hash. The template docs (docs/getting_started.md)
describe generating symbols/splits; follow that next time instead of guessing.

## Traps hit

- `Encounter/gc_wii_compilers` GitHub repo does not exist publicly (auth
  prompt) — the compilers live on files.decomp.dev, not GitHub.
- wibo "latest/download/wibo" URL pattern 404s; must use per-asset URLs like
  `releases/download/1.2.0/wibo-x86_64`.
- zsh eats bare `===WORD===` echo args (=cmd expansion); quote them.
