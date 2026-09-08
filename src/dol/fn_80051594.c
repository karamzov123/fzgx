#include "types.h"

struct fn_80051594_Arg0 {
    u8 pad_0[0x350];
    u32 unk_350;
};

extern u32 fn_80053A38(u32);
extern u32 fn_80053A84(u32, u32);
extern u32 memset(u32, u32, u32);

u32 fn_80051594(struct fn_80051594_Arg0 *arg0, u32 arg1) {
    u32 v0;
    u32 v1;
    u32 v2;
    u32 t7, t8;
    v0 = arg0->unk_350;
    memset(((u32)arg0 + 852), 0, 48);
    memset(((u32)arg0 + 900), 0, 52);
    memset(((u32)arg0 + 952), 0, 256);
    memset(((u32)arg0 + 1208), 0, 256);
    memset(((u32)arg0 + 1464), 0, 768);
    memset(*(u32 *)((u8 *)(u32)arg0 + 0), 0, 768);
    memset(((u32)arg0 + 2232), 0, 768);
    *(u8 *)((u8 *)(u32)arg0 + 837) = 0;
    *(u32 *)((u8 *)(u32)arg0 + 840) = 0;
    v1 = *(u32 *)((u8 *)(u32)arg0 + 848);
    v2 = v1;
    if (v1 != 0) {
    *(u32 *)((u8 *)(u32)arg0 + 848) = 0;
    t7 = fn_80053A38(v2);
    v2 = t7;
    v2 = v0;
    t8 = fn_80053A84(v2, 8212);
    v2 = t8;
    *(u32 *)((u8 *)(u32)arg0 + 848) = v2;
    }
    *(u32 *)((u8 *)(u32)arg0 + 844) = arg1;
    return v2;
}
