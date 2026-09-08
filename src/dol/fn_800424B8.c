#include "types.h"

extern u32 lbl_801798C0[];
extern u32 memset(u32, u32, u32);

void fn_800424B8(void) {
    memset((u32)&lbl_801798C0, 0, 2496);
}
