#include "types.h"

extern u32 fn_800433F4(u32, u32);

struct fn_80042198_Arg0 {
    u8 pad_0[0x4];
    u32 unk_4;
    u32 unk_8;
};

void fn_80042198(struct fn_80042198_Arg0 *arg0, u32 arg1) {
    arg0->unk_8 = arg1;
    fn_800433F4(arg0->unk_4, arg1);
}
