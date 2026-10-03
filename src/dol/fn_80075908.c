#include "types.h"
struct Sig_fn_800775EC_fn_800775EC_Arg0 {
    u8 pad_0[0xC];
    u8 unk_C;
    u8 unk_D;
    u8 unk_E;
};
struct Sig_fn_80075C78_fn_80075C78_Arg0 {
    u32 unk_0;
    u8 pad_4[0x3C];
    u32 unk_40;
};
struct fn_80075908_State { u32 a[11]; };
struct fn_80075908_Callback { u32 a[3]; struct fn_80075908_State state; };
typedef void (*fn_80075908_Fn0)(void *);
struct fn_80075908_lbl_801A3220 {
    u8 unk_0;
    u8 unk_1;
    u8 pad_2[0x6];
    u32 unk_8;
    u32 unk_C;
    u8 pad_10[0x4];
    u32 unk_14;
    u32 unk_18;
};
extern s32 fn_80076CD4(void *, void *, s32, void *);
extern struct fn_80075908_lbl_801A3220 lbl_801A3220;
extern u32 fn_80072168(u32);
extern u32 fn_80074D88(u32, void *, void *);
extern fn_80075908_Fn0 lbl_801A6D40;
extern u32 lbl_801A6D58;
extern u32 lbl_801A6D84;
extern u32 lbl_801A6D88;
extern void fn_8007264C(u32, s32, s32, s32, u8);
extern void fn_80073678(u32);
extern void fn_80073898(u32);
extern void fn_80074660(u32);
extern void fn_80074788(u32);
extern void fn_80075080(void *);
extern void fn_80075C78(struct Sig_fn_80075C78_fn_80075C78_Arg0 *);
extern void fn_80077240(u32, void *, void *);
extern void fn_80077384(s32 *);
extern void fn_80077488(void *, void *);
extern void fn_800775EC(struct Sig_fn_800775EC_fn_800775EC_Arg0 *);
extern void fn_80077654(u32);
extern void fn_80077714(u32);

#pragma opt_common_subs off
void fn_80075908(u32 arg0, u32 arg1) {
    s32 v0;
    s32 v1;
    u32 v2;
    struct fn_80075908_State loc_54;
    struct fn_80075908_Callback loc_1C;
    struct { u32 a[4]; u16 b[2]; } loc_8;
    s32 t7;
    loc_54.a[2] = 30;
    loc_54.a[3] = 0;
    loc_54.a[5] = 64;
    loc_54.a[6] = 0;
    loc_54.a[0] = 0;
    loc_54.a[1] = 0;
    loc_54.a[4] = 0;
    loc_54.a[7] = 1;
    loc_54.a[8] = 0;
    loc_54.a[3] = 1;
    loc_54.a[2] = 45;
    loc_54.a[5] = 76;
    loc_54.a[6] = 4;
    fn_800775EC((struct Sig_fn_800775EC_fn_800775EC_Arg0 *)arg0);
    if ((*(u32 *)arg0 & 1) == 0) {
        fn_80075080((void *)arg0);
    }
    fn_80072168(((*(u32 *)arg0 >> 2) & 1) ^ 1);
    loc_8.a[2] = 10;
    loc_8.a[3] = 5;
    loc_8.b[0] = 0;
    loc_8.b[1] = 0;
    loc_8.a[0] = 0;
    loc_8.a[1] = 0;
    if ((*(u32 *)arg0 & 1) != 0) {
        fn_80077488((void *)arg0, &loc_8);
    } else {
        fn_80077384((s32 *)arg0);
    }
    if ((s32)lbl_801A3220.unk_14 != (s32)loc_8.a[2]) {
        lbl_801A3220.unk_14 = loc_8.a[2];
        loc_8.a[0] = 1;
    }
    if ((s32)lbl_801A3220.unk_18 != (s32)loc_8.a[3]) {
        lbl_801A3220.unk_18 = loc_8.a[3];
        loc_8.a[1] = 1;
    }
    if ((*(u32 *)arg0 & 0x80) != 0 || (s32)lbl_801A6D58 != 0) {
        fn_80077240(arg0, &loc_54, &loc_8);
    } else {
        v0 = 4;
        v1 = 0;
        fn_8007264C(0, 10, 0, v0, v1);
        t7 = fn_80076CD4((void *)arg0, &loc_54, arg1, &loc_8);
        if (t7 == 0) {
            fn_80077240(arg0, &loc_54, &loc_8);
        }
    }
    if ((*(u32 *)arg0 & 0x200) != 0) {
        fn_80074D88(arg0, &loc_54, &loc_8);
    }
    if (lbl_801A6D40 != 0) {
        loc_1C.a[0] = lbl_801A3220.unk_0;
        loc_1C.a[1] = arg0;
        loc_1C.a[2] = arg1;
        loc_1C.state = loc_54;
        lbl_801A6D40(&loc_1C);
        loc_54 = loc_1C.state;
    }
    if ((s32)lbl_801A6D88 != 0) {
        if ((s32)lbl_801A3220.unk_8 != (s32)loc_54.a[0]) {
            lbl_801A3220.unk_8 = loc_54.a[0];
            fn_80077654(loc_54.a[0]);
        }
        v2 = loc_54.a[0] + 1;
        loc_54.a[0] = v2;
    }
    if ((s32)lbl_801A6D84 != 0) {
        if ((s32)lbl_801A3220.unk_C != (s32)loc_54.a[0]) {
            lbl_801A3220.unk_8 = loc_54.a[0];
            fn_80077714(loc_54.a[0]);
        }
        loc_54.a[0]++;
    }
    fn_80074788(1);
    fn_80073678(loc_54.a[0] & 0xFF);
    fn_80074660(loc_54.a[1] & 0xFF);
    fn_80073898(loc_54.a[4] & 0xFF);
    fn_80075C78((struct Sig_fn_80075C78_fn_80075C78_Arg0 *)arg0);
    lbl_801A3220.unk_0 = 0;
    if ((*(u32 *)arg0 & 0x80) != 0) {
        lbl_801A3220.unk_1 = 0;
    } else {
        lbl_801A3220.unk_1 = *(u8 *)(arg0 + 0x12);
    }
}
#pragma opt_common_subs reset

