#include "types.h"

extern u8 lbl_1_bss_718E0[23016];
extern u32 lbl_1_bss_7F0C0[5336];
extern void fn_1_F8918(u8 *, u32 *, u8 *);
extern u32 lbl_1_bss_84420[2];
extern u16 lbl_1_data_414[36];

void fn_1_F79C8(void) {
    fn_1_F8918(&lbl_1_bss_718E0[0x1c0], lbl_1_bss_7F0C0, lbl_1_bss_718E0);
    lbl_1_bss_84420[0] = (u32)((u8 *)lbl_1_bss_7F0C0 + 0x4b5c);
    lbl_1_data_414[0] = 0xffff;
}
