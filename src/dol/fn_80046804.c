#include "types.h"

struct fn_80046804_Arg2 {
    u16 unk_0;
};


s32 fn_80046804(u32 arg0, u32 arg1, struct fn_80046804_Arg2 *arg2) {
    u32 v0;
    s16 v1;
    v0 = arg0;
    if ((s32)arg1 < 16) {
    v0 = -1;
    return v0;
    }
    if (*(u16 *)((u8 *)v0 + 0) != 32769) {
    v0 = -2;
    return v0;
    }
    v1 = *(s16 *)((u8 *)v0 + 2);
    v0 = 0;
    arg2->unk_0 = (v1 + 4);
    return v0;
}
