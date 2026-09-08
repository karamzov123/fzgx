#include "types.h"


struct fn_80045F58_Arg0 {
    u8 pad_0[0x88];
    u32 unk_88;
    u32 unk_8C;
};

void fn_80045F58(struct fn_80045F58_Arg0 *arg0, u32 arg1, u32 arg2) {
    arg0->unk_8C = (arg0->unk_8C + arg2);
    arg0->unk_88 = (arg0->unk_88 + arg2);
}
