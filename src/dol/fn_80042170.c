#include "types.h"

extern u32 fn_800433D0(u32, u32);

struct fn_80042170_Arg0 {
    u8 pad_0[0x4];
    u32 unk_4;
    u8 pad_8[0x30];
    u32 unk_38;
};

void fn_80042170(struct fn_80042170_Arg0 *arg0, u32 arg1) {
    arg0->unk_38 = arg1;
    fn_800433D0(arg0->unk_4, arg1);
}
