#include "types.h"

extern u32 SIDecodeType(u32);
extern u32 SIGetType(void);

void SIProbe(void) {
    u32 t0;
    t0 = SIGetType();
    SIDecodeType(t0);
}
