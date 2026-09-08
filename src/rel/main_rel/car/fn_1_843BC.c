#include "types.h"

extern f32 lbl_1_rodata_3530[22];
extern f32 fn_1_86A2C(void *);

typedef struct {
    u8 _pad[0x32c];
    void *field_32c;
} Fn1843BCObject;

f32 fn_1_843BC(Fn1843BCObject *obj) {
    if (obj == NULL) {
        return lbl_1_rodata_3530[0];
    }

    if (obj->field_32c == NULL) {
        return lbl_1_rodata_3530[0];
    }

    return fn_1_86A2C(obj->field_32c);
}
