#include "types.h"


struct fn_12_7E8_Arg0 {
    u8 pad_0[0x10];
    u32 unk_10;
    u32 unk_14;
    u32 unk_18;
};
struct fn_12_7E8_Arg1 {
    u32 unk_0;
};
struct fn_12_7E8_Arg2 {
    u32 unk_0;
};

u32 fn_12_7E8(struct fn_12_7E8_Arg0 *arg0, struct fn_12_7E8_Arg1 *arg1, struct fn_12_7E8_Arg2 *arg2) {
    if ((s32)arg0->unk_10 != 1) {
    arg1->unk_0 = 0;
    arg2->unk_0 = 0;
    return (u32)arg0;
    }
    arg1->unk_0 = arg0->unk_14;
    arg2->unk_0 = arg0->unk_18;
}
