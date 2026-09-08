#include "types.h"

/* fzgx:begin fn_1_48690 */
extern void *fn_1_48730(void);

u16 fn_1_48690(void) {
    void *value = fn_1_48730();
    if (value != 0) {
        return *(u16 *)((u8 *)value + 8);
    }
    return 8;
}
/* fzgx:end fn_1_48690 */

/* fzgx:begin fn_1_4877C */
// fn_1_4877C: empty in retail (single blr).
void fn_1_4877C(void) {
}
/* fzgx:end fn_1_4877C */
