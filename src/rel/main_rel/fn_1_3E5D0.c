#include "types.h"

extern u32 lbl_1_bss_25B88[4];
extern u32 lbl_1_bss_25CA4[11];
extern void fn_1_435C(u32 value);
extern void fn_1_4310(s32 value);
extern void fn_1_4811C(s32 value);
extern void fn_1_A8F78(void);

void fn_1_3E5D0(void) {
    fn_1_435C(lbl_1_bss_25B88[0]);
    fn_1_4310(0);
    fn_1_435C(lbl_1_bss_25CA4[0]);
    fn_1_4310(0);
    fn_1_4811C(0x8a);
    fn_1_A8F78();
}
