#include "types.h"


struct fn_8005A668_Arg0 {
    u8 pad_0[0xA0];
    u16 unk_A0;
};

void fn_8005A668(struct fn_8005A668_Arg0 *arg0, u32 arg1) {
    if (arg0 == 0) { return; }
    arg0->unk_A0 = arg1;
}
