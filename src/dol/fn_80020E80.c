#include "types.h"

extern u32 lbl_801A6AE0;
extern u32 lbl_8015E880[1440];

void fn_80020E80(u32 *out) {
    *out = (u32)((u8 *)lbl_8015E880 + lbl_801A6AE0 * 0x780 + 0x500);
}
