#include "types.h"


struct fn_80036104_Arg0 {
    u8 pad_0[0x14];
    u32 unk_14;
};

u32 fn_80036104(struct fn_80036104_Arg0 *arg0) {
    return arg0->unk_14;
}
