#include "types.h"


struct fn_80031D60_Arg0 {
    u8 pad_0[0xC];
    u32 unk_C;
    u32 unk_10;
};

void fn_80031D60(struct fn_80031D60_Arg0 *arg0, u32 arg1, u32 arg2) {
    arg0->unk_C = arg1;
    arg0->unk_10 = arg2;
}
