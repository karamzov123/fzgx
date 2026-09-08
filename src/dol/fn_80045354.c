#include "types.h"

extern u32 fn_8004E1B0(u32);

struct fn_80045354_Arg0 {
    u8 pad_0[0x8];
    u32 unk_8;
};

void fn_80045354(struct fn_80045354_Arg0 *arg0) {
    fn_8004E1B0(arg0->unk_8);
    *(u32 *)((u8 *)(u32)arg0 + 4) = 0;
}
