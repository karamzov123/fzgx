#include "types.h"

struct fn_80026EAC_Arg0 {
    u8 pad_0[0x18];
    u32 unk_18;
};

extern u32 lbl_80176160[];
extern u32 lbl_801A6B88;

u32 fn_80026EAC(struct fn_80026EAC_Arg0 *arg0, u32 arg1) {
    u32 v0;
    if (lbl_801A6B88 == 3) { return (u32)arg0; }
    v0 = (u32)((u8 *)&lbl_80176160 + (arg0->unk_18 * 96));
    *(u32 *)((u8 *)v0 + 16) = arg1;
    *(u32 *)((u8 *)v0 + 4) = (*(u32 *)((u8 *)v0 + 4) | 0x40000000);
    return v0;
}
