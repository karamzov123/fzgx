#include "types.h"

/* fzgx:begin fn_1_D2E18 */
// fn_1_D2E18: 3-way byte comparison (a > b returns 1, a < b returns -1, a == b returns 0)
s32 fn_1_D2E18(u8* a, u8* b) {
    u8 va = *a;
    u8 vb = *b;
    s32 d1;
    s32 d2;
    if (va > vb) {
        return 1;
    }
    d1 = va - vb;
    d2 = vb - va;
    return (d1 | d2) >> 31;
}
/* fzgx:end fn_1_D2E18 */

/* fzgx:begin fn_1_D3144 */
extern u32 lbl_1_bss_7AD00;

// fn_1_D3144: Conditionally call function pointer from BSS
void fn_1_D3144(void) {
    u32 fn_ptr = lbl_1_bss_7AD00;
    if (fn_ptr != 0) {
        ((void (*)(void))fn_ptr)();
    }
}
/* fzgx:end fn_1_D3144 */
