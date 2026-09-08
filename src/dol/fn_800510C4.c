#include "types.h"


struct fn_800510C4_Arg0 {
    u8 pad_0[0x348];
    u32 unk_348;
};

u32 fn_800510C4(struct fn_800510C4_Arg0 *arg0) {
    return ((u32)__cntlzw((12 - arg0->unk_348)) >> 5);
}
