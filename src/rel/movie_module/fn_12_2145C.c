#include "types.h"

extern struct fn_12_2145C_lbl_12_bss_6990 lbl_12_bss_6990;

struct fn_12_2145C_Arg0 {
    u32 unk_0;
    u32 unk_4;
    u32 unk_8;
    u32 unk_C;
    u32 unk_10;
    u32 unk_14;
    u32 unk_18;
};
struct fn_12_2145C_lbl_12_bss_6990 {
    u32 unk_0;
    u32 unk_4;
    u32 unk_8;
    u32 unk_C;
    u32 unk_10;
    u32 unk_14;
    u32 unk_18;
};

u32 fn_12_2145C(struct fn_12_2145C_Arg0 *arg0) {
    lbl_12_bss_6990.unk_0 = arg0->unk_0;
    lbl_12_bss_6990.unk_4 = arg0->unk_4;
    lbl_12_bss_6990.unk_8 = ((arg0->unk_8 + 31) & ~0x1F);
    lbl_12_bss_6990.unk_C = arg0->unk_C;
    lbl_12_bss_6990.unk_10 = arg0->unk_10;
    lbl_12_bss_6990.unk_14 = arg0->unk_14;
    lbl_12_bss_6990.unk_18 = ((arg0->unk_18 + 31) & ~0x1F);
    return arg0->unk_18;
}
