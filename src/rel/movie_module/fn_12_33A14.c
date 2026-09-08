#include "types.h"

extern u32 fn_12_35670(u32, u32);

struct fn_12_33A14_Arg0 {
    u8 pad_0[0xB4];
    u32 unk_B4;
    u32 unk_B8;
};

void fn_12_33A14(struct fn_12_33A14_Arg0 *arg0) {
    arg0->unk_B4 = 0;
    arg0->unk_B8 = 0;
    fn_12_35670((u32)arg0, 0);
}
