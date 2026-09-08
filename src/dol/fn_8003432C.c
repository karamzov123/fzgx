#include "types.h"


struct fn_8003432C_Arg2 {
    u32 unk_0;
};

u32 fn_8003432C(u32 arg0, u32 arg1, struct fn_8003432C_Arg2 *arg2) {
    u32 v0;
    v0 = ((((((arg0 & 0xFFFF) << 2) | 0xC8000000) & 0xFFC00FFF) | ((arg1 & 0xFFFF) << 12)) & 0xFF3FFFFF);
    arg2->unk_0 = *(u32 *)((u8 *)v0 + 0);
    return v0;
}
