#include "types.h"

extern u32 MWTRACE(u32, ...);
extern u32 fn_8008E324(u32, u32, u32);
extern u32 fn_8008EDF0(u32, u32);
extern u32 lbl_80095D84[];
extern u32 lbl_80095D98[];
extern u32 lbl_801A5658[];
extern u32 lbl_801A5E58[];

s32 ddh_cc_initialize(u32 arg0, u32 arg1) {
    MWTRACE(1, (u32)&lbl_80095D84);
    fn_8008EDF0(arg0, arg1);
    MWTRACE(1, (u32)&lbl_80095D98);
    fn_8008E324((u32)&lbl_801A5E58, (u32)&lbl_801A5658, 2048);
    return 0;
}
