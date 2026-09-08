#include "types.h"

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Vec3;

extern void lbl_8006D7B0(s32);
extern void fn_8006E294(Vec3 *);
extern s32 lbl_8006D24C(f32, f32);
extern void fn_8006E324(s16);
extern void fn_8006E398(s32);

void fn_1_9CCE8(s32 arg0) {
    Vec3 value;
    s32 result;

    switch (arg0) {
    case 1:
        lbl_8006D7B0(arg0);
        break;
    case 2:
        fn_8006E294(&value);
        result = lbl_8006D24C(value.z, value.y);
        fn_8006E324((s16)(result - 0x4000));
        break;
    case 3:
        fn_8006E294(&value);
        result = lbl_8006D24C(value.x, value.z);
        fn_8006E398(result);
        break;
    default:
        break;
    }
}
