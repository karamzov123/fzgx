#include "types.h"

extern u32 fn_80006BDC(u32, ...);
extern u32 fn_80017160(u32, u32);
extern u32 lbl_801220C0[];
extern u32 lbl_801220CC[];

void fn_80006B70(u32 arg0, u32 arg1) {
    fn_80006BDC((u32)&lbl_801220C0, arg0);
    fn_80006BDC((u32)&lbl_801220CC);
    *(u32 *)((u8 *)arg1 + 0) = 0;
    fn_80017160(arg0, (arg1 + 4));
}
