#include "types.h"

// Donor seed for fn_80058498 (addr 0x80058498)
// Extracted from adxt_800570DC.c (original name: ADXT_ProcessStreamUpdate)

SjRing* fn_80058498(void* a0, int a1, int a2)
{
    int i;
    SjRing* entry;

    svmEnterCritical();

    for (i = 0; i < 256; i++) {
        if (((SjRing*)lbl_8018B2A4)[i].unk4 == 0) {
            break;
        }
    }

    if (i == 256) {
        entry = 0;
    } else {
        entry = &((SjRing*)lbl_8018B2A4)[i];
        entry->unk4 = 1;
        entry->unkPtr = lbl_80132300;
        entry->unk1C = (int)a0;
        entry->unk20 = a1;
        entry->unk24 = a2;
        entry->unk8 = (void*)lbl_80092330;
        entry->unk38 = (int (*)())fn_800586E4;
        entry->unk3C = entry;

        svmEnterCritical();
        entry->unkC = 0;
        entry->unk10 = entry->unk20;
        entry->unk14 = 0;
        entry->unk18 = 0;
        entry->unk28 = 0;
        entry->unk2C = 0;
        entry->unk30 = 0;
        entry->unk34 = 0;
        svmExitCritical();
    }

    svmExitCritical();
    return entry;
}
