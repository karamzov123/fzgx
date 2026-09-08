#include "types.h"

extern f32 lbl_1_rodata_3C2C[33];

extern void fn_80008BA8(void *dst, const void *src, int size);
extern void lbl_8006D7DC(void *value);
extern void fn_8006E424(int value);
extern void lbl_8006DB74(void *value);

typedef struct Fn1_88DBCInner {
    u8 pad_000[0x1FC];
    f32 value_1FC;
} Fn1_88DBCInner;

typedef struct Fn1_88DBCObject {
    u8 pad_000[2];
    s8 field_002;
    u8 pad_003[0x208 - 3];
    u8 field_208[0x124];
    Fn1_88DBCInner *field_32C;
    void *field_330;
    u8 pad_334[0x86];
    s16 field_3BA;
} Fn1_88DBCObject;

void fn_1_88DBC(Fn1_88DBCObject *object) {
    u8 value[12];

    if (object == 0) {
        return;
    }
    if (object->field_3BA != 0) {
        return;
    }
    if (object->field_002 - 1 < 0) {
        return;
    }

    fn_80008BA8(value, object->field_208, 12);
    lbl_8006D7DC(value);

    if (object->field_32C != 0) {
        fn_8006E424((int)(lbl_1_rodata_3C2C[0] * -object->field_32C->value_1FC));
    }

    lbl_8006DB74(object->field_330);
}
