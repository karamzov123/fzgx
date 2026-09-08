#include "types.h"

struct fn_8008B784_lbl_8015B884 {
    u32 unk_0;
    u32 unk_4;
    u32 unk_8;
};
struct fn_8008B784_gTRKState {
    u8 pad_0[0x98];
    u32 unk_98;
};

extern struct fn_8008B784_gTRKState gTRKState[];
extern struct fn_8008B784_lbl_8015B884 lbl_8015B884;
extern u32 MWTRACE(u32, ...);
extern u32 gTRKCPUState[];
extern u32 lbl_80095BA8[];

s32 fn_8008B784(u32 arg0, u32 arg1) {
    struct fn_8008B784_lbl_8015B884 *p_lbl_8015B884;
    u32 v0;
    u32 v1;
    u32 t0;
    v0 = arg0;
    if ((s32)arg1 != 0) {
    v0 = 1795;
    } else {
    p_lbl_8015B884 = (struct fn_8008B784_lbl_8015B884 *)&lbl_8015B884;
    p_lbl_8015B884->unk_8 = v0;
    v0 = 1;
    p_lbl_8015B884->unk_4 = 0;
    p_lbl_8015B884->unk_0 = 1;
    t0 = MWTRACE(v0, (u32)&lbl_80095BA8);
    v0 = t0;
    v0 = (u32)&gTRKCPUState;
    v1 = p_lbl_8015B884->unk_4;
    *(u32 *)((u8 *)v0 + 504) = (*(u32 *)((u8 *)v0 + 504) | 1024);
    if ((s32)p_lbl_8015B884->unk_4 == 0 || (s32)v1 == 16) {
    v0 = p_lbl_8015B884->unk_8;
    p_lbl_8015B884->unk_8 = (v0 - 1);
    }
    v0 = 0;
    gTRKState[0].unk_98 = 0;
    }
    return v0;
}
