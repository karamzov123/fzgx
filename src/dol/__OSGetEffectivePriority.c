#include "types.h"


struct __OSGetEffectivePriority_Arg0 {
    u8 pad_0[0x2D4];
    u32 unk_2D4;
    u8 pad_2D8[0x1C];
    u32 unk_2F4;
};

u32 __OSGetEffectivePriority(struct __OSGetEffectivePriority_Arg0 *arg0) {
    u32 v0;
    u32 v1;
    u32 v2;
    u32 v3;
    u32 v4;
    v0 = arg0->unk_2F4;
    v1 = arg0->unk_2D4;
    v2 = v0;
    while (v2 != 0) {
    v3 = *(u32 *)((u8 *)v2 + 0);
    if (v3 != 0) {
    v4 = *(u32 *)((u8 *)v3 + 720);
    if ((s32)v4 < (s32)v1) {
    v1 = v4;
    }
    }
    v2 = *(u32 *)((u8 *)v2 + 16);
    }
    return v1;
}
