#include "types.h"

extern u32 fn_8008EEAC(void);

s32 ddh_cc_initinterrupts(void) {
    fn_8008EEAC();
    return 0;
}
