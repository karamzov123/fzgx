#include "types.h"

extern char lbl_1_data_40798[];
extern u32 lbl_1_bss_8B56C[24];
extern void fn_8000C49C(char *format, ...);

void fn_1_12D354(void *arg0, void *arg1, void *arg2) {
    char *base = lbl_1_data_40798;
    u32 i;

    if (arg2 == 0) {
        fn_8000C49C(base + 0x358, 0x388, base + 0x368);
    }

    for (i = 0; i < 12; i++) {
        if (lbl_1_bss_8B56C[i * 2] == 0) {
            lbl_1_bss_8B56C[i * 2] = (u32)arg2;
            lbl_1_bss_8B56C[i * 2 + 1] = 0;
            return;
        }
    }

    fn_8000C49C(base + 0x358, 0x394, base + 0x390);
}
