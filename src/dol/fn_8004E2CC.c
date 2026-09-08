#include "types.h"


struct fn_8004E2CC_Arg0 {
    u8 pad_0[0x34];
    u16 unk_34;
    u16 unk_36;
    u16 unk_38;
};

void fn_8004E2CC(struct fn_8004E2CC_Arg0 *arg0, u32 arg1, u32 arg2, u32 arg3) {
    arg0->unk_34 = arg1;
    arg0->unk_36 = arg2;
    arg0->unk_38 = arg3;
}
