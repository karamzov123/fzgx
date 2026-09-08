#include "types.h"

extern u32 gTRKState[41];

void TRKTargetSetInputPendingPtr(void* ptr) {
    gTRKState[40] = (u32)ptr;
}
