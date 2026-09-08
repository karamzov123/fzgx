#include "types.h"


struct fn_80047ADC_Arg0 {
    u8 pad_0[0x3C];
    u32 unk_3C;
    u8 pad_40[0x8];
    u32 unk_48;
    u8 pad_4C[0xC];
    u32 unk_58;
    u32 unk_5C;
    u32 unk_60;
};

void fn_80047ADC(struct fn_80047ADC_Arg0 *arg0, u32 arg1, u32 arg2, u32 arg3) {
    arg0->unk_58 = arg1;
    arg0->unk_5C = arg2;
    arg0->unk_3C = arg3;
    arg0->unk_60 = arg3;
    arg0->unk_48 = 284;
}
