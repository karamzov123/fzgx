#include "types.h"

extern u32 lbl_801A66E8;

u32 fn_800068F4(u32 value) {
    u32 old_value = lbl_801A66E8;
    lbl_801A66E8 = value;
    return old_value;
}
