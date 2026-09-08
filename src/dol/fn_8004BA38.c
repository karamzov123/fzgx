#include "types.h"


struct fn_8004BA38_Arg0 {
    u8 pad_0[0x88];
    u32 unk_88;
};

void fn_8004BA38(struct fn_8004BA38_Arg0 *arg0, u32 arg1) {
    arg0->unk_88 = arg1;
}
