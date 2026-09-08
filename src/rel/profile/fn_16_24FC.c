#include "types.h"

extern struct fn_16_24FC_lbl_1_bss_8B614 lbl_1_bss_8B614;
extern struct fn_16_24FC_lbl_1_bss_9F8 lbl_1_bss_9F8;

struct fn_16_24FC_lbl_1_bss_9F8_0_E20 {
    u8 pad_0[0x8];
    u16 unk_8;
    u8 pad_A[0xA];
};
struct fn_16_24FC_lbl_1_bss_8B614 {
    u8 pad_0[0x8];
    s16 unk_8;
};
struct fn_16_24FC_lbl_1_bss_9F8 {
    struct fn_16_24FC_lbl_1_bss_9F8_0_E20 unk_0[1];
};

s32 fn_16_24FC(void) {
    return (-((lbl_1_bss_9F8.unk_0[lbl_1_bss_8B614.unk_8].unk_8 >> 9) & 0x1));
}
