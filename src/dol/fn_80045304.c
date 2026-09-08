#include "types.h"


struct fn_80045304_Arg0 {
    u8 pad_0[0x94];
    u32 unk_94;
};

u32 fn_80045304(struct fn_80045304_Arg0 *arg0) {
    return arg0->unk_94;
}
