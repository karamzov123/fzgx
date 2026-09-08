#include "types.h"

extern u32 fn_80011194(u32);
extern u32 lbl_801A6B28;
extern u32 lbl_801A6B30;

void fn_80021B84(void) {
    lbl_801A6B28 = 1;
    fn_80011194((u32)&lbl_801A6B30);
}
