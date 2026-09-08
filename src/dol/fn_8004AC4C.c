#include "types.h"


struct fn_8004AC4C_Arg0 {
    u8 pad_0[0x34];
    u32 unk_34;
    u32 unk_38;
};

void fn_8004AC4C(struct fn_8004AC4C_Arg0 *arg0, u32 arg1, u32 arg2) {
    arg0->unk_34 = arg1;
    arg0->unk_38 = arg2;
}
