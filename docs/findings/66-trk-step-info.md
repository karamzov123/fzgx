# Finding 66 — MetroTRK step and stop/exception info helpers

Symbolic-only rename wave on `contrib-20260825-trk-step-info`.

From the existing MetroTRK audit in `findings/31-trk-names.md`:

- `fn_8008B784` -> `TRKTargetDoStep`
  - Updates `gTRKStepStatus`, enables trace, and clears the stopped state.
- `fn_8008B830` -> `TRKTargetAddExceptionInfo`
  - Builds exception metadata and appends the instruction read through the
    target-memory helper.
- `fn_8008B8B4` -> `TRKTargetAddStopInfo`
  - Builds stop metadata from the saved CPU PC/instruction state.
- `fn_8008C6E4` -> `TRKTargetReadInstruction`
  - Wraps `TRKTargetAccessMemory` for a four-byte instruction read and is
    called by the stop/exception info helpers.

These names match the Melee MetroTRK target implementation and the existing
call/data-flow evidence; no logic was changed.

Verification: fresh configure+ninja completed with exit 0; resulting
`build/GFZE01/main.dol` SHA-1 is
`421c88106697d3275a3fc26fb7a01bf6d816b271`.
