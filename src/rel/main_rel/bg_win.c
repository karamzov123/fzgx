#include "types.h"

/* fzgx:begin fn_1_15E220 */
extern void fn_1_4060(void);

void fn_1_15E220(u8 *value) {
    u8 state = *value;

    if (state == 0) {
        fn_1_4060();
    } else if (state != 0xff) {
        *value = state - 1;
    }
}
/* fzgx:end fn_1_15E220 */
