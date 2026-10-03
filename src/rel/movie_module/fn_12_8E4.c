#include "types.h"

typedef struct Fn12_8E4_Self {
    u32 unk_00;
    u32 unk_04;
    s32 field_08;
    s32 field_0c;
    u32 unk_10;
    u32 unk_14;
    u32 unk_18;
    u32 unk_1c;
    u32 field_20;
    u32 unk_24;
    u32 unk_28;
    u32 unk_2c;
    u32 unk_30;
    s32 field_34;
    u32 field_38;
} Fn12_8E4_Self;

typedef struct Fn12_8E4_Arg1 {
    u32 unk_00;
    u32 unk_04;
    u32 unk_08;
    s32 unk_0c;
    u32 unk_10[15];
    u32 unk_4c;
} Fn12_8E4_Arg1;

extern u8 lbl_12_rodata_194[248];
extern void fn_12_309C(u32, u32, void *);
extern void fn_12_3710(u32, u32, u32);
extern void fn_12_3DBDC(void *, void *, u32);

void fn_12_8E4(Fn12_8E4_Self *self, Fn12_8E4_Arg1 *arg1, s32 arg2, void *arg3) {
    struct { u32 v[6]; } vec_a;
    struct { u32 v[6]; } vec_b;
    s32 x;
    s32 y;
    u32 mode;
    s32 cond;

    vec_a.v[0] = arg1->unk_04;
    vec_a.v[1] = arg1->unk_08;
    vec_a.v[2] = arg1->unk_0c;
    vec_a.v[3] = 0x40;

    if (self->field_08 == 0) {
        x = (s32)arg1->unk_08;
    } else {
        x = self->field_08;
    }
    if (self->field_0c == 0) {
        y = arg1->unk_0c / 2;
    } else {
        y = self->field_0c;
    }

    switch (arg2) {
    case 0x10:
        mode = 1;
        break;
    case 0x18:
        mode = 2;
        break;
    case 0x20:
        mode = 3;
        break;
    case 0:
    default:
        fn_12_309C(0, 0, lbl_12_rodata_194);
        mode = 0;
        break;
    }

    vec_b.v[0] = mode;
    vec_b.v[1] = (u32)arg3;
    vec_b.v[2] = (u32)x;
    vec_b.v[3] = (u32)y;
    vec_b.v[4] = arg1->unk_08;
    vec_b.v[5] = (u32)(arg1->unk_0c / 2);

    switch (arg2) {
    case 0x10:
        break;
    case 0x20:
        cond = (self->field_34 != 100);
        if (cond == 1) {
            self->field_34 = 13;
            fn_12_3710(self->field_20, arg1->unk_4c, self->field_38);
        }
        fn_12_3DBDC(&vec_a, &vec_b, self->field_38);
        break;
    }
}
