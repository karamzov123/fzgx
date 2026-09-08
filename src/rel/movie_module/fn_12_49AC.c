#include "types.h"

extern u32 fn_12_5EC(u32, u32);

struct fn_12_49AC_Arg0 {
    u8 pad_0[0x58];
    u32 unk_58;
};

void fn_12_49AC(struct fn_12_49AC_Arg0 *arg0) {
    fn_12_5EC(arg0->unk_58, *(u32 *)((u8 *)arg0 + 92));
}
