#include "types.h"

struct fn_80020EA0_Arg0 {
    u32 unk_0;
};

extern u32 lbl_8015D200[];
extern u32 lbl_801A6AE4;

void fn_80020EA0(struct fn_80020EA0_Arg0 *arg0) {
    arg0->unk_0 = (u32)((u8 *)&lbl_8015D200 + (lbl_801A6AE4 * 1920));
}
