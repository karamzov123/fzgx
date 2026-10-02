# 272 — link-mismatched matches were quarantined; two object-perfect bodies closed

Date: 2026-10-02. Tooling fix: 433cc19f. Matches: 16f2892e, b0ef3c3a, 24909e08.

## The bottleneck

A match can pass the per-object oracle at 100% and still be rejected by the 16-target
link. `verify` preserved the body to `.fzgx/attempts/<key>.linkfail.*.c` and then
uncarved the unit. Three things compounded, and none of them was visible as a failure:

1. **The diagnostic could not reach its own population.** `why_link()` documented "a
   rejected match keeps its split range", but `uncarve(..., split=False)` removes it, so
   the function returned `no unit for this function` for exactly the cases it exists for.

2. **The body reached nobody.** Every consumer — `stuck.best_bodies`,
   `context.build_context`, `unsearched_near_misses` — selects saved bodies through
   `attempts.best_body_path`, which verify left `NULL`, with no metadata sidecar
   (0 written against 2112 for ordinary saves). Five functions sat at
   `best_percent 100.0` and unmatched with a working reconstruction nobody could see.

3. **The waste was preferentially funded.** `choose()` sorts by `-best`, so these were
   the *first* targets picked. The object oracle cannot see the link, so a matcher has no
   tool that shows which bytes moved: each session re-derived the same body from a stub
   and failed identically. 130 symbols hit this class; 452 attempts were spent on it.

## Fix

`verify` now writes the sidecar and `best_body_path` for a rejected body; `why_link`
re-carves from the preserved body, diagnoses, and uncarves again; both fleet selectors
retire link-mismatched symbols from *model* dispatch. Verified: `why-link fn_1_207DC`
returns a real diagnosis (naming `fn_1_14CB4.o`), the tree restores byte-for-byte with no
stray stub or split range, and `ninja build/GFZE01/ok` passes.

With the bodies reachable, `fn_1_207DC` and `fn_1_21950` matched and link-verified
directly from their preserved reconstructions — `fn_1_207DC` at **zero checks**, from
`fixup preflight: inline helper at 622`.

## fn_1_58248: 100% at the object, blocked only by lint

A third function was in the same visible state but a different class, and the first
explanation offered (masked-word shortlist) was **wrong**. Measured directly:

    base.matched = True, base.percent = 100.0, unit_fully_matches -> None
    try_fix  ->  matched=False, label='lifetime pointer entry->unk_04 at every site'

So the body really was object-perfect; `release()` submitted only when
`fx["matched"]` was true, and `try_fix` had downgraded it. The gate was `source_lint`,
not structure:

    S2 volatile without a justification comment   (x2)
    A1 hardcoded address 0xCC008000; use a symbol
    A2 literal cast to pointer; declare an extern symbol

`0xCC008000` is `__GXFifo` (`config/GFZE01/ldscript.tpl`), a hardware register block.
**Rewriting it to `extern u8 __GXFifo[]` was wrong**: the match fell to 91.07%, because
the symbol form changes codegen and the folded literal is what retail emits. The
canonical fix is the documented opt-out on the existing spelling:

    /* fzgx-allow: A1,A2 ... */   on the #define
    /* fzgx-allow: S2 ... */      on each volatile member

Lint clean, `percent 100.0` retained, `try_fix -> matched=True label=input`, submitted and
link-verified (24909e08).

Two lessons worth carrying:

- **Measure before theorising.** The masked-word theory was plausible and wrong; only
  `oracle.check` + `try_fix` on the real body identified lint as the sole blocker.
- **Lint can gate an object-perfect body.** When a saved body scores 100% but is not
  accepted, read `.fzgx/fixup/sessions/<key>/report.json` for `object_matched` plus
  `source_lint` / `extra_helpers` / `extra_data` / `link_rejected` before rewriting C.
  Prefer a justification comment over a rewrite: comments are free, source changes are
  not, and an unnecessary rewrite costs the match.

Result: unmatched functions at >=100% went 5 -> 2 (`fn_1_41CB8`, `fn_1_D7C44` remain,
both still awaiting `why-link`).