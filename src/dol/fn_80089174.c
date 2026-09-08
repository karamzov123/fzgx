#include "types.h"

extern u32 fn_80003590(u32, u32, u32);

struct fn_80089174_Arg0 {
    u8 pad_0[0x8];
    u32 unk_8;
    u32 unk_C;
};

u32 fn_80089174(struct fn_80089174_Arg0 *arg0, u32 arg1) {
    u32 v0;
    u32 t0;
    arg0->unk_8 = 0;
    arg0->unk_C = 0;
    v0 = (u32)arg0;
    if ((s32)arg1 == 0) {
    v0 += 16;
    t0 = fn_80003590(v0, 0, 2176);
    v0 = t0;
    }
    return v0;
}
