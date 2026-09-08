#include "types.h"


struct fn_800288B4_Arg0 {
    u8 pad_0[0x10];
    u32 unk_10;
};
struct fn_800288B4_Arg1 {
    u32 unk_0;
};

void fn_800288B4(struct fn_800288B4_Arg0 *arg0, struct fn_800288B4_Arg1 *arg1) {
    arg1->unk_0 = arg0->unk_10;
    arg0->unk_10 = (u32)arg1;
}
