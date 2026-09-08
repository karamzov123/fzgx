#include "types.h"

extern struct fn_8008B6BC_gTRKCPUState gTRKCPUState;

struct fn_8008B6BC_gTRKCPUState {
    u8 pad_0[0x80];
    u32 unk_80;
};

u32 fn_8008B6BC(void) {
    return gTRKCPUState.unk_80;
}
