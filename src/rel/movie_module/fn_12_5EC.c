#include "types.h"

extern u32 lbl_12_rodata_34;
extern u32 strncmp(u32, u32, u32);

u32 fn_12_5EC(u32 arg0, u32 arg1) {
    u32 v0;
    u32 t0;
    v0 = arg0;
    if (arg0 == 0 || (s32)arg1 < 0) {
    v0 = 0;
    } else {
    v0 += 18;
    t0 = strncmp(v0, (u32)&lbl_12_rodata_34, 1);
    v0 = t0;
    v0 = ((u32)__cntlzw(v0) >> 5);
    }
    return v0;
}
