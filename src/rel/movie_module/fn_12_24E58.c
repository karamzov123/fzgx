#include "types.h"

extern u32 fn_12_21D60(u32, u32, u32);

struct fn_12_24E58_Arg0 {
    u8 pad_0[0x1AB4];
    u32 unk_1AB4;
};

s32 fn_12_24E58(struct fn_12_24E58_Arg0 *arg0) {
    fn_12_21D60((u32)arg0, arg0->unk_1AB4, 1);
    return 0;
}
