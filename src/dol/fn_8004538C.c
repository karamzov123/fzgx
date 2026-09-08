#include "types.h"


struct fn_8004538C_Arg0 {
    u8 pad_0[0x4];
    u32 unk_4;
};

void fn_8004538C(struct fn_8004538C_Arg0 *arg0) {
    if ((s32)arg0->unk_4 != 0) { return; }
    arg0->unk_4 = 1;
}
