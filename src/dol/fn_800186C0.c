#include "types.h"

extern u32 DVDLowWaitCoverClose(u32);
extern void cbForStateMotorStopped_800186E8(void);

void fn_800186C0(void) {
    DVDLowWaitCoverClose((u32)cbForStateMotorStopped_800186E8);
}
