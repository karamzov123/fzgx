#include "types.h"

extern s32 lbl_12_bss_8;
extern void fn_12_4934(void);
extern void fn_12_35EC(void);
extern void fn_12_49F8(void);

void fn_12_33D0(void) {
    if (lbl_12_bss_8 > 0) {
        fn_12_4934();
        fn_12_35EC();
        fn_12_49F8();
        lbl_12_bss_8--;
    }
}
