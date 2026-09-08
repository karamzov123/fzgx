#include "types.h"

struct OSLinkFixed_Arg0 {
    u8 pad_0[0x1C];
    u32 unk_1C;
};

extern u32 Link(u32, u32, u32);

u32 OSLinkFixed(struct OSLinkFixed_Arg0 *arg0, u32 arg1) {
    u32 v0;
    u32 t0;
    v0 = (u32)arg0;
    if (arg0->unk_1C > 3 || arg0->unk_1C < 3) {
    v0 = 0;
    } else {
    t0 = Link(v0, arg1, 1);
    v0 = t0;
    }
    return v0;
}
