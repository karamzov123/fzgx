#include "types.h"

extern struct TRKInitializeTarget_gTRKState gTRKState;
extern u32 fn_8008B0E0(void);
extern u32 lbl_801A5638[];

struct TRKInitializeTarget_gTRKState {
    u8 pad_0[0x8C];
    u32 unk_8C;
    u8 pad_90[0x8];
    u32 unk_98;
};

s32 TRKInitializeTarget(void) {
    u32 t0;
    gTRKState.unk_98 = 1;
    t0 = fn_8008B0E0();
    gTRKState.unk_8C = t0;
    lbl_801A5638[0] = 0xE0000000;
    return 0;
}
