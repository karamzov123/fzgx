# Block-based translation-unit files (2026-09-08)

One function per file was the matcher workspace leaking into the tree. The
canonical source is now one file per recovered translation unit, in retail
order, while the link, the oracle and the agents stay per function.

## Layout

```
src/rel/main_rel/camera.c            one file per TU from tus.json, blocks in address order
  #include "rel/main_rel/globals.h"  prologue (frozen: a block never changes its neighbours)
  /* fzgx:begin fn_1_88D8 */         tooling-owned markers
  ...                                declarations the function needs, its data, the function
  /* fzgx:end fn_1_88D8 */
build/GFZE01/gen/rel/main_rel/camera/fn_1_88D8.c   generated = prologue + block; one split range,
                                                   one object, hash-checked like before
.fzgx/work/<key>.c                   an agent's private copy; the tree is untouched until submit
```

- `claim` gives the agent a work copy; `write_unit`/`check` compile that copy
  (assembled with the TU prologue) into the unit's own object and objdiff one
  symbol; `submit` splices the copy in as a block under a write-only lock;
  `release` keeps the best copy under `.fzgx/attempts/`. Sixteen agents on one
  file never wait for each other.
- A block flagged `noprologue` compiles with its own includes only (a private
  typedef that clashes with the header). Includes an agent adds stay inside its
  block; `fzgx tu-hoist` moves them into the prologue when every block of the
  file still compiles. `fzgx tu-check` compiles the whole file as one unit, the
  goal state a TU reaches once its private declarations are gone.
- `fzgx tu-migrate --module M` converts a module once `tus.json` exists.
  Functions without a TU (the DOL, prolog/epilog stubs) keep one file each.

## Migration

| module | blocks | TU files | dropped nonmatching units | noprologue |
|---|---|---|---|---|
| main_rel | 617 | 47 | 412 | 29 |
| customize, title, story, sel, replay, pilotpoint, winning, interview, option | 89 | 15 | 0 | 0 |

All 16 targets hash after each step. The 412 nonmatching units (best attempts
of released functions) left the build: their bodies are in `.fzgx/attempts/`
and reach the next attempt through the context bundle; their split ranges went
back to the auto units, so a later claim re-carves them. camera.c is one file
of 47 blocks; `tu-check` reports one error left in it (a private
`GameCameraEntry` copy in a block that predates the header).

The migration itself landed inside the commit "carve: 3 units for batch
blocks-m1" (the batch runner's carve commit picked up the pending tree); the
content is right, the message is not.

## Flow test

| batch | flow | result | cost |
|---|---|---|---|
| blocks-r1 | revise two camera.c blocks | 2/2 kept, spliced, link-verified | $0.012 |
| blocks-m1 | match three small main.rel functions | 3/3 matched at the object; 1 rejected at link | $0.013 |

The rejection was a design flaw, fixed the same hour: `splice` merged the new
block's `#include "rel/main_rel/rep_memcard.h"` into the shared prologue, which
made four older blocks of that file (private typedefs) stop compiling. The
prologue is now frozen at splice time; the block was resubmitted from its saved
work copy and verified.

## What the file structure recovers, and what it cannot

Recovered from the ROM: which functions belong to which source file and in what
order (function emission order is source order), each file's own data block,
assert line numbers as checkpoints. Not recoverable: fully inlined statics,
macros, comments, which globals were file-scope statics. dtk also split some
records across neighbouring bss symbols; those show as several globals until
the `fzgx oversize` merge lands.
