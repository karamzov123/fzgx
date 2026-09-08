#include "types.h"

struct fn_80026D90_Arg0 {
    u8 pad_0[0x18];
    u32 unk_18;
};

extern u32 lbl_80176160[];

u32 fn_80026D90(struct fn_80026D90_Arg0 *arg0, u32 arg1) {
    u32 v0;
    v0 = (u32)((u8 *)&lbl_80176160 + (arg0->unk_18 * 96));
    *(u32 *)((u8 *)v0 + 8) = arg1;
    *(u32 *)((u8 *)v0 + 4) = (*(u32 *)((u8 *)v0 + 4) | 0x10000000);
    return v0;
}
