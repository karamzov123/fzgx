#include "types.h"


struct fn_80057DA8_Arg0 {
    u8 pad_0[0x20];
    u32 unk_20;
};

u32 fn_80057DA8(struct fn_80057DA8_Arg0 *arg0) {
    return arg0->unk_20;
}
