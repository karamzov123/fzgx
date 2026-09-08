#include "types.h"

extern f32 lbl_1_rodata_6D70;
extern f64 fn_80088538(f32);
extern f64 fn_800883E8(f32);

void fn_1_F56F8(f32 *out, f32 *in) {
    f32 value;
    f32 scale;
    f32 zero;

    value = fn_80088538(in[3]);
    scale = fn_800883E8(value);
    zero = lbl_1_rodata_6D70;
    out[3] = zero;
    if (scale > zero) {
        out[0] = value * in[0] / scale;
        out[1] = value * in[1] / scale;
        out[2] = value * in[2] / scale;
    } else {
        out[2] = zero;
        out[1] = zero;
        out[0] = zero;
    }
}
