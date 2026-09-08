#include "types.h"


struct fn_80011358_Arg0 {
    u8 pad_0[0x2D4];
    u32 unk_2D4;
};

u32 fn_80011358(struct fn_80011358_Arg0 *arg0) {
    return arg0->unk_2D4;
}
