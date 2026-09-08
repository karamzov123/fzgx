#include "types.h"

extern u32 fn_80019B78(void);

s32 DVDClose(void) {
    fn_80019B78();
    return 1;
}
