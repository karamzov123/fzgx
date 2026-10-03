#include "types.h"

typedef struct AdxtVoice {
    u8 pad00[0x10];
    s32 unk10;
    u8 pad14[0x18];
    s32 unk2C;
} AdxtVoice;

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
