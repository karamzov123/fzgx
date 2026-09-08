#include "types.h"

typedef struct Fn1249EC {
    u8 _pad58[0x58];
    u32 field_58;
    u32 field_5c;
} Fn1249EC;

void fn_12_49EC(Fn1249EC *self, u32 value_58, u32 value_5c) {
    self->field_58 = value_58;
    self->field_5c = value_5c;
}
