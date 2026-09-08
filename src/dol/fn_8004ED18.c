#include "types.h"

extern u32 fn_8004ED3C(u32);

struct fn_8004ED18_Arg0 {
    u8 pad_0[0xC];
    u32 unk_C;
};

void fn_8004ED18(struct fn_8004ED18_Arg0 *arg0) {
    fn_8004ED3C(arg0->unk_C);
}
