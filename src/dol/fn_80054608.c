#include "types.h"

struct fn_80054608_lbl_801873D8 {
    u32 unk_0;
};
struct fn_80054608_lbl_80091388 {
    u32 unk_0;
};

extern struct fn_80054608_lbl_80091388 lbl_80091388[];
extern struct fn_80054608_lbl_801873D8 lbl_801873D8[];
extern u32 fn_80051958(u32);
extern u32 lbl_801873EC[];
extern u32 memset(u32, u32, u32);

u32 fn_80054608(u32 arg0, u32 arg1) {
    u32 v0;
    u32 v1;
    u32 t0, t1;
    v0 = lbl_80091388[0].unk_0;
    v1 = arg1;
    if ((s32)lbl_801873D8[0].unk_0 == 0) {
    t0 = fn_80051958(v0);
    v0 = t0;
    v1 = 0;
    v0 = (u32)&lbl_801873EC;
    t1 = memset((u32)v0, v1, 64);
    v0 = t1;
    }
    v1 = (u32)&lbl_801873D8;
    v0 = *(u32 *)((u8 *)v1 + 0);
    *(u32 *)((u8 *)v1 + 0) = (v0 + 1);
    return v0;
}
