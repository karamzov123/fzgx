#include "types.h"


struct fn_80071CE0_Arg0 {
    u8 pad_0[0x34];
    u32 unk_34;
};

u32 fn_80071CE0(struct fn_80071CE0_Arg0 *arg0) {
    return arg0->unk_34;
}
