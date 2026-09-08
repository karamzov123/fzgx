#include "types.h"

extern u32 lbl_12_bss_4C0[42];
extern void *memset(void *dest, int value, u32 size);

void fn_12_35F0(void) {
    memset(lbl_12_bss_4C0, 0, 0xa8);
    lbl_12_bss_4C0[1] = 8;
}
