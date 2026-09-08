#include "types.h"

extern int fn_8002A958(u8 byte);

typedef struct {
    u8 pad[0x2];
    u16 field_0x2;
    s32 field_0x4;
} OffsetStruct;

typedef struct {
    u8 byte_0;
    u8 pad[0x23];
    OffsetStruct *ptr_0x24;
} ArgStruct;

// fn_1_AB93C: main_rel .text:0x000AB93C size 0x50
void fn_1_AB93C(ArgStruct *arg) {
    u8 byte_val = arg->byte_0;
    int result = fn_8002A958(byte_val);

    OffsetStruct *s = arg->ptr_0x24;
    s->field_0x4 = result;

    s = arg->ptr_0x24;
    if (s->field_0x4 != -1) {
        s->field_0x2 = 0;
    }
}
