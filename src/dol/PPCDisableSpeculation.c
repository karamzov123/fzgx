#include "types.h"

extern u32 PPCMfhid0(void);
extern u32 fn_80009FD4(u32);

u32 PPCDisableSpeculation(void) {
    u32 t0;
    t0 = PPCMfhid0();
    return fn_80009FD4((t0 | 512));
}
