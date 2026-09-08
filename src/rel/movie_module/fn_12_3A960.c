#include "types.h"

typedef struct Fn12_3A960_Object {
    u8 _pad[0x64];
    u32 value;
} Fn12_3A960_Object;

void fn_12_3A960(Fn12_3A960_Object *object, u32 value) {
    object->value = value;
}
