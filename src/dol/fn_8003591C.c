#include "types.h"

typedef struct GXContext {
    u8 pad_0[0x204];
    u32 field_204;
    u8 pad_208[0x2ec];
    u32 field_4f4;
} GXContext;

extern GXContext* gx;

// The write-gather pipe is a memory-mapped hardware register.
#define GX_FIFO_U8 (*(volatile u8 *)0xCC008000u) // fzgx-allow: A2 hardware register
#define GX_FIFO_U32 (*(volatile u32 *)0xCC008000u) // fzgx-allow: A2 hardware register

void fn_8003591C(u8 value) {
    GXContext* context = gx;
    context->field_204 = (context->field_204 & 0xffffff8fu) | ((u32)value << 4);
    GX_FIFO_U8 = 0x10;
    GX_FIFO_U32 = 0x1009;
    GX_FIFO_U32 = value;
    context->field_4f4 |= 4;
}
