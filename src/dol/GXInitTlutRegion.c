#include "types.h"


struct GXInitTlutRegion_Arg0 {
    u32 unk_0;
};

void GXInitTlutRegion(struct GXInitTlutRegion_Arg0 *arg0, u32 arg1, u32 arg2) {
    arg0->unk_0 = 0;
    arg0->unk_0 = ((arg0->unk_0 & ~0x3FF) | ((u32)(arg1 - 0x80000) >> 9));
    arg0->unk_0 = ((arg0->unk_0 & 0xFFE003FF) | (arg2 << 10));
    arg0->unk_0 = ((arg0->unk_0 & 0xFFFFFF) | 0x65000000);
}
