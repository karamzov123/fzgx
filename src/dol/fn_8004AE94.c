#include "types.h"

typedef struct AdxtVoice {
    u8 pad0[0x10];
    s32 unk10;
    u8 pad14[0x40];
    s32 unk54;
} AdxtVoice;

// Donor seed for fn_8004AE94 (addr 0x8004AE94)
// Extracted from adxt_8004AC04.c (original name: fn_8004AE94)

int fn_8004AE94(AdxtVoice* p, int n)
{
    p->unk54 = n;
    if (p->unk54 * 2048 > p->unk10) {
        p->unk54 = p->unk10 / 2048 + (p->unk10 % 2048 > 0);
    }
    return p->unk54;
}
