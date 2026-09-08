#include "types.h"

extern struct TRKTargetStopped_gTRKState gTRKState;

struct TRKTargetStopped_gTRKState {
    u8 pad_0[0x98];
    u32 unk_98;
};

u32 TRKTargetStopped(void) {
    return gTRKState.unk_98;
}
