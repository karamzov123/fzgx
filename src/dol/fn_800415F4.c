#include "types.h"

extern u32 fn_8004550C(u32);

struct fn_800415F4_Arg0 {
    u8 pad_0[0x4];
    u32 unk_4;
};

void fn_800415F4(struct fn_800415F4_Arg0 *arg0) {
    fn_8004550C(arg0->unk_4);
}
