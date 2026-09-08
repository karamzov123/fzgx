#include "types.h"

extern u32 fn_800454D4(u32);

struct fn_80041530_Arg0 {
    u8 pad_0[0x4];
    u32 unk_4;
};

void fn_80041530(struct fn_80041530_Arg0 *arg0) {
    fn_800454D4(arg0->unk_4);
}
