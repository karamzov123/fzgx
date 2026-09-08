#include "types.h"

extern u32 fn_8004E2B0(u32, u32, u32, u32);
extern u32 fn_8004E2DC(u32, u32, u32);

struct fn_80045468_Arg0 {
    u8 pad_0[0x8];
    u32 unk_8;
};

void fn_80045468(struct fn_80045468_Arg0 *arg0) {
    fn_8004E2DC(arg0->unk_8, ((u32)arg0 + 168), ((u32)arg0 + 172));
    fn_8004E2B0(*(u32 *)((u8 *)(u32)arg0 + 8), ((u32)arg0 + 162), ((u32)arg0 + 164), ((u32)arg0 + 166));
}
