# 298 -- 71% of fleet attempts target symbols that cannot be linked from C

The fleet's `choose()` filters on size bands, virgin/mid pools, family lockout, and
`link_failed()`. It does not filter on **split coverage**, and split coverage is a hard
prerequisite for a body to land at all (finding 279).

Measured over every `fleet-v2*` attempt in the ledger:

    fleet-targeted symbols            1910
      uncoverable (no split range)    1186   attempts 2941   matched    0
      landable  (has split range)      724   attempts 1690   matched  410

**2941 attempts on uncoverable symbols produced zero matches. The same 1690 attempts on
landable symbols produced 410.** No landable symbol with any attempts has failed to match at
least once. The split is not a formality -- it is the difference between work that can land and
work that cannot.

(Counts exclude `fn_12_21C00` and `fn_17_7728`, which were carved to landable after this
measurement was taken -- see commit aa2df2f3.)

Top-30 fleet-targeted symbols, 274 of their attempts (71%) are on uncoverable symbols:

    fn_1_135D7C   attempts=28  best=99.61    UNCOVERED
    fn_1_2D038    attempts=28  best=99.68    UNCOVERED
    fn_1_53E40    attempts=28  best=99.61    UNCOVERED
    fn_1_4068C    attempts=27  best=99.64    UNCOVERED
    fn_12_21C00   attempts=23  best=99.21    UNCOVERED   (now carved)
    fn_1_4270C    attempts=22  best=99.59    UNCOVERED
    fn_1_1384C    attempts=21  UNCOVERED
    fn_17_7728    attempts=20  best=99.97    UNCOVERED   (now carved)
    fn_800288C4   attempts=22  LANDABLE
    movie:_prolog attempts=15  LANDABLE

## Why the existing guard does not catch this

`link_failed()` retires symbols whose last attempt matched the object and was rejected by the
link. That is the right idea and it works -- but a symbol with **no split range never reaches a
link test**, so it never records `link-mismatch` and is never retired. The filter is currently
empty (0 symbols) and structurally cannot see this class.

`fzgx carve` is the legitimate fix and it needs no lease, no model, and no fleet: it adds the
`.text` range and registers the unit as `nonmatching`, so the retail object keeps linking until
a body is accepted.

## The lever

Add a coverage filter to `choose()`, symmetric to `link_failed()`. Two policies, both cheap:

  **Prefer landable** (recommended): rank covered symbols ahead of uncovered ones within each
  existing pool, so the fleet converts what it has instead of re-deriving unlandable bodies.

  **Or carve-on-touch**: when a symbol reaches its attempt cap while still uncovered, run
  `fzgx carve` on it rather than spending another session. `carve` is deterministic, offline,
  and idempotent; it converts a dead slot into a landable one for the next session.

Either way the win is the 2941 attempts' worth of work that currently lands nothing. Measured
conversion on landable symbols is 410/1690 = **24.3%**; on uncovered it is **0%**.

## Corroborating bottleneck: the free-tier lane produces nothing

Same query, by model:

    model                            total  matched  zero-check
    stealth/space-bunny-alpha        1691      143         198
    gpt-6.1-sol                       885      119         208
    opencode/space-bunny-free         449       76           2
    claude-opus-5-5                   156       41          37
    gemini-3.8-flash-high             145       25           1
    stepfun/step-5-preview:free        85        0          60
    claude-sonnet-5-5                  63        6           7

`stepfun/step-5-preview:free` is **all 90 harness timeouts in the last 300
attempts** (median 616s, max 1904s) and has matched **0 of 85**. 79 of its attempts ran zero
checks. It is configured for four surge lanes in `fleet_multi.py` with `effort: low` and a
240s compiler-silence bound. The lane is not slow, it is not converting -- four concurrency
slots are occupied by a model that has never produced a match.

`gpt-6-luna` (168 attempts, 0 matched, 25 zero-check) is a second candidate for retirement.

### Resolution: implemented, and deliberately not overridden

The coverage filter above is implemented in `fleet_multi.py` (commit 875ff7b8). The lane
question is **not** an implementation change and was left alone.

`~/.cache/fzgx-agents/model-policy.json` pins all four surge lanes to
`stepfun/step-5-preview:free` with an explicit owner-directed reason string, and
`control-v3.json` enables exactly one of the four. Repinning a lane is a provider/cost
decision, not a code defect, so the measurement is recorded here for the owner to act on
rather than silently changed underneath the policy file.

The timeout signature is worth stating precisely, because it is the actionable part: 52 of the
85 attempts ended in `harness timeout (rc=130)` with **zero checks**, meaning the session never
reached a compiler cycle at all. With `effort: low` and a 240s compiler-silence bound already
in `fleet_provider`, the remaining suspect is upstream thinking time before the first tool
receipt, not the compiler. If the lane is kept, the lever to try first is a
no-tool-receipt grace bound shorter than `SESSION_TIMEOUT` (1800s) so a dead session releases
its slot in ~300s instead of ~1900s.
