#include "types.h"

extern u32 OSGetArenaHi(void);
extern u32 OSGetArenaLo(void);
extern u32 OSSetArenaHi(u32);
extern u32 OSSetArenaLo(u32);
extern u8 lbl_801A6620;

u32 fn_80070CF8(u32 arg0) {
    u32 v0;
    s32 v2;
    u32 v1;
    u32 v3;
    u32 t0, t1, t2, t3;
    v0 = (arg0 + 31);
    v1 = (v0 & ~0x1F);
    v3 = v0;
    if (lbl_801A6620 != 0) {
    t0 = OSGetArenaHi();
    v3 = t0;
    v2 = (v3 - v1);
    v3 = v2;
    t1 = OSSetArenaHi(v3);
    v3 = t1;
    } else {
    t2 = OSGetArenaLo();
    v3 = t2;
    v2 = v3;
    v3 = (v2 + v1);
    t3 = OSSetArenaLo(v3);
    v3 = t3;
    }
    return v2;
}
