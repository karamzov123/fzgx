#include "types.h"

// Donor seed for fn_800478C0 (addr 0x800478C0)
// Extracted from criadx_80047464.c (original name: fn_800478C0)

void fn_800478C0(void* arg0)
{
    AdxSpan sp;
    short s8_2;
    short s8_1;
    AdxHandle* p = (AdxHandle*)arg0;
    int i;
    AdxObj* obj;
    AdxHandle* q;
    AdxHandle* r;
    int j;
    AdxHandle* s;
    int k;
    AdxChan* ch;

    if (p->unk1 == 1) {
        obj = p->unk4[2];
        q = p;
        r = p;
        for (i = 0; i < p->unk58; i++) {
            q->unk4[0]->vt->fn18(q->unk4[0], 1, 2, &sp);
            if (sp.len == 0) break;
            r->unk2C8[0] = r->unk2CC[0] = *(short*)sp.ptr;
            q->unk4[0]->vt->fn1C(q->unk4[0], 1, &sp);
            q = (AdxHandle*)((char*)q + 4);
            r = (AdxHandle*)((char*)r + 2);
        }
        if (i < p->unk58) return;
        for (j = 0; j < p->unk58; j++) {
            p->unk88[j] = p->unk2C8[j];
            p->unk8C[j] = p->unk2CC[j];
        }
        j = fn_80048340(p, obj);
        if (j == 0) return;
        p->unk2C += j;
        s = p;
        for (k = 0; k < p->unk58; k++) {
            ch = s->unk80;
            CRI_SPSD_parser((short)p->unk64, p->unk5C, &s8_1, &s8_2);
            set_chan(ch, s8_1, s8_2);
            set_chan(ch->unk88, s8_1, s8_2);
            s = (AdxHandle*)((char*)s + 4);
        }
        p->unk1 = 2;
    } else if (p->unk1 == 2) {
        fn_80047A50(p);
    }
}
