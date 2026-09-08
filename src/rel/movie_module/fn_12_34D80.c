#include "types.h"

extern u32 lbl_12_bss_1B9A4;
extern u32 fn_80059428(u32, u32, u32);

void fn_12_34D80(u32 arg0, u32 arg1) {
    lbl_12_bss_1B9A4 = fn_80059428(6, arg0, arg1);
}
