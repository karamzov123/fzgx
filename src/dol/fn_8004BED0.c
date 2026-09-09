#include "types.h"

struct fn_8004BED0_AdxtObj {
    u8 pad_0[0x40];
    s16 unk_40;
};

extern u32 fn_800474E4(u32);
extern u8 lbl_80090DCC[];

s32 fn_8004BED0(struct fn_8004BED0_AdxtObj* p) {
    if (p == 0) {
        fn_800474E4((u32)&lbl_80090DCC);
        return 0;
    }
    return p->unk_40;
}
