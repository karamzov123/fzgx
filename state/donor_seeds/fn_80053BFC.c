#include "types.h"

// Donor seed for fn_80053BFC (addr 0x80053BFC)
// Extracted from adxt_80053A30.c (original name: fn_80053BFC)

void fn_80053BFC(void)
{
    AdxtGlobal* g = (AdxtGlobal*)lbl_80187370;

    if (g->ref_count == 0) {
        memset((char*)g + 0x10, 0, 0x40);
        if (g->initialized == 0) {
            char* src1 = (char*)lbl_801319E0 + 0x800;
            char* dst1;
            long n;
            char* src2;
            char* dst2;

            g->table1 = (float*)(((unsigned int)lbl_801319E0 + 0x1F) & ~0x1F);
            dst1 = (char*)g->table1 + 0x800;

            for (n = 0x2AB; n > 0; n--) {
                *dst1 = *src1;
                *(dst1 - 1) = *(src1 - 1);
                *(dst1 - 2) = *(src1 - 2);
                src1 -= 3;
                dst1 -= 3;
            }

            for (n = 0; n < 0x200; n++) {
                g->table1[n] *= lbl_80091350[0];
            }

            src2 = (char*)adxt_sincos_table + 0x2000;
            g->table2 = (void*)(((unsigned int)adxt_sincos_table + 0x1F) & ~0x1F);
            dst2 = (char*)g->table2 + 0x2000;

            for (n = 0xAAB; n > 0; n--) {
                *dst2 = *src2;
                *(dst2 - 1) = *(src2 - 1);
                *(dst2 - 2) = *(src2 - 2);
                src2 -= 3;
                dst2 -= 3;
            }

            g->initialized = 1;
        }
    }
    g->ref_count++;
}
