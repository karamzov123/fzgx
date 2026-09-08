#include "types.h"

extern struct fn_8008B484_gTRKState gTRKState;

struct fn_8008B484_gTRKState {
    u8 pad_0[0x98];
    u32 unk_98;
};

s32 fn_8008B484(void) {
    gTRKState.unk_98 = 1;
    return 0;
}
