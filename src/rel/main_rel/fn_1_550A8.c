#include "types.h"

extern u8 lbl_1_bss_6C710[136];
extern void *lbl_801A66CC;
extern void fn_800794F0(u8 *data, void *value, s32 size);

void fn_1_550A8(void) {
    fn_800794F0(lbl_1_bss_6C710, (u8 *)lbl_801A66CC + 0x50, 0x84);
}
