#include "types.h"

extern struct TRKTargetSetStopped_gTRKState gTRKState;

struct TRKTargetSetStopped_gTRKState {
    u8 pad_0[0x98];
    u32 unk_98;
};

void TRKTargetSetStopped(u32 arg0) {
    gTRKState.unk_98 = arg0;
}
