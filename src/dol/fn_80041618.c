#include "types.h"

extern u32 fn_80045514(u32);

struct fn_80041618_Arg0 {
    u8 pad_0[0x4];
    u32 unk_4;
};

void fn_80041618(struct fn_80041618_Arg0 *arg0) {
    fn_80045514(arg0->unk_4);
}
