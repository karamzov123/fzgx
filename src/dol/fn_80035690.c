#include "types.h"


struct fn_80035690_Arg3 {
    u8 pad_0[0x34];
    f32 unk_34;
    f32 unk_38;
    f32 unk_3C;
};

f32 fn_80035690(f32 arg0, f32 arg1, f32 arg2, struct fn_80035690_Arg3 *arg3) {
    arg3->unk_34 = (-arg0);
    arg3->unk_38 = (-arg1);
    arg3->unk_3C = (-arg2);
    return (-arg1);
}
