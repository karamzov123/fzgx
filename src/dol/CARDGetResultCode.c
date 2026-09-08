#include "types.h"

extern u32 lbl_80177960[];

u32 CARDGetResultCode(u32 arg0) {
    u32 v0;
    v0 = arg0;
    if ((s32)arg0 < 0 || (s32)arg0 >= 2) {
    v0 = -128;
    return v0;
    }
    v0 = (u32)((u8 *)&lbl_80177960 + (v0 * 272));
    v0 = *(u32 *)((u8 *)v0 + 4);
    return v0;
}
