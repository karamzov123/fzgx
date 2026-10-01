#include "types.h"
#include "dolphin/hw_regs.h"

typedef struct GXData {
    u8 pad_000[0x1c];
    u32 vat_a[8];
    u32 vat_b[8];
    u32 vat_c[8];
    u8 pad_07c[0x477];
    u8 vat_dirty;
} GXData;

typedef union PPCWGPipe {
    u8 u8;
    u16 u16;
    u32 u32;
    f32 f32;
} PPCWGPipe;

extern GXData *const gx;
volatile PPCWGPipe GXWGFifo : GX_FIFO_BASE;  /* fzgx-allow: A2,S2 GX write-gather FIFO (hw_regs.h) */

void fn_80033650(void) {
    u8 i;

    for (i = 0; i < 8; i++) {
        if (gx->vat_dirty & (1 << (u8)i)) {
            GXWGFifo.u8 = 8;
            GXWGFifo.u8 = i | 0x70;
            GXWGFifo.u32 = gx->vat_a[i];
            GXWGFifo.u8 = 8;
            GXWGFifo.u8 = i | 0x80;
            GXWGFifo.u32 = gx->vat_b[i];
            GXWGFifo.u8 = 8;
            GXWGFifo.u8 = i | 0x90;
            GXWGFifo.u32 = gx->vat_c[i];
        }
    }
    gx->vat_dirty = 0;
}
