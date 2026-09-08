#include "types.h"


struct fn_8004E2DC_Arg0 {
    u8 pad_0[0x28];
    s16 unk_28;
    s16 unk_2A;
    s16 unk_2C;
    s16 unk_2E;
};
struct fn_8004E2DC_Arg1 {
    u16 unk_0;
    u16 unk_2;
};
struct fn_8004E2DC_Arg2 {
    u16 unk_0;
    u16 unk_2;
};

void fn_8004E2DC(struct fn_8004E2DC_Arg0 *arg0, struct fn_8004E2DC_Arg1 *arg1, struct fn_8004E2DC_Arg2 *arg2) {
    arg1->unk_0 = arg0->unk_28;
    arg2->unk_0 = arg0->unk_2A;
    arg1->unk_2 = arg0->unk_2C;
    arg2->unk_2 = arg0->unk_2E;
}
