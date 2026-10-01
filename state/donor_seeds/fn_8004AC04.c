#include "types.h"

// Donor seed for fn_8004AC04 (addr 0x8004AC04)
// Extracted from adxt_8004AC04.c (original name: ADXT_GetVoiceByAxHandle)

void fn_8004AC04(AdxtVoice* p, int a)
{
    if (a >= 0) {
        p->unk2C = a;
        return;
    }
    {
        int q;
        q = p->unk10 % 2048 > 0;
        q = (p->unk10 / 2048) + q;
        p->unk2C = q;
    }
}
