#include "types.h"

extern f32 lbl_1_rodata_67A8;
extern void fn_80008BA8(void *dst, const void *src, u32 size);

void fn_1_E5884(void *base, const void *value) {
    if (value == 0) {
        f32 zero = lbl_1_rodata_67A8;
        *(f32 *)((char *)base + 0xfc) = zero;
        *(f32 *)((char *)base + 0xf8) = zero;
        *(f32 *)((char *)base + 0xf4) = zero;
    } else {
        fn_80008BA8((char *)base + 0xf4, value, 0xc);
    }
}
