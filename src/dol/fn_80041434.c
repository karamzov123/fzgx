#include "types.h"

extern u32 fn_80045468(u32);

struct fn_80041434_Arg0 {
    u8 pad_0[0x4];
    u32 unk_4;
};

void fn_80041434(struct fn_80041434_Arg0 *arg0) {
    fn_80045468(arg0->unk_4);
}
