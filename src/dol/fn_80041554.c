#include "types.h"

extern u32 fn_800454DC(u32);

struct fn_80041554_Arg0 {
    u8 pad_0[0x4];
    u32 unk_4;
};

void fn_80041554(struct fn_80041554_Arg0 *arg0) {
    fn_800454DC(arg0->unk_4);
}
