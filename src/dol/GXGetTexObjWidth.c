#include "types.h"


struct GXGetTexObjWidth_Arg0 {
    u8 pad_0[0x8];
    u32 unk_8;
};

u32 GXGetTexObjWidth(struct GXGetTexObjWidth_Arg0 *arg0) {
    return ((arg0->unk_8 & 0x3FF) + 1);
}
