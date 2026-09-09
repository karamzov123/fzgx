#include "types.h"

extern s32 lbl_80187370[];
extern u32 lbl_80187380[];
extern u32 memset(u32, u32, u32);

void fn_80053BB4(void) {
    if (--lbl_80187370[0] == 0) {
        memset((u32)&lbl_80187380, 0, 0x40);
    }
}
