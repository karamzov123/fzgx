#include "types.h"


struct fn_800421C0_Arg0 {
    u8 pad_0[0x1];
    u8 unk_1;
};

s32 fn_800421C0(struct fn_800421C0_Arg0 *arg0) {
    return (s8)arg0->unk_1;
}
