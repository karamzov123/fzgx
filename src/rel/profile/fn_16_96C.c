#include "types.h"

struct fn_16_96C_lbl_1_bss_8B614 {
    u8 pad_0[0x4];
    u32 unk_4;
};

extern struct fn_16_96C_lbl_1_bss_8B614 lbl_1_bss_8B614;
extern u16 lbl_1_bss_968;
extern u32 fn_16_A38(void);
extern u32 fn_16_AA8(void);
extern u32 fn_1_4C10(void);

void fn_16_96C(u32 arg0) {
    u32 v0;
    u32 v1;
    u32 v2;
    fn_16_AA8();
    v0 = arg0;
    if ((s32)fn_16_A38() == 0 && (s32)fn_1_4C10() == 0) {
    v1 = lbl_1_bss_8B614.unk_4;
    v2 = v1;
    if ((v1 & ~0x7FFFFFFF) != 0) {
    v2 = (v2 & 0x7FFFFFFF);
    lbl_1_bss_8B614.unk_4 = v2;
    lbl_1_bss_968 = 1;
    } else {
    if ((v2 & 0x40000000) != 0) {
    v2 = (v2 & 0xBFFFFFFF);
    lbl_1_bss_8B614.unk_4 = v2;
    lbl_1_bss_968 = 8;
    }
    }
    }
}
