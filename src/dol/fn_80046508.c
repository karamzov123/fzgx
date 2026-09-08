#include "types.h"


struct fn_80046508_Arg0 {
    u8 pad_0[0x20];
    u32 unk_20;
};

void fn_80046508(struct fn_80046508_Arg0 *arg0, u32 arg1) {
    arg0->unk_20 = arg1;
}
