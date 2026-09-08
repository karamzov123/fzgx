#include "types.h"

extern u32 lbl_8015BD40[];
extern u32 strncpy(u32, u32, u32);

void fn_80006FE0(u32 arg0, u32 arg1) {
    strncpy(arg0, (u32)&lbl_8015BD40, arg1);
}
