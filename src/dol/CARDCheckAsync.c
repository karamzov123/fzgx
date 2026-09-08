#include "types.h"

extern u32 CARDCheckExAsync(u32, void *, u32);

void CARDCheckAsync(u32 arg0, u32 arg1) {
    f64 loc_10;
    /* frame */
    CARDCheckExAsync(arg0, &loc_10, (arg1 + 0));
}
