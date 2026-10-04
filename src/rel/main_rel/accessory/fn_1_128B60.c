#include "types.h"
#include "rel/main_rel/game.h"
extern void _savegpr_27(void);
extern void _restgpr_27(void);
extern u32 lbl_801A63C0;

static inline s32 next_rand(void) {
    lbl_801A63C0 = lbl_801A63C0 * 0x676A4B6B + 0x33cb;
    return (lbl_801A63C0 >> 16) & 0x7fff;
}

void fn_1_128B60(u8 *a0, u8 *a1, u8 *a2, u8 *a3, u8 *a4, u8 *a5, u8 *a6, u8 *a7, u8 *a8, u8 *a9) {
    *a0 = next_rand() % 10;
    *a1 = (*a0 + 1) % 10;
    *a2 = (*a0 + 2) % 10;
    *a3 = next_rand() % 10;
    *a4 = (*a3 + 1) % 10;
    *a5 = (*a3 + 2) % 10;
    *a6 = next_rand() % 10;
    *a7 = (*a6 + 1) % 10;
    *a8 = (*a6 + 2) % 10;
    lbl_801A63C0 = lbl_801A63C0 * 0x676A4B6B + 0x33cb;
    if ((lbl_801A63C0 >> 16) & 1) {
        *a2 = 15;
    }
    lbl_801A63C0 = lbl_801A63C0 * 0x676A4B6B + 0x33cb;
    if ((lbl_801A63C0 >> 16) & 1) {
        *a5 = 15;
    }
    lbl_801A63C0 = lbl_801A63C0 * 0x676A4B6B + 0x33cb;
    if ((lbl_801A63C0 >> 16) & 1) {
        *a8 = 15;
    }
    *a9 = next_rand() % 20;
}
