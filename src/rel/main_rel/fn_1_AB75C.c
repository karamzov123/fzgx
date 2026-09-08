#include "types.h"

extern int fn_8002C0A0(u8 byte);
extern void fn_8002E90C(u8 byte, int value);
extern f32 lbl_1_rodata_4CA8;

typedef struct {
    u8 pad[0x2];
    u16 field_0x2;
} OffsetStruct;

typedef struct {
    u8 byte_0;
    u8 pad_0x1[0x23];
    OffsetStruct *ptr_0x24;
    u8 pad_0x28[0x2];
    u8 flags_0x2a;
    u8 pad_0x2b[0x5];
    s32 field_0x30;
    u8 pad_0x34[0x4];
    f32 field_0x38;
} ArgStruct;

// fn_1_AB75C: main_rel .text:0x000AB75C size 0x68
void fn_1_AB75C(ArgStruct *arg) {
    arg->ptr_0x24->field_0x2 = 0x12c;
    arg->field_0x30 = fn_8002C0A0(arg->byte_0);
    arg->field_0x38 = lbl_1_rodata_4CA8;
    arg->flags_0x2a |= 2;
    fn_8002E90C(arg->byte_0, 0);
}
