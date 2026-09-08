#include "types.h"

extern struct fn_12_34E4C_lbl_12_bss_1B8A0 lbl_12_bss_1B8A0;
extern u32 fn_80058BFC(void);

struct fn_12_34E4C_lbl_12_bss_1B8A0 {
    u8 pad_0[0x100];
    u32 unk_100;
    u32 unk_104;
    u32 unk_108;
    u32 unk_10C;
};

void fn_12_34E4C(void) {
    struct fn_12_34E4C_lbl_12_bss_1B8A0 *p_lbl_12_bss_1B8A0;
    p_lbl_12_bss_1B8A0 = (struct fn_12_34E4C_lbl_12_bss_1B8A0 *)&lbl_12_bss_1B8A0;
    fn_80058BFC();
    p_lbl_12_bss_1B8A0->unk_10C = 0;
    p_lbl_12_bss_1B8A0->unk_108 = 0;
    p_lbl_12_bss_1B8A0->unk_104 = 0;
    p_lbl_12_bss_1B8A0->unk_100 = 0;
}
