#include "types.h"


struct fn_80057B34_Arg0 {
    u8 pad_0[0xC];
    u32 unk_C;
    u32 unk_10;
    u8 pad_14[0x4];
    u32 unk_18;
};

void fn_80057B34(struct fn_80057B34_Arg0 *arg0) {
    arg0->unk_C = arg0->unk_18;
    arg0->unk_10 = 0;
}
