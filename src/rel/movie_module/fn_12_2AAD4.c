#include "types.h"


struct fn_12_2AAD4_Arg0 {
    u32 unk_0;
};

u32 fn_12_2AAD4(struct fn_12_2AAD4_Arg0 *arg0) {
    if ((u32)arg0 == 0) { return (u32)arg0; }
    if ((s32)arg0->unk_0 == 4) {
    arg0->unk_0 = 2;
    return (u32)arg0;
    }
    arg0->unk_0 = 0;
}
