#include "types.h"

extern void fn_1_F7578(void);
extern u8 lbl_1_bss_7F028;

void fn_1_F7308(void) {
    fn_1_F7578();
    lbl_1_bss_7F028 &= 0xFE;
}
