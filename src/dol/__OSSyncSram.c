#include "types.h"

extern struct __OSSyncSram_Scb_8015BFC0 Scb_8015BFC0;

struct __OSSyncSram_Scb_8015BFC0 {
    u8 pad_0[0x4C];
    u32 unk_4C;
};

u32 __OSSyncSram(void) {
    return Scb_8015BFC0.unk_4C;
}
