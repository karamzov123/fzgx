#include "types.h"

extern int Reschedule_801A6800;
extern int OSDisableInterrupts(void);
extern void OSRestoreInterrupts(int level);

int OSEnableScheduler(void) {
    int level;
    int reschedule;

    level = OSDisableInterrupts();
    reschedule = Reschedule_801A6800;
    Reschedule_801A6800 = reschedule - 1;
    OSRestoreInterrupts(level);
    return reschedule;
}
