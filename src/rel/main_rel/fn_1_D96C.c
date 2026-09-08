#include "types.h"

// fn_1_D96C: main_rel .text:0x0000D96C size 0x10
// Returns address of lbl_1_bss_1018 + 4 bytes (offset by 2 u16 elements)

extern u16 lbl_1_bss_1018[18];

u16* fn_1_D96C(void) {
    return &lbl_1_bss_1018[2];
}
