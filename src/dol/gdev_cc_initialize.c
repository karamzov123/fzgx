#include "types.h"

extern u32 MWTRACE(u32, ...);
extern u32 fn_8008E324(u32, u32, u32);
extern u32 fn_8008F838(u32, u32);
extern u32 lbl_80095E64[];
extern u32 lbl_80095E78[];
extern u32 lbl_801A5E78[];
extern u32 lbl_801A6378[];

s32 gdev_cc_initialize(u32 arg0, u32 arg1) {
    MWTRACE(1, (u32)&lbl_80095E64);
    fn_8008F838(arg0, arg1);
    MWTRACE(1, (u32)&lbl_80095E78);
    fn_8008E324((u32)&lbl_801A6378, (u32)&lbl_801A5E78, 1280);
    return 0;
}
