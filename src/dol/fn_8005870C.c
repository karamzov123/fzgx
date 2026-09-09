#include "types.h"

extern s32 lbl_8018F2A8[];
extern u32 lbl_8018F2AC[];
extern u32 memset(u32, u32, u32);

void fn_8005870C(void) {
    if (--lbl_8018F2A8[0] == 0) {
        memset((u32)&lbl_8018F2AC, 0, 0xC00);
    }
}
