#include "types.h"

extern u32 fn_8004559C(u32);

struct fn_800416A8_Arg0 {
    u8 pad_0[0x4];
    u32 unk_4;
};

void fn_800416A8(struct fn_800416A8_Arg0 *arg0) {
    fn_8004559C(arg0->unk_4);
}
