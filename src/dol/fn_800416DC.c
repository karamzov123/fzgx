#include "types.h"


struct fn_800416DC_Arg0 {
    u8 pad_0[0x3C];
    u32 unk_3C;
};

void fn_800416DC(struct fn_800416DC_Arg0 *arg0, u32 arg1) {
    arg0->unk_3C = arg1;
}
