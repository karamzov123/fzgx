#include "types.h"

struct fn_80088B00_Arg0 {
    u8 pad_0[0x8];
    u32 unk_8;
};

extern u32 MWTRACE(u32, ...);
extern u32 fn_8008D398(u32, u32);
extern u32 lbl_80095678[];

s32 fn_80088B00(struct fn_80088B00_Arg0 *arg0) {
    u32 t0;
    t0 = fn_8008D398(((u32)arg0 + 16), arg0->unk_8);
    MWTRACE(1, (u32)&lbl_80095678, t0);
    return 0;
}
