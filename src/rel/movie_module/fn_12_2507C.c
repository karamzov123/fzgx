#include "types.h"

extern u32 fn_12_24A88(u32, u32);
extern u32 fn_12_6A90(u32);

struct fn_12_2507C_Arg0 {
    u8 pad_0[0x1AEC];
    u32 unk_1AEC;
};

s32 fn_12_2507C(struct fn_12_2507C_Arg0 *arg0) {
    u32 v0;
    u32 t0, t1;
    t0 = fn_12_6A90(*(u32 *)((u8 *)arg0->unk_1AEC + 0));
    v0 = t0;
    if ((s32)t0 != 0) {
    v0 = (u32)arg0;
    t1 = fn_12_24A88(v0, (0xFF000000 + 3338));
    v0 = t1;
    } else {
    v0 = 0;
    }
    return v0;
}
