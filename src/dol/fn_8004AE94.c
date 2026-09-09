#include "types.h"

struct fn_8004AE94_AdxtVoice {
    u8 pad_0[0x10];
    s32 unk_10;
    u8 pad_14[0x40];
    s32 unk_54;
};

s32 fn_8004AE94(struct fn_8004AE94_AdxtVoice* p, s32 n) {
    p->unk_54 = n;
    if (p->unk_54 * 2048 > p->unk_10) {
        p->unk_54 = p->unk_10 / 2048 + (p->unk_10 % 2048 > 0);
    }
    return p->unk_54;
}
