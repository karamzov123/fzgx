#include "types.h"

// Donor seed for fn_8005A7F8 (addr 0x8005A7F8)
// Extracted from adxt_8005A24C.c (original name: fn_8005A7F8)

void fn_8005A7F8(AdxtSlot* p, int rate)
{
    short sync[7];
    int i;
    int adj_rate;

    if (p == 0) {
        return;
    }

    p->unk24 = rate;

    for (i = 0; i < p->unk2; i++) {
        svm_enter_critical_wrapper();
        if (p->unk8[i] != 0) {
            if (p->unkA0 == 1) {
                if (rate == 32000 && p->unkA2 == 0 && p != 0) {
                    p->unkA4 = 0;
                    p->unkA2 = 1;
                }
                adj_rate = (rate * 1124 + 1124) / 1125;
                sync[0] = (unsigned short)((unsigned int)adj_rate / 32000);
                sync[1] = (unsigned short)((unsigned int)(adj_rate << 8) / 125);
            } else {
                sync[0] = rate / 32000;
                sync[1] = (unsigned int)((rate << 8) / 125) & 0xffff;
            }
            sync[2] = 0;
            sync[3] = 0;
            sync[4] = 0;
            sync[5] = 0;
            sync[6] = 0;
            AXSetVoiceState_cached(p->unk8[i], p->unkA4);
            AXVPBSyncChannelA(p->unk8[i], sync);
        }
        svm_exit_critical_wrapper();
    }
}
