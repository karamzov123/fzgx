#include "types.h"

extern u32 fn_800414D0(u32);
extern u32 fn_800474E4(u32);
extern u32 fn_8004EDE4(u32, u32);
extern u32 lbl_80090DF8[];

u32 fn_8004BF0C(u32 arg0, u32 arg1) {
    u32 v0;
    u32 v1;
    u32 t0, t1, t2;
    v0 = arg0;
    v1 = arg1;
    if (arg0 == 0) {
    v0 = (u32)&lbl_80090DF8;
    t0 = fn_800474E4((u32)v0);
    v0 = t0;
    } else {
    *(u16 *)((u8 *)arg0 + 64) = v1;
    v0 = *(u32 *)((u8 *)arg0 + 4);
    t1 = fn_800414D0(v0);
    v0 = t1;
    v1 = (s16)v0;
    v0 = *(u32 *)((u8 *)arg0 + 12);
    v1 = (*(s16 *)((u8 *)arg0 + 64) + v1);
    t2 = fn_8004EDE4(v0, v1);
    v0 = t2;
    }
    return v0;
}
