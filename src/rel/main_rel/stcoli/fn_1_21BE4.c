#include "types.h"

extern void fn_1_18214(void *, void *, int);
extern f32 lbl_1_rodata_6D0;
extern void fn_1_21950(void *, f32, f32);

typedef struct Fn_1_21BE4 {
    u32 flags;
    unsigned char pad04[0x38];
    f32 field3c;
    f32 field40;
} Fn_1_21BE4;

void fn_1_21BE4(Fn_1_21BE4 *self, void *arg) {
    f32 value2;
    f32 value1;

    fn_1_18214(self, arg, 0);
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
