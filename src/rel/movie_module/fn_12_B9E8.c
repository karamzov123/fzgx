#include "types.h"
struct Sig_fn_800589BC_fn_800589BC_Arg2 {
    u32 unk_0;
    u32 unk_4;
};
struct Sig_fn_800589BC_fn_800589BC_Arg3 {
    u32 unk_0;
    u32 unk_4;
};

typedef u32 (*fn_12_B9E8_Fn0)(u32, u32, u32, u32);
typedef u32 (*fn_12_B9E8_Fn1)(u32, u32, u32);
typedef u32 (*fn_12_B9E8_Fn2)(u32, u32, void *);
struct fn_12_B9E8_Arg0 {
    u8 pad_0[0x1ec];
    u32 unk_1ec;
    u32 unk_1f0;
    u32 unk_1f4;
    u32 unk_1f8;
    u32 unk_1fc;
    u32 unk_200;
    u8 pad_204[0x80];
    u32 unk_284;
    u32 unk_288;
    u8 pad_28c[0x103c];
    u32 unk_12c8;
    u32 unk_12cc;
    u32 unk_12d0;
    u8 pad_12d4[0x30];
    u32 unk_1304;
};
typedef struct MovieHdrView {
    u8 pad_0[0x12c8];
    u32 f_12c8;
} MovieHdrView;

extern u32 fn_800589BC(u32, u32, struct Sig_fn_800589BC_fn_800589BC_Arg2 *, struct Sig_fn_800589BC_fn_800589BC_Arg3 *);

#pragma opt_lifetimes off
s32 fn_12_B9E8(struct fn_12_B9E8_Arg0 *arg0, u32 arg1) {
    u32 v5;
    u32 v0;
    u32 v2;
    struct { u32 value; } v4;
    s32 v3;
    u32 v8;
    u32 v6;
    u32 v1;
    u32 v9;
    u32 v10;
    u32 v11;
    struct Sig_fn_800589BC_fn_800589BC_Arg3 loc_8;
    /* frame */
    arg0->unk_1304 = 2;
    *(u32 *)((u8 *)(u32)arg0 + 512) = (arg0->unk_200 + 1);
    v0 = *(u32 *)((u8 *)arg1 + 0);
    ((fn_12_B9E8_Fn0)*(u32 *)((u8 *)v0 + 24))(arg1, 1, (0x80000000 - 1), (u32)&arg0->unk_12c8);
    v1 = (((~0x3) & (arg0->unk_12c8)));
    v2 = *(u32 *)((u8 *)v1 + 4);
    v3 = (s32)((arg0->unk_12c8 - v1) << 3);
    v1 = v1 + 8;
    if (v3 >= 32) {
    v2 = *(u32 *)((u8 *)v1 + 0);
    v1 = v1 + 4;
    v3 -= 32;
    }
    if (v3 != 0) {
    v2 = (v2 << v3);
    }
    v4.value = *(u32 *)((u8 *)v1 + 0);
    v1 = v1 + 4;
    if (v3 >= 7) {
    v3 -= 7;
    if (v3 != 0) {
    v5 = (v2 | ((u32)v4.value >> (25 - v3)));
    v6 = ((u32)v5 >> 7);
    v5 = ((u32)v4.value << v3);
    } else {
    v6 = ((u32)v2 >> 7);
    v5 = v4.value;
    }
    v4.value = *(u32 *)((u8 *)v1 + 0);
    v1 = v1 + 4;
    } else {
    v6 = ((u32)v2 >> 7);
    v5 = (v2 << 25);
    v3 += 25;
    }
    *(u32 *)((u8 *)(u32)arg0 + 508) = (v6 & 0x3F);
    *(u32 *)((u8 *)(u32)arg0 + 504) = ((v6 >> 6) & 0x3F);
    *(u32 *)((u8 *)(u32)arg0 + 500) = ((v6 >> 13) & 0x3F);
    *(u32 *)((u8 *)(u32)arg0 + 496) = ((v6 >> 19) & 0x1F);
    *(u32 *)((u8 *)(u32)arg0 + 492) = ((u32)v6 >> 24);
    *(u32 *)((u8 *)(u32)arg0 + 644) = ((u32)v5 >> 31);
    if (v3 == 31) {
    v5 = v4.value;
    v3 = 0;
    v1 = v1 + 4;
    } else {
    v6 = (v5 << 1);
    v5 = v6;
    v3++;
    }
    *(u32 *)((u8 *)(u32)arg0 + 648) = ((u32)v5 >> 31);
    if (v3 == 31) {
    v3 = 0;
    v1 = ((4) + (v1));
    } else {
    v3++;
    }
    *(u32 *)((u8 *)(u32)arg0 + 4816) = (v3 & 0x7);
    v8 = *(u32 *)((u8 *)arg0 + 4808);
    v9 = (v3 - *(u32 *)((u8 *)arg0 + 4816));
    v1 = (v1 + ((s32)(v9 + 7) >> 3));
    fn_800589BC(((u32)arg0 + 4808), ((v1 - 8) - v8), (struct Sig_fn_800589BC_fn_800589BC_Arg2 *)((u32)arg0 + 4808), (struct Sig_fn_800589BC_fn_800589BC_Arg3 *)&loc_8);
    v8 = *(u32 *)((u8 *)arg1 + 0);
    v10 = v8;
    ((fn_12_B9E8_Fn1)*(u32 *)((u8 *)v10 + 32))(arg1, 0, (u32)&((MovieHdrView *)(u32)arg0)->f_12c8);
    v11 = *(u32 *)((u8 *)arg1 + 0);
    ((fn_12_B9E8_Fn2)*(u32 *)((u8 *)v11 + 28))(arg1, 1, &loc_8);
    return 0;
}
#pragma opt_lifetimes reset

