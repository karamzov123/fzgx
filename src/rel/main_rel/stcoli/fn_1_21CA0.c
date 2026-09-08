#include "types.h"

extern f32 lbl_1_rodata_6D0;
extern void fn_1_21950(void *, f32, f32);

typedef struct Fn_1_21CA0 {
    u32 flags;
    unsigned char pad04[0x38];
    f32 field3c;
    f32 field40;
} Fn_1_21CA0;

void fn_1_21CA0(Fn_1_21CA0 *self, void *arg) {
    f32 value2;
    f32 value1;

    if ((self->flags & 0x800) != 0) {
        value2 = self->field3c;
    } else {
        value2 = lbl_1_rodata_6D0;
    }
    if ((self->flags & 0x400) != 0) {
        value1 = self->field40;
    } else {
        value1 = lbl_1_rodata_6D0;
    }
    fn_1_21950(arg, value1, value2);
}
