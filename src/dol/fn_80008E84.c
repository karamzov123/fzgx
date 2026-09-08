#include "types.h"

extern u32 fn_8000951C(u32);
extern u32 fn_800095A4(void);
extern u32 lbl_801A6410;

u32 fn_80008E84(u32 arg0) {
    u32 v0;
    fn_8000951C(arg0);
    v0 = lbl_801A6410;
    lbl_801A6410 = arg0;
    fn_800095A4();
    return v0;
}
