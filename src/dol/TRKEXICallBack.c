#include "types.h"

extern u32 OSEnableScheduler(u32, u32);
extern u32 TRKLoadContext(u32, u32);

void TRKEXICallBack(u32 arg0, u32 arg1) {
    OSEnableScheduler(arg0, arg1);
    TRKLoadContext(arg1, 1280);
}
