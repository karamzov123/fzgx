#include "types.h"

extern struct fn_80065890_lbl_801A6C80_T *lbl_801A6C80;

struct fn_80065890_lbl_801A6C80_T {
    u8 pad_0[0x5B20];
    u32 unk_5B20;
};

void fn_80065890(u32 arg0) {
    lbl_801A6C80->unk_5B20 = arg0;
}
