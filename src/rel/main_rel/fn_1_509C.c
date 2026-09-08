#include "types.h"

extern u32 lbl_1_bss_F58;
extern u32 lbl_1_bss_7AC40[2];
extern u32 lbl_1_bss_7AC20[8];

s32 fn_8008023C(u32 *, u32 *, u32);

s32 fn_1_509C(void) {
    lbl_1_bss_7AC40[0] = lbl_1_bss_F58;
    if (fn_8008023C(lbl_1_bss_7AC20, lbl_1_bss_7AC40, 4) == 0) {
        return 0;
    }
    lbl_1_bss_7AC20[0] = lbl_1_bss_7AC40[0];
    return 1;
}
