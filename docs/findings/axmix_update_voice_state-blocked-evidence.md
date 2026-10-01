# axmix_update_voice_state-blocked-evidence

unit: main/game/axmix_80026EE0
symbol: axmix_update_voice_state
size: 728 B (0x2d8)
section: .text
attempts: 0/12
best: 0.000
canonical_head: af176505cc4f01f1f4aeb05c395e918701f66889
source_sha: fde15ae1ffc2cb62ec717452cfd778a74238c91b91eb230aef8241993a5be7a4
context_sha: e07052a385a37a34
compiler: GC/1.2.5 (authoritative pin)
verdict: blocked-evidence
reasons:
  - no natural-C reference and no verified runtime trace
  - live destination is asm; 100% discriminate is false-positive conversion evidence
neighbors:
  - 0.611 axmix_device_ctrl_accumulate_mix (inline_asm_required)
  - 0.033 fn_80077488 (main/game/model_80074D88)
refs:
  - axmix_mix_voice_state, axmix_param_ramp_step, axmix_param_ramp_step_dup, axmix_set_voice_param_08, axmix_set_voice_volume, axmix_set_voice_volume_clamped, fn_80023228, fn_80026E2C, fn_80026EAC, fn_80028A78
traps: T1/T3/T14 sda21 relocs, T2 ADDR16 data-global, T9 fixed-coefficient ABI
next: wait for fresh reference body or verified runtime trace; do not generate pure-C candidate yet.
