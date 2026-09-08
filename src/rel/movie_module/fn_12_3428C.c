#include "types.h"

extern u32 fn_800567EC(u32);

struct fn_12_3428C_Arg0 {
    u8 pad_0[0x4C];
    u32 unk_4C;
};

void fn_12_3428C(struct fn_12_3428C_Arg0 *arg0) {
    if (arg0->unk_4C != 0) {
    fn_800567EC(arg0->unk_4C);
    }
}
