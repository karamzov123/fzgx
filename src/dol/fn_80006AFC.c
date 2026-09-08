#include "types.h"

extern u32 fn_80006B30(u32);
extern u32 fn_80083DB0(u32, u32);
extern u32 lbl_8015BD40[];
extern u32 lbl_801A63F8;

void fn_80006AFC(void) {
    fn_80083DB0((u32)&lbl_8015BD40, (u32)&lbl_801A63F8);
    fn_80006B30(0);
}
