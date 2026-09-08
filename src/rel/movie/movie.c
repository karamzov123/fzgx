#include "types.h"

/* fzgx:begin fn_5_3E54 */
extern u32 lbl_5_bss_AF4;
extern u32 lbl_5_bss_B90;

void fn_5_3E54(u32 arg0) {
    lbl_5_bss_B90 = *(u32 *)((u8 *)&lbl_5_bss_AF4 + ((s8)arg0 << 2));
}
/* fzgx:end fn_5_3E54 */
