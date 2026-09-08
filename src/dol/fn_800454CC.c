#include "types.h"


struct fn_800454CC_Arg0 {
    u8 pad_0[0xBC];
    u32 unk_BC;
};

u32 fn_800454CC(struct fn_800454CC_Arg0 *arg0) {
    return arg0->unk_BC;
}
