#include "types.h"

extern u32 fn_800891B4(u32);

struct TRKDestructEvent_Arg0 {
    u8 pad_0[0x8];
    u32 unk_8;
};

void TRKDestructEvent(struct TRKDestructEvent_Arg0 *arg0) {
    fn_800891B4(arg0->unk_8);
}
