#include "types.h"

extern u32 RunQueueHint_801A67FC;
extern u32 SelectThread(u32);

void __OSReschedule(void) {
    if ((s32)RunQueueHint_801A67FC != 0) {
    SelectThread(0);
    }
}
