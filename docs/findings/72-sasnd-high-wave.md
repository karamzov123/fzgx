# Finding 72 — sasnd high-confidence channel wave

Batched symbolic-only rename wave on `contrib-20260825-sasnd-high`.

Applied the high-confidence sasnd queue from the supervisor’s read-only
summary for `gamehead_8005C120.c` (`sasnd Ver 1.13h`):

- `fn_800647B8` -> `SndRefreshChannelVoices`
- `fn_80063F38` -> `SndSendParamToChannelVoices`
- `fn_80064D4C` -> `SndReleaseProcsForVoices`
- `fn_800657F4` -> `SndStartChannelSequence`
- `fn_800672E4` -> `SndGetSequenceStatus`
- `fn_800631EC` -> `SndInitProcTable`
- `fn_80065D70` -> `SndFreeChannel`
- `fn_80065890` -> `SndSetCallback2`
- `fn_8006589C` -> `SndSetCallback1`
- `fn_800658A8` -> `SndSetCallback0`

Evidence: sasnd manager/channel structure, callback-slot stores, proc-table
initialization, channel sequence state, voice fan-out/release call graphs,
and direct role verification in the read-only scout summary. Existing sasnd
names from findings/46 were treated as occupied anchors. No logic changed.

Verification: one fresh configure+ninja wave completed successfully; resulting
`build/GFZE01/main.dol` SHA-1 is
`421c88106697d3275a3fc26fb7a01bf6d816b271`.
