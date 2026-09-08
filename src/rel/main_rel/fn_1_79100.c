#include "types.h"

extern void fn_1_9A864(void);
extern u8 fn_1_7B074(void);
extern u32 lbl_1_bss_6D770;
extern u32 lbl_1_bss_6D774;
extern void fn_8006FDEC(void);
extern void fn_80071718(void *arg);
extern void fn_800711A8(void *arg);
extern void fn_1_14CB4(void);
extern void fn_1_FA84(void);
extern s16 lbl_1_data_1DFA4[6];

void fn_1_79100(void) {
    fn_1_9A864();
    if (!fn_1_7B074()) {
        if (lbl_1_bss_6D770 != 0 || lbl_1_bss_6D774 != 0) {
            fn_8006FDEC();
        }
        if (lbl_1_bss_6D770 != 0) {
            fn_80071718((void *)lbl_1_bss_6D770);
            lbl_1_bss_6D770 = 0;
        }
        if (lbl_1_bss_6D774 != 0) {
            fn_800711A8((void *)lbl_1_bss_6D774);
            lbl_1_bss_6D774 = 0;
        }
    }
    fn_1_14CB4();
    fn_1_FA84();
    lbl_1_data_1DFA4[4] = -1;
}
