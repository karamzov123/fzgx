#include "types.h"

/* fzgx:begin fn_8_10084 */
extern struct fn_8_10084_lbl_8_bss_550 lbl_8_bss_550;
extern u32 fn_80083DB0(u32, u32);

struct fn_8_10084_lbl_8_bss_550 {
    u32 unk_0;
};

void fn_8_10084(u32 arg0) {
    fn_80083DB0(arg0, (lbl_8_bss_550.unk_0 + 40));
}
/* fzgx:end fn_8_10084 */

/* fzgx:begin fn_8_100B4 */
extern u8 lbl_8_bss_555;

void fn_8_100B4(void) {
    lbl_8_bss_555 = 1;
}
/* fzgx:end fn_8_100B4 */
