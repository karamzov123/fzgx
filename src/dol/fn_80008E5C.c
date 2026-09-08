#include "types.h"

extern u32 fn_80009AA8(u32, u32, u32, u32);
extern u32 lbl_801A6414;

void fn_80008E5C(u32 arg0, u32 arg1) {
    fn_80009AA8(arg0, arg1, (u32)&lbl_801A6414, 0);
}
