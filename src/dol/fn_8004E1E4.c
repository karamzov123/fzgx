#include "types.h"


struct fn_8004E1E4_Arg0 {
    u8 pad_0[0xC];
    u32 unk_C;
    u32 unk_10;
};

void fn_8004E1E4(struct fn_8004E1E4_Arg0 *arg0) {
    if ((s32)arg0->unk_C != 0) { return; }
    arg0->unk_10 = 0;
    arg0->unk_C = 1;
}
