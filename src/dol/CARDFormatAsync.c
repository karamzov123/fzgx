#include "types.h"

extern u32 __CARDFormatRegionAsync(u32, u32, u32);
extern u32 fn_8002A744(u32, u32);

u32 CARDFormatAsync(u32 arg0, u32 arg1) {
    u32 t0;
    t0 = fn_8002A744(arg0, arg1);
    return __CARDFormatRegionAsync(((arg0 + 0) + 0), (t0 + 0), ((arg1 + 0) + 0));
}
