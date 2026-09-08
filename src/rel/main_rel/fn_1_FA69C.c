#include "types.h"

extern u8 *lbl_1_bss_84420;

u8 fn_1_FA69C(s32 index) {
    if (index >= 0x29) {
        return 0;
    }
    return lbl_1_bss_84420[index + 0x5c];
}
