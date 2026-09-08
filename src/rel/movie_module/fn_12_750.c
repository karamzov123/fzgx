#include "types.h"

typedef struct Fn12750Object {
    u8 _pad30[0x30];
    u32 value;
} Fn12750Object;

void fn_12_750(Fn12750Object *self, u32 value) {
    self->value = value;
}
