#include "types.h"

extern u32 OSRegisterVersion(u32);
extern u32 fn_80020874(u32);
extern u32 fn_80020D58(u32);
extern u32 fn_800218EC(u32);
extern u32 fn_80021C7C(u32);
extern u32 fn_80022420(u32);
extern u32 fn_80022EA4(u32);
extern u32 lbl_801A64E8;

u32 fn_80020778(void) {
    u32 t0, t1, t2, t3, t4, t5;
    t0 = OSRegisterVersion(lbl_801A64E8);
    t1 = fn_80020874(t0);
    t2 = fn_80022EA4(t1);
    t3 = fn_80022420(t2);
    t4 = fn_80020D58(t3);
    t5 = fn_800218EC(t4);
    return fn_80021C7C(t5);
}
