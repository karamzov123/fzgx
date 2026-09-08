#include "types.h"


struct fn_80057B48_Arg0 {
    u8 pad_0[0x1C];
    u32 unk_1C;
    u32 unk_20;
};

void fn_80057B48(struct fn_80057B48_Arg0 *arg0, u32 arg1, u32 arg2) {
    arg0->unk_1C = arg1;
    arg0->unk_20 = arg2;
}
