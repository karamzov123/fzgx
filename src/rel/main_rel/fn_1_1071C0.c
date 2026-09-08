#include "types.h"

extern u8 lbl_1_bss_86EC4;
extern u32 lbl_1_bss_86EC0;

void fn_1_1071C0(void) {
    if (lbl_1_bss_86EC4 != 0) {
        lbl_1_bss_86EC4 = 0;
        lbl_1_bss_86EC0 = 0;
    }
}
