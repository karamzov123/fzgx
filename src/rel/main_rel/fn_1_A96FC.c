#include "types.h"

extern u32 fn_1_451C(void);
extern void fn_8008069C(void *buffer, char *format, ...);
extern void fn_1_A5AA0(void *buffer, void *destination);

extern s16 lbl_1_bss_960;
extern u32 lbl_1_bss_71670;
extern u32 lbl_1_data_35A70[18];
extern char lbl_1_data_35AB8[11];
extern u8 lbl_1_bss_71674[8];

void fn_1_A96FC(void) {
    u8 buffer[0x48];

    lbl_1_bss_71670 = fn_1_451C();
    fn_8008069C(buffer, lbl_1_data_35AB8,
                lbl_1_data_35A70[lbl_1_bss_960]);
    fn_1_A5AA0(buffer, lbl_1_bss_71674);
}
