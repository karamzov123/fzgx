#include "types.h"

extern u8 lbl_1_bss_6C8EC[2144];
extern void fn_1_76650(void *arg);
extern u8 lbl_1_bss_6D14C[92];

void fn_1_72848(void) {
    u8 i;

    for (i = 0; i < 8; i++) {
        fn_1_76650(&lbl_1_bss_6C8EC[i * 0x10c]);
    }
    lbl_1_bss_6D14C[0] = 0;
}
