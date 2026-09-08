#include "types.h"

extern u8 lbl_1_bss_6EAA8[8];
extern u8 lbl_1_bss_6EAB0;

extern void fn_8001EA30(u8 *arg0, int arg1);
extern void fn_80020530(void);

void fn_1_9FF9C(void) {
    fn_8001EA30(lbl_1_bss_6EAA8, 2);
    fn_80020530();
    lbl_1_bss_6EAB0 = 0;
}
