#include "types.h"

extern struct fn_12_330E0_lbl_12_bss_1B75C lbl_12_bss_1B75C;

struct fn_12_330E0_Arg0 {
    u32 unk_0;
    u32 unk_4;
    u32 unk_8;
    u32 unk_C;
};
struct fn_12_330E0_lbl_12_bss_1B75C {
    u8 pad_0[0x4];
    u32 unk_4;
};

u32 fn_12_330E0(struct fn_12_330E0_Arg0 *arg0) {
    u32 v0;
    arg0->unk_0 = 0;
    arg0->unk_4 = 0;
    arg0->unk_8 = 0;
    arg0->unk_C = 0;
    v0 = lbl_12_bss_1B75C.unk_4;
    lbl_12_bss_1B75C.unk_4 = (v0 - 1);
    return v0;
}
