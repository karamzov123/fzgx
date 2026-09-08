#include "types.h"


struct fn_80071470_E16_u32 { u32 unk_0; u8 pad_4[0xC]; };
struct fn_80071470_Arg0 {
    u8 pad_0[0x8];
    struct fn_80071470_E16_u32 unk_8[1];
};

u32 fn_80071470(struct fn_80071470_Arg0 *arg0, u32 arg1) {
    return ((u32)arg0 + arg0->unk_8[arg1].unk_0);
}
