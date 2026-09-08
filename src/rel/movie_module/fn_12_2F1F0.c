#include "types.h"


struct fn_12_2F1F0_E68_u32 { u32 unk_0; u8 pad_4[0x40]; };
struct fn_12_2F1F0_Arg0 {
    u8 pad_0[0x1AA0];
    struct fn_12_2F1F0_E68_u32 unk_1AA0[1];
};

u32 fn_12_2F1F0(struct fn_12_2F1F0_Arg0 *arg0, u32 arg1) {
    return arg0->unk_1AA0[arg1].unk_0;
}
