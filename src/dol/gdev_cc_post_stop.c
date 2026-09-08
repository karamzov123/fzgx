#include "types.h"

extern u32 fn_8008F458(void);

s32 gdev_cc_post_stop(void) {
    fn_8008F458();
    return 0;
}
