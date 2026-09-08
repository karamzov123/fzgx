#include "types.h"

extern u32 fn_12_77C(u32, u32, u32, u32);

struct fn_12_35108_Arg0 {
    u8 pad_0[0xA8];
    u32 unk_A8;
};

void fn_12_35108(struct fn_12_35108_Arg0 *arg0, u32 arg1, u32 arg2, u32 arg3) {
    fn_12_77C(arg0->unk_A8, arg1, arg2, ((u32)__cntlzw((1 - arg3)) >> 5));
}
