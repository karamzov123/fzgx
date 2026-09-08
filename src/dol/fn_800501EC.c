#include "types.h"


struct fn_800501EC_Arg0 {
    u8 pad_0[0x10];
    u32 unk_10;
};

u32 fn_800501EC(struct fn_800501EC_Arg0 *arg0) {
    return arg0->unk_10;
}
