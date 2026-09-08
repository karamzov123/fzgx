#include "types.h"

typedef struct {
    u8 pad[0x3bc];
    u32 value;
} Object;

void fn_1_8E448(Object *obj, u32 value) {
    obj->value = value;
}
