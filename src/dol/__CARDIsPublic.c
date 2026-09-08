#include "types.h"


struct __CARDIsPublic_Arg0 {
    u8 unk_0;
};

s32 __CARDIsPublic(struct __CARDIsPublic_Arg0 *arg0) {
    u32 v0;
    v0 = (u32)arg0;
    if (arg0->unk_0 == 255) {
    v0 = -4;
    return v0;
    }
    if ((*(u8 *)((u8 *)v0 + 52) & 0x4) != 0) {
    v0 = 0;
    return v0;
    }
    v0 = -10;
    return v0;
}
