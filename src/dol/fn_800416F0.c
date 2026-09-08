#include "types.h"


struct fn_800416F0_Arg0 {
    u8 pad_0[0x34];
    u32 unk_34;
};

void fn_800416F0(struct fn_800416F0_Arg0 *arg0, u32 arg1) {
    arg0->unk_34 = arg1;
}
