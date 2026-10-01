# axmix_80026EE0_blocked-evidence_dossier

unit: main/game/axmix_80026EE0
next: axmix_mix_voice_state, axmix_device_ctrl_accumulate_mix
attempts: 0/12
canonical_head: af176505cc4f01f1f4aeb05c395e918701f66889
source_sha: fde15ae1ffc2cb62ec717452cfd778a74238c91b91eb230aef8241993a5be7a4
context_sha: e07052a385a37a34
compiler: GC/1.2.5 (authoritative pin)
discriminate: 100.0 across GC/1.2.5, GC/1.2.5n, GC/1.3, GC/1.3.2, GC/1.3.2r
verdict: blocked-evidence / inline_asm_required
blocker:
  - no ready queue batch for worker `integ`; no gate/commit path this turn.
  - `axmix_mix_voice_state` is blocked-evidence (no natural-C ref, no runtime trace).
  - `axmix_device_ctrl_accumulate_mix` is inline_asm_required / ineligible_pure_c.
  - 100% discriminate score is false-positive conversion evidence because live destination is still asm and requires an `asm{}` block, not plain natural C.
next required: wait for fresh ready-batch/lease event or select a different unit/symbol.
