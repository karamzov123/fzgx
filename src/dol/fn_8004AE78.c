#include "types.h"


struct fn_8004AE78_Arg0 {
    u8 pad_0[0x8];
    u32 unk_8;
};

s32 fn_8004AE78(struct fn_8004AE78_Arg0 *arg0) {
    u32 v0;
    v0 = (u32)arg0;
    if (arg0->unk_8 != 0) {
    v0 = *(u32 *)((u8 *)v0 + 84);
    return v0;
    }
    v0 = 0;
    return v0;
}
