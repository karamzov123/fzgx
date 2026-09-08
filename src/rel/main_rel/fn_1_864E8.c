#include "types.h"

extern u8 *lbl_1_bss_6D838;

u32 fn_1_864E8(int index) {
    return *(u32 *)(lbl_1_bss_6D838 + index * 0x620);
}
