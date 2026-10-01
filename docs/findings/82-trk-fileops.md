# Finding 82 — MetroTRK file-operation clients

Batched symbolic-only MetroTRK wave on `contrib-20260825-trk-fileops`.

Applied three previously missed client-operation names:

- `fn_8008A80C` -> `TRK_PositionFile` (request command `0xD4`)
- `fn_8008A91C` -> `TRK_CloseFile` (request command `0xD3`)
- `fn_8008AA04` -> `TRK_OpenFile` (request command `0xD2`)

The bodies construct the corresponding MetroTRK file-operation requests and
send them through the request path. The command IDs and request layouts match
the MetroTRK client-operation study in `findings/31-trk-names.md`. Duplicate
`TRKReleaseMutex` proposals at separate stub/implementation addresses were
excluded rather than assigning one name to multiple semantics.

Verification: one fresh configure+ninja wave completed successfully; resulting
`build/GFZE01/main.dol` SHA-1 is
`421c88106697d3275a3fc26fb7a01bf6d816b271`.
