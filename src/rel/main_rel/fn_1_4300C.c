#include "types.h"

typedef struct Fn14300CObject {
    u8 pad0[2];
    u16 flags;
    f32 value;
    u8 pad1[0x38];
} Fn14300CObject;

extern void fn_1_43264(void *arg0, f32 arg1);

void fn_1_4300C(Fn14300CObject *object) {
    fn_1_43264((u8 *)object + 0x40, object->value);
    if (object->flags & 1) {
        fn_1_43264((u8 *)object + 0x58, object->value);
    }
}
