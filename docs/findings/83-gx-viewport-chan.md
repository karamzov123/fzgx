# Finding 83 — GX channel control and viewport APIs

Batched symbolic-only GX wave on `contrib-20260825-gx-viewport-chan`.

Applied two previously missed direct GX API names:

- `fn_800375F0` -> `GXSetChanCtrl`
  - Emits the GX channel-control FIFO command and packs the channel,
    enable, light-mask, material, and attenuation fields.
  - Direct callers include GX initialization/render setup paths.
- `fn_80038DE8` -> `GXSetViewport`
  - Performs the viewport x/y/width/height/depth transform and writes the GX
    viewport state/FIFO values.
  - Matches the GX SDK viewport API body and existing render call sites.

The cached scout evidence identifies both as direct GX SDK matches; neither
symbol was occupied before this wave.

Verification: one fresh configure+ninja wave completed successfully; resulting
`build/GFZE01/main.dol` SHA-1 is
`421c88106697d3275a3fc26fb7a01bf6d816b271`.
