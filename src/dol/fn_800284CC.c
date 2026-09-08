#include "types.h"


struct fn_800284CC_Arg0 {
    u8 pad_0[0x4];
    u32 unk_4;
    u8 unk_8;
    u8 unk_9;
};

void fn_800284CC(struct fn_800284CC_Arg0 *arg0) {
    arg0->unk_4 = 2;
    arg0->unk_8 = 64;
    arg0->unk_9 = 127;
}
