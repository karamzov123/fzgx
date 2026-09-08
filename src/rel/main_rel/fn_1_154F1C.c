#include "types.h"

extern u32 lbl_1_bss_8EDF0[76];
extern void *memset(void *dest, int value, u32 size);

void fn_1_154F1C(void) {
    u32 *ptr = &lbl_1_bss_8EDF0[13];

    lbl_1_bss_8EDF0[7] = 0;
    lbl_1_bss_8EDF0[5] = 0;
    lbl_1_bss_8EDF0[6] = 0;
    memset(ptr, 0, 250);
    *(u8 *)ptr = 0;
}
