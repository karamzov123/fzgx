#include "types.h"

extern s8 lbl_1_bss_6EAA6;
extern u8 lbl_1_bss_6EAA4;

extern void fn_80067974(void);
extern void fn_800280F0(void);
extern void fn_80026FE0(void);

void fn_1_9FF58(void) {
    if (lbl_1_bss_6EAA6) {
        fn_80067974();
        fn_800280F0();
        fn_80026FE0();
    }
    lbl_1_bss_6EAA4 = 0;
}
