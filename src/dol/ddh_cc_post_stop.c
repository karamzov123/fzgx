#include "types.h"

extern u32 fn_8008F44C(void);

s32 ddh_cc_post_stop(void) {
    fn_8008F44C();
    return 0;
}
