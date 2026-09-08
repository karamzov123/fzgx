#include "types.h"

extern u32 ReserveEXI2Port(void);
extern u32 TRKSwapAndGo(void);
extern u32 TRKTargetSetStopped(u32);
extern u32 UnreserveEXI2Port(void);

s32 TRKTargetContinue(void) {
    TRKTargetSetStopped(0);
    UnreserveEXI2Port();
    TRKSwapAndGo();
    ReserveEXI2Port();
    return 0;
}
