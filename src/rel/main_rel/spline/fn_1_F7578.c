#include "types.h"

extern u8 lbl_1_bss_7F02C[16];
extern u16 lbl_1_bss_7F03C[8];
extern void fn_8001D32C(u32 *);
extern void fn_80008BEC(void *, int, u32);

void fn_1_F7578(void) {
    u32 values[4];

    values[0] = 2;
    values[1] = 2;
    values[2] = 2;
    values[3] = 2;
    fn_8001D32C(values);
    fn_80008BEC(lbl_1_bss_7F02C, 0, 0x10);
    fn_80008BEC(lbl_1_bss_7F03C, 0, 0x10);
}
