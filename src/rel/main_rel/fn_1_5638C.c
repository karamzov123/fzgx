#include "types.h"

extern u8 lbl_1_bss_6C7A4[40];
extern void fn_80074A7C(void);

void fn_1_5638C(u32 value) {
    *(u32 *)(lbl_1_bss_6C7A4 + 0x20) = value;
    fn_80074A7C();
}
