#include "types.h"

typedef struct {
    u32 unk_0;
    u8 pad_4[0x46];
    u8 unk_4A;
} Obj_1_bss_F68_Target;

extern Obj_1_bss_F68_Target *lbl_1_bss_F68;
extern f32 lbl_1_rodata_188;
extern s32 fn_1_8708(u8 arg0, f32 *arg1, f32 *arg2);

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
