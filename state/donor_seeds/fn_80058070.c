#include "types.h"

// Donor seed for fn_80058070 (addr 0x80058070)
// Extracted from adxt_800570DC.c (original name: fn_80058070)

void fn_80058070(SjRing* p, int mode, SjSpan* span)
{
    if (span->len <= 0 || span->ptr == 0) {
        return;
    }

    svmEnterCritical();

    if (mode == 1) {
        p->unkC += span->len;

        {
            int offset = (int)span->ptr - (int)p->unk1C;
            if (offset < p->unk24) {
                int n = p->unk24 - offset;
                if (span->len < n) {
                    n = span->len;
                }
                {
    {
    char* d = (char*)offset + p->unk1C;
    d = (char*)p->unk20 + (int)d;
    memcpy(d, (void*)span->ptr, n);
}
}
            }
        }

        {
            int end_offset = (int)span->ptr - (int)p->unk1C + span->len;
            if (end_offset > p->unk20) {
                int n = end_offset - p->unk20;
                if (span->len < n) {
                    n = span->len;
                }
                memcpy((void*)p->unk1C, (char*)p->unk1C + (end_offset - n), n);
            }
        }

        p->unk34 += span->len;
    } else if (mode == 0) {
        p->unk10 += span->len;
        p->unk2C += span->len;
    } else {
        span->len = 0;
        span->ptr = 0;
        if (p->unk38 != 0) {
            p->unk38(p->unk3C, -3);
        }
    }

    svmExitCritical();
}
