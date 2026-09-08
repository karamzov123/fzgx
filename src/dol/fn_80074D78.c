#include "types.h"

extern u32 lbl_801A6D58;

void fn_80074D78(u32 arg0) {
    lbl_801A6D58 = ((u32)__cntlzw(arg0) >> 5);
}
