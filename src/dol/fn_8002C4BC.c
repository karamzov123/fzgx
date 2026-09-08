#include "types.h"

typedef struct {
    u8 pad[0x84];
    u32 value;
} Fn8002C4BCObject;

u32 fn_8002C4BC(Fn8002C4BCObject *object) {
    return object->value;
}
