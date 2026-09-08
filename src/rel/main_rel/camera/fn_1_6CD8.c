#include "rel/main_rel/globals.h"

extern void fn_1_6D2C(u32);

// Pass the camera state status to the next camera update stage.
void fn_1_6CD8(void) {
    u32 value;

    if (lbl_1_bss_F68 == 0) {
        value = 0;
    } else if ((lbl_1_bss_F68->unk_0 & ((u32)1 << 31)) != 0) {
        value = 0;
    } else {
        value = lbl_1_bss_F68->unk_4A;
    }

    fn_1_6D2C(value);
}
