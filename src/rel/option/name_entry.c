#include "types.h"

/* fzgx:begin fn_4_D848 */
extern struct fn_4_D848_lbl_4_bss_5680 lbl_4_bss_5680;
extern u32 fn_80083DB0(u32, u32);

struct fn_4_D848_lbl_4_bss_5680 {
    u32 unk_0;
};

void fn_4_D848(u32 arg0) {
    fn_80083DB0(arg0, (lbl_4_bss_5680.unk_0 + 40));
}
/* fzgx:end fn_4_D848 */

/* fzgx:begin fn_4_D878 */
extern u8 lbl_4_bss_5685;

void fn_4_D878(void) {
    lbl_4_bss_5685 = 1;
}
/* fzgx:end fn_4_D878 */
