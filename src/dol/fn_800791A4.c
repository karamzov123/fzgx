#include "types.h"

extern u32 fn_800702E4(u32);
extern u32 fn_8007048C(void);
extern u32 lbl_801A6DB0;
extern u32 lbl_801A6DB8;
extern u8 lbl_801A6DB4;

void fn_800791A4(void) {
    u32 t0, t1;
    t0 = fn_8007048C();
    lbl_801A6DB4 = t0;
    t1 = fn_800702E4((t0 & 0xFF));
    lbl_801A6DB0 = t1;
    if (lbl_801A6DB4 != 0) {
    lbl_801A6DB8 = 1;
    }
}
