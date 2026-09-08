#include "types.h"

typedef struct Fn12_3A3CC_Object {
    u8 _pad[0x40];
    u32 value;
} Fn12_3A3CC_Object;

extern u32 fn_12_2D888(u32 value);

u32 fn_12_3A3CC(Fn12_3A3CC_Object *object) {
    u32 value;

    if (object != 0) {
        value = object->value;
    } else {
        value = 0;
    }
    return fn_12_2D888(value);
}
