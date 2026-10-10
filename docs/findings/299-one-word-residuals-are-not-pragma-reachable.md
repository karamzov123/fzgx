# 299 -- the one-word residuals are not reachable through pragmas; `scheduling` is the only real lever and it is too strong

# Finding 297 recorded four functions whose best body differs from retail by one
# instruction, and classified three of them as register-allocation choices. This is
# the systematic follow-up: a sweep of every MWCC pragma the project already uses,
# against the schedule-class residuals and the register-allocation class.
#
# Two corrections to the tooling assumed by earlier sessions, both found here:
#
# **`#pragma opt_scheduling` does not exist.** It is silently ignored -- no warning, no
# effect, identical output. The real pragma is `#pragma scheduling`, used 24 times in
# `src/`. Anything that probed `opt_scheduling` was measuring nothing.
#
# **Unknown pragmas fail silently.** `opt_scheduling off` compiled with rc=0 and empty
# stdout/stderr while changing zero instructions. `docs/REGISTER_REPAIR.md` records the
# same class of defect for `fn_1_4DE04`, and it is exactly the failure mode an A/B tool
# exists to catch: flagcell reports "the two cells compiled identically", which reads as
# "no effect" rather than "your setting was never parsed".
#
# ## What was measured
#
# Pragmas present in `src/`, applied one at a time to the best saved body:
#
#     peiphole off, opt_common_subs off, opt_dead_assignments off,
#     opt_strength_reduction off, opt_loop_invariants off, opt_lifetimes off,
#     scheduling off, scheduling on
#
# Results, differing rows against retail:
#
# | function | as-is | best pragma | pragma rows |
# | --- | ---: | --- | ---: |
# | fn_1_4068C (schedule) | 2 | none | 2 |
# | fn_1_466B0 (schedule) | 2 | none | 2 |
#
# `#pragma scheduling off` is the only setting that moves either, and it moves it the
# wrong way: 14 and 25 differing rows, scores 71.4% and 80.3% against 99.6% and 99.7%.
# Disabling the scheduler does not reorder two instructions, it discards the whole
# schedule. `scheduling on` is a no-op, as expected, since it is the default.
#
# Every other pragma is either a no-op or strictly harmful (`opt_strength_reduction off`
# takes fn_1_466B0 from 2 rows to 44).
#
# ## Conclusion
#
# A two-instruction swap in the emitted schedule is not reachable from the source or from
# any pragma the toolchain accepts. Combined with the register-allocation residuals in
# finding 297, this closes the compiler-option axis for the one-word tier: no pragma, no
# -O level, and no flag combination tested changes them, while the surrounding
# instructions are byte-identical.
#
# What is left for these functions is either an accepted hand-edit in the .s (the route
# the asm units already use, see commit e92c4bd4) or a register-allocation change in the
# MWCC build, neither of which is a source-shape problem. Recorded so the pragma axis is
# not swept a third time.
