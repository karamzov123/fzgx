#include "types.h"

struct fn_80020F6C_Arg0 {
    u32 unk_0;
};

extern u32 lbl_8015E880[];
extern u32 lbl_801A6AE8;

void fn_80020F6C(struct fn_80020F6C_Arg0 *arg0) {
    arg0->unk_0 = (u32)((u8 *)&lbl_8015E880 + (lbl_801A6AE8 * 1920));
}
