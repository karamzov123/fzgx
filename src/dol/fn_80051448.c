#include "types.h"

struct Sig_fn_800501F4_fn_800501F4_Arg0 {
    u8 pad_0[0x8];
    s32 unk_8;
    s32 unk_C;
    s32 unk_10;
};

typedef struct Sig_pair {
    u32 a;
    u32 b;
} Sig_pair;

typedef struct Sig_fn_800519B0_Fn800519B0Arg3 {
    u8 field_0;
    u8 field_1;
    u8 field_2;
    s8 field_3;
    u32 field_4;
    u32 field_8;
    s16 field_C;
    s16 field_E;
    u8 pad_10[0x14];
    Sig_pair field_24;
    u32 field_2C;
    Sig_pair field_30;
    u32 field_38;
    u8 field_3C;
    u8 field_3D;
} Sig_fn_800519B0_Fn800519B0Arg3;

struct fn_80051448_Arg0 {
    u8 pad_0[0x34C];
    struct Sig_fn_800501F4_fn_800501F4_Arg0 *unk_34C;
    u8 pad_350[0x34];
    s32 field_384;
    s32 field_388;
    s32 field_38C;
    s32 field_390;
    u8 pad_394[0xC];
    Sig_pair field_3A0;
    s32 field_3A8;
    Sig_pair field_3AC;
    s32 field_3B4;
};

struct fn_80051448_Locals {
    u32 field_8;
    Sig_fn_800519B0_Fn800519B0Arg3 buf;
};

extern s32 fn_800501F4(struct Sig_fn_800501F4_fn_800501F4_Arg0 *, s32);
extern s32 fn_800519B0(u8 *, u32, u32 *, Sig_fn_800519B0_Fn800519B0Arg3 *);
extern u8 lbl_80187130[512];
extern void * memset(void *, int, u32);

#pragma opt_common_subs off
#pragma opt_propagation off
void fn_80051448(struct fn_80051448_Arg0 *arg0) {
    u8 *p;
    u8 *end;
    struct Sig_fn_800501F4_fn_800501F4_Arg0 *v0;
    struct fn_80051448_Locals locals;
    u32 lab_t2;
    struct Sig_fn_800501F4_fn_800501F4_Arg0 *t;

    t = arg0->unk_34C;
    v0 = arg0->unk_34C;
    if (t != 0) {
        lab_t2 = 0x40;
        memset(&locals.buf, 0, lab_t2);
        locals.field_8 = 0;
        p = lbl_80187130;
        end = p + 4;
        while (p != end) {
            *p = fn_800501F4(v0, 8);
            p++;
        }
        fn_800519B0(lbl_80187130, 4, &locals.field_8, 0);
        end = end + locals.field_8;
        while (p != end) {
            *p = fn_800501F4(v0, 8);
            p++;
        }
        if (fn_800519B0(lbl_80187130, 0x200, &locals.field_8, &locals.buf) >= 0) {
            arg0->field_388 = locals.buf.field_3;
            arg0->field_38C = locals.buf.field_4;
            arg0->field_390 = locals.buf.field_8;
            arg0->field_3A0 = locals.buf.field_24;
            arg0->field_3A8 = locals.buf.field_2C;
            arg0->field_3AC = locals.buf.field_30;
            arg0->field_3B4 = locals.buf.field_38;
            arg0->field_384 = 1;
        }
    }
}
#pragma opt_common_subs reset
