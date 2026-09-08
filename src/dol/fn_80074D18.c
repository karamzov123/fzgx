#include "types.h"

extern u32 lbl_801A6D64;

void fn_80074D18(u32 arg0) {
    lbl_801A6D64 = ((u32)__cntlzw(arg0) >> 5);
}
