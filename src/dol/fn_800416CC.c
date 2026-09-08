#include "types.h"


struct fn_800416CC_Arg0 {
    u8 pad_0[0x44];
    u32 unk_44;
};

void fn_800416CC(struct fn_800416CC_Arg0 *arg0, u32 arg1) {
    arg0->unk_44 = arg1;
}
