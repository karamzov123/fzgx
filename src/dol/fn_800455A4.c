#include "types.h"


struct fn_800455A4_Arg0 {
    u8 pad_0[0x3C];
    u32 unk_3C;
};

u32 fn_800455A4(struct fn_800455A4_Arg0 *arg0) {
    return arg0->unk_3C;
}
