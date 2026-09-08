#include "types.h"

/* fzgx:begin fn_1_58158 */
extern u32 lbl_801A6410;
extern u32 lbl_1_bss_6C840;
extern char lbl_1_data_1C68C[9];
extern void fn_1_46B4(u32, u32, void*, u32);

// fn_1_58158: loads global values and calls fn_1_46B4.
void fn_1_58158(void) {
    u32 v1 = lbl_801A6410;
    u32 v2 = lbl_1_bss_6C840;
    fn_1_46B4(v1, v2, &lbl_1_data_1C68C, 0x3a6);
}
/* fzgx:end fn_1_58158 */

/* fzgx:begin fn_1_584AC */
extern u32 lbl_1_data_1D628;

// fn_1_584AC: linear congruential generator.
u32 fn_1_584AC(void) {
    u32 state = lbl_1_data_1D628;
    u32 next = state * 0x41c64e6du + 0x3039u;
    lbl_1_data_1D628 = next;
    return (next >> 16) & 0x7FFFu;
}
/* fzgx:end fn_1_584AC */
