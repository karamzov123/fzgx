#include "types.h"
#include "rel/main_rel/globals.h"

extern f32 lbl_1_data_1AEA8[616];

void fn_1_4955C(f32 value1, f32 value2) {
    s16 index = *(s16 *)&lbl_1_bss_4B9CC.unk_C;
    f32 scale = ((f32 *)lbl_1_data_1AEA8)[index * 14 + 13];

    // Scale both font dimensions using the active font's metrics.
    lbl_1_bss_4B9CC.unk_1C = value1 * scale;
    lbl_1_bss_4B9CC.unk_20 = value2 * scale;
}
