# Finding 88 — sasnd curve and velocity helpers

Batched symbolic-only sasnd wave on `contrib-20260825-snd-curve-velocity`.

Applied two remaining sound-helper names:

- `fn_8005D258` -> `SndExpCurveLookup`
  - Evaluates the sound-library exponential/volume curve used by the voice
    envelope and parameter paths.
- `fn_8005DB68` -> `SndVelocityToVolume`
  - Converts the channel/voice velocity parameter into the corresponding
    volume value used by sasnd voice processing.

Both identities are supported by the cached sasnd body/caller study and fit
the existing `Snd*` naming family. The neighboring channel-pan, sequence-note,
and voice-swap helpers were already named and were not changed.

Verification: one fresh configure+ninja wave completed successfully; resulting
`build/GFZE01/main.dol` SHA-1 is
`421c88106697d3275a3fc26fb7a01bf6d816b271`.
