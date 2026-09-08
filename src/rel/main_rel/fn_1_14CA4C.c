#include "types.h"

extern u8 lbl_1_bss_8E518[5];
extern u8 lbl_1_bss_3C30[5308];
extern void fn_1_A2D84(u32, u8 *);

void fn_1_14CA4C(void) {
    u8 *state = lbl_1_bss_8E518;
    if (state[4] == 0) {
        u8 limit;

        state[4] = 1;
        if (state[5] == 4) {
            limit = lbl_1_bss_3C30[8];
        } else {
            limit = lbl_1_bss_3C30[9];
        }
        if (limit > state[0x30]) {
            fn_1_A2D84(0xA9010400, state);
        }
    }
}
