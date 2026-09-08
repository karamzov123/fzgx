#include "types.h"

struct fn_80051958_lbl_80187110 {
    u32 unk_0;
};

extern struct fn_80051958_lbl_80187110 lbl_80187110[];
extern u32 fn_80053BFC(void);
extern u32 lbl_80187330[];
extern u32 memset(u32, u32, u32);

u32 fn_80051958(u32 arg0, u32 arg1) {
    u32 v0;
    u32 v1;
    u32 t0, t1;
    v0 = arg0;
    v1 = arg1;
    if ((s32)lbl_80187110[0].unk_0 == 0) {
    t0 = fn_80053BFC();
    v0 = t0;
    v1 = 0;
    v0 = (u32)&lbl_80187330;
    t1 = memset((u32)v0, v1, 64);
    v0 = t1;
    }
    v1 = (u32)&lbl_80187110;
    v0 = *(u32 *)((u8 *)v1 + 0);
    *(u32 *)((u8 *)v1 + 0) = (v0 + 1);
    return v0;
}
