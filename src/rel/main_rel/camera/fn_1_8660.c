#include "rel/main_rel/globals.h"

extern f32 lbl_1_rodata_188;
extern s32 fn_1_8708(u8 mode, f32 *value0, f32 *value1);

// Reads the camera values, falling back when the camera state cannot provide them.
s32 fn_1_8660(f32 *value0, f32 *value1) {
    f32 result0;
    f32 result1;
    u8 mode;
    Obj_1_bss_F68_Target *obj;
    f32 fallback;

    obj = lbl_1_bss_F68;
    if (obj == 0) {
        mode = 0;
    } else if ((obj->unk_0 & ((u32)1 << 31)) != 0) {
        mode = 0;
    } else {
        mode = obj->unk_4A;
    }

    if (fn_1_8708(mode, &result0, &result1) != 0) {
        *value0 = result0;
        *value1 = result1;
        return 1;
    }

    fallback = lbl_1_rodata_188;
    *value0 = fallback;
    *value1 = fallback;
    return -1;
}
