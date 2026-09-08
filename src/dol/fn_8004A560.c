#include "types.h"


struct fn_8004A560_Arg0 {
    u8 pad_0[0x14];
    u32 unk_14;
    u32 unk_18;
};
struct fn_8004A560_Arg1 {
    u32 unk_0;
};
struct fn_8004A560_Arg2 {
    u32 unk_0;
};

s32 fn_8004A560(struct fn_8004A560_Arg0 *arg0, struct fn_8004A560_Arg1 *arg1, struct fn_8004A560_Arg2 *arg2) {
    arg1->unk_0 = arg0->unk_18;
    arg2->unk_0 = arg0->unk_14;
    return 1;
}
