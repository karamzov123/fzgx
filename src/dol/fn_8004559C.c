#include "types.h"

typedef struct Fn8004559CObject {
    u8 pad[0x98];
    s16 value;
} Fn8004559CObject;

s16 fn_8004559C(Fn8004559CObject *self) {
    return self->value;
}
