#include "types.h"

extern u32 fn_80045354(u32);

struct fn_800420F4_Arg0 {
    u8 pad_0[0x4];
    u32 unk_4;
};

void fn_800420F4(struct fn_800420F4_Arg0 *arg0) {
    fn_80045354(arg0->unk_4);
    *(u8 *)((u8 *)(u32)arg0 + 1) = 0;
}
