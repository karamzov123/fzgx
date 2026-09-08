#include "types.h"

typedef struct Fn12_8C8 {
    u8 _pad08[8];
    u32 field_08;
    u32 field_0c;
} Fn12_8C8;

void fn_12_8C8(Fn12_8C8 *self, u32 value_08, u32 value_0c) {
    self->field_08 = value_08;
    self->field_0c = value_0c;
}
