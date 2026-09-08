#include "types.h"


struct fn_80028568_Arg0 {
    u8 pad_0[0x4];
    u32 unk_4;
    u32 unk_8;
};

void fn_80028568(struct fn_80028568_Arg0 *arg0) {
    arg0->unk_4 = 10;
    arg0->unk_8 = 0;
}
