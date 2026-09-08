#include "types.h"

struct fn_800775EC_Arg0 {
    u8 pad_0[0xC];
    u8 unk_C;
    u8 unk_D;
    u8 unk_E;
};
struct fn_800775EC_lbl_801A3220 {
    u8 pad_0[0x10];
    u8 unk_10;
    u8 unk_11;
    u8 unk_12;
    u8 pad_13[0x35];
    u32 unk_48;
    u32 unk_4C;
};

extern struct fn_800775EC_lbl_801A3220 lbl_801A3220[];

void fn_800775EC(struct fn_800775EC_Arg0 *arg0) {
    u8 v0;
    v0 = lbl_801A3220[0].unk_11;
    if (lbl_801A3220[0].unk_10 == arg0->unk_C && lbl_801A3220[0].unk_11 == arg0->unk_D) {
    v0 = lbl_801A3220[0].unk_12;
    if (v0 == arg0->unk_E) { return; }
    }
    lbl_801A3220[0].unk_48 = 0;
    lbl_801A3220[0].unk_4C = 0;
    lbl_801A3220[0].unk_10 = arg0->unk_C;
    lbl_801A3220[0].unk_11 = arg0->unk_D;
    lbl_801A3220[0].unk_12 = arg0->unk_E;
}
