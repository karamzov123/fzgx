#include "types.h"

extern u32 fn_80045FA4(void);
extern u32 lbl_801798C0[];
extern u32 memset(u32, u32, u32);

void fn_800424E8(void) {
    fn_80045FA4();
    memset((u32)&lbl_801798C0, 0, 2496);
}
