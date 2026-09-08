#include "types.h"

extern u32 lbl_801A66E4;

u32 fn_80006904(u32 arg0) {
    u32 v0;
    v0 = lbl_801A66E4;
    lbl_801A66E4 = arg0;
    return v0;
}
