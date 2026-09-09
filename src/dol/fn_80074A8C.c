#include "types.h"

extern const f32 lbl_801A7488;
extern const f32 lbl_801A7484;
extern u8 lbl_801A6D80[4];
extern u32 lbl_801A6D88[2];

void fn_80074A8C(f32 a, f32 b, f32 c, f32 d) {
    if (lbl_801A7488 != a || lbl_801A7488 != b ||
        lbl_801A7488 != c || lbl_801A7488 != d) {
        u8 red;
        u8 green;
        u8 blue;
        u8 alpha;
        green = (u8)(lbl_801A7484 * b);
        blue = (u8)(lbl_801A7484 * c);
        alpha = (u8)(lbl_801A7484 * d);
        lbl_801A6D88[0] = 1;
        lbl_801A6D80[0] = ((u8)(lbl_801A7484 * a));
        lbl_801A6D80[1] = green;
        lbl_801A6D80[2] = blue;
        lbl_801A6D80[3] = alpha;
    } else {
        lbl_801A6D88[0] = 0;
        lbl_801A6D80[0] = 0xFF;
        lbl_801A6D80[1] = 0xFF;
        lbl_801A6D80[2] = 0xFF;
        lbl_801A6D80[3] = 0xFF;
    }
}
