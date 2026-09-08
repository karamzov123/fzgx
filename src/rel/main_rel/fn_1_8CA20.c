#include "types.h"

// fn_1_8CA20: Load value from BSS and return with offset applied

extern u32 lbl_1_bss_6D83C;

u32 fn_1_8CA20(void) {
    return lbl_1_bss_6D83C + 0x3a8;
}
