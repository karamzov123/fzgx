#include "types.h"

extern u32 fn_8004541C(u32);

struct fn_80041410_Arg0 {
    u8 pad_0[0x4];
    u32 unk_4;
};

void fn_80041410(struct fn_80041410_Arg0 *arg0) {
    fn_8004541C(arg0->unk_4);
}
