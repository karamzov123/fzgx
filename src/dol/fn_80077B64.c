#include "types.h"

struct fn_80077B64_Arg0 {
    u8 pad_0[0x4];
    u32 unk_4;
};

extern u32 OSReport(u32, ...);
extern u32 lbl_8015AD48[];

s32 fn_80077B64(struct fn_80077B64_Arg0 *arg0) {
    u32 v0;
    u32 t0;
    v0 = (u32)arg0;
    if ((arg0->unk_4 & 0x18) != 0) {
    v0 = (v0 + *(u32 *)((u8 *)v0 + 32));
    } else {
    v0 = (u32)&lbl_8015AD48;
    t0 = OSReport((u32)v0);
    v0 = t0;
    v0 = 0;
    }
    return v0;
}
