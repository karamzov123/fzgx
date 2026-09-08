#include "types.h"

extern u32 lbl_8017D708[];
extern u32 memset(u32, u32, u32);

s32 fn_8004B7A0(void) {
    memset((u32)&lbl_8017D708, 0, 3680);
    return 1;
}
