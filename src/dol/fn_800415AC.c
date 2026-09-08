#include "types.h"

extern u32 fn_800454FC(u32);

struct fn_800415AC_Arg0 {
    u8 pad_0[0x4];
    u32 unk_4;
};

void fn_800415AC(struct fn_800415AC_Arg0 *arg0) {
    fn_800454FC(arg0->unk_4);
}
