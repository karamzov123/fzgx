#include "types.h"

extern u32 fn_8008023C(u32, u32, u32);
extern u32 lbl_80090928[];

u32 fn_80045FEC(u32 arg0) {
    u32 t0;
    t0 = fn_8008023C(arg0, (u32)&lbl_80090928, 4);
    return ((u32)__cntlzw(t0) >> 5);
}
