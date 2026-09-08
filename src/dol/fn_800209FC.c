#include "types.h"

extern u32 lbl_801A6AB8;

struct fn_800209FC_Arg0 {
    u8 pad_0[0x8];
    u32 unk_8;
};

void fn_800209FC(struct fn_800209FC_Arg0 *arg0) {
    arg0->unk_8 = lbl_801A6AB8;
    lbl_801A6AB8 = (u32)arg0;
}
