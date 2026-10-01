# Finding 71 — CARD API wave

Batched symbolic-only CARD rename wave on `contrib-20260825-card-api`.

Applied 13 HIGH-confidence names from the precomputed CARD queue:

- `fn_8002EA54` -> `__CARDIsPublic`
- `fn_8002F0F8` -> `CARDCreate`
- `fn_8002F428` -> `CARDReadAsync`
- `fn_8002F570` -> `CARDRead`
- `fn_8002F7D8` -> `CARDWriteAsync`
- `fn_8002F8EC` -> `CARDWrite`
- `fn_8002F934` -> `DeleteCallback`
- `fn_8002F9D8` -> `CARDFastDeleteAsync`
- `fn_8002FB04` -> `CARDDeleteAsync`
- `fn_8002FE54` -> `CARDGetStatus`
- `fn_8002FF80` -> `CARDSetStatusAsync`
- `fn_8003013C` -> `CARDRenameAsync`
- `fn_80030338` -> `CARDRename`

Evidence is recorded in `findings/30-card-names.md`: direct EXI command,
control-block, callback, and CARD API body correspondence. Names were filtered
against the current symbol table; ambiguous `::` callback labels and stale or
occupied candidates were excluded.

Verification: one fresh configure+ninja wave completed successfully; resulting
`build/GFZE01/main.dol` SHA-1 is
`421c88106697d3275a3fc26fb7a01bf6d816b271`.
