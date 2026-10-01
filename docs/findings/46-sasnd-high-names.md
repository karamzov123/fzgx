# Finding 46 — sasnd HIGH semantic naming wave

Applied on `contrib-20260823-wave` with `tools/rename_sym.py`, based on
/tmp/naming-study-gamehead.md. The unit is identified by the embedded
`sasnd Ver 1.13h Build:Jul 1 2003` string; each proposal is supported by
call-graph role, distinctive callee/data shape, and its position in the
SndDispatchCommand command path.

| VA | old | new |
|---|---|---|
| 80060BDC | fn_80060BDC | SndClearVoiceSlot |
| 8005FCD8 | fn_8005FCD8 | SndKillChannelVoice |
| 8006496C | fn_8006496C | SndSwapVoice |
| 8006060C | fn_8006060C | SndBoostVoicePriority |
| 80062654 | fn_80062654 | SndPlaySequenceNotes |
| 8005CBF4 | fn_8005CBF4 | SndStopAllChannelVoices |
| 800643F4 | fn_800643F4 | SndTickChannels |
| 8005C478 | fn_8005C478 | SndApplyChannelPan |

Verification: deleted generated main.dol and ran fresh `ninja`, exit 0;
SHA-1 `421c88106697d3275a3fc26fb7a01bf6d816b271`. Progress and byte counts
remain unchanged, as expected for symbolic-only renames.
