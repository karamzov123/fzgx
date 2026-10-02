#include "types.h"

extern u32 lbl_8015BE40[40];
extern u32 lbl_801A6730;

void fn_80009468(void) {
    s32 i;

    for (i = 0; i < 40; i += 5) {
        lbl_8015BE40[i] = -1;
        lbl_8015BE40[i + 1] = 0;
        lbl_8015BE40[i + 2] = 0;
        lbl_8015BE40[i + 3] = 0;
        lbl_8015BE40[i + 4] = 0;
    }
    lbl_801A6730 = -1;
}
