#include "types.h"

extern u32 fn_8008023C(u32, u32, u32);
extern u32 lbl_80090070[];
extern u32 lbl_80090078[];

s32 fn_80042568(u32 arg0) {
    u32 v0;
    if ((s32)fn_8008023C(arg0, (u32)&lbl_80090070, 4) == 0 && (s32)fn_8008023C((arg0 + 8), (u32)&lbl_80090078, 4) == 0) {
    v0 = 1;
    } else {
    v0 = 0;
    }
    return v0;
}
