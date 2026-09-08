#include "types.h"


struct fn_800356AC_Arg0 {
    u8 pad_0[0xC];
    u32 unk_C;
};
struct fn_800356AC_Arg1 {
    u32 unk_0;
};

void fn_800356AC(struct fn_800356AC_Arg0 *arg0, struct fn_800356AC_Arg1 *arg1) {
    arg0->unk_C = arg1->unk_0;
}
