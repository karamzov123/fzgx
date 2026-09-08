#include "types.h"

typedef struct {
    u8 pad_0[0x8];
    u16 field_8;
    u16 field_A;
    u8 pad_C[0x1E];
    u8 flags_2A;
} Object;

void fn_1_ACF0C(Object* obj) {
    obj->field_8 = 0x40;
    obj->field_A = 0xE;
    obj->flags_2A |= 0x4;
}
