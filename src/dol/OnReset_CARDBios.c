#include "types.h"

extern u32 CARDUnmount(u32);

s32 OnReset_CARDBios(u32 arg0) {
    u32 v0;
    if ((s32)arg0 == 0 && ((s32)CARDUnmount(0) == -1 || (s32)CARDUnmount(1) == -1)) {
    v0 = 0;
    } else {
    v0 = 1;
    }
    return v0;
}
