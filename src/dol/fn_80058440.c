#include "types.h"


struct fn_80058440_Arg0 {
    u8 pad_0[0x8];
    u32 unk_8;
};

u32 fn_80058440(struct fn_80058440_Arg0 *arg0) {
    return arg0->unk_8;
}
