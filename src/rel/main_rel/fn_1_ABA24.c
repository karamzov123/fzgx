#include "types.h"

extern s32 fn_8002A988(u8 byte_val, void *arg1, void *arg2);

typedef struct {
    u8 pad[0x2];
    u16 field_0x2;
    s32 field_0x4;
    s32 field_0x8;
    u8 pad_0xc[0x4];
    u8 data[0x8];
} OffsetStruct;

typedef struct {
    u8 byte_0;
    u8 pad[0x23];
    OffsetStruct *ptr_0x24;
} ArgStruct;

void fn_1_ABA24(ArgStruct *arg) {
    s32 result = fn_8002A988(arg->byte_0, arg->ptr_0x24->data,
        &arg->ptr_0x24->data[0x4]);
    arg->ptr_0x24->field_0x4 = result;
    if (arg->ptr_0x24->field_0x4 == 0) {
        arg->ptr_0x24->field_0x8 |= 0x80;
    }
    if (arg->ptr_0x24->field_0x4 != -1) {
        arg->ptr_0x24->field_0x2 = 0;
    }
}
