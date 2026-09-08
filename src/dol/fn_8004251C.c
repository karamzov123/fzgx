#include "types.h"

struct fn_8004251C_Arg0 {
    u8 pad_0[0x9A];
    s16 unk_9A;
};

extern u32 fn_800425D0(u32);
extern u32 fn_80042980(u32);
extern u32 fn_80042D24(u32);

void fn_8004251C(struct fn_8004251C_Arg0 *arg0) {
    if (arg0->unk_9A == 2) {
    fn_800425D0((u32)arg0);
    } else {
    if (arg0->unk_9A == 1) {
    fn_80042980((u32)arg0);
    } else {
    if ((s16)arg0->unk_9A == 0) {
    fn_80042D24((u32)arg0);
    }
    }
    }
}
