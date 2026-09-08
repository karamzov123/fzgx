#include "types.h"

extern u32 lbl_1_bss_646E0;
extern u32 lbl_1_bss_646E4;
extern u32 lbl_1_bss_4E6A8;
extern u32 lbl_1_bss_4E6AC;

void fn_1_520CC(void) {
    u32 value_a;
    u32 value_b;

    value_a = lbl_1_bss_646E0;
    value_b = lbl_1_bss_646E4;
    lbl_1_bss_4E6A8 = value_a;
    lbl_1_bss_4E6AC = value_b;
}
