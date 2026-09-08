#include "types.h"

extern u32 lbl_801A6744;

u32 fn_80009FA4(u32 arg0) {
    return *(u32 *)((u8 *)lbl_801A6744 + (arg0 * 12));
}
