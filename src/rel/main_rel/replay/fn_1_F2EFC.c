#include "types.h"

extern u8 lbl_1_bss_7EFD4;
extern u8 lbl_1_bss_7EFD8[0x44];

void fn_1_F2EFC(void) {
    u8 flags = lbl_1_bss_7EFD4;

    if ((flags & 1) == 0) {
        return;
    }
    if (((flags >> 5) & 1) != 0) {
        return;
    }

    lbl_1_bss_7EFD4 = flags & ~8;
    lbl_1_bss_7EFD8[0x19] = 1;
}
