# Finding 75 — UART and light-controller queue wave

Batched symbolic-only queue wave on `contrib-20260825-uart-lightctrl`.

Applied eight names from the supervisor’s precomputed queue:

- `fn_800157AC` -> `InitializeUART`
- `fn_8007264C` -> `LightCtrl_SetCachedFog`
- `fn_800726C0` -> `LightCtrl_SetCachedFogArray`
- `fn_80072778` -> `LightCtrl_InitChannels4`
- `fn_80072808` -> `LightCtrl_InitChannels2`
- `fn_80072864` -> `LightCtrl_SetCachedCullMode`
- `fn_800728A8` -> `LightCtrl_SetCachedColor_1C`
- `fn_80072AB0` -> `LightCtrl_SetCachedPair_6C`

`InitializeUART` is an exact body match to the Dolphin UART initializer:
console-type guard, non-development fallback, and UART state/magic setup.
The light-controller names are directly supported by the cached shadow-record
and GX write-through evidence: fog setters, channel initialization variants,
cull-mode cache, packed color cache, and the 8-byte pair cache. The ambiguous
`fn_800729B0`/`fn_80072A50` twin was deliberately excluded because one shared
name would obscure the force/non-force distinction.

Verification: one fresh configure+ninja wave completed successfully; resulting
`build/GFZE01/main.dol` SHA-1 is
`421c88106697d3275a3fc26fb7a01bf6d816b271`.
