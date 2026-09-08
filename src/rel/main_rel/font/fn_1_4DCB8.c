#include "types.h"

extern u32 lbl_1_bss_4B9CC[23];

u32 fn_1_4DCB8(u32 value) {
    u32 old_value = lbl_1_bss_4B9CC[22];
    lbl_1_bss_4B9CC[22] = value;
    return old_value;
}

