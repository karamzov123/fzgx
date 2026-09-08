#include "types.h"

extern u32 lbl_1_rodata_45D0[];
extern u8 lbl_1_bss_6F1D8;
extern u32 lbl_1_bss_6F1DC;

extern void fn_80067344(s32, s32, u32, s32);
extern void fn_80067898(u32);

void fn_1_A50C8(s32 index) {
    u32 value;

    if (index > 0x28) {
        value = 0xA9100700;
    } else {
        value = lbl_1_rodata_45D0[index * 2];
    }

    fn_80067344(2, 0xF, 0xB0270000, 0x7F);

    if (!lbl_1_bss_6F1D8 && lbl_1_bss_6F1DC <= 0x2D) {
        fn_80067898(value);
    }
}
