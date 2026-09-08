#include "types.h"

extern u32 fn_8008E76C(u32, u32, u32);

struct fn_8008E324_Arg0 {
    u8 pad_0[0x8];
    u32 unk_8;
};

void fn_8008E324(struct fn_8008E324_Arg0 *arg0, u32 arg1, u32 arg2) {
    u32 v0;
    arg0->unk_8 = arg1;
    *(u32 *)((u8 *)(u32)arg0 + 12) = arg2;
    *(u32 *)((u8 *)(u32)arg0 + 0) = *(u32 *)((u8 *)(u32)arg0 + 8);
    v0 = *(u32 *)((u8 *)(u32)arg0 + 8);
    *(u32 *)((u8 *)(u32)arg0 + 4) = v0;
    *(u32 *)((u8 *)(u32)arg0 + 16) = 0;
    *(u32 *)((u8 *)(u32)arg0 + 20) = *(u32 *)((u8 *)(u32)arg0 + 12);
    fn_8008E76C(((u32)arg0 + 24), v0, arg2);
}
