#include "types.h"

struct fn_800482B8_lbl_8017B5A8 {
    u32 unk_0;    u8 pad_4[0x9CC];
};

extern struct fn_800482B8_lbl_8017B5A8 lbl_8017B5A8[];
extern u32 lbl_8017BF78[];
extern u32 memset(u32, u32, u32);

void fn_800482B8(void) {
    u32 v0;
    v0 = lbl_8017B5A8[0].unk_0;
    lbl_8017B5A8[0].unk_0 = (v0 - 1);
    memset((u32)&lbl_8017BF78, 0, 6016);
}
