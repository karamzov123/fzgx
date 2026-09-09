#include "types.h"

struct fn_80047578_lbl_8017B030 {
    u32 unk_0;
    u32 unk_4;
};

extern struct fn_80047578_lbl_8017B030 lbl_8017B030[];
extern u32 memset(u32, u32, u32);

void fn_80047578(void) {
    struct fn_80047578_lbl_8017B030* state;
    state = (struct fn_80047578_lbl_8017B030*)&lbl_8017B030;
    memset((u32)((u8*)(u32)state + 40), 0, 256);
    state->unk_0 = 0;
    state->unk_4 = 0;
}
