#include "types.h"

extern u32 fn_80045504(u32);

struct fn_800415D0_Arg0 {
    u8 pad_0[0x4];
    u32 unk_4;
};

void fn_800415D0(struct fn_800415D0_Arg0 *arg0) {
    fn_80045504(arg0->unk_4);
}
