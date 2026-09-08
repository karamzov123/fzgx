#include "types.h"


struct fn_80036058_Arg0 {
    u8 pad_0[0xC];
    u32 unk_C;
};

void fn_80036058(struct fn_80036058_Arg0 *arg0, u32 arg1) {
    arg0->unk_C = ((arg0->unk_C & ~0x1FFFFF) | ((arg1 >> 5) & 0x1FFFFFF));
}
