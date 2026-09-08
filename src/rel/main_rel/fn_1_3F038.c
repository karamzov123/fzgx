#include "types.h"

extern u8 lbl_1_bss_26C58;
extern u8 lbl_1_bss_3C11;
extern void fn_1_36330(void);
extern void fn_1_36B10(void);
extern void fn_1_3C98(void);
extern u32 lbl_1_bss_3C30[1327];
extern void fn_1_37AA8(void);
extern void fn_1_37E98(void);

int fn_1_3F038(void) {
    if (lbl_1_bss_26C58 != 0) {
        return 1;
    }
    if (lbl_1_bss_3C11 != 0) {
        fn_1_36330();
        lbl_1_bss_3C11 = 0;
    }
    fn_1_36B10();
    fn_1_3C98();
    if ((lbl_1_bss_3C30[0] & 0x00000004) == 0) {
        fn_1_37AA8();
    }
    if (lbl_1_bss_26C58 != 0) {
        fn_1_37E98();
        return 1;
    }
    return 0;
}
