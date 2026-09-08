#include "types.h"

extern u32 fn_8008F7E4(void);

s32 gdev_cc_initinterrupts(void) {
    fn_8008F7E4();
    return 0;
}
