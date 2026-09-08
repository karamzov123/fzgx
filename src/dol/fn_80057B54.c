#include "types.h"


struct fn_80057B54_Arg0 {
    u8 pad_0[0x8];
    u32 unk_8;
};

u32 fn_80057B54(struct fn_80057B54_Arg0 *arg0) {
    return arg0->unk_8;
}
