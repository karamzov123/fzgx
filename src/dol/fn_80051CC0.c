#include "types.h"

extern u32 fn_8004EBB4(void);
extern u32 fn_8004EBD4(u32);
extern u32 fn_80053EA0(u32);
extern u32 fn_80053EA8(u32);
extern u32 fn_80053EB0(u32);
extern u32 fn_800541EC(u32);
extern u32 fn_80054224(u32);
extern u32 fn_800542BC(u32);

u32 fn_80051CC0(u32 arg0, u32 arg1) {
    u32 v5;
    s32 v4;
    u32 v3;
    u32 v2;
    u32 v1;
    u32 v0;
    u32 t1, t2, t3, t4, t5, t6, t7, t8, t9;
    fn_8004EBD4(arg0);
    v0 = *(u32 *)((u8 *)arg0 + 176);
    t1 = fn_800542BC(v0);
    v1 = t1;
    if ((s32)t1 == 0) {
    v1 = v0;
    *(u32 *)((u8 *)arg0 + 136) = 0;
    t2 = fn_800541EC(v1);
    v1 = t2;
    }
    v2 = v1;
    v3 = arg1;
    if ((s32)*(u32 *)((u8 *)arg0 + 4) == 1) {
    v2 = v0;
    *(u32 *)((u8 *)arg0 + 136) = 0;
    t3 = fn_80054224(v2);
    v2 = t3;
    *(u32 *)((u8 *)arg0 + 4) = 2;
    } else {
    if ((s32)*(u32 *)((u8 *)arg0 + 4) == 2) {
    v4 = 0;
    while (v4 < (s32)*(u32 *)((u8 *)arg0 + 184)) {
    v2 = v0;
    t4 = fn_80053EB0(v2);
    v2 = t4;
    v4++;
    }
    v2 = v0;
    t5 = fn_80053EA0(v2);
    v2 = t5;
    v5 = (v2 - *(u32 *)((u8 *)arg0 + 136));
    v2 = v0;
    *(u32 *)((u8 *)arg0 + 144) = v5;
    v3 = *(u32 *)((u8 *)arg0 + 136);
    *(u32 *)((u8 *)arg0 + 136) = (v3 + *(u32 *)((u8 *)arg0 + 144));
    t6 = fn_80053EA8(v2);
    v2 = t6;
    *(u32 *)((u8 *)arg0 + 148) = v2;
    v2 = v0;
    t7 = fn_800542BC(v2);
    v2 = t7;
    if ((s32)v2 == 3) {
    v2 = v0;
    t8 = fn_800541EC(v2);
    v2 = t8;
    *(u32 *)((u8 *)arg0 + 4) = 0;
    }
    }
    }
    t9 = fn_8004EBB4();
    v2 = t9;
    return v2;
}
