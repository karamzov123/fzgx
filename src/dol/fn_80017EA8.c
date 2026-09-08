#include "types.h"

extern u32 DVDLowStopMotor(u32);
extern void fn_80017ED0(void);

u32 fn_80017EA8(void) {
    return DVDLowStopMotor((u32)fn_80017ED0);
}
