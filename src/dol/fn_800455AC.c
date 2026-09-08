#include "types.h"


struct fn_800455AC_Arg0 {
    u8 pad_0[0x78];
    u32 unk_78;
    u32 unk_7C;
};

void fn_800455AC(struct fn_800455AC_Arg0 *arg0, u32 arg1, u32 arg2) {
    arg0->unk_78 = arg1;
    arg0->unk_7C = arg2;
}
