#include "types.h"

extern struct fn_8002C0A0_lbl_80177960 lbl_80177960;

struct fn_8002C0A0_lbl_80177960_0_E272 {
    u8 pad_0[0xB8];
    u32 unk_B8;
    u8 pad_BC[0x54];
};
struct fn_8002C0A0_lbl_80177960 {
    struct fn_8002C0A0_lbl_80177960_0_E272 unk_0[1];
};

u32 fn_8002C0A0(u32 arg0) {
    return lbl_80177960.unk_0[arg0].unk_B8;
}
