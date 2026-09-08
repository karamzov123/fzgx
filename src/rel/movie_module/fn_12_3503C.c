#include "types.h"

extern void fn_12_4980(void *);

struct Fn12_3503C_Context {
    u8 pad_a8[0xa8];
    void *field_a8;
};

void fn_12_3503C(struct Fn12_3503C_Context *self) {
    fn_12_4980(self->field_a8);
}
