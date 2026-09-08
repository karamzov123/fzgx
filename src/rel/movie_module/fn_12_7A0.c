#include "types.h"

typedef struct Fn127A0Object {
    u8 _pad20[0x20];
    void *value;
} Fn127A0Object;

extern void fn_12_365C(void *value);

void fn_12_7A0(Fn127A0Object *self) {
    fn_12_365C(self->value);
}
