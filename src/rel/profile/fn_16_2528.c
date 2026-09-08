#include "types.h"

extern struct fn_16_2528_lbl_16_bss_860C0 lbl_16_bss_860C0;
extern u32 fn_80074188(u32, u32, u32, u32);

struct fn_16_2528_lbl_16_bss_860C0 {
    s16 unk_0;
    s16 unk_2;
    s16 unk_4;
    s16 unk_6;
};

void fn_16_2528(void) {
    fn_80074188(lbl_16_bss_860C0.unk_0, lbl_16_bss_860C0.unk_2, lbl_16_bss_860C0.unk_4, lbl_16_bss_860C0.unk_6);
}
