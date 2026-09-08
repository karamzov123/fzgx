#include "types.h"

extern u32 lbl_1_bss_6E958[2];
extern void fn_80071718(u32 value);
extern void fn_800711A8(u32 value);

void fn_1_8E1E8(void) {
    if (lbl_1_bss_6E958[1] != 0) {
        fn_80071718(lbl_1_bss_6E958[1]);
        fn_800711A8(lbl_1_bss_6E958[0]);
        lbl_1_bss_6E958[1] = 0;
        lbl_1_bss_6E958[0] = 0;
    }
}
