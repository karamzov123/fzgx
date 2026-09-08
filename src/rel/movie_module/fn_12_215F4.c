#include "types.h"

extern u32 lbl_12_rodata_A78;
extern u32 strncmp(u32, void *, u32);

struct fn_12_215F4_Arg2 {
    u32 unk_0;
};

u32 fn_12_215F4(u32 arg0, u32 arg1, struct fn_12_215F4_Arg2 *arg2) {
    u32 v0;
    u32 t0;
    arg2->unk_0 = 288;
    v0 = arg0;
    if ((s32)arg1 < 288) {
    v0 = 0;
    } else {
    if (*(u8 *)((u8 *)v0 + 0) != 128) {
    v0 = 0;
    } else {
    if (*(u8 *)((u8 *)v0 + 1) != 0) {
    v0 = 0;
    } else {
    v0 += 282;
    t0 = strncmp(v0, &lbl_12_rodata_A78, 6);
    v0 = t0;
    v0 = ((u32)__cntlzw(v0) >> 5);
    }
    }
    }
    return v0;
}
