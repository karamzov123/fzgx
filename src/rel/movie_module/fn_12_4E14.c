#include "types.h"

extern u32 lbl_12_bss_BA0;
extern u32 lbl_12_rodata_5B8;

void * fn_12_4E14(void) {
    lbl_12_bss_BA0 = (u32)&lbl_12_rodata_5B8;
    return &lbl_12_rodata_5B8;
}
