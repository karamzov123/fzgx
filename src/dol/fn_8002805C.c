#include "types.h"

extern f32 lbl_801A7000;
extern f32 lbl_801A7004;
extern u32 fn_80028B2C(f32);
extern u32 fn_80028B34(f32);
extern u32 lbl_801A6B90;

void fn_8002805C(void) {
    lbl_801A6B90 = 0;
    fn_80028B34(lbl_801A7000);
    fn_80028B2C(lbl_801A7004);
}
