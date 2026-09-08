#include "types.h"


struct fn_8004E198_Arg0 {
    u8 pad_0[0xC];
    u32 unk_C;
};

void fn_8004E198(struct fn_8004E198_Arg0 *arg0) {
    if ((s32)arg0->unk_C != 3) { return; }
    arg0->unk_C = 0;
}
