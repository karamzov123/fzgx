#include "types.h"


struct fn_8005A688_Arg0 {
    u8 pad_0[0x80];
    u32 unk_80;
};

void fn_8005A688(struct fn_8005A688_Arg0 *arg0, u32 arg1) {
    if (arg0 == 0) { return; }
    arg0->unk_80 = arg1;
}
