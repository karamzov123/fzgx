#include "types.h"
#include "dolphin/hw_regs.h"

typedef struct GXData {
    u16 unk_000;
    u16 bp_sent_not;
    u8 pad_004[0x438];
    f32 vp_left;
    f32 vp_top;
    f32 vp_width;
    f32 vp_height;
    f32 vp_near;
    f32 vp_far;
    u8 fog_range;
    f32 fog_side_x;
} GXData;

typedef union PPCWGPipe {
    u8 u8;
    u16 u16;
    u32 u32;
    f32 f32;
} PPCWGPipe;

extern GXData *const gx;
volatile PPCWGPipe GXWGFifo : GX_FIFO_BASE;  /* fzgx-allow: A2,S2 GX write-gather FIFO (hw_regs.h) */

extern void fn_80038878(f32 near_z, f32 side_x);

void fn_80038DE8(f32 left, f32 top, f32 width, f32 height, f32 near_z, f32 far_z, u32 field) {
    f32 sx;
    f32 sy;
    f32 sz;
    f32 ox;
    f32 oy;
    f32 oz;
    f32 zmin;
    f32 zmax;
    f32 half_height;

    if (field == 0) {
        top -= 0.5f;
    }
    sx = width / 2.0f;
    sy = -height / 2.0f;
    ox = 342.0f + (left + (width / 2.0f));
    half_height = height / 2.0f;
    oy = 342.0f + (top + half_height);
    zmin = 1.6777215e7f * near_z;
    zmax = 1.6777215e7f * far_z;
    sz = zmax - zmin;
    oz = zmax;
    gx->vp_left = left;
    gx->vp_top = top;
    gx->vp_width = width;
    gx->vp_height = height;
    gx->vp_near = near_z;
    gx->vp_far = far_z;
    if (gx->fog_range != 0) {
        fn_80038878(near_z, gx->fog_side_x);
    }
    GXWGFifo.u8 = 0x10;
    GXWGFifo.u32 = 0x5101a;
    GXWGFifo.f32 = sx;
    GXWGFifo.f32 = sy;
    GXWGFifo.f32 = sz;
    GXWGFifo.f32 = ox;
    GXWGFifo.f32 = oy;
    GXWGFifo.f32 = oz;
    gx->bp_sent_not = 1;
}
