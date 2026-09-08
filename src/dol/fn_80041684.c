#include "types.h"

extern u32 fn_80045594(u32);

struct fn_80041684_Arg0 {
    u8 pad_0[0x4];
    u32 unk_4;
};

void fn_80041684(struct fn_80041684_Arg0 *arg0) {
    fn_80045594(arg0->unk_4);
}
