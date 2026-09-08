#include "types.h"

extern struct fn_800658A8_lbl_801A6C80_T *lbl_801A6C80;

struct fn_800658A8_lbl_801A6C80_T {
    u8 pad_0[0x5B18];
    u32 unk_5B18;
};

void fn_800658A8(u32 arg0) {
    lbl_801A6C80->unk_5B18 = arg0;
}
