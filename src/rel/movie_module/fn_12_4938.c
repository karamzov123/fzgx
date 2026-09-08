#include "types.h"

extern u32 lbl_12_bss_568[140];
extern void *memset(void *dest, int value, u32 size);

void fn_12_4938(void) {
    memset(lbl_12_bss_568, 0, 0x22c);
    lbl_12_bss_568[2] = 8;
    lbl_12_bss_568[1] = 0;
}
