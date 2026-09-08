#include "types.h"


struct fn_80045504_Arg0 {
    u8 pad_0[0x24];
    s16 unk_24;
};

s32 fn_80045504(struct fn_80045504_Arg0 *arg0) {
    return arg0->unk_24;
}
