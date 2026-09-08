#include "types.h"


struct fn_80006DE8_Arg0 {
    u32 unk_0;
    u8 pad_4[0x34];
    u32 unk_38;
};

u32 fn_80006DE8(struct fn_80006DE8_Arg0 *arg0) {
    if ((s32)arg0->unk_0 == 1) { return (u32)arg0; }
    return arg0->unk_38;
}
