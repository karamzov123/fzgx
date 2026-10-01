#include "types.h"

typedef struct SjRing {
    u8 pad_0[0xC];
    int unkC;
    int unk10;
    int unk14;
    int unk18;
    char *unk1C;
    int unk20;
    int unk24;
    int unk28;
    int unk2C;
    int unk30;
    int unk34;
    void (*unk38)(int, int);
    int unk3C;
} SjRing;

typedef struct SjSpan {
    char *ptr;
    int len;
} SjSpan;

extern u32 fn_800576DC(void);
extern u32 fn_80057728(void);
extern void *memcpy(void *, const void *, u32);

void fn_80058070(SjRing* p, int mode, SjSpan* span)
{
    if (span->len <= 0 || span->ptr == 0) {
        return;
    }

    fn_80057728();

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
                    char* d = (char*)offset + (int)p->unk1C;
                    d = (char*)p->unk20 + (int)d;
                    memcpy(d, (void*)span->ptr, n);
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

    fn_800576DC();
}
