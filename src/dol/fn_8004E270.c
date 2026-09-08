#include "types.h"


struct fn_8004E270_Arg0 {
    u8 pad_0[0xC];
    u32 unk_C;
};

u32 fn_8004E270(struct fn_8004E270_Arg0 *arg0) {
    return arg0->unk_C;
}
