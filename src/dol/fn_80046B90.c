#include "types.h"

struct fn_80046B90_Arg2 {
    u8 unk_0;
};
struct fn_80046B90_Arg3 {
    u8 unk_0;
};


s32 fn_80046B90(u32 arg0, u32 arg1, struct fn_80046B90_Arg2 *arg2, struct fn_80046B90_Arg3 *arg3) {
    u32 v0;
    u8 v1;
    v0 = arg0;
    if ((s32)arg1 < 20) {
    v0 = -1;
    return v0;
    }
    if (*(u16 *)((u8 *)v0 + 0) != 32768) {
    v0 = -2;
    return v0;
    }
    if (*(s16 *)((u8 *)v0 + 2) < 16) {
    v0 = -1;
    return v0;
    }
    arg2->unk_0 = *(u8 *)((u8 *)v0 + 18);
    v1 = *(u8 *)((u8 *)v0 + 19);
    v0 = 0;
    arg3->unk_0 = v1;
    return v0;
}
