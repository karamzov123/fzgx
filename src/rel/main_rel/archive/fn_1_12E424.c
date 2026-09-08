#include "types.h"

extern u32 lbl_1_bss_8B3A0[83];
extern u32 fn_1_95120(void *arg);

u8 fn_1_12E424(void *arg0, u8 *arg1) {
    if (lbl_1_bss_8B3A0[37] & (1u << 31)) {
        return 1;
    }
    return (u8)fn_1_95120(arg1 + 0x148);
}
