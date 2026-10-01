# Finding 73 — sasnd pan-mix helper

Symbolic-only rename on `contrib-20260825-sasnd-panmix`.

- `fn_80063EF4` -> `SndCalcPanMix`
- The function is a compact fixed-point blend helper: it reads the bank
  pan/mix mode flag, shifts the signed inputs into the sasnd fixed-point
  representation, and combines the two channel values.
- It is called by `SndApplyChannelPan` and the channel voice update path. The
  role and call graph were re-verified in the supervisor’s read-only sasnd
  naming summary; no logic changed.

Verification: one fresh configure+ninja wave completed successfully; resulting
`build/GFZE01/main.dol` SHA-1 is
`421c88106697d3275a3fc26fb7a01bf6d816b271`.
