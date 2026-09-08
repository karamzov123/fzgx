#include "types.h"

extern struct fn_80026E84_lbl_80176160 lbl_80176160;

struct fn_80026E84_lbl_80176160_0_E96 {
    u8 pad_0[0x4];
    u32 unk_4;
    u8 pad_8[0x58];
};
struct fn_80026E84_Arg0 {
    u8 pad_0[0x18];
    u32 unk_18;
};
struct fn_80026E84_lbl_80176160 {
    struct fn_80026E84_lbl_80176160_0_E96 unk_0[1];
};

u32 fn_80026E84(struct fn_80026E84_Arg0 *arg0) {
    u32 v0;
    v0 = (u32)((u8 *)&lbl_80176160 + (arg0->unk_18 * 96));
    *(u32 *)((u8 *)v0 + 4) = ((lbl_80176160.unk_0[arg0->unk_18].unk_4 | 0x40000000) | 2);
    return v0;
}
