#include "types.h"

extern struct fn_800209DC_lbl_8015D100 lbl_8015D100;

struct fn_800209DC_Arg0 {
    u32 unk_0;
    u8 pad_4[0x8];
    u32 unk_C;
};
struct fn_800209DC_lbl_8015D100 {
    u32 unk_0;
};

void fn_800209DC(struct fn_800209DC_Arg0 *arg0) {
    arg0->unk_0 = lbl_8015D100.unk_0;
    lbl_8015D100.unk_0 = (u32)arg0;
    arg0->unk_C = 0;
}
