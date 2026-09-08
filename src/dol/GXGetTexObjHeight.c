#include "types.h"


struct GXGetTexObjHeight_Arg0 {
    u8 pad_0[0x8];
    u32 unk_8;
};

u32 GXGetTexObjHeight(struct GXGetTexObjHeight_Arg0 *arg0) {
    return (((arg0->unk_8 >> 10) & 0x3FF) + 1);
}
