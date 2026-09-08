#include "types.h"

extern struct fn_16_2328_lbl_16_bss_150 lbl_16_bss_150;
extern u32 fn_1_3F8C(u32, u32, u32, u32);
extern u32 fn_1_435C(u32);
extern u32 fn_1_4A00(u32, u32, u32);
extern u32 lbl_16_bss_21C;
extern u32 lbl_16_bss_B0B4;
extern u32 lbl_16_data_82B00;
extern u32 lbl_16_data_82D98;
extern void fn_16_6CD8(void);

struct fn_16_2328_lbl_16_bss_150 {
    u32 unk_0;
};

void fn_16_2328(void) {
    u32 t2;
    fn_1_435C(lbl_16_bss_150.unk_0);
    fn_1_4A00(1, 15, lbl_16_bss_150.unk_0);
    lbl_16_bss_B0B4 = 0;
    t2 = fn_1_3F8C((u32)&lbl_16_data_82D98, (u32)fn_16_6CD8, 0, 8);
    lbl_16_data_82B00 = t2;
    lbl_16_bss_21C = -1;
}
