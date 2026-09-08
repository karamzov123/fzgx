#include "types.h"


struct fn_800416D4_Arg0 {
    u8 pad_0[0x40];
    u32 unk_40;
};

void fn_800416D4(struct fn_800416D4_Arg0 *arg0, u32 arg1) {
    arg0->unk_40 = arg1;
}
