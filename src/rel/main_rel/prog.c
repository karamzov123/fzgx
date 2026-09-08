#include "types.h"

/* fzgx:begin fn_1_A6F1C */
extern u32 lbl_1_bss_6F5F0;

u32 fn_1_A6F1C(void) {
    return lbl_1_bss_6F5F0;
}
/* fzgx:end fn_1_A6F1C */

/* fzgx:begin fn_1_A6F6C */
extern s16 lbl_1_bss_962;
extern u32 lbl_1_data_341B8;

s32 fn_1_A6F6C(void) {
    s32 result = 0;

    if (lbl_1_bss_962 == 0x99) {
        if (*(s8 *)((u8 *)lbl_1_data_341B8 + 4) == 5) {
            result = 1;
        }
    }

    return result;
}
/* fzgx:end fn_1_A6F6C */

/* fzgx:begin fn_1_A728C */
// fn_1_A728C: returns a constant.
int fn_1_A728C(void) {
    return 0;
}
/* fzgx:end fn_1_A728C */
