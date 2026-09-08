#include "types.h"

extern u8 lbl_1_bss_6F1D8;
extern u32 lbl_1_bss_6F1DC;
extern void fn_80067898(u32 arg0);

void fn_1_A2D84(u32 arg0) {
    if (lbl_1_bss_6F1D8 == 0 && lbl_1_bss_6F1DC <= 0x2D) {
        fn_80067898(arg0);
    }
}
