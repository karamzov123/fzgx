#include "types.h"


struct fn_80058434_Arg0 {
    u8 pad_0[0x38];
    u32 unk_38;
    u32 unk_3C;
};

void fn_80058434(struct fn_80058434_Arg0 *arg0, u32 arg1, u32 arg2) {
    arg0->unk_38 = arg1;
    arg0->unk_3C = arg2;
}
