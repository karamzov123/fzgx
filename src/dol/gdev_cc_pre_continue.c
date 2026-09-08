#include "types.h"

extern u32 fn_8008F454(void);

s32 gdev_cc_pre_continue(void) {
    fn_8008F454();
    return 0;
}
