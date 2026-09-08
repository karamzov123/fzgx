#include "types.h"


struct fn_800416F8_Arg0 {
    u8 pad_0[0x2C];
    u32 unk_2C;
};

u32 fn_800416F8(struct fn_800416F8_Arg0 *arg0) {
    return arg0->unk_2C;
}
