#include "types.h"

struct fn_80020F1C_Arg0 {
    u32 unk_0;
};

extern u32 lbl_8015E880[];
extern u32 lbl_801A6AC4;
extern u32 lbl_801A6AE0;

u32 fn_80020F1C(struct fn_80020F1C_Arg0 *arg0) {
    if (lbl_801A6AC4 != 0) {
    arg0->unk_0 = (u32)((u8 *)&lbl_8015E880 + (lbl_801A6AE0 * 1920));
    return (u32)arg0;
    }
    arg0->unk_0 = 0;
}
