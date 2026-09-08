#include "types.h"

extern u32 fn_12_22254(u32, u32, u32, u32);

struct fn_12_24DD0_Arg0 {
    u8 pad_0[0x1AB4];
    u32 unk_1AB4;
};

void fn_12_24DD0(struct fn_12_24DD0_Arg0 *arg0, u32 arg1, u32 arg2) {
    fn_12_22254((u32)arg0, arg0->unk_1AB4, arg1, arg2);
}
