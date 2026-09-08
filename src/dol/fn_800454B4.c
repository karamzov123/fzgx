#include "types.h"


struct fn_800454B4_Arg0 {
    u8 pad_0[0xD2];
    s16 unk_D2[1];
};

s16 fn_800454B4(struct fn_800454B4_Arg0 *arg0, u32 arg1) {
    return arg0->unk_D2[arg1];
}
