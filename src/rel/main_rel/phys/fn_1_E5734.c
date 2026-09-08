#include "types.h"

typedef struct {
    u32 unk0;
    f32 value;
} E5734Obj;

// Returns the float at offset 4 of the object.
f32 fn_1_E5734(E5734Obj *self) {
    return self->value;
}
