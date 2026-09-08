#include "types.h"


struct fn_800416E4_Arg0 {
    u8 pad_0[0x48];
    u32 unk_48;
    u32 unk_4C;
};

void fn_800416E4(struct fn_800416E4_Arg0 *arg0, u32 arg1, u32 arg2) {
    arg0->unk_48 = arg1;
    arg0->unk_4C = arg2;
}
