#include "types.h"

extern s32 fn_1_C24A4(void);
extern u8 fn_1_B7C00(void);
extern s32 lbl_1_bss_8CA40;

void fn_1_131154(void) {
    if (fn_1_C24A4() == 0 && fn_1_B7C00() == 0) {
        lbl_1_bss_8CA40 = -1;
    }
}
