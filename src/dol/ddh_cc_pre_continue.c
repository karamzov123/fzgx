#include "types.h"

extern u32 fn_8008F450(void);

s32 ddh_cc_pre_continue(void) {
    fn_8008F450();
    return 0;
}
