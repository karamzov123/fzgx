#include "types.h"

extern u32 fn_80058EF4(u32, u32);
extern u32 lbl_8017B030[];
extern u32 lbl_8017B034[];

void fn_80047548(u32 arg0, u32 arg1) {
    lbl_8017B030[0] = arg0;
    lbl_8017B034[0] = arg1;
    fn_80058EF4(arg0, arg1);
}
