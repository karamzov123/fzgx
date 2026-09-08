#include "types.h"

extern u32 lbl_801A6D10;
extern u32 lbl_801A6D14;
extern u32 lbl_801A6D18;
extern u32 lbl_801A6D1C;
extern u32 lbl_801A6D20;
extern u32 lbl_801A6D24;

struct fn_80070D60_Arg0 {
    u32 unk_0;
    u32 unk_4;
    u32 unk_8;
    u32 unk_C;
    u32 unk_10;
    u32 unk_14;
};

void fn_80070D60(struct fn_80070D60_Arg0 *arg0) {
    lbl_801A6D24 = arg0->unk_0;
    lbl_801A6D20 = arg0->unk_4;
    lbl_801A6D1C = arg0->unk_8;
    lbl_801A6D18 = arg0->unk_C;
    lbl_801A6D14 = arg0->unk_10;
    lbl_801A6D10 = arg0->unk_14;
}
