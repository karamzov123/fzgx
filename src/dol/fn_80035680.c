#include "types.h"


struct fn_80035680_Arg1 {
    u8 pad_0[0x28];
    f32 unk_28;
    f32 unk_2C;
    f32 unk_30;
};

void fn_80035680(f32 arg0, struct fn_80035680_Arg1 *arg1, f32 arg2, f32 arg3) {
    arg1->unk_28 = arg0;
    arg1->unk_2C = arg2;
    arg1->unk_30 = arg3;
}
