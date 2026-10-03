#include "types.h"

extern u8 lbl_80095A6C[];
extern u8 lbl_80095A74[];
extern void MWTRACE(u32, ...);

void fn_8008A764(u32 arg0, u32 arg1) {
    s32 v0;
    u8 *v1;
    v0 = 0;
    v1 = (u8 *)arg0;
    while (v0 < (s32)arg1) {
        MWTRACE(8, (lbl_80095A6C), v1[v0]);
        if (v0 % 16 == 15) {
            MWTRACE(8, (lbl_80095A74));
        }
        v0++;
    }
    MWTRACE(8, lbl_80095A74);
}
