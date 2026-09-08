#include "types.h"

extern s32 fn_1_5910(void);
extern f32 lbl_1_rodata_7960[43];
extern f32 lbl_1_rodata_7A40[2];
extern s32 fn_1_58C4(void);
extern void lbl_8006DCA4(void);
extern void fn_1_57714(s32 arg0);
extern void fn_1_57720(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
extern void fn_1_57CD0(s32 arg0, void *arg1);

void fn_1_1067A8(void *arg0, f32 arg1, f32 arg2) {
    s32 result;
    s32 value;
    s32 count;
    s32 stride;
    u8 *base;
    f32 scale;
    f32 offset;

    scale = arg1;
    offset = arg2;
    result = fn_1_5910();

    value = (s32)(lbl_1_rodata_7960[0] * scale / offset + lbl_1_rodata_7A40[0]);
    count = fn_1_58C4();
    if (count <= 2) {
        lbl_8006DCA4();
        fn_1_57714(value);
        fn_1_57720(1, 4, 5, 0);
        base = (u8 *)arg0;
        stride = *(s32 *)(base + 0xC000);
        fn_1_57CD0((stride & 0x7FFF) << 1, base + (result * stride << 5) + 0x4000);
        fn_1_57714(6);
    }
}
