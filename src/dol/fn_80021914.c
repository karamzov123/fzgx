#include "types.h"

extern u32 lbl_801A6B08;

void fn_80021914(u32 arg0) {
    if (lbl_801A6B08 == arg0) { return; }
    lbl_801A6B08 = arg0;
}
