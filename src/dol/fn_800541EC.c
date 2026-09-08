#include "types.h"

extern u32 fn_80051678(u32);

struct fn_800541EC_Arg0 {
    u32 unk_0;
};

void fn_800541EC(struct fn_800541EC_Arg0 *arg0) {
    fn_80051678(arg0->unk_0);
    *(u8 *)((u8 *)(u32)arg0 + 5) = 0;
}
