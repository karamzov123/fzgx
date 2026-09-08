#include "types.h"


struct fn_8006C17C_Arg0 {
    u8 pad_0[0x28];
    u32 unk_28;
};

s32 fn_8006C17C(struct fn_8006C17C_Arg0 *arg0) {
    arg0->unk_28 = (arg0->unk_28 & ~0x1);
    return 0;
}
