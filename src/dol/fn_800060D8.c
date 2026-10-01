#include "types.h"

struct fn_800060D8_Glyph {
    void *image;
    u16 code;
    u16 width;
};

typedef struct {
    u32 dummy[8];
} fn_800060D8_GXTexObj;

typedef union {
    u8 u8;
    u16 u16;
    u32 u32;
    f32 f32;
} GXWGFifoReg;

#define GXWGFifoAddress ((u32)(0xCC00 << 16) + 0x8000)
// Hardware FIFO register is intentionally volatile.
#define GXWGFifo (*(volatile GXWGFifoReg *)GXWGFifoAddress)

extern s16 lbl_801A66EC;
extern struct fn_800060D8_Glyph lbl_8015B940[];

extern s32 fn_80006334(u8 *);
extern void GXInitTexObj(fn_800060D8_GXTexObj *, void *, u16, u16, u32, u32, u32, u8);
extern void GXInvalidateTexAll(void);
extern void fn_80073778(fn_800060D8_GXTexObj *, s32);
extern void fn_8003462C(u32, u32, u32);

static inline struct fn_800060D8_Glyph *fn_800060D8_find(s32 code) {
    struct fn_800060D8_Glyph *glyph = lbl_8015B940;
    s32 i;

    for (i = 0; i < lbl_801A66EC; i++) {
        if (glyph->code == code) {
            return glyph;
        }
        glyph++;
    }
    return 0;
}

void fn_800060D8(u8 *str, f32 x, f32 y, f32 z) {
    fn_800060D8_GXTexObj texObj;
    struct fn_800060D8_Glyph *glyph;
    u32 code;
    f64 bottom;
    f32 depth;
    f32 bot;
    f32 right;

    depth = -z;
    bottom = 24.0 + y;
    while (*str != 0) {
        if (fn_80006334(str)) {
            code = *(u16 *)str;
            str += 2;
        } else {
            code = *str;
            str += 1;
        }
        if (code == 0x20) {
            x += 8.0;
            continue;
        }
        glyph = fn_800060D8_find(code);
        if (glyph == 0) {
            continue;
        }
        GXInitTexObj(&texObj, glyph->image, 0x18, 0x18, 0, 0, 0, 0);
        GXInvalidateTexAll();
        fn_80073778(&texObj, 0);
        right = x + (f32)glyph->width;
        bot = bottom;
        fn_8003462C(0x80, 7, 4);
        GXWGFifo.f32 = x;
        GXWGFifo.f32 = y;
        GXWGFifo.f32 = depth;
        GXWGFifo.f32 = 0.0f;
        GXWGFifo.f32 = 0.0f;
        GXWGFifo.f32 = right;
        GXWGFifo.f32 = y;
        GXWGFifo.f32 = depth;
        GXWGFifo.f32 = glyph->width / 24.0;
        GXWGFifo.f32 = 0.0f;
        GXWGFifo.f32 = right;
        GXWGFifo.f32 = bot;
        GXWGFifo.f32 = depth;
        GXWGFifo.f32 = glyph->width / 24.0;
        GXWGFifo.f32 = 1.0f;
        GXWGFifo.f32 = x;
        GXWGFifo.f32 = bot;
        GXWGFifo.f32 = depth;
        GXWGFifo.f32 = 0.0f;
        GXWGFifo.f32 = 1.0f;
        x += (f32)glyph->width;
    }
}
