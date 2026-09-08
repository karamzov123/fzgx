#include "types.h"

extern u32 SIRegisterPollingHandler(u32);
extern u32 SIUnregisterPollingHandler(u32);
extern u32 lbl_801A6994;
extern void fn_8001DDD0(void);

u32 fn_8001DE30(u32 arg0) {
    u32 v0;
    v0 = lbl_801A6994;
    lbl_801A6994 = arg0;
    if (arg0 != 0) {
    SIRegisterPollingHandler((u32)fn_8001DDD0);
    } else {
    SIUnregisterPollingHandler((u32)fn_8001DDD0);
    }
    return v0;
}
