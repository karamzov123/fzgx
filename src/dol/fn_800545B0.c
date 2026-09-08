#include "types.h"

struct fn_800545B0_lbl_801873D8 {
    u32 unk_0;
};

extern struct fn_800545B0_lbl_801873D8 lbl_801873D8[];
extern u32 fn_8005190C(void);
extern u32 lbl_801873EC[];
extern u32 memset(u32, u32, u32);

u32 fn_800545B0(void) {
    u32 v0;
    u32 v1;
    u32 t0, t1;
    if ((s32)lbl_801873D8[0].unk_0 == 1) {
    v1 = 0;
    v0 = (u32)&lbl_801873EC;
    t0 = memset((u32)v0, v1, 64);
    v0 = t0;
    t1 = fn_8005190C();
    v0 = t1;
    }
    v1 = (u32)&lbl_801873D8;
    v0 = *(u32 *)((u8 *)v1 + 0);
    *(u32 *)((u8 *)v1 + 0) = (v0 - 1);
    return v0;
}
